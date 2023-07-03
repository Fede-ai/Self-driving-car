#pragma once
#include <vector>
#include <random>

class Ai
{
public:
	Ai(int* inNeur, int inSizeNeur);
	void calculateOutput();	

	void setRandomValues();
	void setInputs(float inputs[]);
	float getOutput(int n);

private:
	float calculateNeuron(int layer, int n);	
	float activationFunction(float num);

	int random(int min, int max);

	int* nNeur;
	int nLayer;

	std::vector<std::vector<float>> neur;

	std::vector<std::vector<std::vector<float>>> weight;
	std::vector<std::vector<float>> bias;
};

