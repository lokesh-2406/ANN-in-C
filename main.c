#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "img.h"
#include "neural.h"
#include "matrix.h"


void train()
{
	int noOfImgs = 60000;
	Img** images = csvImgs("mnist_train.csv", noOfImgs);
	NeuralNetwork* net = createNet(784, 28, 10, 0.1);
	networkTrainImgs(net, images, noOfImgs);
	int numberbatch=100;
	for(int j=0;j<numberbatch;j++)
	networkTrainImgs2(net,images,j);
	networkSave(net, "testing_net");
	
	imgs_free(images, noOfImgs);
	networkFree(net);
}

void test()
{
	int noOfImgs = 10000;
	Img** images = csvImgs("mnist_test.csv", noOfImgs);
	NeuralNetwork* net1 = networkLoad("testing_net");
	double score = networkPredictIamges(net1, images, 10000);
	printf("Accuracy Percentage: %.2f%%\n", score*100.0);
	

	imgs_free(images, noOfImgs);
	networkFree(net1);

}

void predict()
{
	char input[200];
	printf("Enter name of file: ");
	scanf("%s", input);
	Img** images = csvImgs(input, 1);
	NeuralNetwork* net3 = networkLoad("testing_net");
	int prediction = returnPredictedNumber(net3, images, 1);
	printf("The prediction for number is: %d\n", prediction);

}

int main() {

	srand(time(NULL));

	printf("Welcome to Handwritten Digit Recognizer!");
	printf("\n0. Train Network \n1. Test Network \n2. Recognize Handwritten Digit\n");
	int n;
	printf("Enter option number: ");
	scanf("%d", &n);

	//TRAINING
	if(n==0)
		train();

	//TESTING
	if(n==1)
		test();

	//PREDICT
	if(n==2)
		predict();
	
	return 0;
	
}
