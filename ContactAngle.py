#!/bin/env python

import cv
from pylab import *

class ContactAngleFinder:
    def __init__(self,infile):
        self.infile=infile
        self.cap=cv.CaptureFromFile(self.infile)

        self.fwidth=cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_WIDTH)
        self.fheight=cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_HEIGHT)
        self.numframes=cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_COUNT)

        self.thresh=110
        self.framenum=0
        self.baseleft=264
        self.baseright=264
        self.upperlim=220

        cv.NamedWindow("ContactAngle",1);
        cv.CreateTrackbar("Frame", "ContactAngle",\
                              self.framenum, self.numframes, self.set_framenum);
        cv.CreateTrackbar("Threshold", "ContactAngle",\
                              self.thresh, 255, self.set_thresh)
        cv.CreateTrackbar("BaseLeft", "ContactAngle", \
                              self.baseleft, self.fheight, self.set_baseleft)
        cv.CreateTrackbar("BaseRight", "ContactAngle", \
                              self.baseright, self.fheight, self.set_baseright)
        cv.CreateTrackbar("UpperLimit", "ContactAngle",\
                              self.upperlim, self.fheight, self.set_upperlim)

        cv.SetMouseCallback("ContactAngle",self.processAll)

        figure(1)
        show()

    def processAll(self,event,x,y,flags,param):
        if event == cv.CV_EVENT_LBUTTONDOWN:
            df=file('data.txt','w')
            df.write('# contact angle data for file %s\n' % self.infile)
 
            for ii in range(self.numframes):
                print('processing frame %i' % ii)
                self.framenum=ii
                frame,al,ar=self.process()
                df.write('%i, %f, %f\n' % (frame,al,ar))

            df.close()

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
        
        cv.Line(self.frame,(0,self.baseleft),(self.fwidth,self.baseleft),cv.CV_RGB(0,255,0))
        cv.Line(self.frame,(0,self.upperlim),(self.fwidth,self.upperlim),cv.CV_RGB(0,0,255))

        clf()
  
        ### left angle ####

        # go row-wise from left to right and note the position of the
        # first black pixel

        x0=arange(-(self.baseleft-self.upperlim),0)
        y0=zeros(len(x0))

        for row in range(self.upperlim,self.baseleft):
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
            al=180-(arctan(1.0/abs(p[1]))*180/pi)
        else:
            al=(arctan(1.0/abs(p[1]))*180/pi)

        # used for tilt correction further down
        basepointleft=(y0[-1],self.baseleft)

        ### right angle ###

        # go row-wise from right to left and note the position of the
        # first black pixel

        x0=arange(-(self.baseright-self.upperlim),0)
        y0=zeros(len(x0))

        for row in range(self.upperlim,self.baseright):
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
            ar=180-(arctan(1.0/abs(p[1]))*180/pi)
        else:
            ar=(arctan(1.0/abs(p[1]))*180/pi)

        # used for tilt correction further down
        basepointright=(y0[-1],self.baseright)

        # draw lines showing the contact angle

        if al>90.0:
            cv.Line(self.frame, basepointleft, \
                        (basepointleft[0]-100, \
                             basepointleft[1]-100*tan((180.0-al)/180*pi)),\
                        cv.CV_RGB(0,255,0))
        else:
            cv.Line(self.frame, basepointleft, \
                        (basepointleft[0]+100, \
                             basepointleft[1]-100*tan((al)/180*pi)),\
                        cv.CV_RGB(0,255,0))

        if ar>90.0:
            cv.Line(self.frame, basepointright, \
                        (basepointright[0]+100, \
                             basepointright[1]-100*tan((180.0-ar)/180*pi)),\
                        cv.CV_RGB(0,255,0))
        else:
            cv.Line(self.frame, basepointright, \
                        (basepointright[0]-100, \
                             basepointright[1]-100*tan((ar)/180*pi)),\
                        cv.CV_RGB(0,255,0))

        
        cv.ShowImage("ContactAngle", self.frame)

        return (self.framenum,al,ar)

    def run(self):
        self.process()
        cv.WaitKey(0)

    def set_framenum(self,pos):
        self.framenum=pos
        self.process()

    def set_thresh(self,pos):
        self.thresh=pos
        self.process()

    def set_baseleft(self,pos):
        self.baseleft=pos
        self.process()

    def set_baseright(self,pos):
        self.baseright=pos
        self.process()

    def set_upperlim(self,pos):
        self.upperlim=pos
        self.process()

if __name__ == '__main__':

    ContactAngleFinder('test.avi').run()

