
SUBDIRS = audio

# The console user interface.

bin_PROGRAMS = sidplay2

sidplay2_SOURCES = IniConfig.cpp IniConfig.h \
args.cpp keyboard.cpp keyboard.h main.cpp menu.cpp \
player.cpp player.h

sidplay2_LDADD = ./audio/null/libnull.a \
./audio/wav/libwav.a \
$(AUDIO_DRV) $(AUDIO_LDFLAGS) \
$(LIBSIDPLAY2_LDFLAGS) $(LIBSIDUTILS_LDFLAGS) \
$(BUILDERS_LDFLAGS)
# $(LIBLTDL_LDFLAGS)

# Remove bad default includes
DEFAULT_INCLUDES=
