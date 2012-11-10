#!/bin/bash

CDPATH=
SCRIPT_PATH="$(cd "$(dirname "$0")" && pwd)"
TARGET_PATH="$HOME/.local/incubation-projects"

(
   cd "$SCRIPT_PATH"
   set -x; set -e;

   L="http://downloads.sourceforge.net/project/boost/boost/1.46.1/boost_1_46_1.tar.bz2"
   N="$(basename "$L")"
   I="$TARGET_PATH/.$N.installed"

   mkdir -p "$TARGET_PATH"

   if [ ! -f "$I" ]
   then
      if [ ! -f "$N" ]
      then
         wget "$L"
      fi

      P="boost_1_46_1"

      rm -rf "$P"

      (
         set -x; set -e;

         tar -xjf "$N"
         cd "$P"
         ./bootstrap.sh --prefix="$TARGET_PATH/$P"
         ./bjam --prefix="$TARGET_PATH/$P" install
         touch "$I"
      )

      rm -rf "$P"
   fi

   echo "Installed $N"
)
