AC_INIT([sidplay-libs], [2.1.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

AC_CANONICAL_HOST
case "$host" in
    *linux*|*-k*bsd*-gnu|*mingw*)
        hardsid=yes
        HARDSIDLIB=-lhardsid-builder
        ;;
esac
case "$host" in
    *mingw32* )
        LDFLAGS="${LDFLAGS} -no-undefined"
        MINGW32=yes
        ;;
esac
AC_SUBST([HARDSIDLIB])
AM_CONDITIONAL([HARDSID], [test "x$hardsid" = "xyes"])
AM_CONDITIONAL([MINGW32], [test "x$MINGW32" = "xyes"])

dnl Initialize libtool.
LT_INIT([win32-dll])

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

dnl Use C++ for tests.
AC_LANG([C++])

CHECK_VISIBILITY

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN

AC_CHECK_SIZEOF([char])

if test $ac_cv_sizeof_char != 1; then
  AC_MSG_ERROR([code cannot work correctly on this platform as no real 8 bit data type supported!])
fi

dnl Checks for library functions.
AC_CHECK_FUNCS([strncasecmp strcasecmp])

dnl Checks for C++ library functions.
AC_CHECK_HEADERS([sstream])

AC_SYS_LONG_FILE_NAMES

dnl Check for nothrow allocator.
MY_CHECK_EXCEPTIONS

dnl Check ANSI C++ ios features
MY_CHECK_IOS_BIN
MY_CHECK_IOS_OPENMODE

dnl libtool-style version-info number
LIBSIDPLAYCUR=1
LIBSIDPLAYREV=1
LIBSIDPLAYAGE=0
LIBSIDPLAYVERSION=$LIBSIDPLAYCUR:$LIBSIDPLAYREV:$LIBSIDPLAYAGE
PKGSIDPLAYVERSION=2.$LIBSIDPLAYCUR.$LIBSIDPLAYREV

LIBSIDUTILSCUR=0
LIBSIDUTILSREV=4
LIBSIDUTILSAGE=0
LIBSIDUTILSVERSION=$LIBSIDUTILSCUR:$LIBSIDUTILSREV:$LIBSIDUTILSAGE
PKGSIDUTILSVERSION=1.$LIBSIDUTILSCUR.$LIBSIDUTILSREV

LIBRESIDCUR=0
LIBRESIDREV=1
LIBRESIDAGE=0
LIBRESIDVERSION=$LIBRESIDCUR:$LIBRESIDREV:$LIBRESIDAGE

LIBHARDSIDCUR=0
LIBHARDSIDREV=1
LIBHARDSIDAGE=0
LIBHARDSIDVERSION=$LIBHARDSIDCUR:$LIBHARDSIDREV:$LIBHARDSIDAGE

AC_SUBST(LIBSIDPLAYVERSION)
AC_SUBST(PKGSIDPLAYVERSION)
AC_SUBST(LIBSIDUTILSVERSION)
AC_SUBST(PKGSIDUTILSVERSION)
AC_SUBST(LIBRESIDVERSION)
AC_SUBST(LIBHARDSIDVERSION)


dnl Email addresses
AH_BOTTOM([/* Email addresses */
#define S_A_WHITE_EMAIL "sidplay2@yahoo.com"
])

AC_CONFIG_FILES([
Makefile
libsidplay2.spec
libsidplay2.pc
libsidutils.spec
libsidutils.pc
siddefs.h
])

AC_OUTPUT
