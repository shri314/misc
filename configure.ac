AC_INIT([lib314], [0.1], [bug-report@weyoun.shri314.net], [lib314], [http://weyoun.shri314.net/])
AC_PREREQ([2.59])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.9.6 -Wall no-define subdir-objects foreign])
#AM_SILENT_RULES([yes])
AC_PROG_CXX([g++48])

AC_DEFUN([MY_XSUBST], [AC_SUBST([$1], ["$$1 $2"])])
MY_XSUBST([CXXFLAGS], [-Wall -Werror -pedantic -g3 -O0 -fno-inline -std=c++11 -Wno-unused-local-typedefs])
MY_XSUBST([CXXFLAGS], [-fmessage-length=0])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
