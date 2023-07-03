#pragma once
#include <iostream>
#include <chrono>
#include "SFML/Graphics.hpp"
#include "ai.h"
#include "wall.h"
#include "car.h"

class Game
{
public:
	Game(sf::RenderWindow& inWindow);
	void play();	

private: 	
	void update();
	void draw();

	void sensorWall(sf::RectangleShape& sensor, sf::VertexArray wall);
	bool collision(sf::RectangleShape car, sf::VertexArray wall);
	sf::Vector2f vertexRect(sf::RectangleShape rect, int n);
	uint64_t getTime();

	sf::RenderWindow& window;
	static constexpr int nCars = 10;
	Car cars[nCars];

	sf::Vector2f lastMousePos;
	bool isMoving = false;
	std::vector<Wall> walls;
	bool isBuilding = false;

	bool countTime = true;
	uint64_t lastTime = getTime();
};