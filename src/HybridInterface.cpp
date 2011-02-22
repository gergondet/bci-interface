#include <bci-interface/HybridInterface.h>
#include <iostream>

namespace bciinterface
{

struct HybridInterfaceImpl : public SSVEPInterface, public P300Interface
{
private:
    unsigned int m_paradigm; // 0 : SSVEP, 1 : P300
    unsigned int m_width, m_height;
    BackgroundSprite * m_backgroundsprite;
    sf::RenderWindow * m_app;
public:
    HybridInterfaceImpl(unsigned int width, unsigned int height, unsigned int mode) : 
        SSVEPInterface(width, height) , P300Interface(width, height, mode),
        m_paradigm(0), 
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

        while(m_app->IsOpened())
        {
            unsigned int  * cmdSSVEP = new unsigned int(100);
            unsigned int  * cmdP300 = new unsigned int(200);
            while(*cmdP300 != *cmdSSVEP)
            {
               SSVEPInterface::AddSquare(7, 60, 125, 5, 390, 255, 0, 0, 255); 
               SSVEPInterface::AddSquare(12, 60, 765, 5, 390, 255, 0, 0, 255); 
               SSVEPInterface::AddSquare(5, 60, 125, 405, 390, 255, 0, 0, 255); 
               SSVEPInterface::AddSquare(9, 60, 765, 405, 390, 255, 0, 0, 255); 
               SSVEPInterface::DisplayLoop(m_app, cmd);
               std::cout << "First iteration of SSVEPInterface got cmd " << *cmd << std::endl;
               SSVEPInterface::CleanUpSquares();

               unsigned int x0, y0;
               switch(*cmd)
               {
                   case 1:
                       x0 = 65; y0 = 5; break;
                   case 2:
                       x0 = 705; y0 = 5; break;
                   case 3:
                       x0 = 65; y0 = 405; break;
                   case 4:
                       x0 = 705; y0 = 405; break;
                   default:
                       x0 = 65; y0 = 5; break;
               }

               SSVEPInterface::AddSquare(7, 60, x0, y0, 190, 255, 0, 0, 255); 
               SSVEPInterface::AddSquare(12, 60, 320+x0, y0, 190, 255, 0, 0, 255); 
               SSVEPInterface::AddSquare(5, 60, x0, 200+y0, 190, 255, 0, 0, 255); 
               SSVEPInterface::AddSquare(9, 60, 320+x0, 200+y0, 190, 255, 0, 0, 255); 
               SSVEPInterface::DisplayLoop(m_app, cmd);
               std::cout << "Second iteration of SSVEPInterface got cmd " << *cmd << std::endl;
               SSVEPInterface::CleanUpSquares();

               P300Interface::AddObject(new P300Object("7" , x0, y0, 190, 190, 255, 0, 0)); 
               P300Interface::AddObject(new P300Object("12", 320+x0, y0, 190, 190, 255, 0, 0)); 
               P300Interface::AddObject(new P300Object("5" , x0, 200+y0, 190, 190, 255, 0, 0)); 
               P300Interface::AddObject(new P300Object("9" , 320+x0, 200+y0, 190, 190, 255, 0, 0)); 
               P300Interface::DisplayLoop(m_app, cmd);
               std::cout << "P300 got cmd " << *cmd << std::endl;
               P300Interface::ClearObjects();
            }   

            delete cmdSSVEP;
            delete cmdP300;
            m_app->Close();
        }

        m_app->Close();
    }

private:
    inline void SwitchParadigm()
    {
        std::cout << "Switching paradigm!" << std::endl;
        if(m_paradigm == 0)
        {
            m_paradigm = 1;
        }
        else
        {
            m_paradigm = 0;
        }
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

void HybridInterface::AddSquare(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a)
{
    m_impl->AddSquare(frequency, screenFrequency, x, y, size, r, g, b, a);
}

} // namespace bciinterface

