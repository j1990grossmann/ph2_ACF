Objs            = AsyncSerial.o BufferedAsyncSerial.o Hameg4040.o Keithley2700.o Keithley2410.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC -Wcpp 
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=

#Boost
IncludeDirs     =  /usr/local/include/boost/
IncludePaths            = $(IncludeDirs:%=-I%) 
LinkParam = -lpthread -lncurses -lboost_thread -lboost_system
LibraryDirs = /usr/local/lib ../lib
LibraryPaths = $(LibraryDirs:%=-L%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) $(LibraryPaths) $(LinkParam) -c -o $@ $<

all: $(Objs) 
	$(CC) -std=c++11 -shared -o libSMUTools.so $(Objs) -pthread 
	mv libSMUTools.so ../lib
# 	$(CXX)  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(LibraryPaths) -o async main.cc -lSMUTools $(LinkParam)


clean:
	rm -f *.o
	rm -f *.so
