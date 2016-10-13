##########################
include config/CommonMakefile.am

##########################
TESTS += File_test

File_test_SOURCES = \
	tests/File_test.cpp \
	utils/File.cpp

##########################
TESTS += EPollReactor_test

EPollReactor_test_SOURCES = \
	tests/EPollReactor_test.cpp \
	utils/SocketImplBase.cpp

##########################
noinst_PROGRAMS += ABC

ABC_SOURCES = \
	utils/ABC.cpp \
	utils/File.cpp

##########################
