#ifndef NEURAL_H
#define NEURAL_H

#include "matrix.h"
#include "img.h"

typedef struct {
	int iNeurons;
	int hNeurons;
	int oNeurons;
	double alpha;
	Mat* hWeights;
	Mat* oWeights;
} NeuralNetwork;

double sigmoid(double input);
Mat* sigmoidDerivative(Mat* m);
Mat* softmax(Mat* m);

Mat* forwardProp(Mat *l1Inputs, Mat *weights);
void backProp(NeuralNetwork *net, Mat *L2_Val, Mat *L1_Val, Mat *error, int n);

NeuralNetwork* createNet(int iNeuronsCount, int hNeuronsCount, int oNeuronsCount, double learningRate);
void trainNetwork(NeuralNetwork* net, Mat* input_data, Mat* output_data);
void networkTrainImgs(NeuralNetwork* net, Img** imgs, int batch_size);
Mat* networkPredictImg(NeuralNetwork* net, Img* img);
double networkPredictIamges(NeuralNetwork* net, Img** imgs, int n);
Mat* networkPredict(NeuralNetwork* net, Mat* input_data);
void networkSave(NeuralNetwork* net, char* file_string);
NeuralNetwork* networkLoad(char* file_string);
void networkPrint(NeuralNetwork* net);
void networkFree(NeuralNetwork* net);

#endif