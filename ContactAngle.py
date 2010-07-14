#!/bin/env python

import cv

class ContactAngleFinder:
    def __init__(self,infile):
        self.cap=cv.CaptureFromFile(infile)

        self.fwidth=cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_WIDTH)
        self.fheight=cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_HEIGHT)
        self.numframes=cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_COUNT)

        self.thresh=110
        self.framenum=0
        self.lowerlim=264
        self.upperlim=220

        cv.NamedWindow("ContactAngle",1);
        cv.CreateTrackbar("LowerLimit", "ContactAngle", \
                              self.lowerlim, self.fheight, self.set_lowerlim)
        cv.CreateTrackbar("UpperLimit", "ContactAngle",\
                              self.upperlim, self.fheight, self.set_upperlim)
        cv.CreateTrackbar("Threshold", "ContactAngle",\
                              self.thresh, 255, self.set_thresh)
        cv.CreateTrackbar("Frame", "ContactAngle",\
                              self.framenum, self.numframes, self.set_framenum);


    def process(self):
        print(self.framenum,self.thresh,self.lowerlim,self.upperlim)
        cv.SetCaptureProperty(self.cap,cv.CV_CAP_PROP_POS_FRAMES,self.framenum)
        self.frame=cv.QueryFrame(self.cap)
        self.edges=cv.CreateMat(self.fheight,self.fwidth,cv.CV_8UC1)
        cv.CvtColor(self.frame, self.edges, cv.CV_BGR2GRAY)
        # cv.GaussianBlur(self.edges, self.edges, Size(7,7), 1.5, 1.5)
        # cv.Canny(self.edges, self.edges, low, high, 3)
        cv.EqualizeHist(self.edges, self.edges)
        cv.Threshold(self.edges,self.edges,self.thresh,255,cv.CV_THRESH_BINARY)
  
        cv.CvtColor(self.edges, self.frame, cv.CV_GRAY2BGR);
        
        cv.Line(self.frame,(0,self.lowerlim),(self.fwidth,self.lowerlim),cv.CV_RGB(0,255,0))
        cv.Line(self.frame,(0,self.upperlim),(self.fwidth,self.upperlim),cv.CV_RGB(0,0,255))
  
        # // go row-wise from left to right and note the position of the
        # // first black pixel
        # int row, col;
        # for (row=self.upperlim;row<self.lowerlim;row++) {
        #   const char* Mi = self.edges.ptr<char>(row);
        #   for (col=0;col<self.edges.cols;col++) {
        #     if (Mi[col]==0) {
        #       std::cout << row << " " << col << std::endl;
        #       circle(self.frame, Point(col,row),1, CV_RGB(255,0,0));
        #       break;
        #     }
        #   }
        # }
    
        cv.ShowImage("ContactAngle", self.frame)

    def run(self):
        self.process()
        cv.WaitKey(0)

    def set_framenum(self,pos):
        self.framenum=pos
        self.process()

    def set_thresh(self,pos):
        self.thresh=pos
        self.process()

    def set_lowerlim(self,pos):
        self.lowerlim=pos
        self.process()

    def set_upperlim(self,pos):
        self.upperlim=pos
        self.process()

if __name__ == '__main__':

    ContactAngleFinder('test.avi').run()

