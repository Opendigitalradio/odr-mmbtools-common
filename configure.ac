# Copyright (C) 2019 Matthias P. Braendli, http://opendigitalradio.org


AC_PREREQ(2.69)
AC_INIT([ODR-mmbTools-common], [0.1.0], [matthias.braendli@mpb.li])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_CONFIG_SRCDIR([src/Socket.cpp])
AC_CONFIG_HEADER([config.h])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

AX_CXX_COMPILE_STDCXX(17,noext,mandatory)

# Checks for libraries.
AX_PTHREAD([], AC_MSG_ERROR([requires pthread]))

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# EDI and ZMQ output metadata require TAI-UTC offset, which requires downloading the IETF TAI bulletin
AC_CHECK_LIB(curl, curl_easy_init, [CURL_LIBS="-lcurl"])
have_curl=$ac_cv_lib_curl_curl_easy_init
AS_IF([test "x$have_curl" = "xyes"], [
       AC_DEFINE(HAVE_CURL, [1], [Define if cURL is available])
       ])
AS_IF([test "x$have_curl" = "xno"], [AC_MSG_WARN([cURL not found, timestamps will not work])])

PKG_CHECK_MODULES([SRT], [srt], have_srt=yes, have_srt=no)
AS_IF([test "x$enable_srt" = "xyes"],
      [AC_DEFINE(HAVE_SRT, [1], [Define if SRT is available])])

AC_LANG_PUSH([C++])
AX_CHECK_COMPILE_FLAG([-Wduplicated-cond], [CXXFLAGS="$CXXFLAGS -Wduplicated-cond"], [], ["-Werror"])
AX_CHECK_COMPILE_FLAG([-Wduplicated-branches], [CXXFLAGS="$CXXFLAGS -Wduplicated-branches"], [], ["-Werror"])
AX_CHECK_COMPILE_FLAG([-Wlogical-op], [CXXFLAGS="$CXXFLAGS -Wlogical-op"], [], ["-Werror"])
AX_CHECK_COMPILE_FLAG([-Wrestrict], [CXXFLAGS="$CXXFLAGS -Wrestrict"], [], ["-Werror"])
AX_CHECK_COMPILE_FLAG([-Wdouble-promotion], [CXXFLAGS="$CXXFLAGS -Wdouble-promotion"], [], ["-Werror"])
AX_CHECK_COMPILE_FLAG(["-Wformat=2"], [CXXFLAGS="$CXXFLAGS -Wformat=2"], [], ["-Werror"])

# Linux defines MSG_NOSIGNAL, some other systems have SO_NOSIGPIPE instead
AC_MSG_CHECKING(for MSG_NOSIGNAL)
AC_COMPILE_IFELSE([ AC_LANG_PROGRAM([[
                    #include <sys/socket.h>
                    int f = MSG_NOSIGNAL;
                    ]])],
                   [ AC_MSG_RESULT(yes)
                     AC_DEFINE(HAVE_MSG_NOSIGNAL, 1, [Define this symbol if you have MSG_NOSIGNAL]) ],
                   [ AC_MSG_RESULT(no) ])

AC_MSG_CHECKING(for SO_NOSIGPIPE)
AC_COMPILE_IFELSE([ AC_LANG_PROGRAM([[
                    #include <sys/socket.h>
                    int f = SO_NOSIGPIPE;
                    ]])],
                   [ AC_MSG_RESULT(yes)
                     AC_DEFINE(HAVE_SO_NOSIGPIPE, 1, [Define this symbol if you have SO_NOSIGPIPE]) ],
                   [ AC_MSG_RESULT(no) ])

AC_LANG_POP([C++])

# ZeroMQ
AX_ZMQ([4.0.0], [], AC_MSG_ERROR(ZeroMQ 4.0.0 is required))

AC_DEFINE([HAVE_ZEROMQ], [1], [Define if ZeroMQ available])

AM_CONDITIONAL([IS_GIT_REPO], [test -d '.git'])

AC_SUBST([CFLAGS], ["$CFLAGS $SRT_CFLAGS $PTHREAD_CFLAGS"])
AC_SUBST([CXXFLAGS], ["$CXXFLAGS $SRT_CFLAGS $PTHREAD_CFLAGS"])
AC_SUBST([LIBS], ["$SRT_LIBS $CURL_LIBS $PTHREAD_LIBS"])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo
echo " cUrl enabled = $have_curl"
echo " SRT enabled = $have_srt"
echo
echo "***********************************************"
echo

if test "$have_curl" = "no" ; then
echo
echo "WARNING! cURL not found: ClockTAI will not work"
echo
fi

