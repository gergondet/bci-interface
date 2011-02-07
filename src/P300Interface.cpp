#include "bci-interface/P300Interface.h"

#include <boost/thread.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>

#include <bci-middleware/P300Client.h>
#include <bci-interface/P300ObjectVector.h>

namespace bciinterface
{

struct P300InterfaceImpl
{

private:
    BackgroundSprite * m_backgroundSprite;
    bool m_updateBackgroundManually;
    unsigned int m_width, m_height;
    bool m_pausable, m_pause, m_close;
    unsigned int m_nbtrials;
    float m_flashtime, m_interflashtime, m_intercycletime;
    unsigned int m_nbObjects;
    bool m_training;
    std::vector<int> m_trainingset;
    P300ObjectVector m_p300objects;
    bcimw::P300Client * m_p300client;
    sf::RenderWindow * m_app;
public:
    P300InterfaceImpl(unsigned int width, unsigned int height, unsigned int mode) :
        m_backgroundSprite(0), m_updateBackgroundManually(false),
        m_width(width), m_height(height), m_pausable(true), m_pause(false), m_close(false),
        m_nbtrials(4), m_flashtime(0.060), m_interflashtime(0.010), m_intercycletime(1.0),
        m_nbObjects(36), m_training(false) , m_p300client(0),
        m_app(0)
    {
        if(mode == 1)
        {
            m_training = true;
            m_trainingset.push_back(3);
            m_trainingset.push_back(14);
            m_trainingset.push_back(18);
            m_trainingset.push_back(19);
            m_trainingset.push_back(27);
            m_trainingset.push_back(1);
        }
    }
    ~P300InterfaceImpl()
    {
        delete m_app;
    }
    void SetNbTrials(const unsigned int nbTrials)
    {   
        m_nbtrials = nbTrials;
    }
    void SetFlashTime(const float flashTime)
    {   
        m_flashtime = flashTime;
    }
    void SetInterFlashTime(const float interFlashTime)
    {   
        m_interflashtime = interFlashTime;
    }
    void SetInterCycleTime(const float interCycleTime)
    {   
        m_intercycletime = interCycleTime;
    }
    void AddObject(P300Object * object)
    {
        Pause();

        m_p300objects.AddObject(object);

        Resume();
    }
    void RemoveObject(const std::string & name)
    {
        Pause();

        m_p300objects.RemoveObject(name);

        Resume();
    }
    void ClearObjects()
    {
        Pause();

        m_p300objects.ClearObjects();

        Resume();
    }

    void StartP300Client(const std::string & serverName, unsigned short serverPort)
    {
        delete m_p300client;
        m_p300client = new bcimw::P300Client(serverName, serverPort);
    }

    void SetBackgroundSprite(BackgroundSprite * sprite)
    {
        m_backgroundSprite = sprite;
    }

    void SetUpdateBackgroundManually(bool enable)
    {
        if(!m_app) /* no more change after interface launch */
        {
            m_updateBackgroundManually = enable;
        }
    }

    void UpdateBackground(unsigned char * img)
    {
        if(m_updateBackgroundManually && m_backgroundSprite)
        {
            m_backgroundSprite->UpdateFromBuffer(img);
        }
    }

    void DisplayLoop(sf::RenderWindow * app)
    {
        if(!m_backgroundSprite)
        {
            std::cerr << "Call SetBackgroundSprite before launching interface display loop" << std::endl;
            return;
        }
        if(!m_p300client)
        {
            std::cerr << "Call StartP300Client before launching interface display loop" << std::endl;
            return;
        }

        m_app = app;

        DisplayLoop();

        m_app = 0;
        
    }
    
    void DisplayLoop(bool fullscreen)
    {
        if(!m_backgroundSprite)
        {
            std::cerr << "Call SetBackgroundSprite before launching interface display loop" << std::endl;
            return;
        }
        if(!m_p300client)
        {
            std::cerr << "Call StartP300Client before launching interface display loop" << std::endl;
            return;
        }

        if(fullscreen)
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "p300-interface", sf::Style::Fullscreen);
            m_app->ShowMouseCursor(false);
        }
        else
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "p300-interface");
        }

        DisplayLoop();

        m_app->Close();
    }

    void DisplayLoop()
    {
        sf::Clock clock;
        time_t t1,t2;
        unsigned int frameCount = 0;
        m_close = false;

        boost::thread * th;
        if(!m_updateBackgroundManually)
        {
            m_backgroundSprite->Initialize();
            th = new boost::thread(boost::bind(&BackgroundSprite::UpdateLoop, m_backgroundSprite));
        }

        time(&t1);
        while(!m_close && m_app->IsOpened())
        {
            if(m_training)
            {
                m_pausable = false;
                for(int i = 0; i < m_trainingset.size(); ++i)
                { 
                    clock.Reset();
                    while(clock.GetElapsedTime() < 2*m_intercycletime)
                    {
                            frameCount++;
                            Display(m_trainingset[i] - 1);
                    }
                    while(clock.GetElapsedTime() < m_intercycletime)
                    {
                            frameCount++;
                            Display(-1);
                    }
                    
                    unsigned int apparitionCount = m_nbObjects*m_nbtrials;
                    while(apparitionCount > 0 )
                    {
                        /* Randomly select a new object to highlight */
                        apparitionCount--;
                        unsigned int idx = m_p300client->GetID();
                        clock.Reset();
                        while(!m_close && m_app->IsOpened() && clock.GetElapsedTime() < m_flashtime)
                        {
                            frameCount++;
                            Display(idx - 1);
                        }

                        clock.Reset();
                        while(!m_close && m_app->IsOpened() && clock.GetElapsedTime() < m_interflashtime) 
                        {
                            frameCount++;
                            Display(-1);
                        }
                    }

                    unsigned int cmd = m_p300client->GetID();
                    std::cerr << "Got cmd " << cmd << std::endl;
                    
                    clock.Reset();
                    while(clock.GetElapsedTime() < 2*m_intercycletime)
                    {
                        frameCount++;
                        Display(cmd - 1);
                    }
                }
                m_close = true;
            }
            else
            {
                if(frameCount < 100 || m_pause)
                {
                    /* Just draw the background when in pause */
                    frameCount++;
                    Display(-1);
                }
                else
                {
                    /* Enter a P300 cycle, can not be paused now */
                    m_pausable = false;
                    unsigned int apparitionCount = m_nbObjects*m_nbtrials;
                    while(apparitionCount > 0 )
                    {
                        /* Randomly select a new object to highlight */
                        apparitionCount--;
                        unsigned int idx = m_p300client->GetID();
                        clock.Reset();
                        while(!m_close && m_app->IsOpened() && clock.GetElapsedTime() < m_flashtime)
                        {
                            frameCount++;
                            Display(idx - 1);
                        }

                        clock.Reset();
                        while(!m_close && m_app->IsOpened() && clock.GetElapsedTime() < m_interflashtime) 
                        {
                            frameCount++;
                            Display(-1);
                        }
                    }

                    /* P300 cycle finished, get the command and then wait go for next cycle */
                    unsigned int cmd = m_p300client->GetID();
                    std::cerr << "Got cmd " << cmd << std::endl;
                    m_pausable = true;
                    clock.Reset();
                    while(!m_close && m_app->IsOpened() && clock.GetElapsedTime() < 2*m_intercycletime)
                    {
                            frameCount++;
                            Display(cmd - 1);
                    }
                }
            }
        }
        time(&t2);
        std::cerr << frameCount << " frames in " << (t2-t1) << "s so roughly: " << frameCount/(t2-t1) << " fps" << std::endl;
        if(!m_updateBackgroundManually)
        {
            m_backgroundSprite->Close();
            if(th)
            {
                th->join();
                delete th;
            }
        }
    }
    void Pause()
    {
        while(!m_pausable);
        m_pause = true;
    }
    void Resume()
    {
        m_pause = false;
    }
    void Close()
    {
        m_close = true;
    }

private:
    inline void ProcessEvents()
    {
        sf::Event Event;
        while (m_app->GetEvent(Event))
        {
            if (Event.Type == sf::Event::Closed)
                m_app->Close();
            if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::Escape || Event.Key.Code == sf::Key::Q ) )
                m_app->Close();
            if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::S ) )
                m_close = true;
        }
    }

    inline void DrawBackground()
    {
        sf::Sprite * sprite = m_backgroundSprite->GetSprite();

        if(sprite)
        {
            sprite->Resize(m_width, m_height);
            m_app->Draw(*sprite);
        }
    }

    inline void NoDisplay()
    {
        m_app->Clear();
        
        ProcessEvents();
        
        DrawBackground();
        
        m_app->Display();
    }

    inline void Display(int activeObject)
    {
        m_app->Clear();
        
        ProcessEvents();
        
        DrawBackground();
        
        m_p300objects.DrawObjects(m_app, activeObject);

        m_app->Display();
    }
    

}; // struct P300InterfaceImpl 

P300Interface::P300Interface(unsigned int width, unsigned int height, unsigned int mode)
: m_impl(new P300InterfaceImpl(width, height, mode))
{
}

void P300Interface::AddObject(P300Object * object)
{
    m_impl->AddObject(object);
}

void P300Interface::RemoveObject(const std::string & name)
{
    m_impl->RemoveObject(name);
}

void P300Interface::ClearObjects()
{
    m_impl->ClearObjects();
}

void P300Interface::StartP300Client(const std::string & serverName, unsigned short serverPort)
{
    m_impl->StartP300Client(serverName, serverPort);
}

void P300Interface::SetBackgroundSprite(BackgroundSprite * sprite)
{
    m_impl->SetBackgroundSprite(sprite);
}

void P300Interface::SetUpdateBackgroundManually(bool enable)
{
    m_impl->SetUpdateBackgroundManually(enable);
}

void P300Interface::UpdateBackground(unsigned char * img)
{
    m_impl->UpdateBackground(img);
}

void P300Interface::DisplayLoop(sf::RenderWindow * app)
{
    m_impl->DisplayLoop(app);
}

void P300Interface::DisplayLoop(bool fullscreen)
{
    m_impl->DisplayLoop(fullscreen);
}

void P300Interface::Pause()
{
    m_impl->Pause();
}

void P300Interface::Resume()
{
    m_impl->Resume();
}

void P300Interface::Close()
{
    m_impl->Close();
}

void P300Interface::SetNbTrials(const unsigned int nbTrials)
{   
    m_impl->SetNbTrials(nbTrials);
}

void P300Interface::SetFlashTime(const float flashTime)
{
    m_impl->SetFlashTime(flashTime);
}

void P300Interface::SetInterFlashTime(const float interFlashTime)
{
    m_impl->SetInterFlashTime(interFlashTime);
}

void P300Interface::SetInterCycleTime(const float interCycleTime)
{
    m_impl->SetInterCycleTime(interCycleTime);
}

} //namespace bcinterface

void has_P300Interface()
{}

