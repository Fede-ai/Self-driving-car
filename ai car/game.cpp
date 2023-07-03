#include "game.h"

Game::Game(sf::RenderWindow& inWindow)
	:
	window(inWindow)
{
}

void Game::play()
{	
	update();
	draw();
}

void Game::update()
{	
	sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition());
	sf::Vector2f mouseP = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	//handle view movement and walls creation
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (window.hasFocus() && !isBuilding && !sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			sf::View view(window.getView());
			float x = window.getView().getSize().x / window.getSize().x;
			float y = window.getView().getSize().y / window.getSize().y;
			view.move((lastMousePos - mousePos).x * x, (lastMousePos - mousePos).y * y);
			window.setView(view);
			isMoving = true;
		}
		else
		{
			isMoving = false;
		}

		if (window.hasFocus() && !isMoving)
		{
			if (!isBuilding)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				{
					Wall wall;
					wall.resize(2);
					wall.setPrimitiveType(sf::Lines);
					wall[0].position = mouseP;
					wall[1].position = mouseP;
					wall[0].color = sf::Color::Black;
					wall[1].color = sf::Color::Black;

					walls.push_back(wall);
					isBuilding = true;
				}
			}
			else
			{
				walls[walls.size() - 1][1].position = mouseP;
				walls[walls.size() - 1].isEditing = false;
			}
		}	
	}
	else if (isBuilding)
	{
		isBuilding = false;
	}
	lastMousePos = mousePos;
	
	
	for (int i = 0; i < nCars; i++)
	{
		cars[i].move();
		cars[i].resetSensors();	
		
		for (int a = 0; a < 9; a++)
		{
			for (int b = 0; b < walls.size(); b++)
			{
				if (!walls[b].isEditing)
				{
					sensorWall(cars[i].getSensor(a), walls[b]);
				}
			}	
		}
	}
}

void Game::draw()
{
	for (int a = 0; a < walls.size(); a++)
	{
		window.draw(walls[a]);
	}
	
	for (int i = 0; i < nCars; i++)
	{
		cars[i].draw(window);
	}
}

void Game::sensorWall(sf::RectangleShape& sensor, sf::VertexArray wall)
{	
	//calculate car info
	float angSens = 360 - sensor.getRotation();
	float coefSens = tan(angSens * 3.1415 / 180);
	float dislocSens = sensor.getPosition().y + coefSens * sensor.getPosition().x;

	//calculate wall info
	float coefWall = -(wall[0].position.y - wall[1].position.y) / (wall[0].position.x - wall[1].position.x);
	float dislocWall = wall[0].position.y + coefWall * wall[0].position.x;
	float xInter = (dislocSens - dislocWall) / (coefSens - coefWall);
	float yInter = xInter * coefSens - dislocSens;

	if (sensor.getGlobalBounds().contains(xInter, -yInter) && wall.getBounds().contains(xInter, -yInter))
	{
		float xDist = sensor.getPosition().x - xInter;
		float yDist = sensor.getPosition().y + yInter;
		float dist = sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
		sensor.setSize(sf::Vector2f(std::min(dist, sensor.getSize().x), 1));
	}
}

bool Game::collision(sf::RectangleShape car, sf::VertexArray wall)
{
	sf::Vector2f rect1Point[4];
	sf::Vector2f rect2Point[4];
	float rect1Proj[4];
	float rect2Proj[4];
	/*
	for (int i = 0; i < 4; i++)
	{
		rect1Point[i] = vertexRect(rect1, i);
		rect2Point[i] = vertexRect(rect2, i);
	}

	float angles[4] = {
		360 - rect1.getRotation(),
		360 - rect1.getRotation() + 90,
		360 - rect2.getRotation(),
		360 - rect2.getRotation() + 90
	};

	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			float ipot = sqrt(std::pow(rect1Point[b].x, 2) + std::pow(rect1Point[b].y, 2));
			float ang = acos(rect1Point[b].x / ipot) * 180 / 3.1415;
			float angg = angles[a] + ang;
			if (rect1Point[b].y <= 0)
			{
				angg = angles[a] - ang;
			}
			rect1Proj[b] = cos(angg * 3.1415 / 180) * ipot;
		}
		for (int b = 0; b < 4; b++)
		{
			float ipot = sqrt(std::pow(rect2Point[b].x, 2) + std::pow(rect2Point[b].y, 2));
			float ang = acos(rect2Point[b].x / ipot) * 180 / 3.1415;
			float angg = angles[a] + ang;
			if (rect2Point[b].y <= 0)
			{
				angg = angles[a] - ang;
			}
			rect2Proj[b] = cos(angg * 3.1415 / 180) * ipot;
		}

		float max1, min1, max2, min2;

		max1 = std::max(std::max(std::max(rect1Proj[3], rect1Proj[2]), rect1Proj[1]), rect1Proj[0]);
		min1 = std::min(std::min(std::min(rect1Proj[3], rect1Proj[2]), rect1Proj[1]), rect1Proj[0]);
		max2 = std::max(std::max(std::max(rect2Proj[3], rect2Proj[2]), rect2Proj[1]), rect2Proj[0]);
		min2 = std::min(std::min(std::min(rect2Proj[3], rect2Proj[2]), rect2Proj[1]), rect2Proj[0]);

		if (!((min1 > min2 && min1 < max2) || (max1 > min2 && max1 < max2) || (min2 > min1 && min2 < max1) || (max2 > min1 && max2 < max1)))
		{
			return false;
		}
	}*/
	return true;
}

//n: 0 = top-left; 1 = top-right; 2 = bot-right; 3 = bot-left
sf::Vector2f Game::vertexRect(sf::RectangleShape rect, int n)
{
	float incVertices[4];

	float ipot = sqrt(std::pow(rect.getSize().x, 2) + std::pow(rect.getSize().y, 2));
	float diagInc = acos(rect.getSize().x / ipot) * 180 / 3.1415;

	incVertices[0] = 360 - rect.getRotation() + diagInc;
	incVertices[1] = 360 - rect.getRotation() - diagInc;
	incVertices[2] = 360 - rect.getRotation() + 180 + diagInc;
	incVertices[3] = 360 - rect.getRotation() + 180 - diagInc;

	return (rect.getPosition() + sf::Vector2f(cos(incVertices[n] * 3.1415 / 180) * ipot / 2, -sin(incVertices[n] * 3.1415 / 180) * ipot / 2));
}

uint64_t Game::getTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}