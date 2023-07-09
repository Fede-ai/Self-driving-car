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

void Car::moveCar()
{
	//apply rotation
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

void Car::drawCar(sf::RenderWindow& window)
{	
	window.draw(car);

	for (int i = 0; i < 9; i++)
	{
		window.draw(sensors[i]);
	}
}

void Car::collide(std::vector<Wall> walls)
{

}

void Car::crash()
{

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

void Car::updateSensors(std::vector<Wall> walls)
{
	for (int sensor = 0; sensor < 9; sensor++)
	{
		for (int wall = 0; wall < walls.size(); wall++)
		{
			if (!walls[wall].isEditing)
			{
				//calculate car info
				float angSens = 360 - sensors[sensor].getRotation();
				float coefSens = tan(angSens * 3.1415 / 180);
				float dislocSens = sensors[sensor].getPosition().y + coefSens * sensors[sensor].getPosition().x;

				//calculate wall info
				float coefWall = -(walls[wall][0].position.y - walls[wall][1].position.y) / (walls[wall][0].position.x - walls[wall][1].position.x);
				float dislocWall = walls[wall][0].position.y + coefWall * walls[wall][0].position.x;
				float xInter = (dislocSens - dislocWall) / (coefSens - coefWall);
				float yInter = xInter * coefSens - dislocSens;

				if (sensors[sensor].getGlobalBounds().contains(xInter, -yInter) && walls[wall].getBounds().contains(xInter, -yInter))
				{
					float xDist = sensors[sensor].getPosition().x - xInter;
					float yDist = sensors[sensor].getPosition().y + yInter;
					float dist = sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
					sensors[sensor].setSize(sf::Vector2f(std::min(dist, sensors[sensor].getSize().x), 1));
				}
			}
		}
	}
}