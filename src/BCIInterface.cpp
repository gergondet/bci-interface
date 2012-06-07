#include <bci-interface/BCIInterface.h>

#include <bci-interface/EventHandler.h>

#include <bci-interface/Background.h>
#include <bci-interface/DisplayObject.h>

#include <bci-interface/CommandReceiver.h>
#include <bci-interface/CommandInterpreter.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include <SFML/Graphics.hpp>

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
    sf::RenderWindow * m_app;
    std::ofstream m_fpslog;

    bool m_take_screenshot;
    unsigned int m_screenshot_index;

    std::vector<EventHandler *> m_handlers;

    Background * m_background;
    boost::thread * m_backgroundth;

    std::vector<DisplayObject *> m_objects;
    std::vector<DisplayObject *> m_objects_non_owned;

    CommandReceiver * m_receiver;
    boost::thread * m_receiverth;

    CommandInterpreter * m_interpreter;

public:
    BCIInterfaceImpl(const BCIInterface & ref, unsigned int width, unsigned int height)
    : m_ref(ref), m_width(width), m_height(height), m_in_paradigm(false), m_close(false), m_app(0), m_fpslog("/tmp/bciinterface_fps.log"), 
        m_take_screenshot(false), m_screenshot_index(0),
        m_handlers(0),
        m_background(0), m_backgroundth(0),
        m_objects(0), m_objects_non_owned(0),
        m_receiver(0), m_receiverth(0),
        m_interpreter(0)
    {}

    ~BCIInterfaceImpl()
    {
        m_fpslog.close();
        m_objects_non_owned.resize(0);
        for(size_t i = 0; i < m_objects.size(); ++i)
        {
            delete m_objects[i];
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
        m_objects.push_back(object);
    }

    void AddNonOwnedObject(DisplayObject * object)
    {
        m_objects_non_owned.push_back(object);
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

    void SetCommandInterpreter(CommandInterpreter * interpreter)
    {
        m_interpreter = interpreter;
    }

    void Clean()
    {
        while(m_handlers.size() > 0)
        {
            EventHandler * tmp = m_handlers.back();
            delete tmp;
            m_handlers.pop_back();
        }
        while(m_objects.size() > 0)
        {
            DisplayObject * tmp = m_objects.back();
            delete tmp;
            m_objects.pop_back();
        }
        m_objects_non_owned.resize(0);
    }

    void DisplayLoop(bool fullscreen)
    { 
        if(fullscreen)
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen);
            m_app->ShowMouseCursor(false);
        }
        else
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface");
        }


//        m_app->EnableVerticalSync(true);

        DisplayLoop();

        m_app->Close();
    }

    sf::RenderWindow * DisplayLoop(sf::RenderWindow * app, bool fullscreen, int * cmd, float timeout = 0)
    {
        if(!app)
        {
            if(fullscreen)
            {
                m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen);
                m_app->ShowMouseCursor(false);
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

        while(!m_close && in_paradigm == m_in_paradigm && m_app->IsOpened())
        {
            unsigned int newFrameCount = 6*clock.GetElapsedTime()*0.01;
            /* cheat when missing a frame */
            frameCount = newFrameCount > frameCount+1?frameCount+1:newFrameCount;

            m_app->Clear(sf::Color(0x77, 0x77, 0x77, 255));

            /* Process events */
            sf::Event event;
            while(m_app->PollEvent(event))
            {
                if(event.Type == sf::Event::Closed || 
                    ( event.Type == sf::Event::KeyPressed && ( event.Key.Code == sf::Keyboard::Escape || event.Key.Code == sf::Keyboard::Q ) ))
                {   
                    if(cmd)
                    {
                        *cmd = -1;
                    }
                    Close(); 
                }
                if( event.Type == sf::Event::KeyPressed && event.Key.Code == sf::Keyboard::F1 ) 
                {
                    m_take_screenshot = true;
                }
                for(size_t i = 0; i < m_handlers.size(); ++i)
                {
                    m_handlers[i]->Process(event);
                }
                for(size_t i = 0; i < m_objects.size(); ++i)
                {
                    m_objects[i]->Process(event, m_ref);
                }
                for(size_t i = 0; i < m_objects_non_owned.size(); ++i)
                {
                    m_objects_non_owned[i]->Process(event, m_ref);
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
                if(clock.GetElapsedTime() > timeout)
                {
                    in_cmd = m_receiver->GetCommand();
                }
                bool interpreter_status = m_interpreter->InterpretCommand(in_cmd, m_objects);
                if(cmd && interpreter_status) 
                { 
                    *cmd = in_cmd; 
                    m_in_paradigm = false; 
                    SetCommandInterpreter(0);
                }
            }

            /* Draw background */
            if(m_background)
            {
                m_background->Draw(m_app);
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

            /* Log fps regularly */
            if(frameCount % 100 == 99)
            {
                uint32_t frametime = m_app->GetFrameTime();
                if(frametime != 0)
                {
                    m_fpslog << 1000/frametime << " fps" << std::endl;
                }
                else
                {
                    m_fpslog << "2000 fps" << std::endl;
                }
            } 

            m_app->Display();
            if(m_take_screenshot)
            {
                sf::Image screen = m_app->Capture();
                std::stringstream ss;
                ss << "/tmp/bci-interface-screen-" << std::setfill('0') << std::setw(3) << m_screenshot_index << ".png";
                screen.SaveToFile(ss.str().c_str());
                m_take_screenshot = false;
                m_screenshot_index++;
            }
        }
//        m_fpslog.close();

/*        if(!cmd)
*        {
*            // Actual and final exit
*            if(m_background)
*            {
*                m_background->Close();
*                m_backgroundth->join();
*                delete m_backgroundth;
*                m_backgroundth = 0;
*            }
*            if(m_receiver)
*            {
*                m_receiver->Close();
*                m_receiverth->join();
*                delete m_receiverth;
*                m_receiverth = 0;
*            }
*        } */
    }  

    void Close()
    {
        m_close = true;
        m_in_paradigm = false;
        m_app->Close(); 
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

void BCIInterface::SetCommandInterpreter(CommandInterpreter * interpreter)
{
    m_impl->SetCommandInterpreter(interpreter);
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

