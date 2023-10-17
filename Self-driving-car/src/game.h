#pragma once
#include <iostream>
#include <thread>
#include "SFML/Graphics.hpp"
#include "ai.h"
#include "car.h"

class Game
{
public:
	Game(sf::RenderWindow& inWindow);
	void frame();

private: 		
	void takeConsoleInputs();

	void input();
	void update();
	void draw();

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
	const int maxUpdatePs = 400;	
	const int maxDrawPs = 60;
	bool outputPsStat = false;

	static constexpr int nCars = 200;
	std::vector<Car> cars;
	std::vector<sf::VertexArray> walls;

	std::vector<int> aiSizeVector;

	bool isMoving = false;
	bool isBuilding = false;
	bool isPaused = true;
	bool canPause = false;
	bool canDeleteWall = false;
};