#!/bin/bash

DIRECTORY=Plugin

CACTUSBIN=/opt/cactus/bin
CACTUSLIB=/opt/cactus/lib
CACTUSINCLUDE=/opt/cactus/include

# BOOST
BOOST_LIB=/opt/cactus/lib
BOOST_INCLUDE=/opt/cactus/include

#CACTUS
if [[ -d "$CACTUSBIN" ]] && [[ -d "$CACTUSLIB" ]] && [[ -d "$CACTUSINCLUDE" ]] && [[ -d "$BOOST_LIB" ]] && [[ -d "$BOOST_INCLUDE" ]]; then
    printf "Cactus found export environment variables for Cactus and BOOST\n"
    export $CACTUSBIN
    export $CACTUSLIB
    export $CACTUSINCLUDE
    export $BOOST_LIB
    export $BOOST_INCLUDE 
else
    printf "Cactus not found try to compile Tools only\n"
fi
#ROOT
if [ -n "$ROOTSYS" ]; then
    echo "\$ROOTSYS = $ROOTSYS"
    echo "source thisroot.sh"
    source $ROOTSYS/bin/thisroot.sh
else
    echo -e "\e[31mROOTSYS environment variable not set.\e[39m"
    echo -e "\e[31mPlease export ROOTSYS pointing to a valid root installation.\e[39m"
fi

#Ph2_ACF
export BASE_DIR=$(pwd)

#ANTENNA
export ANTENNALIB=CMSPh2_AntennaDriver/lib

export PATH=$BASE_DIR/bin:$PATH
export LD_LIBRARY_PATH=$BASE_DIR/$ANTENNALIB:$BASE_DIR/RootWeb/lib:/opt/cactus/lib:$BASE_DIR/lib:${LD_LIBRARY_PATH}

if [ -d "$BASE_DIR/$DIRECTORY" ]; then
    printf "Plugin found\n Add LD_LIBRARY_PATH and PATH for Plugins\n"
    source "$BASE_DIR/$DIRECTORY/setup.sh"
fi
