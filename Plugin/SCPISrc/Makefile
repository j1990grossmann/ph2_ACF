CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC 
CCFlagsRoot	= `root-config --cflags --evelibs --glibs`
#DevFlags                    = -D__CBCDAQ_DEV__
DevFlags                   =

IncludeDirs     =  ../ ../SMUTools

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /usr/local/lib ../lib 
LibraryPaths = $(LibraryDirs:%=-L%)

ExternalObjects= $(LibraryPaths)  -lSMUTools -lPh2_no_uhal_Utils -lpthread -lncurses -lboost_thread -lboost_system -lboost_filesystem
ExtObjectsRoot= 
# ExtObjectsRoot= -lPh2_Tools 

binaries=noisescan hameg hv_control

all: rootflags clean $(binaries) 

rootflags:
	$(eval CCFlags += $(CCFlagsRoot))
	$(eval ExternalObjects += $(ExtObjectsRoot))

noroot: clean $(binariesNoRoot)

hameg: Hameg4040.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../binSMUTools

keithleytest: KeithleyTest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../binSMUTools

noisescan: NoiseScan.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../binSMUTools

hv_control: HV_Controller.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	mv $@ ../binSMUTools

.PHONY: clean

clean:
	rm -f *.o $(binaries)
