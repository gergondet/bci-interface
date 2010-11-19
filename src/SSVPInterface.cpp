#include <ssvp-interface/SSVPInterface.h>

#include <SFML/Graphics.hpp>

namespace ssvpinterface
{

struct SSVPInterfaceImpl
{
    private:
        std::vector<FlickeringSquare *> m_squares;
        unsigned int m_width;
        unsigned int m_height;
        bool closeRequest;
        bool crossEnable;
        sf::RenderWindow * app;
    public:
        SSVPInterfaceImpl(unsigned int width, unsigned int height) : m_width(width), m_height(height), closeRequest(false), crossEnable(false), app(0)
        {
            m_squares.resize(0);
        }

        ~SSVPInterfaceImpl()
        {
            for(int i = 0; i < m_squares.size(); ++i)
            {
                if(m_squares[i])
                {
                    delete m_squares[i];
                }
            }
            if(app)
            {
                delete app;
            }
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
                for(int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->SetArrowDisplay(true);
                }
            }
            else
            {
                for(int i = 0; i < m_squares.size(); ++i)
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
            for(int i = 0; i < m_squares.size(); ++i)
            {
                m_squares[i]->SetSquareDisplay(enable);
            }
        }

        void EnableCross(bool enable)
        {
            crossEnable = enable;
            if(!crossEnable)
            {
                for(int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->SetArrowDisplay(false);
                }
            }
            else
            {
                for(int i = 0; i < m_squares.size(); ++i)
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
            app->SetFramerateLimit(60);
    
            unsigned int frameCount = 0;

            while(!closeRequest && app->IsOpened())
            {
                app->Clear();

                sf::Event Event;
                while (app->GetEvent(Event))
                {
                    // Close window : exit
                    if (Event.Type == sf::Event::Closed)
                        app->Close();
                    if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::Escape || Event.Key.Code == sf::Key::Q ) )
                        app->Close();
                }
        
        
                for(int i = 0; i < m_squares.size(); ++i)
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
        
                frameCount++;
                for(int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->UpdateForNewFrame(frameCount);
                }
            }
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

