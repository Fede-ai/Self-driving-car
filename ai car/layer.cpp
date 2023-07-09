#include "layer.h"
#include <random>

Layer::Layer(int sizeBefore, int size)
{
	for (int a = 0; a < size; a++)
	{
		neurons.push_back(0);
		biases.push_back(0);

		std::vector<double> neuronWeights;
		for (int b = 0; b < sizeBefore; b++)
		{
			neuronWeights.push_back(0);
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