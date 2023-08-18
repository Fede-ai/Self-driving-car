#pragma once
#include <vector>
#include <random>
#include "layer.h"

class Ai
{
public:
	Ai(std::vector<int> size);
	void calculateOutput(std::vector<double> inputs);

	void setRandomValues();
	double getOutput(int n);

private:

	std::vector<Layer> layers;
};