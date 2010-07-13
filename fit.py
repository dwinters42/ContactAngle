from pylab import *


ion()
x0,y0=loadtxt('data.txt',unpack=True)
x=y0-max(y0)
y=(x0-max(x0))*-1

clf()
plot(x,y,'o')
p=polyfit(x[-10:],y[-10:],1)
xx=arange(min(x),max(x)+3)
plot(xx,polyval(p,xx),'r-')
axis('equal')
axhline(0)
grid(True)

a=180-(arctan(-p[0]+p[1])*180/pi)
print a
