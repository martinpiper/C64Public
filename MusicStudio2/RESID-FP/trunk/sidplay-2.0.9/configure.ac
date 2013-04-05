dnl Process this file with autoconf to produce a configure script.
AC_INIT(Makefile.am)
AC_CONFIG_AUX_DIR(unix)
AC_CONFIG_MACRO_DIR(unix)
AM_CONFIG_HEADER(unix/config.h)
LT_INIT

dnl Setup library CURRENT, REVISION and AGE
AM_INIT_AUTOMAKE(sidplay,2.0.9)

AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX

dnl Initialise Libtool.
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN

dnl Checks what version of Unix we have and soundcard support
AC_CHECK_HEADERS(sys/ioctl.h linux/soundcard.h machine/soundcard.h  \
soundcard.h sys/audio.h sun/audioio.h sun/dbriio.h sys/audioio.h  \
audio.h dmedia/audio.h sys/soundcard.h sys/asoundlib.h alsa/asoundlib.h \
pulse/simple.h)

AH_TOP(
[/* Define supported audio driver */
#undef HAVE_HARDSID
#undef HAVE_ALSA
#undef HAVE_MMSYSTEM
#undef HAVE_OSS
#undef HAVE_PULSE
])

dnl Audio Subsystem Test
AC_MSG_CHECKING([which audio subsystem is available])
audiodrv_libadd=""
case "$host" in
    *freebsd*) AC_DEFINE(HAVE_OSS)
        audiodrv_libadd="./audio/oss/liboss.a"
        AC_MSG_RESULT(oss)
        ;;
    *linux*)
        # pulse / alsa / OSS -- we should really throw most of this
        # machine-dependency check away, and just look for the headers and
        # decide what to compile in based on availability.
        if test "$ac_cv_header_pulse_simple_h" == "yes"; then
            AC_DEFINE(HAVE_PULSE)
            audiodrv_libadd="./audio/pulse/libpulse.a"
            AC_MSG_RESULT(pulse)
            AC_CHECK_LIB(pulse, main, [AUDIO_LDFLAGS=-lpulse-simple AC_SUBST(AUDIO_LDFLAGS)])
        elif test "$ac_cv_header_sys_asoundlib_h" == "yes" ||
                test "$ac_cv_header_alsa_asoundlib_h" == "yes"; then
            AC_DEFINE(HAVE_ALSA)
            audiodrv_libadd="./audio/alsa/libalsa.a"
            AC_MSG_RESULT(alsa)
            AC_CHECK_LIB(asound, main, [AUDIO_LDFLAGS=-lasound AC_SUBST(AUDIO_LDFLAGS)])
        else
            AC_DEFINE(HAVE_OSS)
            audiodrv_libadd="./audio/oss/liboss.a"
            AC_MSG_RESULT(oss)
        fi
        ;;
    *netbsd*) AC_DEFINE(HAVE_OSS)
        audiodrv_libadd="./audio/oss/liboss.a"
        AC_MSG_RESULT(oss)
        AC_CHECK_LIB(ossaudio, main, [AUDIO_LDFLAGS=-lossaudio AC_SUBST(AUDIO_LDFLAGS)]) 
        ;;
    *qnx*) AC_DEFINE(HAVE_ALSA)
        audiodrv_libadd="./audio/alsa/libalsa.a"
        AC_MSG_RESULT(alsa)
        AC_CHECK_LIB(asound, main, [AUDIO_LDFLAGS=-lasound AC_SUBST(AUDIO_LDFLAGS)])
        ;;
    *cygwin*) AC_DEFINE(HAVE_MMSYSTEM)
        audiodrv_libadd="./audio/mmsystem/libmmsystem.a"
        AC_MSG_RESULT(mmsystem)
        AUDIO_LDFLAGS=-lwinmm
        AC_SUBST(AUDIO_LDFLAGS)
        ;;
    *mingw*) AC_DEFINE(HAVE_MMSYSTEM)
        # is this -DHAVE_MINGW really necessary? nothing references it locally
        CXXFLAGS="$CXXFLAGS -DHAVE_MINGW"
        audiodrv_libadd="./audio/mmsystem/libmmsystem.a"
        AC_MSG_RESULT(mmsystem)
        AUDIO_LDFLAGS=-lwinmm
        AC_SUBST(AUDIO_LDFLAGS)
        ;;
    *)  AC_MSG_RESULT(wav only)
        ;;
esac

AUDIO_DRV="$audiodrv_libadd"
AC_SUBST(AUDIO_DRV)

dnl Use C++ for tests.
AC_LANG_CPLUSPLUS

dnl Check for nothrow allocator.
MY_CHECK_EXCEPTIONS

dnl Check ANSI C++ ios features
MY_CHECK_IOS_BIN
MY_CHECK_IOS_OPENMODE

AC_CHECK_LIB(sidplay2, main)
AC_CHECK_LIB(sidutils, main)
AC_CHECK_LIB(resid-builder, main)
AC_CHECK_LIB(hardsid-builder, main)

dnl Add absolute directory for header files
CXXFLAGS="$CXXFLAGS -DHAVE_UNIX -I\$(top_srcdir)/unix -I/usr/include/sidplay"

AC_OUTPUT(
Makefile \
doc/Makefile \
doc/en/Makefile \
src/Makefile \
src/audio/Makefile \
src/audio/alsa/Makefile \
src/audio/mmsystem/Makefile \
src/audio/null/Makefile \
src/audio/oss/Makefile \
src/audio/pulse/Makefile \
src/audio/wav/Makefile \
unix/Makefile \
unix/sidplay2.spec \
)
