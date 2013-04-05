
SUBDIRS = MD5 ini

AM_CPPFLAGS = \
-I$(top_srcdir)/include/sidplay/utils \
-I$(top_srcdir)/../libsidplay/include/sidplay \
-I$(top_srcdir)/../libsidplay/include

if HAVE_VISIBILITY
  AM_CXXFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
endif

# The library.
lib_LTLIBRARIES = libsidutils.la

libsidutils_la_SOURCES = SidDatabase.cpp SidFilter.cpp SidTuneMod.cpp SidUsage.cpp smm.cpp smm0.h

libsidutils_la_LIBADD  = \
$(top_builddir)/src/MD5/libMD5.la \
$(top_builddir)/src/ini/libini.la \
$(top_builddir)/../libsidplay/src/libsidplay2.la

# We substitute the libtool-specific library version in configure.in.
libsidutils_la_LDFLAGS = -version-info $(LIBVERSION)
