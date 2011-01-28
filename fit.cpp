// Copyright (c) 2010,2011 Daniel Gruber <daniel@tydirium.org>

// Permission to use, copy, modify, and distribute this software for
// any purpose with or without fee is hereby granted, provided that the
// above copyright notice and this permission notice appear in all
// copies.

// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
// WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
// AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
// DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
// OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
// TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include "fit.h"


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
