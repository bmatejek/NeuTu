NeuTu-EM
=====

[![Build Status](https://drone.io/github.com/janelia-flyem/NeuTu/status.png)](https://drone.io/github.com/janelia-flyem/NeuTu/latest)

Software for proofreading EM connectomics

## Download Source Code (Not needed for installation)

    git clone -b flyem_release git@github.com:bmatejek525/NeuTu.git

Also need Qt4.8.5 installed.

## Required Changes

In neurolabi/cpp/lib/CmakeLists.txt.qc, change ${QT_DIR} to your Qt installation directory.

## Installation

    cd NeuTu

    /* the <qmake_path> should not include the final bin directory. The script searches in <qmake_path>/bin/qmake. */
    ./build.sh <qmake_path> <qmake_spec_path>

    cd neurolabi/lib
    tar -xvzf hdf5-1.8.12.tar.gz
    cd hdf5-1.8.12
    ./configure
    make
    cd ../../..

    cd neurolabi/cpp/lib
    ./build.sh

## Manual

https://www.dropbox.com/s/cnewjf7bdm3qbdj/manual.pdf?dl=0

## Installing NeuronComplete Skeletonization

This assumes that Neuroproof_minimal is already installed. Change the first four commands of the CMakeLists.txt file to the right directories. Some of the paths may be different so you may need to change other commands in the CMakeLists.txt file.

     cd NeuronComplete
     mkdir build
     cd build
     cmake ..
     make

Contact Ting Zhao: zhaot@janelia.hhmi.org for any questions or comments.