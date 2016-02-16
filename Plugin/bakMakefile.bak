Objs                    = Antenna.o 
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC -Wcpp -L/usr/lib64/ -lusb
DevFlags		=

#IncludeDirs     =  /opt/cactus/include ../

#IncludePaths            = $(IncludeDirs:%=-I%)

%.o: %.cc %.h 
	$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) -c -o $@ $<

all: $(Objs)
	$(CC) -std=c++11 -shared -L/usr/lib64/ -lusb -o libPh2_Antenna.so $(Objs) -pthread
	mv libPh2_Antenna.so lib

clean:
	rm -f *.o