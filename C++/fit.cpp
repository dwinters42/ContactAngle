#include "fit.h"
#include <iostream>

void printmatrix(cv::Mat mat) {
  int row, col; 
  
  std::cout << std::endl << std::endl;

  for (row=0; row<mat.rows; row++) {
    double* Mi = mat.ptr<double>(row);
    for (col=0;	col<mat.cols; col++) {
      std::cout << Mi[col] << " ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << std::endl;
}

void printarray(double *array, int numofpoints) {
  int row;

  std::cout << std::endl << std::endl;

  for (row=0; row<numofpoints; row++)
    std::cout << array[row] << ", ";

  std::cout << std::endl << std::endl;
}
  
cv::Mat vandermonde(int degree, double *x, int numofpoints) {
  int row, col;

  cv::Mat v(numofpoints,degree+1,CV_64FC1);

  for (row=0; row<numofpoints; row++) {
    double* Mi = v.ptr<double>(row);
    for (col=0; col<degree+1; col++) {
      Mi[col]=pow(x[row],degree-col);
    }
  }
  return v;
}


int polyfit(double *fit_results, double *xvals, double *yvals, int degree, int numofpoints) {
  int row;
  cv::Mat y(numofpoints,1,CV_64FC1,yvals);
  
  cv::Mat v;
  v=vandermonde(degree,xvals,numofpoints);
	
  cv::Mat vtrans, vadj;
  cv::transpose(v,vtrans);
  cv::invert(vtrans*v,vadj);

  cv::Mat x;
  x=vadj*vtrans*y;

  for (row=0;row<degree+1;row++) {
    double* Mi = x.ptr<double>(row);
    fit_results[row]=Mi[0];
  }

  return 0;
}
