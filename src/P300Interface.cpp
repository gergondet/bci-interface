#include "bci-interface/P300Interface.h"

#include "bci-interface/BackgroundSprite.h"

#include <boost/thread.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>

#include <bci-middleware/P300Client.h>

namespace bciinterface
{

std::ofstream debug("debug.log");

struct NamedShape
{
    NamedShape() : name(""), shape(0) {}
    NamedShape(const std::string & name, sf::Shape * shape) : name(name), shape(shape) {}
    std::string name;
    sf::Shape * shape;
};

struct P300InterfaceImpl
{

private:
    BackgroundSprite m_backgroundSprite;
    unsigned int m_width, m_height;
    bool m_pausable, m_pause, m_close;
    unsigned int m_nbtrials;
    float m_flashtime, m_interflashtime, m_intercycletime;
    unsigned int m_nbObjects;
    std::vector<NamedShape> m_objectsActive;
    std::vector<NamedShape> m_objectsInactive;
    bool m_training;
    std::vector<int> m_trainingset;
    bcimw::P300Client * m_p300client;
    sf::RenderWindow * m_app;
public:
    P300InterfaceImpl(unsigned int width, unsigned int height, unsigned int mode) :
        m_backgroundSprite("hrp2010v", 4242),
        m_width(width), m_height(height), m_pausable(true), m_pause(false), m_close(false),
        m_nbtrials(4), m_flashtime(0.060), m_interflashtime(0.010), m_intercycletime(1.0),
        m_nbObjects(36) , m_training(false) , m_p300client(0),
        m_app(0)
    {
        if(mode == 1)
        {
            m_training = true;
            for(int i = 0; i < 4; ++i)
            {
                m_trainingset.push_back(i+1);
            }
        }
        m_objectsActive.resize(0);
        m_objectsInactive.resize(0);
    }
    ~P300InterfaceImpl()
    {
        for(size_t i = 0; i < m_objectsActive.size(); ++i)
        {
            delete m_objectsActive[i].shape;
            delete m_objectsInactive[i].shape;
        }
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
        for(size_t i = 0; i < m_objectsActive.size(); ++i)
        {
            if(m_objectsActive[i].name == object->name)
            {
                    std::cerr << "Cannot add object with name " << object->name << ", object already exists" << std::endl;
                    Resume();
                    return;
            }
        }
        sf::Shape * shapeActive = new sf::Shape();
        sf::Shape * shapeInactive = new sf::Shape();
        sf::Color colorActive(object->r, object->g, object->b, 255);
        sf::Color colorInactive(object->r, object->g, object->b, 120);

        shapeActive->AddPoint(object->x, object->y, colorActive);
        shapeActive->AddPoint(object->x+object->size_x, object->y, colorActive);
        shapeActive->AddPoint(object->x+object->size_x, object->y+object->size_y, colorActive);
        shapeActive->AddPoint(object->x, object->y+object->size_y, colorActive);

        shapeInactive->AddPoint(object->x, object->y, colorInactive);
        shapeInactive->AddPoint(object->x+object->size_x, object->y, colorInactive);
        shapeInactive->AddPoint(object->x+object->size_x, object->y+object->size_y, colorInactive);
        shapeInactive->AddPoint(object->x, object->y+object->size_y, colorInactive);

        m_objectsActive.push_back(NamedShape(object->name, shapeActive));
        m_objectsInactive.push_back(NamedShape(object->name, shapeInactive));

        /* m_nbObjects++; */

        Resume();
    }
    void RemoveObject(const std::string & name)
    {
        Pause();
        {
            for(std::vector<NamedShape>::iterator it = m_objectsActive.begin(); it != m_objectsActive.end(); ++it)
            {
                if((*it).name == name)
                {
                    delete (*it).shape;
                    m_objectsActive.erase(it);
                }
            }
        }
        {
            for(std::vector<NamedShape>::iterator it = m_objectsInactive.begin(); it != m_objectsInactive.end(); ++it)
            {
                if((*it).name == name)
                {
                    delete (*it).shape;
                    m_objectsInactive.erase(it);
                }
            }
        }
        /* m_nbObjects--; */
        Resume();
    }
    void ClearObjects()
    {
        Pause();
        for(size_t i = 0; i < m_objectsInactive.size(); ++i)
        {
            delete m_objectsInactive[i].shape;
            delete m_objectsActive[i].shape;
        }
        m_objectsInactive.resize(0);
        m_objectsActive.resize(0);
        /* m_nbObjects = 0; */
        Resume();
    }

    void StartP300Client(const std::string & serverName, unsigned short serverPort)
    {
        delete m_p300client;
        m_p300client = new bcimw::P300Client(serverName, serverPort);
    }

    void DisplayLoop(bool fullscreen)
    {
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

        sf::Clock clock;
        time_t t1,t2;
        unsigned int frameCount = 0;

        m_backgroundSprite.Initialize();
        boost::thread th(boost::bind(&BackgroundSprite::UpdateLoop, &m_backgroundSprite));

        time(&t1);
        while(!m_close && m_app->IsOpened())
        {
            if(m_training)
            {
                m_pausable = false;
                for(int i = 0; i < m_trainingset.size(); ++i)
                { 
                    clock.Reset();
                    while(clock.GetElapsedTime() < m_intercycletime)
                    {
                            frameCount++;
                            Display(m_trainingset[i] - 1);
                    }
                    while(clock.GetElapsedTime() < 2*m_intercycletime)
                    {
                            frameCount++;
                            Display();
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
                            Display();
                        }
                    }

                    unsigned int cmd = m_p300client->GetID();
                    std::cerr << "Got cmd " << cmd << std::endl;

                    clock.Reset();
                    while(clock.GetElapsedTime() < m_intercycletime)
                    {
                            frameCount++;
                            NoDisplay();
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
                    Display();
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
                            Display();
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
        m_backgroundSprite.Close();
        th.join();
        m_app->Close();
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
        }
    }

    inline void DrawBackground()
    {
        sf::Sprite * sprite = m_backgroundSprite.GetSprite();

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

    inline void Display()
    {
        m_app->Clear();
        
        ProcessEvents();
        
        DrawBackground();
        
        for(size_t i = 0; i < m_objectsInactive.size(); ++i)
        {
            m_app->Draw(*(m_objectsInactive[i].shape));
        }

        m_app->Display();
    }
    inline void Display(unsigned int activeObject)
    {
        m_app->Clear();
        
        ProcessEvents();
        
        DrawBackground();
        
        for(size_t i = 0; i < m_objectsInactive.size(); ++i)
        {
            if(i != activeObject)
            {
                m_app->Draw(*(m_objectsInactive[i].shape));
            }
            else
            {
                m_app->Draw(*(m_objectsActive[i].shape));
            }
        }

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

