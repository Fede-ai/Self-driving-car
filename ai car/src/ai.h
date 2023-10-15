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

	static Ai merge(Ai ai1, Ai ai2);

private:
	std::vector<int> size;
	std::vector<Layer> layers;
};