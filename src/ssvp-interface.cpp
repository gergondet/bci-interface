#include <ssvp-interface/FlickeringSquare.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>

using namespace ssvpinterface;

int main(int argc, char * argv[])
{
    int winW = 1600;
    int winH = 900;
    sf::RenderWindow app(sf::VideoMode(winW, winH), "ssvp-interface", sf::Style::Fullscreen);
    app.UseVerticalSync(true);
    app.SetFramerateLimit(60);
    sf::Clock clock;


    std::vector<FlickeringSquare *> squares;
    squares.push_back(new FlickeringSquare(5,60, winW/2-50, 50));
    squares.push_back(new FlickeringSquare(10,60, winW-150, winH/2-50));
    squares.push_back(new FlickeringSquare(15,60, winW/2-50, winH-150));
    squares.push_back(new FlickeringSquare(20,60, 50, winH/2-50));
    unsigned int frameCount = 0;

    clock.Reset();
    while (app.IsOpened())
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


        for(int i = 0; i < squares.size(); ++i)
        {
            app.Draw(*(squares[i]->GetShape()));
        }

        app.Display();

        frameCount++;
        for(int i = 0; i < squares.size(); ++i)
        {
            squares[i]->UpdateForNewFrame(frameCount);
        }
    }

    return 0;
}
