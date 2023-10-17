#pragma once
#include <vector>

class Layer
{
public:
	//input layer is not a layer
	//layer obj contains the nodes and the wheights between the nodes of this layer and the previus one

	Layer(int inSizeBefore, int inSize);
	Layer(Layer lyr1, Layer lyr2);

	void calculateLayer(std::vector<double> neuronsBefore);
	void setRandomLayerValues();
	double activationFunction(double num);

	std::vector<double> neurons;
	std::vector<double> biases;	
	//how to access correct weight -> weights[thisNeuron][neuronBefore]
	std::vector<std::vector<double>> weights;

	static int random(int min, int max);

private:
	int size = 0;
	int sizeBefore = 0;
};