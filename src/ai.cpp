#include "ai.h"
#include <chrono>

Ai::Ai(std::vector<int> size)
{
	for (int i = 1; i < size.size(); i++)
	{
		layers.push_back(Layer(size[i - 1], size[i]));
	}	
	std::srand(static_cast<unsigned int>(std::time(nullptr))); 
}
Ai::Ai(Ai first, Ai second)
{
	for (int neur = 0; neur < first.layers[0].neurons.size(); neur++)
	{
		for (int neurBef = 0; neurBef < 9; neurBef++)
		{
			int random = Layer::random(0, 100);
			if (random < 47) {}
			else if (random < 94)
			{
				first.layers[0].weights[neur][neurBef] = second.layers[0].weights[neur][neurBef];
			}
			else
			{
				
			}
		}	
	}
	for (int layer = 1; layer < first.layers.size(); layer++)
	{
		for (int neur = 0; neur < first.layers[layer].neurons.size(); neur++)
		{
			for (int neurBef = 0; neurBef < first.layers[layer-1].neurons.size(); neurBef++)
			{
				int random = Layer::random(0, 100);
				if (random < 47) {}
				else if (random < 94)
				{
					first.layers[layer].weights[neur][neurBef]
				}
				else
				{

				}
			
			}	
		}
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