#!/bin/env python

import cv
from pylab import *

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

        figure(1)
        show()

    def process(self):
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

        clf()
  
        ### left angle ####

        # go row-wise from left to right and note the position of the
        # first black pixel

        x0=arange(-(self.lowerlim-self.upperlim),0)
        y0=zeros(len(x0))

        for row in range(self.upperlim,self.lowerlim):
            for col in range(0,self.edges.cols):
                if self.edges[row,col]  == 0:
                    y0[row-self.upperlim]=col
                    cv.Circle(self.frame, (col,row), 1, cv.CV_RGB(255,0,0));
                    break

        # fit the contact angle
        y=y0-y0[0]
        x=(x0-max(x0))*-1

        plot(x,y,'bo')
        p=polyfit(x,y,2)
        xx=arange(min(x),max(x)+3)
        plot(xx,polyval(p,xx),'g-')

        if (p[1]<0):
            a=180-(arctan(1.0/abs(p[1]))*180/pi)
        else:
            a=(arctan(1.0/abs(p[1]))*180/pi)

        print a

        # draw a line showing the contact angle
        if a>90.0:
            cv.Line(self.frame,\
                        (y0[-1],self.lowerlim),\
                        (y0[-1]-100,self.lowerlim-100*tan((180.0-a)/180*pi)),\
                        cv.CV_RGB(0,255,0))
        else:
            cv.Line(self.frame,\
                        (y0[-1],self.lowerlim),\
                        (y0[-1]+100,self.lowerlim-100*tan((a)/180*pi)),\
                        cv.CV_RGB(0,255,0))


        ### right angle ###

        # go row-wise from right to left and note the position of the
        # first black pixel

        x0=arange(-(self.lowerlim-self.upperlim),0)
        y0=zeros(len(x0))

        for row in range(self.upperlim,self.lowerlim):
            for col in range(self.edges.cols-1,0,-1):
                if self.edges[row,col] == 0:
                    y0[row-self.upperlim]=col
                    cv.Circle(self.frame, (col,row), 1, cv.CV_RGB(255,0,0));
                    break

        # fit the contact angle
        y=y0-y0[0]
        x=(x0-max(x0))*-1

        plot(x,y,'ro')
        p=polyfit(x,y,2)
        xx=arange(min(x),max(x)+3)
        plot(xx,polyval(p,xx),'g-')
        axis('equal')
        grid(True)
        draw()

        if (p[1]>0):
            a=180-(arctan(1.0/abs(p[1]))*180/pi)
        else:
            a=(arctan(1.0/abs(p[1]))*180/pi)

        print a

        # draw a line showing the contact angle
        if a>90.0:
            cv.Line(self.frame,\
                        (y0[-1],self.lowerlim),\
                        (y0[-1]+100,self.lowerlim-100*tan((180.0-a)/180*pi)),\
                        cv.CV_RGB(0,255,0))
        else:
            cv.Line(self.frame,\
                        (y0[-1],self.lowerlim),\
                        (y0[-1]+100,self.lowerlim-100*tan((a)/180*pi)),\
                        cv.CV_RGB(0,255,0))

        
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

