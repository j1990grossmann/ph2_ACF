Objs                    = Calibration.o FastCalibration.o Channel.o HybridTester.o CMTester.o MuxTest.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC `root-config --cflags --evelibs` -Wcpp 
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=

#Boost
BOOST = /usr/local/include/boost/

IncludeDirs     =  /opt/cactus/include ../ $(BOOST)
# LinkParam = -L$(BOOST)/libs -lpthread -lncurses -lboost_thread -lboost_system

IncludePaths            = $(IncludeDirs:%=-I%) 

LibraryDirs = /usr/local/lib

LibraryPaths = $(LibraryDirs:%=-L%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) $(LibraryPaths) -lpthread -c -o $@ $< 

all: $(Objs) ../HWDescription/Definition.h
	$(CC) -std=c++11 -shared -o libPh2_Tools.so $(Objs) -pthread  
	mv libPh2_Tools.so ../lib

clean:
	rm -f *.o
