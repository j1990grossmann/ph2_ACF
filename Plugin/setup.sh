#!/bin/bash

echo -e "\e[39mExporting environment variables from dependent libraries for Plugins \e[39m" 
if [[ -z "$CACTUSBIN" ]] || [[ -z "$CACTUSLIB" ]] || [[ -z "$CACTUSINCLUDE" ]] || [[ -z "$BOOST_LIB" ]] || [[ -z "$BOOST_INCLUDE" ]]; then
echo -e "\e[31m At least one of the environment PH2_ACF software environment variables is missing\e[39m"
echo -e "\e[31m Try standalone compilation of Plugins and automatic definition of environment vars\e[39m"
fi
#BASE_DIR_PLUGIN
export BASE_DIR_PLUGIN=$(pwd)
# BOOST
if [ -n "$BOOST_LIB" ]; then
    echo "BOOST_LIB = $BOOST_LIB"
else
    echo -e "\e[32mStandalone compilation BOOST_LIB and BOOST_INCLUDE user defined.\e[39m"
    echo -e "\e[32mTry to find a automatic a valid boost installation using \"whereis boost\"\e[39m"
#     Check if boost lib is found if yes extract path
    TMPSTRING=$(whereis boost)
    if [[ $TMPSTRING =~ ^(boost: )(/[^/ ]*)+/?(boost)$ ]]; then
         echo -e "\e[32mFound BOOST extract path\e[39m"
         BOOST_INCLUDE=$(whereis boost | sed 's/boost: // ')
         BOOST_LIB=$(whereis boost | sed 's/boost: // ' | sed 's/\/include\/boost// ')
         export BOOST_INCLUDE
         export BOOST_LIB
    fi
fi
if [[ -n "$BOOST_LIB" ]] || [[ -n "$BOOST_INCLUDE" ]]; then
    echo "BOOST_INCLUDE at $BOOST_INCLUDE"
    echo "BOOST_LIB at $BOOST_LIB"
else
     echo "BOOST not found"
     exit 1
fi

# # QT if installed
# export QTDIR=/usr/local/Trolltech/Qt-4.8.5
# export QMAKESPEC=
# export QTROOTSYSDIR=/usr/local/qtRoot/root

#ROOT
if [ -n "$ROOTSYS" ]; then
    echo "\$ROOTSYS = $ROOTSYS"
    echo "source thisroot.sh"
    source $ROOTSYS/bin/thisroot.sh
else
    echo -e "\e[31m\$ROOTSYS environment variable not set.\e[39m"
    echo -e "\e[31mPlease export ROOTSYS pointing to a valid root installation.\e[39m"
fi

# export PATH=$QTDIR/bin:$BASE_DIR/bin:$PATH
#Path to SCPI Parser testbinaries
export PATH=$BASE_DIR_PLUGIN/examples/test-interactive:$BASE_DIR/bin:$PATH
export PATH=$BASE_DIR_PLUGIN/examples/test-LwIP-netconn:$PATH
export PATH=$BASE_DIR_PLUGIN/examples/test-parser:$PATH
export PATH=$BASE_DIR_PLUGIN/examples/test-tcp:$PATH
export PATH=$BASE_DIR_PLUGIN/examples/test-tcp-srq:$PATH

# export LD_LIBRARY_PATH=$BASE_DIR/RootWeb/lib:/opt/cactus/lib:$QTROOTSYSDIR/lib:$QTDIR/lib:$BASE_DIR/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=$BASE_DIR_PLUGIN/libscpi:$BASE_DIR_PLUGIN/lib:${LD_LIBRARY_PATH}
