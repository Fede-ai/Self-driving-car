#pragma once
#include <vector>
#include <random>

class Layer
{
public:

};

class Ai
{
public:
	Ai(std::vector<int> inSize);
	void calculateOutput();	

	void setRandomValues();
	void setInputs(float inputs[]);
	float getOutput(int n);

private:
	float calculateNeuron(int layer, int n);	
	float activationFunction(float num);

	int random(int min, int max);

	std::vector<int> sizes;

	std::vector<std::vector<float>> neur;

	std::vector<std::vector<std::vector<float>>> weight;
	std::vector<std::vector<float>> bias;
};