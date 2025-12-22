#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "img.h"
#include "neural.h"
#include "matrix.h"
#include <string.h>


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

// int main() {

// 	srand(time(NULL));

// 	printf("Welcome to Handwritten Digit Recognizer!");
// 	printf("\n0. Train Network \n1. Test Network \n2. Recognize Handwritten Digit\n");
// 	int n;
// 	printf("Enter option number: ");
// 	scanf("%d", &n);

// 	//TRAINING
// 	if(n==0)
// 		train();

// 	//TESTING
// 	if(n==1)
// 		test();

// 	//PREDICT
// 	if(n==2)
// 		predict();
	
// 	return 0;
	
// }
int main(int argc, char *argv[]) {
    srand(time(NULL));

    // CLI-only mode: --predict --input <csv> --model <dir>
    int do_predict = 0;
    char input_file[1024] = "";
    char model_dir[1024] = "testing_net"; // default

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--predict") == 0) {
            do_predict = 1;
        } else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            strncpy(input_file, argv[++i], sizeof(input_file)-1);
        } else if (strcmp(argv[i], "--model") == 0 && i + 1 < argc) {
            strncpy(model_dir, argv[++i], sizeof(model_dir)-1);
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Usage:\n");
            printf("  Interactive: ./main\n");
            printf("  CLI predict: ./main --predict --input /path/to/digit.csv --model /path/to/testing_net\n");
            return 0;
        }
    }

    if (do_predict) {
        if (input_file[0] == '\0') {
            fprintf(stderr, "{\"error\":\"--input <file> required\"}\n");
            return 2;
        }

        // Non-interactive predict path.
        Img** images = csvImgs(input_file, 1);
        if (!images) {
            fprintf(stderr, "{\"error\":\"failed to open input file\"}\n");
            return 3;
        }

        NeuralNetwork* net3 = networkLoad(model_dir);
        if (!net3) {
            fprintf(stderr, "{\"error\":\"failed to load model\"}\n");
            imgs_free(images, 1);
            return 4;
        }

        int prediction = returnPredictedNumber(net3, images, 1);

        // Output JSON for easy parsing
        printf("{\"prediction\": %d}\n", prediction);

        imgs_free(images, 1);
        networkFree(net3);
        return 0;
    }

    // Otherwise fall back to interactive behavior (your original menu)
    printf("Welcome to Handwritten Digit Recognizer!\n");
    printf("\n0. Train Network \n1. Test Network \n2. Recognize Handwritten Digit\n");
    int n;
    printf("Enter option number: ");
    if (scanf("%d", &n) != 1) return 0;

    if (n==0) train();
    if (n==1) test();
    if (n==2) predict();

    return 0;
}