#!/usr/bin/make

CXXFLAGS = -g -Wall $(shell pkg-config --cflags opencv) $(shell wx-config --cxxflags)
LDFLAGS = $(shell pkg-config --libs opencv) $(shell wx-config --libs)
#OPENCV_BUILD_3RDPARTY_LIBS = on

all: ContactAngle

ContactAngle: main.o MyFrame.o
	${CXX} -o ContactAngle ${LDFLAGS} main.o MyFrame.o

clean:
	rm -rf ContactAngle *.o

install:
	install -d /usr/local/bin
	install ContactAngle /usr/local/bin/ContactAngle

.PHONY : all ContactAngle clean install