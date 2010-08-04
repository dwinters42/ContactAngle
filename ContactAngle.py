#!/bin/env python

import cv, sys
from pylab import *

class ContactAngleFinder:
    def __init__(self,infile):
        self.infile=infile
        self.cap=cv.CaptureFromFile(self.infile)

        self.fwidth=int(cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_WIDTH))
        self.fheight=int(cv.GetCaptureProperty(self.cap,cv.CV_CAP_PROP_FRAME_HEIGHT))
        self.numframes=int(cv.GetCaptureProperty(self.cap,\
                                                 cv.CV_CAP_PROP_FRAME_COUNT))

        self.thresh=110
        self.framenum=0
        self.baseleft=261
        self.baseright=261
        self.fitrange=122

        cv.NamedWindow("ContactAngle",1);
        cv.CreateTrackbar("Frame", "ContactAngle",\
                              self.framenum, self.numframes, self.set_framenum);
        cv.CreateTrackbar("Threshold", "ContactAngle",\
                              self.thresh, 255, self.set_thresh)
        cv.CreateTrackbar("BaseLeft", "ContactAngle", \
                              self.baseleft, self.fheight, self.set_baseleft)
        cv.CreateTrackbar("BaseRight", "ContactAngle", \
                              self.baseright, self.fheight, self.set_baseright)
        cv.CreateTrackbar("Fitrange", "ContactAngle",\
                              self.fitrange, self.fheight, self.set_fitrange)

        cv.SetMouseCallback("ContactAngle",self.processAll)

        figure(1)
        show()

    def processAll(self,event,x,y,flags,param):
        if event == cv.CV_EVENT_LBUTTONDOWN:
            oldframenum=self.framenum

            outfile=self.infile[:self.infile.index('.')]+'.txt'
            print('output goes to %s' % outfile)
            df=file(outfile,'w')
            df.write('# contact angle data for file %s\n' % self.infile)
            df.write('# thresh = %i\n' % self.thresh)
            df.write('# basepointleft = (%i,%i)\n' % (self.basepointleft[0],self.basepointleft[1]))
            df.write('# basepointright = (%i,%i)\n' % (self.basepointright[0],self.basepointright[1]))
            df.write('# fitrange = %i\n' % self.fitrange)

            tilt,basepoint=self.getTilt()
            print('tilt=%f' % tilt)
  
            for ii in range(self.numframes):
                print('processing frame %i' % ii)
                self.framenum=ii
                frame,al,ar=self.process(tilt,basepoint)
                df.write('%i, %f, %f\n' % (frame,al,ar))

            df.close()

            self.framenum=oldframenum
            self.process()

    def process(self,tilt=0.0,basepoint=(0,0)):

        cv.SetCaptureProperty(self.cap,cv.CV_CAP_PROP_POS_FRAMES,self.framenum)
        self.frame=cv.QueryFrame(self.cap)
        self.edges=cv.CreateMat(self.fheight,self.fwidth,cv.CV_8UC1)
        cv.CvtColor(self.frame,self.edges, cv.CV_BGR2GRAY)
        cv.EqualizeHist(self.edges,self.edges)
        cv.Threshold(self.edges,self.edges,self.thresh,255,cv.CV_THRESH_BINARY)
        cv.CvtColor(self.edges,self.frame,cv.CV_GRAY2BGR);

        if tilt != 0.0:
            temp=cv.CreateMat(self.fheight,self.fwidth,cv.CV_8UC1)
            rot_mat = cv.CreateMat(2,3,cv.CV_32FC1)
            cv.GetRotationMatrix2D(basepoint, -1*tilt, 1.0, rot_mat)
            cv.WarpAffine(self.edges, temp, rot_mat)
            cv.Copy(temp,self.edges)
            temp=cv.CreateMat(self.fheight,self.fwidth,cv.CV_8UC3)
            cv.WarpAffine(self.frame, temp, rot_mat)
            cv.Copy(temp,self.frame)
            baseleft=self.baseleft
            baseright=baseleft
        else:
            baseleft=self.baseleft
            baseright=self.baseright

        clf()
  
        ### left angle ####

        # go row-wise from left to right and note the position of the
        # first black pixel

        x0=arange(-(baseleft-self.fitrange),0)
        y0=zeros(len(x0))

        for row in range(self.fitrange,baseleft):
            for col in range(50,self.edges.cols-50):
                if self.edges[row,col] == 0:
                    y0[row-self.fitrange]=col
                    cv.Circle(self.frame, (col,row), 1, cv.CV_RGB(255,0,0));
                    break

        # fit the contact angle
        y=y0-y0[0]
        x=(x0-max(x0))*-1

        subplot(121)
        plot(y,x,'bo')
        p=polyfit(x,y,2)
        xx=arange(min(x),max(x)+3)
        plot(polyval(p,xx),xx,'g-')
        axis('equal')
        grid(True)

        if (p[1]<0):
            al=180-(arctan(1.0/abs(p[1]))*180/pi)
        else:
            al=(arctan(1.0/abs(p[1]))*180/pi)

        # used for tilt correction further down
        self.basepointleft=(y0[-1],baseleft)

        ### right angle ###

        # go row-wise from right to left and note the position of the
        # first black pixel

        x0=arange(-(baseright-self.fitrange),0)
        y0=zeros(len(x0))

        for row in range(self.fitrange,baseright):
            for col in range(self.edges.cols-51,50,-1):
                if self.edges[row,col] == 0:
                    y0[row-self.fitrange]=col
                    cv.Circle(self.frame, (col,row), 1, cv.CV_RGB(255,0,0));
                    break

        # fit the contact angle
        y=y0-y0[0]
        x=(x0-max(x0))*-1

        subplot(122)
        plot(y,x,'ro')
        p=polyfit(x,y,2)
        xx=arange(min(x),max(x)+3)
        plot(polyval(p,xx),xx,'g-')
        axis('equal')
        grid(True)
        draw()

        if (p[1]>0):
            ar=180-(arctan(1.0/abs(p[1]))*180/pi)
        else:
            ar=(arctan(1.0/abs(p[1]))*180/pi)

        # used for tilt correction further down
        self.basepointright=(y0[-1],baseright)

        cv.Line(self.frame,self.basepointleft,self.basepointright,cv.CV_RGB(0,255,0))

        # draw lines showing the contact angles
        if al>90.0:
            cv.Line(self.frame, self.basepointleft, \
                        (self.basepointleft[0]-100, \
                             self.basepointleft[1]-100*tan((180.0-al)/180*pi)),\
                        cv.CV_RGB(0,255,0))
        else:
            cv.Line(self.frame, self.basepointleft, \
                        (self.basepointleft[0]+100, \
                             self.basepointleft[1]-100*tan((al)/180*pi)),\
                        cv.CV_RGB(0,255,0))

        if ar>90.0:
            cv.Line(self.frame, self.basepointright, \
                        (self.basepointright[0]+100, \
                             self.basepointright[1]-100*tan((180.0-ar)/180*pi)),\
                        cv.CV_RGB(0,255,0))
        else:
            cv.Line(self.frame, self.basepointright, \
                        (self.basepointright[0]-100, \
                             self.basepointright[1]-100*tan((ar)/180*pi)),\
                        cv.CV_RGB(0,255,0))


        cv.ShowImage("ContactAngle", self.frame)
        cv.WaitKey(30)
        
        return (self.framenum,al,ar)

    def getTilt(self):
        for col in range(50,self.edges.cols-50):
            if self.edges[self.baseleft,col] == 0:
                x1=col
                break

        for col in range(self.edges.cols-51,50,-1):
            if self.edges[self.baseright,col] == 0:
                x2=col
                break

        tilt=arctan(float(self.baseleft-self.baseright)/float(x2-x1))*180/pi
        return (tilt,(x1,self.baseleft))

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

    def set_fitrange(self,pos):
        self.fitrange=pos
        self.process()

if __name__ == '__main__':

    infile=sys.argv[1]
    print('loading %s' % infile)
    ContactAngleFinder(infile).run()

