#pragma once
#include <vector>

class Layer
{
public:
	//input layer is not a layer
	//layer obj contains the nodes and the wheights between the nodes of this layer and the previus one

	Layer(int sizeBefore, int size);

	void calculateLayer(std::vector<double> neuronsBefore);
	void setRandomLayerValues();
	double activationFunction(double num);

	std::vector<double> neurons;
	std::vector<double> biases;	
	//how to access correct weight -> weights[thisNeuron][neuronBefore]
	std::vector<std::vector<double>> weights;

	static int random(int min, int max);
};