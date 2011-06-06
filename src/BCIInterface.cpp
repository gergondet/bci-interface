#include <bci-interface/BCIInterface.h>

#include <bci-interface/BackgroundSprite.h>
#include <bci-interface/DisplayObject.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

namespace bciinterface
{

struct BCIInterfaceImpl
{
private:
    unsigned int m_width;
    unsigned int m_height;
    bool m_close;
    sf::RenderWindow * m_app;
    std::ofstream m_fpslog;

    BackgroundSprite * m_background;
    boost::thread * m_backgroundth;

    std::vector<DisplayObject *> m_objects;

public:
    BCIInterfaceImpl(unsigned int width, unsigned int height)
    : m_width(width), m_height(height), m_close(false), m_app(0), m_fpslog("/tmp/bciinterface_fps.log"), 
        m_background(0), m_backgroundth(0),
        m_objects(0)
    {}

    ~BCIInterfaceImpl()
    {
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
        delete m_backgroundth;
        delete m_background;
        delete m_app;
    }

    void SetBackgroundSprite(BackgroundSprite * background)
    {
        if(m_backgroundth)
        {
            /* Change backgroundsprite type while (e.g. camera switch ?) running */
            m_background->Close();
            m_backgroundth->join();
            delete m_backgroundth;
            m_background = background;
            m_backgroundth = new boost::thread(boost::bind(&BackgroundSprite::UpdateLoop, m_background));
        }
        else
        {
            m_background = background;
        }
    }

    void AddObject(DisplayObject * object)
    {
        m_objects.push_back(object);
    }

    void Clean()
    {
        while(m_objects.size() > 0)
        {
            DisplayObject * tmp = m_objects.back();
            delete tmp;
            m_objects.pop_back();
        }
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


        m_app->UseVerticalSync(true);

        DisplayLoop();

        m_app->Close();
    }

    void DisplayLoop(sf::RenderWindow * app, unsigned int * cmd = 0, float timeout = 0)
    {
        m_app = app;

        m_app->UseVerticalSync(true);

        DisplayLoop(cmd, timeout);

        m_app = 0;
    }

    /* Actual loop launched by BCIInterface public functions */
    void DisplayLoop(unsigned int * cmd = 0, float timeout = 0)
    {
        unsigned int frameCount = 0;
        sf::Clock clock;
        m_close = false;

        /* Launch BackgroundSprite thread */
        if(m_background && !m_backgroundth)
        {
            m_backgroundth = new boost::thread(boost::bind(&BackgroundSprite::UpdateLoop, m_background));
        }

        while(!m_close && m_app->IsOpened())
        {
            unsigned int newFrameCount = (unsigned int)floor(clock.GetElapsedTime()*60);
            /* cheat when missing a frame */
            frameCount = newFrameCount > frameCount+1?frameCount+1:newFrameCount;

            m_app->Clear();

            /* Process events */
            sf::Event event;
            while(m_app->GetEvent(event))
            {
                if(event.Type == sf::Event::Closed)
                {   m_app->Close(); }
                if(event.Type == sf::Event::KeyPressed && ( event.Key.Code == sf::Key::Escape || event.Key.Code == sf::Key::Q ) )
                {   m_app->Close(); }
                for(size_t i = 0; i < m_objects.size(); ++i)
                {
                    m_objects[i]->Process(event);
                }
            }

            /* Draw background */
            if(m_background)
            {
                m_app->Draw(*(m_background->GetSprite()));
            }

            /* Draw objects */
            for(size_t i = 0; i < m_objects.size(); ++i)
            {
                m_objects[i]->Display(m_app, frameCount);
            }

            m_app->Display();

            /* Log fps regularly */
            if(frameCount % 60 == 0)
            {
                m_fpslog << 1/m_app->GetFrameTime() << " fps" << std::endl;
            }
        }
        m_fpslog.close();

        if(m_background && !cmd)
        {
            m_background->Close();
            m_backgroundth->join();
            delete m_backgroundth;
        }
    }

    void Close()
    {
        m_close = true;
    }
};

BCIInterface::BCIInterface(unsigned int width, unsigned int height) : m_impl(new BCIInterfaceImpl(width, height))
{}

void BCIInterface::SetBackgroundSprite(BackgroundSprite * background)
{
    m_impl->SetBackgroundSprite(background);
}

void BCIInterface::AddObject(DisplayObject * object)
{
    m_impl->AddObject(object);
}

void BCIInterface::Clean()
{
    m_impl->Clean();
}

void BCIInterface::DisplayLoop(bool fullscreen)
{
    m_impl->DisplayLoop(fullscreen);
}

void BCIInterface::DisplayLoop(sf::RenderWindow * app, unsigned int * cmd, float timeout)
{
    m_impl->DisplayLoop(app, cmd, timeout);
}

void BCIInterface::Close()
{
    m_impl->Close();
}

} // namespace bciinterface

