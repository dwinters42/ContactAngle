#include "cv.h"
#include "highgui.h"
#include <string>
#include <iostream>

using namespace cv;

int main(int, char**)
{
  VideoCapture cap("test.avi");
  if(!cap.isOpened()) {
    std::cerr << "could not open infile " << std::endl;
    return -1;
  }

  int fwidth=cap.get(CV_CAP_PROP_FRAME_WIDTH);
  int fheight=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
  int numframes=cap.get(CV_CAP_PROP_FRAME_COUNT);
  
  Mat edges;
  Mat frame;

  int thresh=110,framenum=0;
  int lowerlim=264, upperlim=220;

  namedWindow("ContactAngle",1);
  //  namedWindow("LeftAngle",2);
  
  createTrackbar("LowerLimit", "ContactAngle", &lowerlim, fheight);
  createTrackbar("UpperLimit", "ContactAngle", &upperlim, fheight);
  createTrackbar("Threshold", "ContactAngle", &thresh, 1000);
  createTrackbar("Frame", "ContactAngle", &framenum, numframes);

  for(;;)
    {
      // XXX don't do busy-waiting
      cap.set(CV_CAP_PROP_POS_FRAMES,framenum);
      cap >> frame;


      cvtColor(frame, edges, CV_BGR2GRAY);
      //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
      //Canny(edges, edges, low, high, 3);
      equalizeHist(edges, edges);
      threshold(edges,edges,thresh,255,THRESH_BINARY);

      cvtColor(edges, frame, CV_GRAY2BGR);

      line(frame,Point(0,lowerlim),Point(fwidth,lowerlim),CV_RGB(0,255,0));
      line(frame,Point(0,upperlim),Point(fwidth,upperlim),CV_RGB(0,0,255));

      // go row-wise from left to right and note the position of the
      // first black pixel
      int row, col;
      for (row=upperlim;row<lowerlim;row++) {
	const char* Mi = edges.ptr<char>(row);
	for (col=0;col<edges.cols;col++) {
	  if (Mi[col]==0) {
	    std::cout << col << std::endl;
	    circle(frame, Point(col,row),1, CV_RGB(255,0,0));
	    break;
	    }
	}
      }

      imshow("ContactAngle", frame);

      if(waitKey(30) >= 0) 
	break;
    }

  return 0;
}

