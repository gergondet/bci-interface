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
        bool running;
    public:
        SSVPInterfaceImpl(unsigned int width, unsigned int height) : m_width(width), m_height(height), running(false)
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
        }

        void AddSquare(FlickeringSquare * square)
        {
            if(square)
                m_squares.push_back(square);
        }

        void AddSquare(int frequency, int screenFrequency, float x, float y, float size, int r, int g, int b, int a)
        {
            FlickeringSquare * square = new FlickeringSquare(frequency, screenFrequency, x, y, size, r, g, b, a);
            AddSquare(square);
        }

        void DisplayLoop()
        {
            sf::RenderWindow app(sf::VideoMode(m_width, m_height), "ssvp-interface", sf::Style::Fullscreen);
            app.UseVerticalSync(true);
            app.SetFramerateLimit(60);
    
            unsigned int frameCount = 0;

            while(app.IsOpened())
            {
                app.Clear();

                sf::Event Event;
                while (app.GetEvent(Event))
                {
                    // Close window : exit
                    if (Event.Type == sf::Event::Closed)
                        app.Close();
                    if( Event.Type == sf::Event::KeyPressed && ( Event.Key.Code == sf::Key::Escape || Event.Key.Code == sf::Key::Q ) )
                        app.Close();
                }
        
        
                for(int i = 0; i < m_squares.size(); ++i)
                {
                    app.Draw(*(m_squares[i]->GetShape()));
                }
        
                app.Display();
        
                frameCount++;
                for(int i = 0; i < m_squares.size(); ++i)
                {
                    m_squares[i]->UpdateForNewFrame(frameCount);
                }
        
            }
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

void SSVPInterface::DisplayLoop()
{
    m_impl->DisplayLoop();
}

} // namespace ssvpinterface

