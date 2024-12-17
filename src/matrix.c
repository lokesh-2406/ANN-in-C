#include "neural.h"
#include "matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 60000
//function to add two matrices
Mat *addMat(Mat *mat1, Mat *mat2)
{
	int i = 0, j = 0;
	Mat *mat = mCreate(mat1->rows, mat1->cols);
	for (i = 0; i < mat1->rows; i++)
		for (j = 0; j < mat2->cols; j++)
			mat->elements[i][j] = mat1->elements[i][j] + mat2->elements[i][j];

	return mat;
}

//function to subtract two matrices
Mat *subMat(Mat *mat1, Mat *mat2)
{
	
	//Mat *matTemp2 = mCopy(mat2);
	Mat *matTemp2 = scalarMulMat(-1, mat2);
	matTemp2 = addMat(mat1, matTemp2);

	return matTemp2;
}

//function to add scalar to each element of matrix
Mat *addScalarMat(double k, Mat *mat)
{
	Mat *matTemp = mCopy(mat);

	int i = 0, j = 0;
	for (int i = 0; i < mat->rows; ++i)
	{
		for (int j = 0; j < mat->cols; ++j)
		{
			matTemp->elements[i][j] += k;
		}
	}

	return matTemp;
}

//function to perform sigmoid operation on each element of matrix
Mat *sigMat(Mat *mat)
{
	int i = 0, j = 0;
	Mat *matTemp = mCopy(mat);
	for (int i = 0; i < mat->rows; ++i)
	{
		for (int j = 0; j < mat->cols; ++j)
		{
			matTemp->elements[i][j] = sigmoid(matTemp->elements[i][j]);
		}
	}
	return matTemp;
}

//function to multiply matrix by scalar
Mat *scalarMulMat(double k, Mat *mat)
{
	int i = 0, j = 0;
	Mat *matTemp = mCopy(mat);
	for (int i = 0; i < mat->rows; i++)
	{
		for (int j = 0; j < mat->cols; j++)
		{
			matTemp->elements[i][j] *= k;
		}
	}
	return matTemp;
}

//function to multiply two matrices element by element
Mat *mulMat(Mat *mat1, Mat *mat2)
{
	int i = 0, j = 0;
	Mat *mat = mCreate(mat1->rows, mat1->cols);
	for (i = 0; i < mat1->rows; i++)
		for (j = 0; j < mat2->cols; j++)
			mat->elements[i][j] = mat1->elements[i][j] * mat2->elements[i][j];

	return mat;
}

//function to perform dot product on two matrices
Mat *dotMat(Mat *mat1, Mat *mat2)
{
	Mat *mat = mCreate(mat1->rows, mat2->cols);
	int i = 0, j = 0;
	for (i = 0; i < mat1->rows; i++)
	{
		for (j = 0; j < mat2->cols; j++)
		{
			double sum = 0;
			for (int k = 0; k < mat2->rows; k++)
			{
				sum = sum + (mat1->elements[i][k] * mat2->elements[k][j]);
			}
			mat->elements[i][j] = sum;
		}
	}
	return mat;
}

//function to return transpose of matrix
Mat *tMat(Mat *mat)
{
	int i = 0, j = 0;
	Mat *matTemp = mCreate(mat->cols, mat->rows);
	for (i = 0; i < mat->rows; i++)
		for (j = 0; j < mat->cols; j++)
			matTemp->elements[j][i] = mat->elements[i][j];

	return matTemp;
}
Mat *mCreate(int row, int col)
{
	// Allocate memory for the matrix struct
	Mat *matrix = malloc(sizeof(Mat));
	if (matrix == NULL)
	{
		return NULL; // Memory allocation failed
	}

	// Set the rows and cols attributes
	matrix->rows = row;
	matrix->cols = col;

	// Allocate memory for the elements
	matrix->elements = malloc(row * sizeof(double *));
	if (matrix->elements == NULL)
	{
		free(matrix); // Free previously allocated memory
		return NULL;  // Memory allocation failed
	}

	// Allocate memory for each row
	for (int i = 0; i < matrix->rows; i++)
	{
		matrix->elements[i] = malloc(matrix->cols * sizeof(double));
		if (matrix->elements[i] == NULL)
		{
			// Free previously allocated memory
			for (int j = 0; j < i; j++)
			{
				free(matrix->elements[j]);
			}
			free(matrix->elements);
			free(matrix);
			return NULL; // Memory allocation failed
		}
	}

	return matrix;
}

void mFill(Mat *m, int val)
{
	for (int i = 0; i < m->rows; i++)
		for (int j = 0; j < m->cols; j++)
			m->elements[i][j] = val; // entering value into matrix
	
}

void mDel(Mat *m)
{
	for (int i = 0; i < m->rows; i++)
	{
		free(m->elements[i]);
	}
	// freeing alllocated memory
	free(m->elements);
	free(m);
	m = NULL;
}

void mDisplay(Mat *m)
{
	printf("Rows: %d Columns: %d\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; i++)
	{
		for (int j = 0; j < m->cols; j++)
		{
			printf("%1.3f ", m->elements[i][j]);
		}
		printf("\n");
	}
}

Mat *mCopy(Mat *m)
{
	Mat *mat = mCreate(m->rows, m->cols);
	for (int i = 0; i < m->rows; i++)
	{
		for (int j = 0; j < m->cols; j++)
		{
			mat->elements[i][j] = m->elements[i][j];
		}
	}
	return mat;
}

void mSave(Mat *m, char *file_string)
{
	FILE *file = fopen(file_string, "w");
	fprintf(file, "%d\n", m->rows);
	fprintf(file, "%d\n", m->cols);
	for (int i = 0; i < m->rows; i++)
	{
		for (int j = 0; j < m->cols; j++)
		{
			fprintf(file, "%.6f\n", m->elements[i][j]);
		}
	}
	printf("Successfully saved matrix to %s\n", file_string);
	fclose(file);
}

Mat *mLoad(char *file_string)
{
	FILE *file = fopen(file_string, "r");

	if (file == NULL)
	{
		printf("Given file does not exist.");
		return NULL;
	}

	char entry[MAXLEN];

	fgets(entry, MAXLEN, file);
	int rows = atoi(entry);

	fgets(entry, MAXLEN, file);
	int cols = atoi(entry);

	Mat *m = mCreate(rows, cols);

	for (int i = 0; i < m->rows; i++)
	{
		for (int j = 0; j < m->cols; j++)
		{
			fgets(entry, MAXLEN, file);
			m->elements[i][j] = strtod(entry, NULL);
		}
	}

	printf("Sucessfully loaded matrix from %s\n", file_string);
	fclose(file);
	return m;
}

double UniDis(double low, double high)
{
	double difference = high - low; 
	int scale = 100000;
	int scaled_difference = (int)(difference * scale);
	return low + (1.0 * (rand() % scaled_difference) / scale);
}

void mRandom(Mat *m, int n)
{
	double min = -1.0 / sqrt(n), max = 1.0 / sqrt(n);
	for (int i = 0; i < m->rows; i++)
	{
		for (int j = 0; j < m->cols; j++)
		{
			m->elements[i][j] = UniDis(min, max);
		}
	}
}

int mMax(Mat *m)
{

	int maxIndex = 0, i, j;
	double max = 0;
	for (i = 0; i < m->rows; i++)
		for (j = 0; j < m->cols; j++)
			if (max < m->elements[i][j])
			{
				maxIndex = i; //updating max index
				max = m->elements[i][j];
			}
		
	return maxIndex;
}

Mat *reshape1D(Mat *m)
{
	// function to tranform matrix into 1D
	Mat *mat;
	int check = 0;
	if (check == 0)
	{
		mat = mCreate(m->rows * m->cols, 1);
	}
	else if (check == 1)
	{
		printf("Error");
	}
	else
	{
		exit(1);
	}
	for (int i = 0; i < m->rows; i++)
	{
		for (int j = 0; j < m->cols; j++)
		{
			if (check == 0)
				mat->elements[i * m->cols + j][0] = m->elements[i][j];
			else if (check == 1)
			{
				printf("Catching Error in reshape1D");
			}
			else
			{
				exit(1);
			}
		}
	}
	return mat;
}