#ifndef IMG_H
#define IMG_H

#include "matrix.h"

typedef struct {
	Mat* img_data;
	int label;
} Img;

Img** csvImgs(char* file_string, int number_of_imgs);
void img_print(Img* img);
void img_free(Img *img);
void imgs_free(Img **imgs, int n);

#endif