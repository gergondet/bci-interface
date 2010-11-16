#include <ssvp-interface/FlickeringSquare.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>

using namespace ssvpinterface;

int main(int argc, char * argv[])
{
    sf::RenderWindow app(sf::VideoMode(800,600), "ssvp-interface");
    app.UseVerticalSync(true);
    sf::Clock clock;


    std::vector<FlickeringSquare *> squares;
    squares.push_back(new FlickeringSquare(5,60, 350, 50));
    squares.push_back(new FlickeringSquare(10,60, 650, 250));
    squares.push_back(new FlickeringSquare(15,60, 350, 450));
    squares.push_back(new FlickeringSquare(20,60, 50, 250));
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
