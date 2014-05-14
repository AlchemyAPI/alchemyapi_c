AC_PREREQ(2.20)
AC_INIT(libalchemyapi, 0.8)
AC_CONFIG_SRCDIR([src/alchemy_api.c])
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects])


dnl --enable-debug
AC_ARG_ENABLE(debug,
[  --enable-debug          Enable debug mode (default disabled)],
[case "${enableval}" in
 yes) debug=true ;;
 no)  debug=false ;;
 *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
esac],[debug=false])

dnl --enable-release
AC_ARG_ENABLE(release,
[  --enable-release        Enable release mode (default disabled)],
[case "${enableval}" in
 yes) release=true ;;
 no)  release=false ;;
 *) AC_MSG_ERROR(bad value ${enableval} for --enable-release) ;;
esac],[release=false])

if test "$debug" = "true"
then
 CFLAGS="${CFLAGS} -DDEBUG -g -O0"
elif test "$release" = "true"
then
 CFLAGS="${CFLAGS} -O6"
else
 CFLAGS="${CFLAGS} -O2"
fi

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_STDC_HEADERS
AC_C_INLINE
AC_PROG_LIBTOOL
AM_SANITY_CHECK

# Checks for libraries.
AC_CHECK_PROG(xml2_config_prog, xml2-config, yes, no)
if test "x$xml2_config_prog" != "xyes"; then
    AC_MSG_ERROR([Unable to find libxml configuration program xml2-config in the path.])
fi
LIBXML_FLAGS=`xml2-config --cflags`
LIBXML_LIBS=`xml2-config --libs`

AC_CHECK_PROG(curl_config_prog, curl-config, yes, no)
if test "x$curl_config_prog" != "xyes"; then
    AC_MSG_ERROR([Unable to find libcurl configuration program curl-config in the path.])
fi
LIBCURL_FLAGS=`curl-config --cflags`
LIBCURL_LIBS=`curl-config --libs`

LIBS="$LIBS $LIBXML_LIBS $LIBCURL_LIBS"
CFLAGS="$CFLAGS $LIBXML_FLAGS $LIBCURL_FLAGS"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC

AC_CONFIG_FILES([Makefile
 include/Makefile
 src/Makefile
 example/Makefile])

AC_OUTPUT
