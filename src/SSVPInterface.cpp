#include <ssvp-interface/SSVPInterface.h>
#include <ssvp-interface/BackgroundSprite.h>

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
        bool crossEnable;
        std::ofstream fpsLog;
        sf::RenderWindow * app;
    public:
        SSVPInterfaceImpl(unsigned int width, unsigned int height) : 
            m_backgroundsprite("hrp2010v", 4242),
            m_width(width), m_height(height), 
            closeRequest(false), crossEnable(false), 
            fpsLog("fps.log"), app(0)
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
        }

        void AddSquare(FlickeringSquare * square)
        {
            if(square)
            {
                m_squares.push_back(square);
            }
        }

        void AddSquare(int frequency, int screenFrequency, float x, float y, ArrowPosition arrowPos, float size, int r, int g, int b, int a)
        {
            FlickeringSquare * square = new FlickeringSquare(frequency, screenFrequency, x, y, arrowPos, size, r, g, b, a);
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

        void EnableArrow(unsigned int squareId)
        {
            if(crossEnable)
            {
                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->SetArrowDisplay(true);
                }
            }
            else
            {
                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    if( squareId - 1  == i)
                    {
                        m_squares[i]->SetArrowDisplay(true);
                    }
                    else
                    {
                        m_squares[i]->SetArrowDisplay(false);
                    }
                }
            }
        }

        void EnableFlash(bool enable)
        {
            for(unsigned int i = 0; i < m_squares.size(); ++i)
            {
                m_squares[i]->SetSquareDisplay(enable);
            }
        }

        void EnableCross(bool enable)
        {
            crossEnable = enable;
            if(!crossEnable)
            {
                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->SetArrowDisplay(false);
                }
            }
            else
            {
                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->SetArrowDisplay(true);
                }
            }
        }

        void DisplayLoop(bool fullScreen)
        {
            if(fullScreen)
                app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "ssvp-interface", sf::Style::Fullscreen);
            else
                app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "ssvp-interface");

            app->UseVerticalSync(true);
            app->ShowMouseCursor(false);

            unsigned int frameCount = 0;
            sf::Clock clock;

            m_backgroundsprite.Initialize();
            boost::thread th(boost::bind(&BackgroundSprite::UpdateLoop, &m_backgroundsprite));

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
                }
        
                sf::Sprite * sprite = m_backgroundsprite.GetSprite();
                if(sprite)
                {
                    sprite->Resize(m_width, m_height);
                    app->Draw(*sprite);
                }

                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    if(m_squares[i]->SquareDisplay())
                    {
                        app->Draw(*(m_squares[i]->GetShape()));
                    }
                    if(m_squares[i]->ArrowDisplay())
                    {
                        app->Draw(*(m_squares[i]->GetArrow()));
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

void SSVPInterface::AddSquare(int frequency, int screenFrequency, float x, float y, ArrowPosition arrowPos, float size, int r, int g, int b, int a)
{
    m_impl->AddSquare(frequency, screenFrequency, x, y, arrowPos, size, r, g, b, a);
}

void SSVPInterface::ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency)
{
    m_impl->ChangeFrequency(squareId, frequency, screenFrequency);
}

void SSVPInterface::EnableArrow(unsigned int squareId)
{
    m_impl->EnableArrow(squareId);
}

void SSVPInterface::EnableFlash(bool enable)
{
    m_impl->EnableFlash(enable);
}

void SSVPInterface::EnableCross(bool enable)
{
    m_impl->EnableCross(enable);
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

