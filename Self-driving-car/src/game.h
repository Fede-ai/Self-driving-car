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
	//11 inputs and 3 outputs
	const std::vector<int> aiSizeVector { 11, 30, 3 };
	sf::Vector2u windowDim = sf::Vector2u(sf::VideoMode::getDesktopMode().width*2/3, sf::VideoMode::getDesktopMode().width*3/8);
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
	const int maxDrawPs = 30;
	uint64_t timeLastInput = getTime();
	uint64_t timeLastUpdate = getTime();
	uint64_t timeLastDraw = getTime();
	bool outputPsStat = false;
	bool outputBest = false;

	uint64_t timeStartGeneration = getTime();
	uint64_t timeGeneration = 0;

	static constexpr int nCars = 100;
	std::vector<Car> cars;
	std::vector<sf::VertexArray> walls;
	std::vector<sf::VertexArray> targets;

	sf::RectangleShape userCar;
	sf::Vector2f userCarVDir = sf::Vector2f(1, 0);

	bool isMoving = false;
	bool isBuildingWall = false;
	bool isBuildingTarget = false;
	bool isPaused = true;
	bool canPause = false;
	bool canDeleteWall = false;
	bool canDeleteTarget = false;
	bool canKillAll = false;
	bool canRestartGen = false;
	bool canRestartSim = false;
};