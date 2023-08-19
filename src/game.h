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
	sf::Vector2f windowDim = sf::Vector2f(sf::VideoMode::getDesktopMode().width*2/3, sf::VideoMode::getDesktopMode().width*3/8);
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
	bool outputPsStat = false;

	static constexpr int nCars = 10;
	Car cars[nCars];
	std::vector<sf::VertexArray> walls;

	bool isMoving = false;
	bool isBuilding = false;
	bool isPaused = true;
	bool canPause = false;
	bool canDeleteWall = false;
};