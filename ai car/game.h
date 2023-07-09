#pragma once
#include <iostream>
#include <thread>
#include "SFML/Graphics.hpp"
#include "ai.h"
#include "wall.h"
#include "car.h"

class Game
{
public:
	Game(sf::RenderWindow& inWindow);
	void frame();

private: 	
	void input();
	void update();
	void draw();

	void ok();

	bool collision(sf::RectangleShape car, sf::VertexArray wall);
	sf::Vector2f vertexRect(sf::RectangleShape rect, int n);
	uint64_t getTime();

	sf::RenderWindow& window;
	sf::Vector2f windowDim = sf::Vector2f(1280, 720);
	sf::ContextSettings settings;
	sf::Vector2f lastMousePos;
	bool canFullscreen = true;
	bool isFullscreen = false;

	uint64_t currentSecond = getTime() / 1000;
	int currentInputPs = 0;
	int currentUpdatePs = 0;
	int currentDrawPs = 0;
	const int maxInputPs = 30;
	const int maxUpdatePs = 200;	
	const int maxDrawPs = 60;

	static constexpr int nCars = 1;
	Car cars[nCars];
	std::vector<Wall> walls;

	bool isMoving = false;
	bool isBuilding = false;
	bool isPaused = true;
	bool canPause = false;
};