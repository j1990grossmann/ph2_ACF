Objs            = AsyncSerial.o BufferedAsyncSerial.o Hameg4040.o Keithley2700.o 
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC -Wcpp 
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=

#Boost
IncludeDirs     =  /usr/local/include/boost/
LinkParam = -lpthread -lncurses -lboost_thread -lboost_system
IncludePaths            = $(IncludeDirs:%=-I%) 
LibraryDirs = /usr/local/lib .
LibraryPaths = $(LibraryDirs:%=-L%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) $(LibraryPaths) $(LinkParam) -c -o $@ $<

all: $(Objs) 
	$(CC) -std=c++11 -shared -o libSMUTools.so $(Objs) -pthread 
	$(CXX) -std=c++11 $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(LibraryPaths) -o async main.cc -lSMUTools $(LinkParam)
	cp -p libSMUTools.so ../lib

clean:
	rm -f *.o
	rm -f *.so
