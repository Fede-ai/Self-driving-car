#include "ai.h"
#include <iostream>

Ai::Ai(std::vector<int> size)
{
	for (int i = 1; i < size.size(); i++)
	{
		layers.push_back(Layer(size[i - 1], size[i]));
	}
	setRandomValues();
}

void Ai::calculateOutput(std::vector<double> inputs)
{
	layers[0].calculateLayer(inputs);

	for (int layer = 1; layer < layers.size(); layer++)
	{
		layers[layer].calculateLayer(layers[layer - 1].neurons);
	}
}

void Ai::setRandomValues()
{	
	for (int layer = 0; layer < layers.size(); layer++)
	{
		layers[layer].setRandomLayerValues();
	}
}

double Ai::getOutput(int n)
{
	return layers[layers.size() - 1].neurons[n];
}