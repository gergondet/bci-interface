#include <bci-interface/BCIInterface.h>

#include <bci-interface/EventHandler.h>

#include <bci-interface/Background.h>
#include <bci-interface/DisplayObject.h>

#include <bci-interface/CommandReceiver.h>
#include <bci-interface/CommandOverrider.h>
#include <bci-interface/CommandInterpreter.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <bci-interface/Utils/Win32.h>

namespace bciinterface
{

struct BCIInterfaceImpl
{
private:
    const BCIInterface & m_ref;
    unsigned int m_width;
    unsigned int m_height;
    bool m_in_paradigm;
    bool m_close;
    bool m_finished;
    sf::RenderWindow * m_app;
    std::ofstream m_fpslog;

    bool m_take_screenshot;
    unsigned int m_screenshot_index;

    std::vector<EventHandler *> m_handlers;

    Background * m_background;
    boost::thread * m_backgroundth;

    std::vector<DisplayObject *> m_active_objects;

    std::vector<DisplayObject *> m_objects;
    std::vector<DisplayObject *> m_objects_non_owned;

    std::vector<DisplayObject *> m_gl_objects;
    std::vector<DisplayObject *> m_gl_objects_non_owned;

    CommandReceiver * m_receiver;
    boost::thread * m_receiverth;

    CommandOverrider * m_overrider;

    CommandInterpreter * m_interpreter;

public:
    BCIInterfaceImpl(const BCIInterface & ref, unsigned int width, unsigned int height)
    : m_ref(ref), m_width(width), m_height(height), m_in_paradigm(false), m_close(false), m_finished(true),
        m_app(0), m_fpslog("/tmp/bciinterface_fps.log"), 
        m_take_screenshot(false), m_screenshot_index(0),
        m_handlers(0),
        m_background(0), m_backgroundth(0),
        m_objects(0), m_objects_non_owned(0),
        m_gl_objects(0), m_gl_objects_non_owned(0),
        m_receiver(0), m_receiverth(0),
        m_overrider(0),
        m_interpreter(0)
    {}

    ~BCIInterfaceImpl()
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
    }

    void InitGL()
    {
        glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
        glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               // Black Background
        glClearDepth(1.0f);                                 // Depth Buffer Setup
        glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
        glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        /* Position the camera */
        glTranslatef(0, 0, 0);
    }

    void Resize()
    {
        glViewport(0,0, m_width, m_height);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Calculate The Aspect Ratio Of The Window
        gluPerspective(45.0f,m_width/m_height,0.1f,100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void AddEventHandler(EventHandler * handler)
    {
        m_handlers.push_back(handler);
    }

    void SetBackground(Background * background)
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

    Background * GetBackground() const
    {
        return m_background;
    }

    void AddObject(DisplayObject * object)
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

    void AddNonOwnedObject(DisplayObject * object)
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

    void SetCommandReceiver(CommandReceiver * receiver)
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

    void SetCommandOverrider(CommandOverrider * overrider)
    {
        m_overrider = overrider;
    }

    void SetCommandInterpreter(CommandInterpreter * interpreter)
    {
        m_interpreter = interpreter;
    }

    CommandInterpreter * GetCommandInterpreter()
    {
        return m_interpreter;
    }

    void Clean()
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
            delete tmp;
            m_active_objects.pop_back();
        }
        m_objects.resize(0);
        m_gl_objects.resize(0);
        m_objects_non_owned.resize(0);
        m_gl_objects_non_owned.resize(0);
    }

    void DisplayLoop(bool fullscreen)
    { 
        if(fullscreen)
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen);
            m_app->setMouseCursorVisible(false);
        }
        else
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface");
        }


//        m_app->EnableVerticalSync(true);

        m_app->setKeyRepeatEnabled(false);

        InitGL();
        Resize();

        DisplayLoop();

        m_app->close();
    }

    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, int * cmd, float timeout = 0)
    {
        if(!app)
        {
            if(fullscreen)
            {
                m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen);
            	m_app->setMouseCursorVisible(false);
            }
            else
            {
                m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface");
            }
        }
        else
        {
            m_app = app;
        }

//        m_app->EnableVerticalSync(true);
        m_app->setKeyRepeatEnabled(false);

        InitGL();
        Resize();

        while(!m_close)
        {
            DisplayLoop(cmd, timeout);
        }

        return m_app;
    }

    /* Actual loop launched by BCIInterface public functions */
    void DisplayLoop(int * cmd = 0, float timeout = 0)
    {
        unsigned int frameCount = 0;
        bool in_paradigm = m_in_paradigm;
        sf::Clock clock;
        m_close = false;
        m_finished = false;
//        if(cmd) { *cmd = 0; }

        /* Launch Background thread */
        if(m_background && !m_backgroundth)
        {
            m_backgroundth = new boost::thread(boost::bind(&Background::UpdateLoop, m_background));
        }

        if(m_receiver && !m_receiverth)
        {
            m_receiverth = new boost::thread(boost::bind(&CommandReceiver::CommandLoop, m_receiver));
        }

        while(!m_close && in_paradigm == m_in_paradigm && m_app->isOpen())
        {
            unsigned int newFrameCount = 6*clock.getElapsedTime().asMilliseconds()*0.01;
            /* cheat when missing a frame */
            frameCount = newFrameCount > frameCount+1?frameCount+1:newFrameCount;

            m_app->clear(sf::Color(0x77, 0x77, 0x77, 255));

            /* Process events */
            sf::Event event;
            while(m_app->pollEvent(event))
            {
                if(event.type == sf::Event::Closed || 
                    ( event.type == sf::Event::KeyPressed && ( event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q ) ))
                {   
                    if(cmd)
                    {
                        *cmd = -1;
                    }
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
                if(cmd && interpreter_status) 
                { 
                    *cmd = in_cmd; 
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
                    m_background->Draw(m_app);
                    m_app->pushGLStates();
                }
                else
                {
                    m_app->pushGLStates();
                    m_background->Draw(m_app);
                }
            }
            else
            {
                m_app->pushGLStates();
            }

            /* Draw objects */
            for(size_t i = 0; i < m_objects.size(); ++i)
            {
                m_objects[i]->Display(m_app, frameCount, clock);
            }
            /* Draw non-owned objects */
            for(size_t i = 0; i < m_objects_non_owned.size(); ++i)
            {
                m_objects_non_owned[i]->Display(m_app, frameCount, clock);
            }

            m_app->popGLStates();

            for(size_t i = 0; i < m_gl_objects.size(); ++i)
            {
                m_gl_objects[i]->Display(m_app, frameCount, clock);
            }
            /* Draw non-owned objects */
            for(size_t i = 0; i < m_gl_objects_non_owned.size(); ++i)
            {
                m_gl_objects_non_owned[i]->Display(m_app, frameCount, clock);
            }

            if(m_take_screenshot)
            {
                sf::Image screen = m_app->capture();
                std::stringstream ss;
                ss << "/tmp/bci-interface-screen-" << std::setfill('0') << std::setw(3) << m_screenshot_index << ".png";
                screen.saveToFile(ss.str().c_str());
                m_take_screenshot = false;
                m_screenshot_index++;
            }

            m_app->display();
        }
        m_finished = true;
    }  

    void Close()
    {
        m_close = true;
        m_in_paradigm = false;
        m_app->close(); 
    }

    bool ParadigmStatus()
    {
        return m_in_paradigm;
    }

    void StopParadigm()
    {
        m_in_paradigm = false;
    }

    void StartParadigm()
    {
        if(!m_close)
        {
            m_in_paradigm = true;
        }
    }

    sf::RenderWindow * GetRenderWindow()
    {
        return m_app;
    }
};

BCIInterface::BCIInterface(unsigned int width, unsigned int height) : m_impl(new BCIInterfaceImpl(*this, width, height))
{}

bool BCIInterface::ParadigmStatus()
{
    return m_impl->ParadigmStatus();
}

void BCIInterface::StartParadigm()
{
    m_impl->StartParadigm();
}

void BCIInterface::StopParadigm()
{
    m_impl->StopParadigm();
}

void BCIInterface::AddEventHandler(EventHandler * handler)
{
    m_impl->AddEventHandler(handler);
}

void BCIInterface::SetBackground(Background * background)
{
    m_impl->SetBackground(background);
}

Background * BCIInterface::GetBackground() const
{
    return m_impl->GetBackground();
}

void BCIInterface::AddObject(DisplayObject * object)
{
    m_impl->AddObject(object);
}

void BCIInterface::AddNonOwnedObject(DisplayObject * object)
{
    m_impl->AddNonOwnedObject(object);
}

void BCIInterface::SetCommandReceiver(CommandReceiver * receiver)
{
    m_impl->SetCommandReceiver(receiver);
}

void BCIInterface::SetCommandOverrider(CommandOverrider * overrider)
{
    m_impl->SetCommandOverrider(overrider);
}

void BCIInterface::SetCommandInterpreter(CommandInterpreter * interpreter)
{
    m_impl->SetCommandInterpreter(interpreter);
}

CommandInterpreter * BCIInterface::GetCommandInterpreter()
{
    return m_impl->GetCommandInterpreter();
}

void BCIInterface::Clean()
{
    m_impl->Clean();
}

void BCIInterface::DisplayLoop(bool fullscreen)
{
    m_impl->DisplayLoop(fullscreen);
}

sf::RenderWindow * BCIInterface::DisplayLoop(sf::RenderWindow * app, bool fullscreen, int * cmd, float timeout)
{
    return m_impl->DisplayLoop(app, fullscreen, cmd, timeout);
}

sf::RenderWindow * BCIInterface::DisplayLoop(sf::RenderWindow * app, int * cmd, float timeout)
{
    return m_impl->DisplayLoop(app, true, cmd, timeout);
}

void BCIInterface::Close()
{
    m_impl->Close();
}

sf::RenderWindow * BCIInterface::GetRenderWindow()
{
    return m_impl->GetRenderWindow();
}

} // namespace bciinterface

