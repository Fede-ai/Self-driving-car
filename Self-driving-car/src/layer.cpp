#include "layer.h"
#include <random>

Layer::Layer(int inSizeBefore, int inSize)
{
	size = inSize;
	sizeBefore = inSizeBefore;

	for (int neur = 0; neur < size; neur++)
	{
		neurons.push_back(0);
		biases.push_back(0);

		std::vector<double> neuronWeights;
		for (int before = 0; before < sizeBefore; before++)
		{
			neuronWeights.push_back(0);
		}
		weights.push_back(neuronWeights);
	}
}

Layer::Layer(Layer lyr1, Layer lyr2)
{
	size = int(lyr1.neurons.size());
	sizeBefore = int(lyr1.weights[0].size());

	for (int neur = 0; neur < size; neur++)
	{
		neurons.push_back(0);

		//set a "random" bias
		int probB = Layer::random(0, 1000);
		if (probB < 495)
		{
			biases.push_back(lyr1.biases[neur]);
		}
		else if (probB < 990)
		{
			biases.push_back(lyr2.biases[neur]);
		}
		else
		{
			biases.push_back(Layer::random(0, 2000) / 1000.f - 1);
		}

		std::vector<double> neuronWeights;
		for (int before = 0; before < sizeBefore; before++)
		{
			int probW = Layer::random(0, 1000);
			if (probW < 495)
			{
				neuronWeights.push_back(lyr1.weights[neur][before]);
			}
			else if (probW < 990)
			{
				neuronWeights.push_back(lyr2.weights[neur][before]);
			}
			else
			{
				neuronWeights.push_back(Layer::random(0, 2000) / 1000.f - 1);
			}			
		}
		weights.push_back(neuronWeights);
	}
}

void Layer::calculateLayer(std::vector<double> neuronsBefore)
{
	for (int thisNeuron = 0; thisNeuron < neurons.size(); thisNeuron++)
	{
		double value = biases[thisNeuron];
		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			value += neuronsBefore[neuronBefore] * weights[thisNeuron][neuronBefore];
		}	
		neurons[thisNeuron] = activationFunction(value);
	}
}

void Layer::setRandomLayerValues()
{
	for (int thisNeuron = 0; thisNeuron < neurons.size(); thisNeuron++)
	{
		biases[thisNeuron] = random(0, 2000) / 1000.f - 1;

		for (int neuronBefore = 0; neuronBefore < weights[thisNeuron].size(); neuronBefore++)
		{
			weights[thisNeuron][neuronBefore] = random(0, 2000) / 1000.f - 1;
		}
	}
}

double Layer::activationFunction(double num)
{
	return 1 / (1 + exp(-num));
}

int Layer::random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return dist6(rng);
}