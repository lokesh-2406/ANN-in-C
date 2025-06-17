#include "neural.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "matrix.h"
#include <math.h>

#define MAXLEN 60000

double sigmoid(double input) {
	return 1.0 / (1 + exp(-1 * input));
}

Mat* sigmoidDerivative(Mat* m) {
	Mat* ones = mCreate(m->rows, m->cols);
	for (int i = 0; i < ones->rows; i++) {
		for (int j = 0; j < ones->cols; j++) {
			ones->elements[i][j] = 1; 
		}
	}
	Mat* subtracted = subMat(ones, m);
	Mat* multiplied = mulMat(m, subtracted);
	mDel(ones);
	mDel(subtracted);
	return multiplied;
}

Mat* softmax(Mat* m) {
    //SoftPlus function
	//Mat* mat = mCreate(m->rows, m->cols);
    // for (int i = 0; i < m->rows; i++) {
    //     for (int j = 0; j < m->cols; j++) {
    //         double z = m->elements[i][j];
    //         mat->elements[i][j] = log(1 + exp(z));
    //     }
    // }
    // return mat;

	double total = 0;
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->cols; j++) {
			total += exp(m->elements[i][j]);
		}
	}
	Mat* mat = mCreate(m->rows, m->cols);
	for (int i = 0; i < mat->rows; i++) {
		for (int j = 0; j < mat->cols; j++) {
			mat->elements[i][j] = exp(m->elements[i][j]) / total;
		}
	}
	return mat;
	
}
//function for forward propagation
Mat* forwardProp(Mat *l1Inputs, Mat *weights){
	Mat* hiddenVals = dotMat(weights, l1Inputs); //before activation function

	//using sigmoid activation function to introduce non-linearity
	for (int i = 0; i < hiddenVals->rows; i++) {
		for (int j = 0; j < hiddenVals->cols; j++) {
			hiddenVals->elements[i][j] = sigmoid(hiddenVals->elements[i][j]);
		}
	}

	return hiddenVals;
}

//function for backward propagation (not optimized)
void  backProp(NeuralNetwork *net, Mat *L2_Val, Mat *L1_Val, Mat *error, int n){
	
	//calculating derivative
	Mat* sigDerFinal = sigmoidDerivative(L2_Val); 
	Mat* mulWithLoss = mulMat(error, sigDerFinal);
	Mat* t_mat = tMat(L1_Val);
	Mat* derLossByWeight = dotMat(mulWithLoss, t_mat);
	

	Mat* updateVal = scalarMulMat(net->alpha, derLossByWeight); //multiplying with learning rate

	
	if(n==2){ 
		Mat* newWeights = addMat(net->oWeights, updateVal);  //finding updated weights
		mDel(net->oWeights);//deleting old weights
		net->oWeights = newWeights;
	}

	if(n==1){
		Mat* newWeights2 = addMat(net->hWeights, updateVal); //finding updated weights
		mDel(net->hWeights);//deleting old weights
		net->hWeights = newWeights2;
	}

	//freeing memory
	mDel(sigDerFinal);
	mDel(mulWithLoss);
	mDel(t_mat);
	mDel(derLossByWeight);
	mDel(updateVal);
}

//function to create and initialize network
NeuralNetwork* createNet(int iNeuronsCount, int hNeuronsCount, int oNeuronsCount, double learningRate) {
	//declaring netwrk
	NeuralNetwork* net = malloc(sizeof(NeuralNetwork));

	//initializing model parameters
	net->iNeurons = iNeuronsCount;
	net->hNeurons = hNeuronsCount;
	net->oNeurons = oNeuronsCount;
	net->alpha = learningRate;

	//creating layers and intializing them with random values
	Mat* hLayer = mCreate(hNeuronsCount, iNeuronsCount);
	Mat* oLayer = mCreate(oNeuronsCount, hNeuronsCount);
	mRandom(hLayer, hNeuronsCount);
	mRandom(oLayer, oNeuronsCount);

	net->hWeights = hLayer;
	net->oWeights = oLayer;

	return net;
}

void trainNetwork(NeuralNetwork* net, Mat* input, Mat* output) {
	
	//Forward Propagation
	Mat* valHidden = forwardProp(input, net->hWeights);
	Mat* resFinal = forwardProp(valHidden, net->oWeights);

	// Find errors 
	Mat* loss = subMat(output, resFinal); //loss function
	Mat* t_mat = tMat(net->oWeights); //transposing matrix
	Mat* hErrors = dotMat(t_mat, loss); //propagating output error to hidden error, weighted by output weights
	mDel(t_mat); //freeing memory

	//Backward Propagation
	
	Mat* tempInput = mCopy(input);
	backProp(net, resFinal, valHidden, loss, 2);
	backProp(net, valHidden, tempInput, hErrors, 1);

	//Freeing memory by deleting matrices
	mDel(loss);
	mDel(hErrors);
	mDel(valHidden);
	mDel(resFinal);
	
	
}
void trainBatch(NeuralNetwork* net, Mat* input, Mat* output) {
	
	//Forward Propagation
	Mat* valHidden = forwardProp(input, net->hWeights);
	Mat* resFinal = forwardProp(valHidden, net->oWeights);

	// Find errors 
	Mat* loss = subMat(output, resFinal); //loss function
	Mat* t_mat = tMat(net->oWeights); //transposing matrix
	Mat* hErrors = dotMat(t_mat, loss); //propagating output error to hidden error, weighted by output weights
	mDel(t_mat); //freeing memory

	//Backward Propagation
	
	Mat* tempInput = mCopy(input);
	backProp(net, resFinal, valHidden, loss, 2);
	backProp(net, valHidden, tempInput, hErrors, 1);

	//Freeing memory by deleting matrices
	mDel(loss);
	mDel(hErrors);
	mDel(valHidden);
	mDel(resFinal);
	
	
}

void networkTrainImgs(NeuralNetwork* net, Img** imgs, int batch_size) {
	
	for (int i = 0; i < batch_size; i++) {

		if (i % 100 == 0) printf("Percentage Completed: %2.3f%%\n", ((i+100.0)/600.0));
		
		Img* cur_img = imgs[i];
		Mat* img_data = reshape1D(cur_img->img_data); 
		Mat* output = mCreate(10, 1);
		for(int j=0;j<10;j++)
		{
			output->elements[j][0] = 0;
		}
		output->elements[cur_img->label][0] = 1; // Setting the result
		
		trainNetwork(net, img_data, output);
		mDel(output);
		mDel(img_data);
	}
}
void networkTrainImgs2(NeuralNetwork* net, Img** imgs, int batch_size)
{
	
	for (int i = batch_size*600; i < (batch_size+1)*600; i++) {

		if (i % 100 == 0) printf("Percentage Completed: %2.3f%%\n", ((i+100.0)/600.0));
		
		Img* cur_img = imgs[i];
		Mat* img_data = reshape1D(cur_img->img_data); 
		Mat* output = mCreate(10, 1);
		for(int j=0;j<10;j++)
		{
			output->elements[j][0] = 0;
		}
		output->elements[cur_img->label][0] = 1; // Setting the result
		
		trainBatch(net, img_data, output);
		mDel(output);
		mDel(img_data);
	}
}

Mat* networkPredictImg(NeuralNetwork* net, Img* img) {
	Mat* img_data = reshape1D(img->img_data);
	Mat* res = networkPredict(net, img_data);
	mDel(img_data);
	return res;
}

double networkPredictIamges(NeuralNetwork* net, Img** imgs, int n) {
	int n_correct = 0;
	for (int i = 0; i < n; i++) {
		Mat* prediction = networkPredictImg(net, imgs[i]);
		if (mMax(prediction) == imgs[i]->label) {
			n_correct++;
		}
		mDel(prediction);
	}
	return 1.0 * n_correct / n;
}

int returnPredictedNumber(NeuralNetwork* net, Img** imgs, int n) {
	Mat* prediction = networkPredictImg(net, imgs[0]);
	int num = mMax(prediction);
	mDel(prediction);
	
	return num;
}

Mat* networkPredict(NeuralNetwork* net, Mat* input_data) {
	Mat* hidden_inputs	= dotMat(net->hWeights, input_data);
	Mat* hidden_outputs = sigMat(hidden_inputs);
	Mat* final_inputs = dotMat(net->oWeights, hidden_outputs);
	Mat* final_outputs = sigMat(final_inputs);
	Mat* result = softmax(final_outputs);

	mDel(hidden_inputs);
	mDel(hidden_outputs);
	mDel(final_inputs);
	mDel(final_outputs);

	return result;
}

void networkSave(NeuralNetwork* net, char* file_string) {
	mkdir(file_string,0777);
	chdir(file_string);
	FILE* descriptor = fopen("descriptor", "w");
	fprintf(descriptor, "%d\n", net->iNeurons);
	fprintf(descriptor, "%d\n", net->hNeurons);
	fprintf(descriptor, "%d\n", net->oNeurons);
	fclose(descriptor);
	mSave(net->hWeights, "hidden");
	mSave(net->oWeights, "output");
	printf("Written successfully to '%s'\n", file_string);
	chdir("-"); 
}

NeuralNetwork* networkLoad(char* file_string) {
	NeuralNetwork* net = malloc(sizeof(NeuralNetwork));
	char entry[MAXLEN];
	chdir(file_string);

	FILE* descriptor = fopen("descriptor", "r");
	fgets(entry, MAXLEN, descriptor);
	net->iNeurons = atoi(entry);
	fgets(entry, MAXLEN, descriptor);
	net->hNeurons = atoi(entry);
	fgets(entry, MAXLEN, descriptor);
	net->oNeurons = atoi(entry);
	fclose(descriptor);
	net->hWeights = mLoad("hidden");
	net->oWeights = mLoad("output");
	printf("Loaded network successfully from '%s'\n", file_string);
	chdir("-"); 
	return net;
}

void networkPrint(NeuralNetwork* net) {
	printf("# of Inputs: %d\n", net->iNeurons);
	printf("# of Hidden: %d\n", net->hNeurons);
	printf("# of Output: %d\n", net->oNeurons);
	printf("Hidden Weights: \n");
	mDisplay(net->hWeights);
	printf("Output Weights: \n");
	mDisplay(net->oWeights);
}

void networkFree(NeuralNetwork *net) {
	mDel(net->hWeights);
	mDel(net->oWeights);
	free(net);
	net = NULL;
}