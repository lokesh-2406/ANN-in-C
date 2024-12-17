#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
	double** elements;
	int rows;
	int cols;
} Mat;

Mat* addMat(Mat* m1, Mat* m2);
Mat* subMat(Mat* m1, Mat* m2);
Mat* addScalarMat(double k, Mat* m);

Mat* scalarMulMat(double k, Mat* m);
Mat* mulMat(Mat* m1, Mat* m2);
Mat* dotMat(Mat* m1, Mat* m2);

Mat* sigMat(Mat* m);

Mat* tMat(Mat* m);
Mat* mCreate(int row, int col);
void mFill(Mat *m, int n);
void mDel(Mat *m);
void mDisplay(Mat *m);
Mat* mCopy(Mat *m);
void mSave(Mat* m, char* file_string);
Mat* mLoad(char* file_string);
void mRandom(Mat* m, int n);
int mMax(Mat* m);
Mat* reshape1D(Mat* m);

#endif