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
	car.setFillColor(sf::Color(Layer::random(0, 200), Layer::random(0, 200), Layer::random(0, 200)));
	car.setOutlineThickness(-2);
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
		//window.draw(sensors[i]);
	}
}

bool Car::collide(std::vector<sf::VertexArray> walls)
{
	for (int nSide = 0; nSide < 4; nSide++)
	{	
		sf::VertexArray side;
		side.resize(2);
		side[0].position = vertexRect(car, nSide);
		side[1].position = vertexRect(car, nSide + 1);

		//calculate side info
		float coefSide = -(side[0].position.y - side[1].position.y) / (side[0].position.x - side[1].position.x);
		float dislocSide = side[0].position.y + coefSide * side[0].position.x;

		for (int wall = 0; wall < walls.size(); wall++)
		{
			//calculate wall info
			float coefWall = -(walls[wall][0].position.y - walls[wall][1].position.y) / (walls[wall][0].position.x - walls[wall][1].position.x);
			float dislocWall = walls[wall][0].position.y + coefWall * walls[wall][0].position.x;

			float xInter = (dislocSide - dislocWall) / (coefSide - coefWall);
			float yInter = xInter * coefSide - dislocSide;

			if (side.getBounds().contains(xInter, -yInter) && walls[wall].getBounds().contains(xInter, -yInter))
			{
				car.setFillColor(sf::Color::Black);
				return true;
			}
		}
	}
	
	return false;
}

//n: 0 = top-left; 1 = top-right; 2 = bot-right; 3 = bot-left
sf::Vector2f Car::vertexRect(sf::RectangleShape rect, int n)
{
	while (n > 3)
	{
		n = n - 4;
	}
	float incVertices[4];

	float ipot = sqrt(std::pow(rect.getSize().x, 2) + std::pow(rect.getSize().y, 2));
	float diagInc = acos(rect.getSize().x / ipot) * 180 / 3.1415;

	incVertices[0] = 360 - rect.getRotation() + diagInc;
	incVertices[1] = 360 - rect.getRotation() - diagInc;
	incVertices[2] = 360 - rect.getRotation() + 180 + diagInc;
	incVertices[3] = 360 - rect.getRotation() + 180 - diagInc;

	return (rect.getPosition() + sf::Vector2f(cos(incVertices[n] * 3.1415 / 180) * ipot / 2, -sin(incVertices[n] * 3.1415 / 180) * ipot / 2));
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

void Car::updateSensors(std::vector<sf::VertexArray> walls)
{
	for (int sensor = 0; sensor < 9; sensor++)
	{			
		//calculate sensor info
		float angSens = 360 - sensors[sensor].getRotation();
		float coefSens = tan(angSens * 3.1415 / 180);	
		float dislocSens = sensors[sensor].getPosition().y + coefSens * sensors[sensor].getPosition().x;

		for (int wall = 0; wall < walls.size(); wall++)
		{
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