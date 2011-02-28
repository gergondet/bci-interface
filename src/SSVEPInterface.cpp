#include <bci-interface/SSVEPInterface.h>
#include <bci-interface/BackgroundSprite.h>

#ifdef WITH_COSHELL
#include <coshell-bci/CoshellBCI.h>
#endif
#include <bci-middleware/SSVEPReceiver.h>

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/thread.hpp>

namespace bciinterface
{

struct SSVEPInterfaceImpl
{
    private:
        BackgroundSprite * m_backgroundsprite;
        bool m_updatebackgroundmanually;
        std::vector<FlickeringSquare *> m_squares;
        std::vector<MoovingCursor *> m_cursors;
        float currentPos;
        sf::Shape currentCursor;
        int compt_begin;
        unsigned int m_width;
        unsigned int m_height;
        bool closeRequest;
        std::ofstream fpsLog;
        sf::RenderWindow * app;
        #ifdef WITH_COSHELL
        coshellbci::CoshellBCI * m_coshellBCI;
        bool m_coshellrunning;
        #endif
    public:
        SSVEPInterfaceImpl(unsigned int width, unsigned int height) : 
            m_backgroundsprite(0), m_updatebackgroundmanually(false),
            m_width(width), m_height(height), 
            closeRequest(false), 
            fpsLog("fps.log"), app(0), compt_begin(0)
            #ifdef WITH_COSHELL
            , m_coshellBCI(new coshellbci::CoshellBCI("hrp2010c", 2809, 1111)), 
            m_coshellrunning(false)
            #endif
        {
            m_squares.resize(0);
        }

        ~SSVEPInterfaceImpl()
        {
            for(unsigned int i = 0; i < m_squares.size(); ++i)
            {
                    delete m_squares[i];
            }
            delete app;
            #ifdef WITH_COSHELL
            delete m_coshellBCI;
            #endif
        }

        void AddSquare(FlickeringSquare * square)
        {
            if(square)
            {
                m_squares.push_back(square);
            }
        }

        void AddSquare(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a)
        {
            FlickeringSquare * square = new FlickeringSquare(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a);
            AddSquare(square);
        }

        void CleanUpSquares()
        {
            for(size_t i = 0; i < m_squares.size(); ++i)
            {
                delete m_squares[i];
            }
            m_squares.resize(0);
        }

        void AddCursor(MoovingCursor * moovingCursor)
        {
        	m_cursors.push_back(moovingCursor);
        }

        void AddCursor(float x_init, float y_init, float size, float x_fin, int r, int g, int b, int a)
        {
        	MoovingCursor * moovingCursor = new MoovingCursor(x_init, y_init, size, x_fin, r, g, b, a);
        	m_cursors.push_back(moovingCursor);
        }

        void ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency)
        {
            if(squareId > 0 && squareId <= m_squares.size())
            {
                if( m_squares[squareId-1] )
                    m_squares[squareId-1]->ChangeFrequency(frequency, screenFrequency);
            }
        }

        void SetBackgroundSprite(BackgroundSprite * sprite)
        {
            m_backgroundsprite = sprite;
        }

        void SetUpdateBackgroundManually(bool enable)
        {
            if(!app)
            {
                m_updatebackgroundmanually = enable;
            }
        }

        void UpdateBackground(unsigned char * img)
        {
            if(m_updatebackgroundmanually && m_backgroundsprite)
            {
                m_backgroundsprite->UpdateFromBuffer(img);
            }
        }

        void EnableFlash(bool enable)
        {
            for(unsigned int i = 0; i < m_squares.size(); ++i)
            {
                m_squares[i]->SetSquareDisplay(enable);
            }
        }

        void DisplayLoop(sf::RenderWindow * appin, unsigned int * cmd)
        {
            if(!m_backgroundsprite)
            {
                std::cerr << "Call SetBackgroundSprite before launching interface display loop" << std::endl;
                return;
            }

            app = appin;

            DisplayLoop(cmd);

            m_coshellrunning = false;

            app = 0;
        }

        void DisplayLoop(bool fullScreen)
        {
            if(!m_backgroundsprite)
            {
                std::cerr << "Call SetBackgroundSprite before launching interface display loop" << std::endl;
                return;
            }

            if(fullScreen)
            {
                app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface", sf::Style::Fullscreen);
                app->ShowMouseCursor(false);
            }
            else
            {
                app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "bci-interface");
            }

            app->UseVerticalSync(true);

            DisplayLoop();

            app->Close();
        }

        void DisplayLoop(unsigned int * cmdOut = 0)
        {
            unsigned int frameCount = 0;
            sf::Clock clock;
            closeRequest = false;

            boost::thread * th = 0;
            if(!m_updatebackgroundmanually)
            {
                m_backgroundsprite->Initialize();
                th = new boost::thread(boost::bind(&BackgroundSprite::UpdateLoop, m_backgroundsprite));
            }

            #ifdef WITH_COSHELL
            m_coshellBCI->Initialize();
            boost::thread * coshellTh = 0;
            #endif

            bcimw::SSVEP_COMMAND bcicmd = bcimw::NONE;
            coshellTh = new boost::thread(boost::bind(&coshellbci::CoshellBCI::CommandLoop, m_coshellBCI));

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
                    if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::S ) )
                        closeRequest = true;
                    #ifdef WITH_COSHELL
                    if( Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
                    {
                        if(not m_coshellrunning)
                        {
                            m_coshellrunning = true;
                            m_coshellBCI->ActiveCoshell();
                        }
                        else
                        {
                            m_coshellBCI->Close();
                            /* do not set m_coshellrunning back to false because we cannot relaunch the walking for now */
                        }
                    }
                    #endif
                }
        
                #ifdef WITH_COSHELL
                bcicmd = m_coshellBCI->GetCurrentCommand();
                #endif

                sf::Sprite * sprite = m_backgroundsprite->GetSprite();
                if(sprite)
                {
                    sprite->Resize(m_width, m_height);
                    app->Draw(*sprite);
                }

                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    if((unsigned int)bcicmd == i+1)
                    {
                        m_squares[i]->Highlight();
                        if(cmdOut) { *cmdOut = (unsigned int)bcicmd; closeRequest = true; }
                    }
                    else
                    {
                        m_squares[i]->Unhighlight();
                    }
                    app->Draw(*(m_squares[i]->GetBlackShape()));
                    if(m_squares[i]->SquareDisplay())
                    {
                        app->Draw(*(m_squares[i]->GetShape()));
                    }
                }


				if (m_cursors.size() !=0 && m_cursors[0]->CursorDisplay()){
					if (compt_begin >= 50)
					{
						currentCursor = *(m_cursors[0]->GetCursor());
						currentPos=currentCursor.GetPosition().x;
						if(currentPos < m_cursors[0]->GetfinalPosX())
						{
							m_cursors[0]->SetCursorX(currentPos+1);
						}
					}
					else
					{
						compt_begin++;
					}
					app->Draw(*(m_cursors[0]->GetCursor()));
				}




        
                app->Display();

                if(frameCount % 60 == 0)
                {
                    fpsLog << 1/app->GetFrameTime() << " fps" << std::endl;
                }
        
            }
            fpsLog.close();
            if(!m_updatebackgroundmanually)
            {
                m_backgroundsprite->Close();
                if(th)
                {
                    th->join();
                    delete th;
                }
            }
            #ifdef WITH_COSHELL
            m_coshellBCI->Close();
            if(coshellTh)
            {
                coshellTh->join();
                delete coshellTh;
            }
            #endif
        }
        
        void Close()
        {
            closeRequest = true;
        }
};

SSVEPInterface::SSVEPInterface(unsigned int width, unsigned height) : m_impl(new SSVEPInterfaceImpl(width, height))
{
}

void SSVEPInterface::AddSquare(FlickeringSquare * square)
{
    m_impl->AddSquare(square);
}

void SSVEPInterface::AddSquare(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a)
{
    m_impl->AddSquare(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a);
}

void SSVEPInterface::CleanUpSquares()
{
    m_impl->CleanUpSquares();
}

void SSVEPInterface::AddCursor(float x_init, float y_init, float size, float x_fin, int r, int g, int b, int a)
{
	m_impl->AddCursor(x_init, y_init, size, x_fin, r, g, b, a);
}

void SSVEPInterface::AddCursor(MoovingCursor * moovingCursor)
{
	m_impl->AddCursor(moovingCursor);
}

void SSVEPInterface::ChangeFrequency(unsigned int squareId, int frequency, int screenFrequency)
{
    m_impl->ChangeFrequency(squareId, frequency, screenFrequency);
}

void SSVEPInterface::SetBackgroundSprite(BackgroundSprite * sprite)
{
    m_impl->SetBackgroundSprite(sprite);
}

void SSVEPInterface::SetUpdateBackgroundManually(bool enable)
{
    m_impl->SetUpdateBackgroundManually(enable);
}

void SSVEPInterface::UpdateBackground(unsigned char * img)
{
    m_impl->UpdateBackground(img);
}

void SSVEPInterface::EnableFlash(bool enable)
{
    m_impl->EnableFlash(enable);
}

void SSVEPInterface::DisplayLoop(sf::RenderWindow * app, unsigned int * cmd)
{
    m_impl->DisplayLoop(app, cmd);
}

void SSVEPInterface::DisplayLoop(bool fullScreen)
{
    m_impl->DisplayLoop(fullScreen);
}

void SSVEPInterface::Close()
{
    m_impl->Close();
}

} // namespace bciinterface

