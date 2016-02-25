CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC 
CCFlagsRoot	= `root-config --cflags --evelibs --glibs`
#DevFlags                    = -D__CBCDAQ_DEV__
DevFlags                   =

IncludeDirs     =  ../../ ../SCPITools

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /usr/local/lib ../libscpi ../SCPIlib
LibraryPaths = $(LibraryDirs:%=-L%)
ExternalObjects= $(LibraryPaths)  -lSCPITools -lSCPIUtils -lpthread -lncurses -lboost_thread -lboost_system -lboost_filesystem
ExtObjectsRoot= 
# ExtObjectsRoot= -lPh2_Tools 

binaries=noisescan hameg hv_control

.PHONY: clean

all: rootflags clean $(binaries) 

rootflags:
	$(eval CCFlags += $(CCFlagsRoot))
	$(eval ExternalObjects += $(ExtObjectsRoot))

noroot: clean $(binariesNoRoot)

hameg: Hameg4040.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	@-mv $@ ../SCPIbin

keithleytest: KeithleyTest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	@-mv $@ ../SCPIbin

noisescan: NoiseScan.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	@-mv $@ ../SCPIbin

hv_control: HV_Controller.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	@-mv $@ ../SCPIbin



clean:
	@-rm -f *.o
	@-rm -f $(binaries:%=../SCPIbin/%)
