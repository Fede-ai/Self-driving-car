#include "car.h"
#include <iostream>

Car::Car(std::vector<int> aiSize)
	:
	ai(aiSize)
{
	//ai = new Ai(aiSize);
	ai.setRandomValues();

	car.setSize(sf::Vector2f(20, 40));
	car.setOrigin(car.getSize().x / 2, car.getSize().y / 2);
	car.setPosition(0, 0);
	car.setFillColor(sf::Color(Layer::random(0, 200), Layer::random(0, 200), Layer::random(0, 200)));
	car.setOutlineThickness(-2);
	car.setOutlineColor(sf::Color::Black);
}

void Car::move(std::vector<sf::VertexArray> walls, std::vector<sf::VertexArray> targets)
{
	//move sensors to current car position and extend them to size 3000
	resetSensors();
	//shorten the sensors based on surrounding walls
	updateSensors(walls);

	//put sensors lengths into a vector and feed them to the ai
	std::vector<double> inputs;
	for (int i = 0; i < 9; i++)
	{
		inputs.push_back(sensors[i].getSize().x);// / 3000.f);
	}
	ai.calculateOutput(inputs);

	if (ai.getOutput(0) > ai.getOutput(1) && ai.getOutput(0) > ai.getOutput(2))
	{
		car.rotate(rotation);
	}
	else if (ai.getOutput(2) > ai.getOutput(0) && ai.getOutput(2) > ai.getOutput(1))
	{
		car.rotate(-rotation);
	}

	//apply the movement
	car.move(sf::Vector2f(sin(car.getRotation() * 3.1415 / 180) * velocity, -cos(car.getRotation() * 3.1415 / 180) * velocity));
	
	if (nextTarget >= targets.size())
		nextTarget = 0;

	//collide with walls/target
	collide(walls, targets[nextTarget]);
}

void Car::draw(sf::RenderWindow& window)
{	
	window.draw(car);

	for (int i = 0; i < 9; i++)
	{
		//window.draw(sensors[i]);
	}
}

void Car::collide(std::vector<sf::VertexArray> walls, sf::VertexArray target)
{
	bool gotTarget = false;
	for (int nSide = 0; nSide < 4; nSide++)
	{	
		sf::VertexArray side;
		side.resize(2);
		side[0].position = vertexRect(car, nSide);
		side[1].position = vertexRect(car, nSide + 1);

		//calculate side info
		float coefSide = -(side[0].position.y - side[1].position.y) / (side[0].position.x - side[1].position.x);
		float dislocSide = side[0].position.y + coefSide * side[0].position.x;


		//calculate target info
		float coefTarget = -(target[0].position.y - target[1].position.y) / (target[0].position.x - target[1].position.x);
		float dislocTarget = target[0].position.y + coefTarget * target[0].position.x;
		float xInter = (dislocSide - dislocTarget) / (coefSide - coefTarget);
		float yInter = xInter * coefSide - dislocSide;

		if (side.getBounds().contains(xInter, -yInter) && target.getBounds().contains(xInter, -yInter) && !gotTarget)
		{
			fitness++;
			nextTarget++;
			gotTarget = true;
		}

		for (auto wall : walls)
		{
			//calculate wall info
			float coefWall = -(wall[0].position.y - wall[1].position.y) / (wall[0].position.x - wall[1].position.x);
			float dislocWall = wall[0].position.y + coefWall * wall[0].position.x;
			float xInter = (dislocSide - dislocWall) / (coefSide - coefWall);
			float yInter = xInter * coefSide - dislocSide;

			if (side.getBounds().contains(xInter, -yInter) && wall.getBounds().contains(xInter, -yInter))
			{
				crash();
				return;
			}
		}
	}
}
//0 = top-left; 1 = top-right; 2 = bot-right; 3 = bot-left
sf::Vector2f Car::vertexRect(sf::RectangleShape rect, int n)
{
	while (n >= 4)
		n = n - 4;
	float incVertices[4];

	float ipot = sqrt(std::pow(rect.getSize().x, 2) + std::pow(rect.getSize().y, 2));
	float diagInc = acos(rect.getSize().x / ipot) * 180 / 3.1415;

	incVertices[0] = 360 - rect.getRotation() + diagInc;
	incVertices[1] = 360 - rect.getRotation() - diagInc;
	incVertices[2] = 360 - rect.getRotation() + 180 + diagInc;
	incVertices[3] = 360 - rect.getRotation() + 180 - diagInc;

	return (rect.getPosition() + sf::Vector2f(cos(incVertices[n] * 3.1415 / 180) * ipot / 2, -sin(incVertices[n] * 3.1415 / 180) * ipot / 2));
}

void Car::crash()
{
	crashed = true;
	car.setFillColor(sf::Color::Black);
}
void Car::reset()
{
	car.setRotation(0);
	car.setPosition(0, 0);
	car.setFillColor(sf::Color(Layer::random(0, 200), Layer::random(0, 200), Layer::random(0, 200)));
	fitness = 0;
	nextTarget = 0;
	crashed = false;
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