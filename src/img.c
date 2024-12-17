#include "img.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 60000

Img** csvImgs(char* file_string, int number_of_imgs) {
	FILE *fp;
	Img** imgs = malloc(number_of_imgs * sizeof(Img*));
	char row[MAXLEN];
	fp = fopen(file_string, "r");

	// Read the first line 
	fgets(row, MAXLEN, fp);
	int i = 0;

	while(feof(fp)!=1 && i<number_of_imgs)
	{ imgs[i] = malloc(sizeof(Img));
		int j=0;
		char num1[10];int k1=0;
		
		fgets(row, MAXLEN, fp);
		imgs[i]->img_data = mCreate(28, 28);
		
		while(row[j]!=',')
		{
			num1[k1]=row[j];
					k1++;
					j++;			
		}
		j++;
		num1[k1]='\0';
		imgs[i]->label=atoi(num1);
		
		for(int curr_row=0;curr_row<28;curr_row++)
		{
			for(int curr_col=0;curr_col<28;curr_col++)
			{
				char num[10];int k=0;
				while(row[j]!=','&&row[j]!='\0')
				{
					num[k]=row[j];
					k++;
					j++;
				}
				
				j++;
				num[k]='\0';
				imgs[i]->img_data->elements[curr_row][curr_col] =atoi(num)/255.0;
			}
		}
		i++;
	}
	fclose(fp);
	return imgs;
}

void img_print(Img* img) {
	mDisplay(img->img_data);
	printf("Img Label: %d\n", img->label);
}

void img_free(Img* img) {
	mDel(img->img_data);
	free(img);
	img = NULL;
}

void imgs_free(Img** imgs, int n) {
	for (int i = 0; i < n; i++) {
		img_free(imgs[i]);
	}
	free(imgs);
	imgs = NULL;
}