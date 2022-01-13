CC=/home/computador/buildroot/buildroot-2021.02.5.2/output/host/bin/arm-buildroot-linux-uclibcgnueabihf-g++

FLAGS=-lsqlite3 -lpthread

TARGET=test.cpp

DEPS=#

OBJCOMP=-otest.rasp

INCLUDE=.

FLAGS+=-I$(INCLUDE)

all: myProgram

myProgram: test.o
	$(CC) -lm -o myProgram test.o $(FLAGS)

main.o: test.cpp
	$(CC) -o -c test.cpp test.h

addSorted.o: buffer.cpp buffer.h
	$(CC) -o -c buffer.cpp

