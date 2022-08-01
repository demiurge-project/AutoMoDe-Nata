#/bin/env bash -e
set -euxo pipefail

USERNAME=`whoami`

# Installation directory (specify a full path!)
INSTALL_DIR=/home/${USERNAME}/argos3-dist

# ARGoS3 repository
ARGOS3_REPOSITORY=git://github.com/ilpincy/argos3.git
ARGOS3_BRANCH=master
ARGOS3_TAG=3.0.0-beta48
ARGOS3_DIR=argos3

# IRIDIA e-puck repository
EPUCK_REPOSITORY=git://github.com/demiurge-project/argos3-epuck.git
EPUCK_BRANCH=master
EPUCK_TAG=v48
EPUCK_DIR=argos3-epuck

#demiurge dao
DAO_REPO=git://github.com/demiurge-project/demiurge-epuck-dao.git
DAO_BRANCH=master
DAO_TAG=master
DAO_DIR=demiurge-epuck-dao

#demiurge loop functions
LOOP_REPO=git://github.com/demiurge-project/experiments-loop-functions.git
LOOP_BRANCH=repertoire
LOOP_TAG=master
LOOP_DIR=experiments-loop-functions

#automode nata
AUTO_REPO=git://github.com/demiurge-project/AutoMoDe-Nata.git
AUTO_BRANCH=master
AUTO_TAG=master
AUTO_DIR=AutoMoDe-Nata

#neat
NEAT_REPO=git://github.com/demiurge-project/ARGoS3-NEAT.git
NEAT_BRANCH=master
NEAT_TAG=master 
NEAT_DIR=ARGoS3-NEAT


function error() {
    echo
    echo "Error, script halted."
    exit 1
}

# Fetches the latest sources from a git repository
# $1: full repository URL
# $2: repository branch
# $3: output directory
function get_code() {
    local REPO=$1
    local BRANCH=$2
    local TAG=$3
    local OUTDIR=$4
  
    if [ ! -d $OUTDIR ]; then
        echo -n "Downloading... "
        git clone --branch=$BRANCH $REPO $OUTDIR || error
        cd $OUTDIR || error
        git checkout $TAG|| error
        cd ..
    else
        echo -n "Updating... "
        cd $OUTDIR || error
        git checkout $TAG|| error
        cd ..
    fi
    echo "done"
}

# Creates the build directory, builds the code, and installs binaries
# $1: source location
# $*: extra CMake args
function do_build_install() {
    local SRC=../$1
    shift
    rm -rf build || error
    echo -n "Compiling... "
    mkdir build || error
    cd build || error
    cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
          -DCMAKE_BUILD_TYPE=Release \
          $* $SRC || error
    make -j4 || error
    echo "done"
    echo "Installing... "
    make install || error
    cd ..
    echo "done"
}

function do_build() {
    local SRC=../$1
    shift
    rm -rf build || error
    echo -n "Compiling... "
    mkdir build || error
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release \
          $* $SRC || error
    make -j4 || error
    cd ..
    echo "done"
}

# Builds and installs ARGoS3
function do_argos3() {
    echo "+++ ARGoS3 +++"
    get_code $ARGOS3_REPOSITORY $ARGOS3_BRANCH $ARGOS3_TAG $ARGOS3_DIR
    cd $ARGOS3_DIR
    do_build_install src -DARGOS_INSTALL_LDSOCONF=OFF -DARGOS_DOCUMENTATION=OFF
    cd ..
    echo -n "Deleting default e-puck... "
    rm -rf \
        $INSTALL_DIR/include/argos3/plugins/robots/e-puck \
        $INSTALL_DIR/lib/argos3/lib*epuck*.so
    echo "done"
}

# Builds and installs e-puck
function do_epuck() {
    echo "+++ e-puck +++"
    export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
    get_code $EPUCK_REPOSITORY $EPUCK_BRANCH $EPUCK_TAG $EPUCK_DIR
    cd $EPUCK_DIR
    # modify CMakeList for cluster compiling without QT (desactivate vision tools)
    sed -i.bak 's/.*VisionTools/#&/' src/plugins/robots/e-puck/CMakeLists.txt
    echo "sed done, desactivate vision... "
    do_build_install src
    cd ..
}


function do_dao() {
    echo "+++ demiurge DAO +++"
    export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
    echo $DAO_REPO $DAO_BRANCH $DAO_TAG $DAO_DIR
    get_code $DAO_REPO $DAO_BRANCH $DAO_TAG $DAO_DIR
    cd $DAO_DIR
    do_build_install .
    cd ..
}

function do_loop() {
    echo "+++ loop functions +++"
    export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
    get_code $LOOP_REPO $LOOP_BRANCH $LOOP_TAG $LOOP_DIR
    cd $LOOP_DIR
    echo "add_subdirectory(complexity)" >> loop-functions/CMakeLists.txt 
    echo "add_subdirectory(NEATvsEvo)" >> loop-functions/CMakeLists.txt
    echo "sed done, activate loop functions.."
    do_build_install .
    cd ..
}

function do_auto() {
    echo "+++ auto +++"
    export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
    get_code $AUTO_REPO $AUTO_BRANCH $AUTO_TAG $AUTO_DIR
    cd $AUTO_DIR
    do_build .
    cd ..
}

function do_neat() {
    echo "+++ neat +++"
    export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
    get_code $NEAT_REPO $NEAT_BRANCH $NEAT_TAG $NEAT_DIR
    cd $NEAT_DIR
    sed -i 's/.*genome_parser/#&/' src/CMakeLists.txt
    do_build .
    cd ..
}

# Creates the setup script
function do_create_setup_script() {
    local SCRIPTFILE=$INSTALL_DIR/bin/setup_argos3
    echo "export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig" > $SCRIPTFILE
    echo "export ARGOS_PLUGIN_PATH=$INSTALL_DIR/lib/argos3" >> $SCRIPTFILE
    echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$ARGOS_PLUGIN_PATH" >> $SCRIPTFILE
    echo "export PATH=$PATH:$INSTALL_DIR/bin" >> $SCRIPTFILE
    echo "export CPATH=$CPATH:$INSTALL_DIR/include" >> $SCRIPTFILE 
    echo "Execute the following command to setup the environment for ARGoS3:"
    echo
    echo "  source $SCRIPTFILE"
}

function do_create_config(){
    local CONFIGFILE=./config.py
    echo "#!/usr/bin/python3" >> $CONFIGFILE
    echo "ARGOS_PREFIX = \"$INSTALL_DIR\"" >> $CONFIGFILE
    echo "CONTROLLERS = {" >> $CONFIGFILE
    echo "    \"nn_rm_1dot1\": \"$PWD/$NEAT_DIR/build/src/controllers/libnn_rm_1dot1.so\", " >> $CONFIGFILE
    echo "    \"automode\": \"$PWD/$AUTO_DIR/build/src/libautomode.so\" }" >> $CONFIGFILE
    echo "LOOP_PREFIX = \"$PWD/$LOOP_DIR/build\"" >> $CONFIGFILE
}

# Erase and re-create installation directory
rm -rf $INSTALL_DIR
mkdir $INSTALL_DIR

mkdir build
cd build
# Go through packages
do_argos3
do_epuck
do_dao
do_loop
do_auto
do_neat

# Create the setup script
do_create_setup_script
do_create_config
