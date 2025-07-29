#include "game.h"

int main()
{
	sf::RenderWindow window;
	Game game(window);

	while (window.isOpen())
	{
		//handle events
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
				view.zoom(float(1 - 0.1 * event.mouseWheelScroll.delta));
				window.setView(view);
			}
		}

		game.frame();
	}

	return 0;
}