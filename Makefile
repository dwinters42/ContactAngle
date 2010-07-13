#!/usr/bin/make

CXXFLAGS = -g -Wall $(shell pkg-config --cflags opencv)
LDFLAGS = $(shell pkg-config --libs opencv)

all: ContactAngle
