Objs            = AsyncSerial.o BufferedAsyncSerial.o Initserial.o Hameg4040.o Keithley2700.o Keithley2410.o RunCounter.o Scan.o HV_control.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC -Wcpp 
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=

IncludeDirs=$(BOOST_INCLUDE)

#Check the LibraryPath if it is there

IncludePaths            = $(IncludeDirs:%=-I%) 
LinkParam = -lpthread -lncurses -lboost_thread -lboost_system -lboost_filesystem -lboost_tokenizer
LibraryDirs = /usr/local/lib ../lib
LibraryPaths = $(LibraryDirs:%=-L%)

%.o: %.cc %.h
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) $(LibraryPaths) $(LinkParam) -c -o $@ $<

SetLibraryPath:
	@echo  "BOOST_INCLUDE at $(BOOST_INCLUDE)"
	@echo  "BOOST_LIB at $(BOOST_LIB)"
all: $(Objs) 
	$(CC) -std=c++11 -shared -o libSMUTools.so $(Objs) -pthread
	mv libSMUTools.so ../SCPIlib/libSMUTools.so
# 	$(CXX)  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(LibraryPaths) -o async main.cc -lSMUTools $(LinkParam)
# 	$(CXX)  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(LibraryPaths) -o keithleytest test2410.cc -lSMUTools $(LinkParam)
clean:
	rm -f *.o
	rm -f *.so
	rm -f ../SCPIlib/libSMUTools.so

