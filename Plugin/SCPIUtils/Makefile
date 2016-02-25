Objs            = argvparser.o pugixml.o 
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -fPIC -Wcpp -L/usr/lib64/ -L/usr/lib/x86_64-linux-gnu
SCPIlib         = ../SCPIlib
#DevFlags                   = -D__CBCDAQ_DEV__
DevFlags		=
NAME = pugixml
IncludeDirs     =  ../

IncludePaths            = $(IncludeDirs:%=-I%)

.PHONY: clean print


%.o: %.cc %.h 
	@$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<
%.o: %.cpp %.hpp
	@$(CXX) -std=c++11  $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<
all: print $(Objs)
	@$(CC) -std=c++11 -shared -L/usr/lib64/ -o libSCPIUtils.so $(Objs) -pthread
	@-if [ ! -d "$(SCPIlib)" ]; then mkdir "$(SCPIlib)"; fi
	@-mv libSCPIUtils.so "$(SCPIlib)"

print:
	@echo '****************************'
	@echo 'Building SCPIUtils'
	@echo '****************************'
clean:
	@-rm -f *.o
	@-rm -f $(SCPIlib)/libSCPIUtils.so
