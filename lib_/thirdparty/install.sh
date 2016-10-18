#!/bin/bash

CDPATH=
SCRIPT_PATH="$(cd "$(dirname "$0")" && pwd)"
TARGET_PATH="$HOME/.local/incubation-projects"

(
   cd "$SCRIPT_PATH"
   set -x; set -e;

   L="http://downloads.sourceforge.net/project/boost/boost/1.54.0/boost_1_54_0.tar.bz2"
   N="$(basename "$L")"
   I="$TARGET_PATH/.$N.installed"

   mkdir -p "$TARGET_PATH"

   if [ ! -f "$I" ]
   then
      if [ ! -f "$N" ]
      then
         wget "$L"
      fi

      P="boost_1_54_0"

      rm -rf "$P"

      (
         set -x; set -e;

         tar -xjf "$N"
         cd "$P"
#         sed -i -e '/using gcc : / { s/.*/using gcc : : g++48 ;/; }' ./tools/build/v2/user-config.jam
         ./bootstrap.sh --without-libraries=python --prefix="$TARGET_PATH/$P"
         ./b2
         ./b2 install
         touch "$I"
      )

      rm -rf "$P"
   fi

   echo "Installed $N"

) 2>&1 | tee -a $SCRIPT_PATH/install.log
