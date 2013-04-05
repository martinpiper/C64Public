
ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign subdir-objects

if HAVE_VISIBILITY
  AM_CXXFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
endif

#=========================================================
EXTRA_DIST = \
sidplay/psiddrv.bin \
sidplay/kernal.bin \
sidplay/poweron.bin \
sidplay/basic.bin \
sidplay/char.bin \
sidplay/c64/c64cia.h \
sidplay/c64/c64vic.h \
sidplay/c64/c64xsid.h \
sidplay/mos6510/cycle_based \
sidplay/AUTHORS \
sidplay/ChangeLog \
sidplay/COPYING \
sidplay/README \
sidplay/TODO \
utils/ini/headings.i \
utils/ini/keys.i \
utils/ini/list.i \
utils/ini/types.i \
utils/AUTHORS \
utils/ChangeLog \
utils/COPYING \
utils/README \
utils/TODO \
builders/resid-builder/resid/AUTHORS \
builders/resid-builder/resid/ChangeLog \
builders/resid-builder/resid/COPYING \
builders/resid-builder/resid/INSTALL \
builders/resid-builder/resid/README \
builders/resid-builder/resid/NEWS \
builders/resid-builder/resid/THANKS \
builders/resid-builder/resid/TODO

#=========================================================
# Libraries
lib_LTLIBRARIES = \
sidplay/libsidplay2.la \
utils/libsidutils.la \
builders/resid-builder/libresid-builder.la

if HARDSID
  lib_LTLIBRARIES += builders/hardsid-builder/libhardsid-builder.la
endif

noinst_LTLIBRARIES = \
builders/resid-builder/resid/libresid.la

#=========================================================
# libsidplay
sidplay_libsidplay2_la_SOURCES = \
sidplay/config.cpp \
sidplay/event.cpp \
sidplay/player.cpp \
sidplay/player.h \
sidplay/psiddrv.cpp \
sidplay/mixer.cpp \
sidplay/reloc65.c \
sidplay/sidplay2.cpp \
sidplay/nullsid.h \
sidplay/psiddrv.a65 \
sidplay/sidenv.h \
sidplay/mos656x/mos656x.cpp \
sidplay/mos656x/mos656x.h \
sidplay/mos6510/mos6510.cpp \
sidplay/mos6510/mos6510.h \
sidplay/mos6510/conf6510.h \
sidplay/mos6510/opcodes.h \
sidplay/mos6526/mos6526.cpp \
sidplay/mos6526/mos6526.h \
sidplay/sid6526/sid6526.cpp \
sidplay/sid6526/sid6526.h \
sidplay/sidtune/IconInfo.cpp \
sidplay/sidtune/InfoFile.cpp \
sidplay/sidtune/MUS.cpp \
sidplay/sidtune/p00.cpp \
sidplay/sidtune/PP20.cpp \
sidplay/sidtune/PP20.h \
sidplay/sidtune/PP20_Defs.h \
sidplay/sidtune/prg.cpp \
sidplay/sidtune/PSID.cpp \
sidplay/sidtune/SidTune.cpp \
sidplay/sidtune/SidTuneCfg.h \
sidplay/sidtune/SidTuneTools.cpp \
sidplay/sidtune/SidTuneTools.h \
sidplay/xsid/xsid.cpp \
sidplay/xsid/xsid.h

sidplay_libsidplay2_la_LDFLAGS = -version-info $(LIBSIDPLAYVERSION)

sidplay_libsidplay2_ladir = $(includedir)/sidplay

sidplay_libsidplay2_la_HEADERS = \
sidplay/sidconfig.h \
sidplay/Buffer.h \
sidplay/c64env.h \
sidplay/component.h \
sidplay/event.h \
sidplay/sid2types.h \
sidplay/sidbuilder.h \
sidplay/sidendian.h \
sidplay/sidplay2.h \
sidplay/SidTune.h \
sidplay/sidtypes.h \
sidplay/sidusage.h \
sidplay/SmartPtr.h

#=========================================================
# libsidutils
utils_libsidutils_ladir = $(includedir)/sidplay/utils

utils_libsidutils_la_HEADERS = \
utils/libini.h \
utils/SidDatabase.h \
utils/SidFilter.h \
utils/SidTuneMod.h \
utils/SidUsage.h

utils_libsidutils_la_SOURCES = \
utils/SidDatabase.cpp \
utils/SidFilter.cpp \
utils/SidTuneMod.cpp \
utils/SidUsage.cpp \
utils/smm.cpp \
utils/smm0.h \
utils/ini/headings.h \
utils/ini/ini.cpp \
utils/ini/ini.h \
utils/ini/keys.h \
utils/ini/list.h \
utils/MD5/MD5.cpp \
utils/MD5/MD5.h \
utils/MD5/MD5_Defs.h

utils_libsidutils_la_LIBADD = sidplay/libsidplay2.la

utils_libsidutils_la_LDFLAGS = -version-info $(LIBSIDUTILSVERSION)

#=========================================================
# resid
builders_resid_builder_resid_libresid_la_SOURCES = \
builders/resid-builder/resid/sid.h \
builders/resid-builder/resid/voice.h \
builders/resid-builder/resid/wave.h \
builders/resid-builder/resid/envelope.h \
builders/resid-builder/resid/filter.h \
builders/resid-builder/resid/extfilt.h \
builders/resid-builder/resid/pot.h \
builders/resid-builder/resid/sid.cc \
builders/resid-builder/resid/voice.cc \
builders/resid-builder/resid/wave.cc \
builders/resid-builder/resid/envelope.cc \
builders/resid-builder/resid/filter.cc \
builders/resid-builder/resid/extfilt.cc \
builders/resid-builder/resid/pot.cc \
builders/resid-builder/resid/version.cc

#=========================================================
# builders
builders_resid_builder_libresid_builder_ladir = $(includedir)/sidplay/builders
builders_resid_builder_libresid_builder_la_HEADERS = \
builders/resid-builder/resid.h

builders_resid_builder_libresid_builder_la_SOURCES = \
builders/resid-builder/resid.cpp \
builders/resid-builder/resid-builder.cpp \
builders/resid-builder/resid-emu.h

builders_resid_builder_libresid_builder_la_LIBADD = \
builders/resid-builder/resid/libresid.la

builders_resid_builder_libresid_builder_la_LDFLAGS = -version-info $(LIBRESIDVERSION)


builders_hardsid_builder_libhardsid_builder_ladir = $(includedir)/sidplay/builders
builders_hardsid_builder_libhardsid_builder_la_HEADERS = \
builders/hardsid-builder/hardsid.h

if MINGW32
  hardsid_src = builders/hardsid-builder/hardsid-win.cpp
else
  hardsid_src = builders/hardsid-builder/hardsid-unix.cpp
endif

builders_hardsid_builder_libhardsid_builder_la_SOURCES = \
$(hardsid_src) \
builders/hardsid-builder/hardsid-builder.cpp  \
builders/hardsid-builder/hardsid-emu.h

builders_hardsid_builder_libhardsid_builder_la_LDFLAGS = -version-info $(LIBHARDSIDVERSION)

#=========================================================
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libsidplay2.pc libsidutils.pc

#=========================================================
# Remove SVN directories from any subdirs that get
# included completely (e.g. win, directx,...).

dist-hook:
	rm -rf `find $(distdir) -path \*/.svn`
