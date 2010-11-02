#!/usr/bin/make

CXXFLAGS = -g -Wall $(shell pkg-config --cflags opencv plplotd-wxwidgets) $(shell wx-config --cxxflags)
LDFLAGS = $(shell pkg-config --libs opencv plplotd-wxwidgets) $(shell wx-config --libs)

CXXFILES=${wildcard *.cpp}
OBJECTS=${patsubst %.cpp, %.o, ${CXXFILES}}
HEADERS=${wildcard *.h}

all: ContactAngle

ContactAngle: ${OBJECTS} ${HEADERS}
	${CXX} -o ContactAngle ${LDFLAGS} ${OBJECTS}

clean:
	rm -rf ContactAngle *.o

install:
	install -d /usr/local/bin
	install ContactAngle /usr/local/bin/ContactAngle

.PHONY : all ContactAngle clean install