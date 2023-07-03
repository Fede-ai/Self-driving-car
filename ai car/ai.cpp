#include "ai.h"

Ai::Ai(int* inNeur, int inSizeNeur)
	:
	nNeur(inNeur)
{	
	nLayer = inSizeNeur;

	std::vector<float> inputLayer;
	neur.push_back(inputLayer);
	setRandomValues();
}

void Ai::calculateOutput()
{
	std::vector<float> inputLayer(neur[0]);
	neur.clear();
	neur.push_back(inputLayer);

	for (int layer = 1; layer < nLayer; layer++)
	{
		std::vector<float> newLayer;
		for (int n = 0; n < nNeur[layer]; n++)
		{
			newLayer.push_back(calculateNeuron(layer, n));
		}
		neur.push_back(newLayer);
	}
}
float Ai::calculateNeuron(int layer, int n)
{
	float thisNeur = 0;
	for (int neurBefore = 0; neurBefore < nNeur[layer - 1]; neurBefore++)
	{
		thisNeur += neur[layer - 1][neurBefore] * weight[layer - 1][neurBefore][n];
	}
	return activationFunction(thisNeur);
}
float Ai::activationFunction(float num)
{	
	return 1 / (1 + exp(-num));
}

void Ai::setRandomValues()
{
	weight.clear();

	for (int layer = 0; layer < nLayer - 1; layer++)
	{
		std::vector<std::vector<float>> weight1;
		for (int before = 0; before < nNeur[layer]; before++)
		{
			std::vector<float> weight2;
			for (int after = 0; after < nNeur[layer + 1]; after++)
			{
				weight2.push_back(random(0, 2000) / 1000.f - 1);
			}
			weight1.push_back(weight2);
		}
		weight.push_back(weight1);
	}
}

void Ai::setInputs(float inputs[])
{
	std::vector<float> inputLayer;
	for (int a = 0; a < nNeur[0]; a++)
	{
		inputLayer.push_back(inputs[a]);
	}
	neur[0] = inputLayer;
}
float Ai::getOutput(int n)
{
	return neur[nLayer - 1][n];
}

int Ai::random(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(min, max);
	return dist6(rng);
}