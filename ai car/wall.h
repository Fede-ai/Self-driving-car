#pragma once
#include "SFML/Graphics.hpp"

struct Wall : public sf::VertexArray
{
	bool isEditing = true;
};