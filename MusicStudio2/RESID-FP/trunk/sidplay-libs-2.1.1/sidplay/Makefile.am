
SUBDIRS    = c64 mos6510 mos6526 mos656x xsid sid6526 sidtune
EXTRA_DIST = psiddrv.bin kernal.bin poweron.bin basic.bin char.bin

AM_CPPFLAGS = \
-I$(top_srcdir)/include/headers \
-I$(top_srcdir)/include/sidplay \
-I$(top_srcdir)/include

if HAVE_VISIBILITY
  AM_CXXFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
endif

# The library.
lib_LTLIBRARIES = libsidplay2.la

libsidplay2_la_SOURCES = config.cpp event.cpp \
player.cpp player.h \
psiddrv.cpp mixer.cpp \
reloc65.c sidplay2.cpp \
nullsid.h psiddrv.a65

libsidplay2_la_LIBADD  = \
$(top_builddir)/src/mos6510/libmos6510.la \
$(top_builddir)/src/mos6526/libmos6526.la \
$(top_builddir)/src/mos656x/libmos656x.la \
$(top_builddir)/src/xsid/libxsid.la \
$(top_builddir)/src/sidtune/libsidtune.la \
$(top_builddir)/src/sid6526/libsid6526.la

# We substitute the libtool-specific library version in configure.in.
libsidplay2_la_LDFLAGS = -version-info $(LIBVERSION)

SUFFIXES = .a65

%.bin : %.a65
	xa -R -G $< -o $(srcdir)/$*.o65
	od -v -An -w8 -tx1 $(srcdir)/$*.o65 | sed -re 's/[[:alnum:]]+/0x&,/g' > $(srcdir)/$@
