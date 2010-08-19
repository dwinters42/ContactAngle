#ifndef FIT_H
#define FIT_H

#include <cv.h>

void printarray(double *, int);
void printmatrix(cv::Mat);

cv::Mat vandermonde(int, double*, int);
int polyfit(double*, double*, double*, int, int);

#endif /* FIT_H */
