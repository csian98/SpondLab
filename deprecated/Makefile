PATH := $(PATH):$(PWD)/src

all :sessa.o utility.o configReader.o exception.o filesystem.o IPC2py.o CNCommunicator.o main.o main cleaner

CC = g++
CXXFLAGS = -Wall -O2 -std=c++2b
OBJS = sessa.o utility.o configReader.o exception.o filesystem.o IPC2py.o CNCommunicator.o main.o

sessa.o : sessa.hpp sessa.cpp
	$(CC) $(CXXFLAGS) -c sessa.cpp

utility.o : utility.hpp utility.cpp
	$(CC) $(CXXFLAGS) -c utility.cpp

configReader.o: configReader.hpp configReader.cpp
	$(CC) $(CXXFLAGS) -c configReader.cpp

exception.o : exception.hpp exception.cpp
	$(CC) $(CXXFLAGS) -c exception.cpp

filesystem.o : filesystem.hpp filesystem.cpp
	$(CC) $(CXXFLAGS) -c filesystem.cpp

IPC2py.o : IPC2py.hpp IPC2py.cpp
	$(CC) $(CXXFLAGS) -c IPC2py.cpp

CNCommunicator.o : CNCommunicator.hpp CNCommunicator.cpp
	$(CC) $(CXXFLAGS) -c CNCommunicator.cpp

main.o : sessa.hpp main.cpp
	$(CC) $(CXXFLAGS) -c main.cpp

main : $(OBJS)
	$(CC) $(CXXFLAGS) $(OBJS) -o main

cleaner : cleaner.cpp exception.o
	$(CC) $(CXXFLAGS) -o cleaner cleaner.cpp exception.o

clean :
	rm -f *.o
