#pragma once
#include "SFML/Graphics.hpp"
#include "ai.h"

class Car
{
public:
	Car(std::vector<int> aiSize);

	void updateCar(std::vector<sf::VertexArray> walls);
	void drawCar(sf::RenderWindow& window);

	void crash();
	void reset();
	bool crashed = false;
	int fitness = 0;
	Ai ai;

private:

	bool collide(std::vector<sf::VertexArray> walls);
	sf::Vector2f vertexRect(sf::RectangleShape rect, int n);

	void resetSensors();
	void updateSensors(std::vector<sf::VertexArray> walls);

	sf::RectangleShape car;
	sf::RectangleShape sensors[9];

	static constexpr int velocity = 5;
	static constexpr int rotation = 4;
};

