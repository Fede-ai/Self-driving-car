#pragma once
#include "SFML/Graphics.hpp"
#include "ai.h"

class Car
{
public:
	Car();
	void move();
	void draw(sf::RenderWindow& window);

	sf::RectangleShape& getSensor(int n);
	void resetSensors();
	void crash();

private:
	Ai* ai = NULL;
	sf::RectangleShape car;
	sf::RectangleShape sensors[9];

	static constexpr int nLayers = 3;
	int aiSize[nLayers] = {9, 6, 2};

	float velocity = 0;
	const float acceleration = 0.05;
	const double maxRotation = 4;
	const float maxVelocity = 5;

	bool crashed = false;
	bool ablePause = true;
	bool paused = true;
};
