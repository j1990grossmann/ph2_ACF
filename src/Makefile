CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC `root-config --cflags --evelibs`
#DevFlags                    = -D__CBCDAQ_DEV__
DevFlags                   =
IncludeDirs     =  /opt/cactus/include ../ /usr/local/include/boost

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /opt/cactus/lib ../lib /usr/local/lib

LibraryPaths = $(LibraryDirs:%=-L%)

ExternalObjects = $(LibraryPaths) -lpthread  -lcactus_extern_pugixml -lcactus_uhal_log -lcactus_uhal_grammars -lcactus_uhal_uhal -lncurses -lboost_thread -lboost_system `root-config --glibs` -lSMUTools -lPh2_Interface -lPh2_Description -lPh2_System -lPh2_Tools -lPh2_Utils


%.o: %.cc %.h
	$(CXX) -std=c++11 $(DevFlags) $(CCFlags) $(UserCCFlags) $(CCDefines) $(IncludePaths) -c -o $@ $<

binaries=systemtest calibrate interfacetest datatest hybridtest cmtest analogmuxtest
all: clean $(binaries)

systemtest: systemtest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin

calibrate: calibrate.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin

interfacetest: interfacetest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin

datatest: datatest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin

hybridtest: hybridtest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin

cmtest: cmtest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	cp $@ ../bin
	
analogmuxtest: analogmuxtest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects) 
	cp $@ ../bin

.PHONY: clean

clean:
	rm -f $(binaries) *.o
