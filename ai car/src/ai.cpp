#include "ai.h"
#include <iostream>

Ai::Ai(std::vector<int> inSize)
{
	size = inSize;
	for (int i = 1; i < inSize.size(); i++)
	{
		layers.push_back(Layer(inSize[i - 1], inSize[i]));
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

Ai Ai::merge(Ai ai1, Ai ai2)
{
	Ai ai(ai1.size);
	
	for (int layer = 1; layer < ai.size.size(); layer++)
	{
		for (int neur = 0; neur < ai.size[layer-1]; neur++)
		{
			//set a "random" bias
			int probB = Layer::random(0, 1000);
			if (probB < 495)
			{
			//	ai.layers[layer-1].biases[neur] = ai1.layers[layer-1].biases[neur];
			}
			else if (probB < 990)
			{
			//	ai.layers[layer-1].biases[neur] = ai2.layers[layer-1].biases[neur];
			}
			//else
			{
				std::cout << layer - 1 << ", " << neur << "\n";
				ai.layers[layer-1].biases[neur] = Layer::random(0, 2000) / 1000.f - 1;
			}
			
			//for (int before = 0; before < ai.size[layer-1]; before++)
			//{
			//	int probW = Layer::random(0, 1000);
			//	if (probW < 495)
			//	{
			//		ai.layers[layer].weights[neur][before] = ai1.layers[layer].weights[neur][before];
			//	}
			//	else if (probW < 990)
			//	{
			//		ai.layers[layer].weights[neur][before] = ai2.layers[layer].weights[neur][before];
			//	}
			//	else
			//	{
			//		ai.layers[layer].weights[neur][before] = Layer::random(0, 2000) / 1000.f - 1;
			//	}
			//}
		}
	}

	return ai;
}
