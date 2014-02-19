#include "BCIInterfaceImpl.h"

#include <X11/Xlib.h>

namespace bciinterface
{

BCIInterfaceImpl::BCIInterfaceImpl(const BCIInterface & ref, unsigned int width, unsigned int height)
: m_ref(ref), m_width(width), m_height(height), m_in_paradigm(false), m_close(false), m_finished(true),
    m_app(0), m_oculus_window(0), m_fpslog("/tmp/bciinterface_fps.log"),
    m_take_screenshot(false), m_screenshot_index(0),
    m_handlers(0),
    m_background(0), m_backgroundth(0),
    m_objects(0), m_objects_non_owned(0),
    m_gl_objects(0), m_gl_objects_non_owned(0),
    m_objects_to_delete(0),
    m_receiver(0), m_receiverth(0),
    m_overrider(0),
    m_interpreter(0)
{}

BCIInterfaceImpl::~BCIInterfaceImpl()
{
    m_close = true;
    while(!m_finished) { usleep(1000); }
    m_fpslog.close();
    m_objects_non_owned.resize(0);
    m_gl_objects_non_owned.resize(0);
    for(size_t i = 0; i < m_active_objects.size(); ++i)
    {
        delete m_active_objects[i];
    }
    if(m_background)
    {
        m_background->Close();
    }
    if(m_backgroundth)
    {
        m_backgroundth->join();
    }
    if(m_receiver)
    {
        m_receiver->Close();
    }
    if(m_receiverth)
    {
        m_receiverth->join();
    }
    delete m_backgroundth;
    delete m_background;
    delete m_receiverth;
    delete m_app;
    delete m_oculus_window;
}

void BCIInterfaceImpl::InitGL()
{
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background
    glClearDepth(1.0f);                                 // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /* Position the camera */
    glTranslatef(0, 0, 0);
}

void BCIInterfaceImpl::Resize()
{
    glViewport(0,0, m_width, m_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,m_width/m_height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void BCIInterfaceImpl::AddEventHandler(EventHandler * handler)
{
    m_handlers.push_back(handler);
}

void BCIInterfaceImpl::SetBackground(Background * background)
{
    if(m_backgroundth)
    {
        /* Change background type while (e.g. camera switch ?) running */
        m_background->Close();
        m_backgroundth->join();
        delete m_backgroundth;
        m_background = background;
        m_backgroundth = new boost::thread(boost::bind(&Background::UpdateLoop, m_background));
    }
    else
    {
        m_background = background;
    }
}

Background * BCIInterfaceImpl::GetBackground() const
{
    return m_background;
}

void BCIInterfaceImpl::AddObject(DisplayObject * object)
{
    m_active_objects.push_back(object);
    if(object->DrawWithGL())
    {
        m_gl_objects.push_back(object);
    }
    else
    {
        m_objects.push_back(object);
    }
}

void BCIInterfaceImpl::AddNonOwnedObject(DisplayObject * object)
{
    if(object->DrawWithGL())
    {
        m_gl_objects_non_owned.push_back(object);
    }
    else
    {
        m_objects_non_owned.push_back(object);
    }
}

void BCIInterfaceImpl::SetCommandReceiver(CommandReceiver * receiver)
{
    if(m_receiverth)
    {
        /* Change receiver while running (e.g. paradigm switch) */
        m_receiver->Close();
        m_receiverth->join();
        delete m_receiverth;
        m_receiver = receiver;
        m_receiverth = new boost::thread(boost::bind(&CommandReceiver::CommandLoop, m_receiver));
    }
    else
    {
        m_receiver = receiver;
    }
}

void BCIInterfaceImpl::SetCommandOverrider(CommandOverrider * overrider)
{
    m_overrider = overrider;
}

void BCIInterfaceImpl::SetCommandInterpreter(CommandInterpreter * interpreter)
{
    m_interpreter = interpreter;
}

CommandInterpreter * BCIInterfaceImpl::GetCommandInterpreter()
{
    return m_interpreter;
}

void BCIInterfaceImpl::Clean()
{
    while(m_handlers.size() > 0)
    {
        EventHandler * tmp = m_handlers.back();
        delete tmp;
        m_handlers.pop_back();
    }
    while(m_active_objects.size() > 0)
    {
        DisplayObject * tmp = m_active_objects.back();
        m_active_objects.pop_back();
        m_objects_to_delete.push_back(tmp);
    }
    if(m_oculus_window)
    {
        m_oculus_window->clearGLcallbacks();
    }
    m_objects.resize(0);
    m_gl_objects.resize(0);
    m_objects_non_owned.resize(0);
    m_gl_objects_non_owned.resize(0);
}

void BCIInterfaceImpl::DisplayLoop(bool fullscreen)
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
    if(fullscreen)
    {
        m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen, contextSettings);
        m_app->setMouseCursorVisible(false);
    }
    else
    {
        m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Close, contextSettings);
    }

    m_display_fun = boost::bind(&sf::RenderWindow::display, m_app);


    m_app->setVerticalSyncEnabled(true);

    m_app->setKeyRepeatEnabled(false);

    InitGL();

    int cmd_out = -1;

    DisplayLoop(*m_app, *m_app, cmd_out);

    m_app->close();
}

sf::RenderWindow * BCIInterfaceImpl::DisplayLoop(sf::RenderWindow * app, bool fullscreen, int & cmd, float timeout)
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
    if(!app)
    {
        if(fullscreen)
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen, contextSettings);
        	m_app->setMouseCursorVisible(false);
        }
        else
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Close, contextSettings);
        }
    }
    else
    {
        m_app = app;
    }

    m_display_fun = boost::bind(&sf::RenderWindow::display, m_app);

    m_app->setVerticalSyncEnabled(true);
    m_app->setKeyRepeatEnabled(false);

    InitGL();

    while(!m_close)
    {
        DisplayLoop(*m_app, *m_app, cmd, timeout);
    }

    return m_app;
}

void BCIInterfaceImpl::InitOculus()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;
    m_oculus_window = new OculusWindow(sf::VideoMode(m_width, m_height), "bci-interface (Oculus)", sf::Style::Close, contextSettings);

    m_display_fun = boost::bind(&OculusWindow::display, m_oculus_window);
}

void BCIInterfaceImpl::OculusDisplayLoop(int & cmd)
{
    XInitThreads();
    if(!m_oculus_window)
    {
        InitOculus();
    }

    m_oculus_window->getApplication().setVerticalSyncEnabled(true);

    while(!m_close)
    {
        DisplayLoop(m_oculus_window->getApplication(), m_oculus_window->getRenderTarget(), cmd);
    }
}

/* Actual loop launched by BCIInterface public functions */
void BCIInterfaceImpl::DisplayLoop(sf::Window & eventWindow, sf::RenderTarget & drawTarget, int & cmd, float timeout)
{
    unsigned int frameCount = 0;
    bool in_paradigm = m_in_paradigm;
    sf::Clock clock;
    m_close = false;
    m_finished = false;
    while(m_objects_to_delete.size())
    {
        DisplayObject * tmp = m_objects_to_delete.back();
        m_objects_to_delete.pop_back();
        delete tmp;
    }
    //if(cmd) { *cmd = 0; }

    /* Launch Background thread */
    if(m_background && !m_backgroundth)
    {
        m_backgroundth = new boost::thread(boost::bind(&Background::UpdateLoop, m_background));
    }

    if(m_receiver && !m_receiverth)
    {
        m_receiverth = new boost::thread(boost::bind(&CommandReceiver::CommandLoop, m_receiver));
    }

    while(!m_close && in_paradigm == m_in_paradigm && eventWindow.isOpen())
    {
        unsigned int newFrameCount = static_cast<unsigned int>(trunc(6*clock.getElapsedTime().asMilliseconds()*0.01));
        /* cheat when missing a frame */
        frameCount = newFrameCount > frameCount+1?frameCount+1:newFrameCount;

//        drawTarget.clear(sf::Color(0x77, 0x77, 0x77, 255));
        drawTarget.clear(sf::Color(0, 0, 0, 255));

        /* Process events */
        sf::Event event;
        while(eventWindow.pollEvent(event))
        {
            if(event.type == sf::Event::Closed ||
                ( event.type == sf::Event::KeyPressed && ( event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q ) ))
            {
                cmd = -1;
                Close();
            }
            if( event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F1 )
            {
                m_take_screenshot = true;
            }
            for(size_t i = 0; i < m_handlers.size(); ++i)
            {
                m_handlers[i]->Process(event);
            }
            for(size_t i = 0; i < m_active_objects.size(); ++i)
            {
                m_active_objects[i]->Process(event, m_ref);
            }
            for(size_t i = 0; i < m_objects_non_owned.size(); ++i)
            {
                m_objects_non_owned[i]->Process(event, m_ref);
            }
            for(size_t i = 0; i < m_gl_objects_non_owned.size(); ++i)
            {
                m_gl_objects_non_owned[i]->Process(event, m_ref);
            }
            if(m_overrider)
            {
                m_overrider->Process(event, m_ref);
            }
            if(m_interpreter)
            {
                m_interpreter->Process(event, m_ref);
            }
        }

        /* Current command of the BCI system */
        if(m_receiver && m_interpreter)
        {
            int in_cmd = 0;
            if(clock.getElapsedTime().asMilliseconds() > timeout)
            {
                in_cmd = m_receiver->GetCommand();
                if(m_overrider && m_overrider->IsOverriding())
                {
                    in_cmd = m_overrider->GetCommand();
                }
            }
            bool interpreter_status = m_interpreter->InterpretCommand(in_cmd, m_active_objects);
            if(interpreter_status)
            {
                cmd = in_cmd;
                m_in_paradigm = false;
                SetCommandInterpreter(0);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /* Draw background */
        if(m_background)
        {
            if(m_background->DrawWithGL())
            {
                m_background->Draw(&drawTarget);
                drawTarget.pushGLStates();
            }
            else
            {
                drawTarget.pushGLStates();
                m_background->Draw(&drawTarget);
            }
        }
        else
        {
            drawTarget.pushGLStates();
        }

        /* Draw objects */
        for(size_t i = 0; i < m_objects.size(); ++i)
        {
            m_objects[i]->Display(&drawTarget, frameCount, clock);
        }
        /* Draw non-owned objects */
        for(size_t i = 0; i < m_objects_non_owned.size(); ++i)
        {
            m_objects_non_owned[i]->Display(&drawTarget, frameCount, clock);
        }

        drawTarget.popGLStates();

        for(size_t i = 0; i < m_gl_objects.size(); ++i)
        {
            m_gl_objects[i]->Display(&drawTarget, frameCount, clock);
        }
        /* Draw non-owned objects */
        for(size_t i = 0; i < m_gl_objects_non_owned.size(); ++i)
        {
            m_gl_objects_non_owned[i]->Display(&drawTarget, frameCount, clock);
        }


        m_display_fun();

        if(m_take_screenshot && m_app)
        {
            sf::Image screen = m_app->capture();
            std::stringstream ss;
            #ifndef WIN32
            ss << "/tmp/bci-interface-screen-" << std::setfill('0') << std::setw(3) << m_screenshot_index << ".png";
            #else
            ss << "C:/bci-interface-screen-" << std::setfill('0') << std::setw(3) << m_screenshot_index << ".png";
            #endif
            screen.saveToFile(ss.str().c_str());
            m_take_screenshot = false;
            m_screenshot_index++;
        }
    }
    m_finished = true;
}

void BCIInterfaceImpl::Close()
{
    m_close = true;
    m_in_paradigm = false;
    if(m_app)
    {
        m_app->close();
    }
}

bool BCIInterfaceImpl::ParadigmStatus()
{
    return m_in_paradigm;
}

void BCIInterfaceImpl::StopParadigm()
{
    m_in_paradigm = false;
}

void BCIInterfaceImpl::StartParadigm()
{
    if(!m_close)
    {
        m_in_paradigm = true;
    }
}

sf::RenderWindow * BCIInterfaceImpl::GetRenderWindow()
{
    return m_app;
}

OculusWindow * BCIInterfaceImpl::GetOculusWindow()
{
    return m_oculus_window;
}

float BCIInterfaceImpl::GetRenderScale()
{
    if(m_oculus_window) return m_oculus_window->getRenderScale();
    return 1.0f;
}

void BCIInterfaceImpl::EnableFPSCounter(sf::Font & font)
{
    if(m_oculus_window) { m_oculus_window->enableFPSCounter(font); }
}

} // namespace bciinterface

