# Makefile Configuration file created on "Tue Jan 31 13:48:21 PST 2012" by mkmkx
# You may edit this file to change any configuration.

# Note: White space are required as sparators
# Comments start with a '# '
# Syntax:
# OPTION = " VALUE "


# Environment variables
ENV = " "

# Compiler name <g++,gcc,java>
CC = " g++ -Wl,-rpath -Wl,$HOME/.local/6.0.0-gcc4.1.2/lib "

# Source language <cpp,cc,C,c,java>
SRCLANG = " cpp "

# Compiler flags (without debug)
CCFLAGS = " -Wall -Werror -O3 -pedantic "

# Output target file (without debug)
OUT = " SERIAL "

# Output target file (with debug)
OUT_DBG = " dbg.SERIAL "

# Target type <exec,lib,jar>
TYPE = " exec "

# Release Directory
REL_DIR = " ../ "

# Compiler flags (with debug)
DBGFLGS = " -Wall -g3 "

# Source search directories
S_PATHS = " ../src "

# Include search directories
I_PATHS = " ../include $HOME/.local/6.0.0-gcc4.1.2/include/boost-1_39 "

# Library search directories
L_PATHS = " $HOME/.local/6.0.0-gcc4.1.2/lib "

# Library names to link
L_NAMES = " boost_serialization-gcc41-mt "

