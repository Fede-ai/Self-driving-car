#pragma once
#include <vector>
#include <random>
#include "layer.h"

class Ai
{
public:
	Ai(std::vector<int> size);
	Ai(Ai ai1, Ai ai2);
	void calculateOutput(std::vector<double> inputs);

	void setRandomValues();
	double getOutput(int n);
private:
	std::vector<int> size;
	std::vector<Layer> layers;
};