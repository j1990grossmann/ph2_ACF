CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -w -Wall -pedantic -std=c++11 -fPIC 
CCFlagsRoot	= `root-config --cflags --evelibs --glibs`
#DevFlags                    = -D__CBCDAQ_DEV__
DevFlags                   =

IncludeDirs     =  ../../ ../SCPITools ../SCPIDevInstrument ../libscpi/inc/scpi

IncludePaths            = $(IncludeDirs:%=-I%)

LibraryDirs = /usr/local/lib ../libscpi ../SCPIlib
LibraryPaths = $(LibraryDirs:%=-L%)
ExternalObjects= $(LibraryPaths)  -lSCPITools -lSCPIUtils -lpthread -lncurses -lboost_thread -lboost_system -lboost_filesystem
ExtObjectsRoot= 
# ExtObjectsRoot= -lPh2_Tools 
BinDir=../SCPIbin
binaries=noisescan hameg hv_control parser


PROG = test-parser

SRCS = Parsertest.cc ../SCPIDevInstrument/scpi-def.c
CFLAGS += -Wextra -Wmissing-prototypes -Wimplicit -I ../libscpi/inc/
LDFLAGS += ../libscpi/dist/libscpi.a -Wl,--as-needed


.PHONY: clean all parser

all: rootflags clean $(binaries)
# all: rootflags clean $(binaries) PROG

rootflags:
	$(eval CCFlags += $(CCFlagsRoot))
	$(eval ExternalObjects += $(ExtObjectsRoot))

noroot: clean $(binariesNoRoot)

hameg: Hameg4040.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	-mkdir -p $(BinDir)
	-mv $@ $(BinDir)/

keithleytest: KeithleyTest.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	-mkdir -p $(BinDir)
	-mv $@ $(BinDir)/

noisescan: NoiseScan.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	-mkdir -p $(BinDir)
	-mv $@ $(BinDir)/

hv_control: HV_Controller.cc
	$(CXX) -std=c++11 $(CCFlags) -o $@ $< $(IncludePaths) $(ExternalObjects)
	-mkdir -p $(BinDir)
	-mv $@ $(BinDir)/
# parser: Parsertest.cc
# 	$(CXX) -std=c++11 $(CCFlags) -Wextra -Wmissing-prototypes -Wimplicit -I../../libscpi/inc/ -o $@ $< $(IncludePaths) $(ExternalObjects) -Wl,--as-needed
# 	-mkdir -p $(BinDir)
# 	-mv $@ $(BinDir)/
parser: $(PROG)

OBJS = $(SRCS:.c=.o)

.cc.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)
	-mkdir -p $(BinDir)
	-mv $@ $(BinDir)/
clean:
	@-rm -f *.o
	@-rm -f $(binaries)
	@-rm -f $(binaries:%=$(BinDir)/%)
	
