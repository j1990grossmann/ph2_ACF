Objs                    = Calibration.o FastCalibration.o Channel.o HybridTester.o CMTester.o MuxTest.o Hameg4040.o AsyncSerial.o BufferedAsyncSerial.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC `root-config --cflags --evelibs` -Wcpp -L$(BOOST)/libs -lpthread -lncurses -lboost_thread -lboost_system
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=
#Boost
BOOST = /usr/local/include/boost

IncludeDirs     =  /opt/cactus/include ../ -I$(BOOST)/
# LinkParam = -L$(BOOST)/libs -lpthread -lncurses -lboost_thread -lboost_system

IncludePaths            = $(IncludeDirs:%=-I%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<

all: $(Objs) ../HWDescription/Definition.h
	$(CC) -std=c++11 -shared -o libPh2_Tools.so $(Objs) -pthread
	mv libPh2_Tools.so ../lib

clean:
	rm -f *.o
