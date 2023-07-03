#include "game.h"

int main()
{
    sf::Vector2f windowDim = sf::Vector2f(1280, 720);
    const int maxFps = 100;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 5;

    sf::RenderWindow window;
    window.create(sf::VideoMode(windowDim.x, windowDim.y), "Game", sf::Style::Default, settings);
    window.setView(sf::View(sf::Vector2f(640, 360), windowDim));
    window.setFramerateLimit(maxFps);

    bool ableChangeMode = true;
    bool isFullscreen = false;

    Game game(window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {  
                window.close();
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                sf::View view(window.getView());
                view.zoom(1 - 0.1 * event.mouseWheelScroll.delta);
                window.setView(view);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            if (ableChangeMode)
            {
                sf::View view(window.getView());

                if (isFullscreen)
                {
                    window.create(sf::VideoMode(windowDim.x, windowDim.y), "Game", sf::Style::Default, settings);
                }
                else
                {
                    window.create(sf::VideoMode(), "Game", sf::Style::Fullscreen, settings);
                }

                window.setFramerateLimit(maxFps);
                window.setView(view);
                isFullscreen = !isFullscreen;
            }
            ableChangeMode = false;
        }
        else
        {
            ableChangeMode = true;
        }

        window.clear(sf::Color(130, 130, 130));

        game.play();

        window.display();
    }

	return 0;
}