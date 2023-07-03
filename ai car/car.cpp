#include "car.h"

Car::Car()
{
	std::vector<int> aiLayers;
	for (int i = 0; i < nLayers; i++)
	{
		aiLayers.push_back(aiSize[i]);
	}
	ai = new Ai(aiLayers);

	car.setSize(sf::Vector2f(20, 40));
	car.setOrigin(car.getSize().x / 2, car.getSize().y / 2);
	car.setPosition(150, 600);
	car.setFillColor(sf::Color::White);
	car.setOutlineThickness(2);
	car.setOutlineColor(sf::Color::Black);
}

void Car::move()
{
	//pause
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		if (ablePause)
		{
			paused = !paused;
			ablePause = false;
		}
	}
	else
	{
		ablePause = true;
	}

	if (!paused && !crashed)
	{	
		//calculate movement
		velocity += acceleration;
		velocity = std::max(std::min(maxVelocity, velocity), 0.f);
		float rotation = std::min(maxRotation * velocity / maxVelocity * 1.5, maxRotation);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			car.rotate(rotation);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			car.rotate(-rotation);
		}

		//apply the calculated movement
		car.move(sf::Vector2f(sin(car.getRotation() * 3.1415 / 180) * velocity, -cos(car.getRotation() * 3.1415 / 180) * velocity));
	}
}

void Car::draw(sf::RenderWindow& window)
{	
	window.draw(car);

	for (int i = 0; i < 9; i++)
	{
		window.draw(sensors[i]);
	}
}

void Car::resetSensors()
{
	for (int i = 0; i < 9; i++)
	{
		sensors[i].setSize(sf::Vector2f(3000, 1));
		sensors[i].setPosition(car.getPosition());
		float ang = car.getRotation() - 10 - 20 * i;
		sensors[i].setRotation(ang);
	}
}

void Car::crash()
{
}

sf::RectangleShape& Car::getSensor(int n)
{
	return sensors[n];
}
