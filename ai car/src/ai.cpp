#include "ai.h"
#include <iostream>

Ai::Ai(std::vector<int> inSize)
{
	size = inSize;
	for (int i = 1; i < size.size(); i++)
	{
		layers.push_back(Layer(size[i - 1], size[i]));
	}
}

Ai::Ai(Ai ai1, Ai ai2)
{
	if (ai1.size != ai2.size)
		std::exit(-1);

	size = ai1.size;

	for (int layer = 1; layer < size.size(); layer++)
	{
		Layer newLayer(ai1.layers[layer - 1], ai2.layers[layer - 1]);
		layers.push_back(newLayer);
	}
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