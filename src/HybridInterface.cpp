#include <bci-interface/HybridInterface.h>
#include <bci-middleware/SSVEPReceiver.h>
#include <iostream>
#include <sstream>

namespace bciinterface
{

struct HybridInterfaceImpl : public SSVEPInterface, public P300Interface
{
private:
    unsigned int m_width, m_height;
    BackgroundSprite * m_backgroundsprite;
    sf::RenderWindow * m_app;
public:
    HybridInterfaceImpl(unsigned int width, unsigned int height, unsigned int mode) : 
        SSVEPInterface(width, height) , P300Interface(width, height, mode),
        m_width(width), m_height(height),
        m_backgroundsprite(0), m_app(0)
    {
        StartP300Client("150.29.145.150", 4242);
        m_backgroundsprite = new BackgroundSprite("hrp2010v", 4242, 640, 480);
        SSVEPInterface::SetBackgroundSprite(m_backgroundsprite);
        P300Interface::SetBackgroundSprite(m_backgroundsprite);
        if(mode == 1)
        {
            P300Interface::SetNbTrials(10);
        }
        else
        {
            P300Interface::SetNbTrials(4);
        }
    }

    ~HybridInterfaceImpl()
    {
        delete m_backgroundsprite;
        delete m_app;
    }

    void DisplayLoop(bool fullscreen)
    {
        if(fullscreen)
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "hybrid-interface", sf::Style::Fullscreen);
            m_app->ShowMouseCursor(false);
        }
        else
        {
            m_app = new sf::RenderWindow(sf::VideoMode(m_width, m_height), "hybrid-interface");
        }

        float timeout = 2.5;
        int x = 2;
        int y = 2;
        unsigned int * cmdSSVEP = new unsigned int(42);

        while(m_app->IsOpened())
        {
            SSVEPInterface::SendCoshellCommand((bcimw::SSVEP_COMMAND)0);
            unsigned int zeroFrequency = 0;
            unsigned int zeroCommand = 0;
            if( y != 0 )
            {
                /* up */
                SSVEPInterface::AddSquare(new FlickeringSquare(7,60, (2*x+1)*m_width/10-75, (2*y-1)*m_height/10-75, 150, 150, 255, 0, 0, 128, true));
            }
            else { zeroFrequency = 7; zeroCommand = 1; AddZeroSquare(zeroFrequency); }
            if( x != 4 )
            {
                /* right */
                SSVEPInterface::AddSquare(new FlickeringSquare(12,60,(2*x+3)*m_width/10-75, (2*y+1)*m_height/10-75, 150, 150, 255, 0, 0, 128, true));
            }
            else { if(zeroFrequency == 0) { zeroFrequency = 12; zeroCommand = 2; AddZeroSquare(zeroFrequency); } }
            if( y != 4 )
            {
                /* down */
                SSVEPInterface::AddSquare(new FlickeringSquare(5,60, (2*x+1)*m_width/10-75, (2*y+3)*m_height/10-75, 150, 150, 255, 0, 0, 128, true));
            }
            else { if(zeroFrequency == 0) { zeroFrequency = 5; zeroCommand = 3; AddZeroSquare(zeroFrequency); } }
            if( x != 0 )
            {
                /* left */
                SSVEPInterface::AddSquare(new FlickeringSquare(9,60, (2*x-1)*m_width/10-75, (2*y+1)*m_height/10-75, 150, 150, 255, 0, 0, 128, true));
            }
            else { if(zeroFrequency == 0) { zeroFrequency = 9; zeroCommand = 4; AddZeroSquare(zeroFrequency); } }
            SSVEPInterface::DisplayLoop(m_app, cmdSSVEP, timeout);
            switch(*cmdSSVEP)
            {
                case 1:
                    if(y > 0) { y -= 1; }
                    break;
                case 2:
                    if(x < 4) { x += 1; }
                    break;
                case 3:
                    if(y < 4) { y += 1; }
                    break;
                case 4:
                    if(x > 0) { x -= 1; }
                    break;
                default:
                    break;
            } 
            if(zeroCommand != 0 && *cmdSSVEP == zeroCommand )
            {
                x = 2;
                y = 2;
            }

            std::vector<std::string> newCommands;
            newCommands.resize(5);
            newCommands[0] = GenerateCommand(x, y);
            if( y == 0 ) { newCommands[1] = GenerateCommand(2, 2); }
            else { newCommands[1] = GenerateCommand(x, y - 1); }
            if( x == 4 ) { newCommands[2] = GenerateCommand(2, 2); }
            else { newCommands[2] = GenerateCommand(x+1, y); }
            if( y == 4 ) { newCommands[3] = GenerateCommand(2, 2); }
            else { newCommands[3] = GenerateCommand(x, y+1); }
            if( x == 0 ) { newCommands[4] = GenerateCommand(2, 2); }
            else { newCommands[4] = GenerateCommand(x-1, y); } 
            SSVEPInterface::SetCoshellCommands(newCommands);

            SSVEPInterface::CleanUpSquares();
        }

//        while(m_app->IsOpened())
//        {
//            unsigned int  * cmdSSVEP1 = new unsigned int(100);
//            unsigned int  * cmdSSVEP2 = new unsigned int(101);
//            unsigned int  * cmdSSVEP3 = new unsigned int(102);
//            unsigned int  * cmdP300 = new unsigned int(200);
//            while(1 || *cmdP300 != *cmdSSVEP3)
//            {
//               SSVEPInterface::AddSquare(7, 60, 0, 0, 640, 400, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(12, 60, 640, 0, 640, 400, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(5, 60, 640, 400, 640, 400, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(9, 60, 0, 400, 640, 400, 255, 0, 0, 128); 
//               SSVEPInterface::DisplayLoop(m_app, cmdSSVEP1);
//               std::cout << "First iteration of SSVEPInterface got cmd " << *cmdSSVEP1 << std::endl;
//               SSVEPInterface::CleanUpSquares();
//
//               unsigned int x0, y0;
//               switch(*cmdSSVEP1)
//               {
//                   case 1:
//                       x0 = 0; y0 = 0; break;
//                   case 2:
//                       x0 = 640; y0 = 0; break;
//                   case 3:
//                       x0 = 640; y0 = 400; break;
//                   case 4:
//                       x0 = 0; y0 = 400; break;
//                   default:
//                       x0 = 0; y0 = 0; break;
//               }
//
//               SSVEPInterface::AddSquare(7, 60, x0, y0, 320, 200, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(12, 60, 320+x0, y0, 320, 200, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(5, 60, 320+x0, 200+y0, 320, 200, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(9, 60, x0, 200+y0, 320, 200, 255, 0, 0, 128); 
//               SSVEPInterface::DisplayLoop(m_app, cmdSSVEP2);
//               std::cout << "Second iteration of SSVEPInterface got cmd " << *cmdSSVEP2 << std::endl;
//               SSVEPInterface::CleanUpSquares();
//
//               unsigned int x1, y1;
//                switch(*cmdSSVEP2)
//                {
//                    case 1:
//                        x1 = x0; y1 = y0; break;
//                    case 2:
//                        x1 = 320+x0; y1 = y0; break;
//                    case 3:
//                        x1 = 320+x0; y1 = 200+y0; break;
//                    case 4:
//                        x1 = x0; y1 = 200+y0; break;
//                    default:
//                        x1 = x0; y1 = y0; break;
//                }
//
//               SSVEPInterface::AddSquare(7, 60, x1, y1, 160, 100, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(12, 60, 160+x1, y1, 160, 100, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(5, 60, 160+x1, 100+y1, 160, 100, 255, 0, 0, 128); 
//               SSVEPInterface::AddSquare(9, 60, x1, 100+y1, 160, 100, 255, 0, 0, 128); 
//               SSVEPInterface::DisplayLoop(m_app, cmdSSVEP3);
//               std::cout << "Third iteration of SSVEPInterface got cmd " << *cmdSSVEP3 << std::endl;
//               SSVEPInterface::CleanUpSquares();
//
//               std::stringstream tmp;
//               tmp << "x";
//               P300Interface::AddObject(new P300Object(tmp.str() , x1, y1, 160, 100, 255, 0, 0)); 
//               tmp << "x";
//               P300Interface::AddObject(new P300Object(tmp.str(), 160+x1, y1, 160, 100, 255, 0, 0)); 
//               tmp << "x";
//               P300Interface::AddObject(new P300Object(tmp.str() , 160+x1, 100+y1, 160, 100, 255, 0, 0)); 
//               tmp << "x";
//               P300Interface::AddObject(new P300Object(tmp.str() , x1, 100+y1, 160, 100, 255, 0, 0)); 
//               unsigned int i = 1;
//               for(unsigned int i = 1; i < 5; ++i)
//               {
//                    switch(i)
//                    {
//                        case 1:
//                            x0 = 0; y0 = 0; break;
//                        case 2:
//                            x0 = 640; y0 = 0; break;
//                        case 3:
//                            x0 = 640; y0 = 400; break;
//                        case 4:
//                            x0 = 0; y0 = 400; break;
//                        default:
//                            x0 = 0; y0 = 0; break;
//                    }
//                    for(unsigned int j = 1; j < 5; ++j)
//                    {
//                        switch(j)
//                        {
//                            case 1:
//                                x1 = x0; y1 = y0; break;
//                            case 2:
//                                x1 = 320+x0; y1 = y0; break;
//                            case 3:
//                                x1 = 320+x0; y1 = 200+y0; break;
//                            case 4:
//                                x1 = x0; y1 = 200+y0; break;
//                            default:
//                                x1 = x0; y1 = y0; break;
//                        }
//                        if( ! (i == *cmdSSVEP2 && j == *cmdSSVEP3) )
//                        {
//                            tmp << "x";
//                            P300Interface::AddObject(new P300Object(tmp.str() , x0, y0, 320, 200, 255, 0, 0)); 
//                            tmp << "x";
//                            P300Interface::AddObject(new P300Object(tmp.str(), 320+x0, y0, 320, 200, 255, 0, 0)); 
//                            tmp << "x";
//                            P300Interface::AddObject(new P300Object(tmp.str() , 320+x0, 200+y0, 320, 200, 255, 0, 0)); 
//                            tmp << "x";
//                            P300Interface::AddObject(new P300Object(tmp.str() , x0, 200+y0, 320, 200, 255, 0, 0)); 
//                        }
//                    }
//               }
//               
//               P300Interface::DisplayLoop(m_app, cmdP300);
//               std::cout << "P300 got cmd " << *cmdP300 << std::endl;
//               P300Interface::ClearObjects();
//
//            }   
//
//            delete cmdSSVEP1;
//            delete cmdSSVEP2;
//            delete cmdSSVEP3;
//            delete cmdP300;
//            m_app->Close();
//        }
//
//        m_app->Close();
    }

private:
    std::string GenerateCommand(int x, int y)
    {
        std::stringstream command;
        if(y == 2)
        {
            command << "set pg.velocitydes [3](0.0001,0.0,";
        }
        else
        {
            command << "set pg.velocitydes [3](" << (2-y)*0.10 << ",0.0,";
        }
        command << (2-x)*0.10<< ")";
        return command.str();
    }

    inline void AddZeroSquare(unsigned int zeroFrequency)
    {
        /* center square to go back to zero speed quickly */
        SSVEPInterface::AddSquare(new FlickeringSquare(zeroFrequency,60, m_width/2-75, m_height/2-75, 150, 150, 255, 0, 0, 128, true));
    }

};

HybridInterface::HybridInterface(unsigned int width, unsigned int height, unsigned int mode) :
    m_impl(new HybridInterfaceImpl(width, height, mode))
{
}

void HybridInterface::DisplayLoop(bool fullscreen)
{
    m_impl->DisplayLoop(fullscreen);
}

void HybridInterface::AddObject(P300Object * object)
{
    m_impl->AddObject(object);
}

void HybridInterface::RemoveObject(const std::string & name)
{
    m_impl->RemoveObject(name);
}

void HybridInterface::ClearObjects()
{
    m_impl->ClearObjects();
}

void HybridInterface::AddSquare(FlickeringSquare * square)
{
    m_impl->AddSquare(square);
}

void HybridInterface::AddSquare(int frequency, int screenFrequency, float x, float y, float size_x, float size_y, int r, int g, int b, int a)
{
    m_impl->AddSquare(frequency, screenFrequency, x, y, size_x, size_y, r, g, b, a);
}

} // namespace bciinterface

