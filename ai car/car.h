#pragma once
#include "SFML/Graphics.hpp"
#include "ai.h"
#include "wall.h"

class Car
{
public:
	Car();
	void moveCar();
	void drawCar(sf::RenderWindow& window);

	void collide(std::vector<Wall> walls);
	void crash();

	void resetSensors();
	void updateSensors(std::vector<Wall> walls);
	bool crashed = false;

private:
	Ai* ai = NULL;
	sf::RectangleShape car;
	sf::RectangleShape sensors[9];

	static constexpr int nLayers = 3;
	int aiSize[nLayers] = {9, 6, 3};

	const int velocity = 5;
	const int rotation = 4;
};

