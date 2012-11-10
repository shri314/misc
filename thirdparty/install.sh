#!/bin/bash -x


L="http://downloads.sourceforge.net/project/boost/boost/1.46.1/boost_1_46_1.tar.bz2" &&
N="$(basename "$L")" &&
(test -f "$N" || wget "$L") &&
tar -xjvf "$N" &&
cd boost_1_46_1 &&
./bootstrap.sh --prefix=$HOME/.local/incubation-projects/boost_1_46_1 &&
./bjam --prefix=$HOME/.local/incubation-projects/boost_1_46_1 install &&
echo "done"

