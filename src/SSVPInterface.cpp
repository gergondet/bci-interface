#include <ssvp-interface/SSVPInterface.h>
#include <ssvp-interface/BackgroundSprite.h>

#include <coshell-bci/CoshellBCI.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/thread.hpp>

namespace ssvpinterface
{

struct SSVPInterfaceImpl
{
    private:
        BackgroundSprite m_backgroundsprite;
        std::vector<FlickeringSquare *> m_squares;
        unsigned int m_width;
        unsigned int m_height;
        bool closeRequest;
        std::ofstream fpsLog;
        sf::RenderWindow * app;
        coshellbci::CoshellBCI * m_coshellBCI;
        bool m_coshellrunning;
    public:
        SSVPInterfaceImpl(unsigned int width, unsigned int height) : 
            m_backgroundsprite("hrp2010v", 4242),
            m_width(width), m_height(height), 
            closeRequest(false), 
            fpsLog("fps.log"), app(0),
            m_coshellBCI(new coshellbci::CoshellBCI("localhost", 2809, 1111)), 
            m_coshellrunning(false)
        {
            m_squares.resize(0);
        }

        ~SSVPInterfaceImpl()
        {
            for(unsigned int i = 0; i < m_squares.size(); ++i)
            {
                    delete m_squares[i];
            }
            delete app;
            delete m_coshellBCI;
        }

        void AddSquare(FlickeringSquare * square)
        {
            if(square)
            {
                m_squares.push_back(square);
            }
        }

        void AddSquare(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a)
        {
            FlickeringSquare * square = new FlickeringSquare(frequency, screenFrequency, x, y, size, r, g, b, a);
            AddSquare(square);
        }

        void ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency)
        {
            if(squareId > 0 && squareId <= m_squares.size())
            {
                if( m_squares[squareId-1] )
                    m_squares[squareId-1]->ChangeFrequency(frequency, screenFrequency);
            }
        }

        void EnableFlash(bool enable)
        {
            for(unsigned int i = 0; i < m_squares.size(); ++i)
            {
                m_squares[i]->SetSquareDisplay(enable);
            }
        }

        void DisplayLoop(bool fullScreen)
        {
            if(fullScreen)
            {
                app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "ssvp-interface", sf::Style::Fullscreen);
                app->ShowMouseCursor(false);
            }
            else
            {
                app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "ssvp-interface");
            }

            app->UseVerticalSync(true);

            unsigned int frameCount = 0;
            sf::Clock clock;

            m_backgroundsprite.Initialize();
            boost::thread th(boost::bind(&BackgroundSprite::UpdateLoop, &m_backgroundsprite));

            m_coshellBCI->Initialize();
            boost::thread * coshellTh = 0;

            while(!closeRequest && app->IsOpened())
            {
                unsigned int newFrameCount = (unsigned int)floor(clock.GetElapsedTime()*60);
                /* cheat when missin frame */
                frameCount = newFrameCount > frameCount +1?frameCount+1:newFrameCount;
                
                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->UpdateForNewFrame(frameCount);
                }
                app->Clear();

                sf::Event Event;
                while (app->GetEvent(Event))
                {
                    if (Event.Type == sf::Event::Closed)
                        app->Close();
                    if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::Escape || Event.Key.Code == sf::Key::Q ) )
                        app->Close();
                    if( not m_coshellrunning && Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
                    {
                        coshellTh = new boost::thread(boost::bind(&coshellbci::CoshellBCI::CommandLoop, m_coshellBCI));
                    }
                }
        
                sf::Sprite * sprite = m_backgroundsprite.GetSprite();
                if(sprite)
                {
                    sprite->Resize(m_width, m_height);
                    app->Draw(*sprite);
                }

                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    app->Draw(*(m_squares[i]->GetBlackShape()));
                    if(m_squares[i]->SquareDisplay())
                    {
                        app->Draw(*(m_squares[i]->GetShape()));
                    }
                }
        
                app->Display();

                if(frameCount % 60 == 0)
                {
                    fpsLog << 1/app->GetFrameTime() << " fps" << std::endl;
                }
        
            }
            fpsLog.close();
            m_backgroundsprite.Close();
            th.join();
            m_coshellBCI->Close();
            coshellTh->join();
            delete coshellTh;
            app->Close();
        }
        
        void Close()
        {
            closeRequest = true;
        }
};

SSVPInterface::SSVPInterface(unsigned int width, unsigned height) : m_impl(new SSVPInterfaceImpl(width, height))
{
}

void SSVPInterface::AddSquare(FlickeringSquare * square)
{
    m_impl->AddSquare(square);
}

void SSVPInterface::AddSquare(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a)
{
    m_impl->AddSquare(frequency, screenFrequency, x, y, size, r, g, b, a);
}

void SSVPInterface::ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency)
{
    m_impl->ChangeFrequency(squareId, frequency, screenFrequency);
}

void SSVPInterface::EnableFlash(bool enable)
{
    m_impl->EnableFlash(enable);
}

void SSVPInterface::DisplayLoop(bool fullScreen)
{
    m_impl->DisplayLoop(fullScreen);
}

void SSVPInterface::Close()
{
    m_impl->Close();
}

} // namespace ssvpinterface

