
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
#include <stdarg.h>
#include <ctime>

#include <boost/thread.hpp>

namespace bciinterface
{

struct SSVEPInterfaceImpl
{
    private:
        BackgroundSprite * m_backgroundsprite;
        bool m_updatebackgroundmanually;
        std::vector<FlickeringSquare *> m_squares;
        int indicePos;
        std::vector<float> m_positionsTabLeft;
        std::vector<float> m_positionsTabRight;

        //cursors
        std::vector<MoovingCursor *> m_cursors;
        float currentPos;
        sf::Shape currentCursor;
        int compt_begin;
        bool nextPosition, previousPosition;

        //points
        std::vector<RandomPoint *> m_points;
        std::vector<float> position;
        int compt_point;
        sf::Shape squareForbidden;
        bool allowedPosition;
        float distance;
		std::ofstream coordonnees;
		time_t t;

        unsigned int m_width;
        unsigned int m_height;
        bool closeRequest;
        std::ofstream fpsLog;
        sf::RenderWindow * app;
        #ifdef WITH_COSHELL
        coshellbci::CoshellBCI * m_coshellBCI;
        #endif
        bool m_coshellrunning;
        boost::thread * m_backgroundTh;
    public:
        SSVEPInterfaceImpl(unsigned int width, unsigned int height) : 
            m_backgroundsprite(0), m_updatebackgroundmanually(false),
            m_width(width), m_height(height), 
            closeRequest(false), 
            fpsLog("fps.log"), app(0), compt_begin(0), compt_point(0),
            indicePos(0), nextPosition(false), previousPosition(false),
            allowedPosition(false), distance(0), coordonnees("../donnees_test/coordonnees_1600_05_05_2010.log", std::ios::out | std::ios::app),
            #ifdef WITH_COSHELL
            m_coshellBCI(new coshellbci::CoshellBCI("192.168.140.2", 2809, 1111)), 
            #endif
            m_coshellrunning(false),
            m_backgroundTh(0)
        {
            m_squares.resize(0);
            position.resize(2);
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
            if(m_backgroundsprite)
            {
                m_backgroundsprite->Close();
                if(m_backgroundTh)
                {
                    m_backgroundTh->join();
                    delete m_backgroundTh;
                }
            }
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

        void AddPoint(RandomPoint * randomPoint)
        {
        	m_points.push_back(randomPoint);
        }

        void AddPoint(float x, float y, float size, float period, int r, int g, int b, int a)
        {
        	RandomPoint * randomPoint = new RandomPoint(x, y, size, period, r, g, b, a);
        	m_points.push_back(randomPoint);
        }

        void ClearPositionsTabs(){
        	m_positionsTabLeft.clear();
        	m_positionsTabRight.clear();
        }

        void AddPositionsTab(float positionX){
        	m_positionsTabLeft.push_back(positionX);
        }

        void AddPositionsTab(float posXSquareLeft, float posXSquareRight, float size, int numPos){
        	std::cout << "parametres : " << posXSquareLeft << ", " << posXSquareRight << ", " << size << ", " << numPos << std::endl;
        	float extend = posXSquareRight - (posXSquareLeft + size);
        	std::cout << "extend : " << extend << std::endl;
        	for(int i=0; i<numPos; i++){
        		m_positionsTabLeft.push_back(posXSquareLeft + (float)i/(2*(float)numPos)*extend);
        		m_positionsTabRight.push_back(posXSquareRight - (float)i/(2*(float)numPos)*extend);
        		std::cout << "posLeft : " << m_positionsTabLeft[i] << ", posRight : " << m_positionsTabRight[i] << std::endl;
        	}
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

        void SetCoshellCommands(const std::vector<std::string> & commands)
        {
            #ifdef WITH_COSHELL
            m_coshellBCI->SetCoshellCommands(commands);
            #endif
        }

        void SendCoshellCommand(int cmd)
        {
            #ifdef WITH_COSHELL
            m_coshellBCI->SendCommand((bcimw::SSVEP_COMMAND)cmd);
            #endif
        }

        void DisplayLoop(sf::RenderWindow * appin, unsigned int * cmd, float timeout)
        {
            if(!m_backgroundsprite)
            {
                std::cerr << "Call SetBackgroundSprite before launching interface display loop" << std::endl;
                return;
            }

            app = appin;
    
            DisplayLoop(cmd, timeout);

//            m_coshellrunning = false;

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

        void DisplayLoop(unsigned int * cmdOut = 0, float timeout = 0)
        {
            unsigned int frameCount = 0;
            sf::Clock clock;
            closeRequest = false;

            if( !m_updatebackgroundmanually && !m_backgroundTh )
            {
                m_backgroundsprite->Initialize();
                m_backgroundTh = new boost::thread(boost::bind(&BackgroundSprite::UpdateLoop, m_backgroundsprite));
            }

            if (!coordonnees) std::cerr << "Error opening" << std::endl;

            #ifdef WITH_COSHELL
            m_coshellBCI->Initialize();
            boost::thread * coshellTh = 0;
            if(cmdOut)
            {
                coshellTh = new boost::thread(boost::bind(&coshellbci::CoshellBCI::CommandLoop, m_coshellBCI, false));
            }
            else    
            {
                coshellTh = new boost::thread(boost::bind(&coshellbci::CoshellBCI::CommandLoop, m_coshellBCI, true));
            }
            #endif

            bcimw::SSVEP_COMMAND bcicmd = bcimw::NONE;

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
                    if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::P ) )
                        nextPosition = true;
                    if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::O ) )
                        previousPosition = true;
                    if( Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Space)
                    {
                        if(not m_coshellrunning)
                        {
                            m_coshellrunning = true;
                            #ifdef WITH_COSHELL
                            m_coshellBCI->ActiveCoshell();
                            #endif
                        }
                        else
                        {
                            #ifdef WITH_COSHELL
                            m_coshellBCI->Close();
                            /* do not set m_coshellrunning back to false because we cannot relaunch the walking for now */
                            #else
                            m_coshellrunning = false;
                            #endif
                        }
                    }
                }
        
                #ifdef WITH_COSHELL
                bcicmd = m_coshellBCI->GetCurrentCommand();
                if(clock.GetElapsedTime() < timeout)
                {
                    bcicmd = (bcimw::SSVEP_COMMAND)0; /* ignore command while in timeout period */
                }
                #endif

                sf::Sprite * sprite = m_backgroundsprite->GetSprite();
                if(sprite)
                {
                    sprite->Resize(m_width, m_height);
                    app->Draw(*sprite);
                }

				if(m_points.size() >= 1)
				{
					if (compt_point==0) {

						while (allowedPosition !=true){
							position[0] = ((double)rand()*(double)m_width/(double)RAND_MAX - m_points[0]->GetSize() );
							position[1] = ((double)rand()*(double)m_height/(double)RAND_MAX - m_points[0]->GetSize() );
							//position [1]= (m_points[0]->GetPosition()[1]);
							squareForbidden= *(m_squares[0]->GetShape());
							std::cout << position[0] << ", " << position[1] << std::endl;

						    std::time(&t);
							coordonnees << std::ctime(&t) << " " << position[0] << ", " << position[1] << std::endl;
							coordonnees << position[0] << ", " << position[1] << std::endl;

							distance =sqrt((position[0]- squareForbidden.GetCenter().x)*(position[0]- squareForbidden.GetCenter().x) + (position[1]- squareForbidden.GetCenter().y)*(position[1]- squareForbidden.GetCenter().y));
							if (distance > (m_squares[0]->GetRadius())) {
								allowedPosition = true;
								m_points[0]->SetPosition(position);
							}

						}
						//std::cout << "1 : " << compt_point << std::endl;
						app->Draw(*(m_points[0]->GetPoint()));
						compt_point++;

					}
					else if (compt_point <= m_points[0]->GetPeriod()){
						app->Draw(*(m_points[0]->GetPoint()));
						//std::cout << "2 : " <<  compt_point << std::endl;
						compt_point++;

					}
					else if ((m_points[0]->GetPeriod()+ 60) >= compt_point){
						//std::cout << "3 : " <<  compt_point << std::endl;
						compt_point++;

					}
					else if (compt_point > m_points[0]->GetPeriod()+ 60){
						//std::cout << "4 : " <<  compt_point << std::endl;
						compt_point=0;
						allowedPosition = false;
					}

				}

                for(unsigned int i = 0; i < m_squares.size(); ++i)
                {
                    if((unsigned int)bcicmd == i+1)
                    {
                        m_squares[i]->Highlight();
                        if(cmdOut && m_coshellrunning) { *cmdOut = (unsigned int)bcicmd; closeRequest = true; }
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


                app->Display();

                if(frameCount % 60 == 0)
                {
                    fpsLog << 1/app->GetFrameTime() << " fps" << std::endl;
                }
        
            }
            fpsLog.close();
            coordonnees.close();
            if(!m_updatebackgroundmanually && !cmdOut)
            {
                m_backgroundsprite->Close();
                if(m_backgroundTh)
                {
                    m_backgroundTh->join();
                    delete m_backgroundTh;
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

void SSVEPInterface::AddPoint(RandomPoint * randomPoint)
{
	m_impl->AddPoint(randomPoint);
}

void SSVEPInterface::AddPoint(float x, float y, float size, float period, int r, int g, int b, int a)
{
	m_impl->AddPoint(x, y, size, period, r, g, b, a);
}

void SSVEPInterface::ClearPositionsTabs(){
	 m_impl->ClearPositionsTabs();
}

void SSVEPInterface::AddPositionsTab(float positionX){
	 m_impl->AddPositionsTab(positionX);
}

void SSVEPInterface::AddPositionsTab(float posXSquareLeft, float posXSquareRight, float size, int numPos){
	 m_impl->AddPositionsTab(posXSquareLeft, posXSquareRight, size, numPos);
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

void SSVEPInterface::SetCoshellCommands(const std::vector<std::string> & commands)
{
    m_impl->SetCoshellCommands(commands);
}

void SSVEPInterface::SendCoshellCommand(int cmd)
{
    m_impl->SendCoshellCommand(cmd);
}

void SSVEPInterface::DisplayLoop(sf::RenderWindow * app, unsigned int * cmd, float timeout)
{
    m_impl->DisplayLoop(app, cmd, timeout);
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

