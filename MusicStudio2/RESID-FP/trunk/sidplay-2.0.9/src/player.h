/***************************************************************************
                          player.h  -  Frontend Player
                             -------------------
    begin                : Sun Oct 7 2001
    copyright            : (C) 2001 by Simon White
    email                : s_a_white@email.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/***************************************************************************
 *  $Log: player.h,v $
 *  Revision 1.10  2004/02/12 05:58:03  s_a_white
 *  Update argurements and help menu handling.
 *
 *  Revision 1.9  2004/01/31 17:07:45  s_a_white
 *  Support of specifing max sids writes forming sid2crc and experimental
 *  TSID2 library support.
 *
 *  Revision 1.8  2003/06/27 21:09:37  s_a_white
 *  Constified some of the interfaces.
 *
 *  Revision 1.7  2003/02/20 18:50:45  s_a_white
 *  sid2crc support.
 *
 *  Revision 1.6  2002/01/29 08:11:43  s_a_white
 *  TSID filename fix
 *
 *  Revision 1.5  2002/01/28 19:40:50  s_a_white
 *  Added TSID support.
 *
 *  Revision 1.4  2001/12/05 22:22:48  s_a_white
 *  Added playerFast flag.
 *
 *  Revision 1.3  2001/12/03 19:17:34  s_a_white
 *  Corrected spelling of BUILDER.
 *
 *  Revision 1.2  2001/12/01 20:16:23  s_a_white
 *  Player changed to ConsolePlayer.
 *
 *  Revision 1.1  2001/11/27 19:10:44  s_a_white
 *  Initial Release.
 *
 ***************************************************************************/

#ifndef _player_h_
#define _player_h_

#include "config.h"

#include <sidplay/sidplay2.h>
#ifdef MSVC_HEADER_LOCATIONS
#include <utils/SidDatabase.h>
#else
#include <sidplay/utils/SidDatabase.h>
#endif
#include "audio/AudioDrv.h"
#include "IniConfig.h"

#ifdef HAVE_TSID
#   if HAVE_TSID > 1
#   include <tsid2/tsid2.h>
#   define TSID TSID2
#   else
#   include <tsid/tsid.h>
#   endif
#endif

typedef enum {black, red, green, yellow, blue, magenta, cyan, white}
    player_colour_t;
typedef enum {tableStart, tableMiddle, tableSeperator, tableEnd}
    player_table_t;
typedef enum {
    playerError = 0, playerRunning, playerPaused, playerStopped,
    playerRestart, playerExit, playerFast = 128,
    playerFastRestart = playerRestart | playerFast,
    playerFastExit = playerExit | playerFast
} player_state_t;

typedef enum {/* Same as EMU_DEFAULT except no soundcard.
                 Still allows wav generation */
              EMU_NONE = 0,
              /* The following require a soundcard */
              EMU_DEFAULT, EMU_RESID, EMU_SIDPLAY1,
              /* The following should disable the soundcard */
              EMU_HARDSID, EMU_SIDSTATION, EMU_COMMODORE,
              EMU_SIDSYN, EMU_END} SIDEMUS;

typedef enum {/* Define possible output sources */
              OUT_NULL = 0,
              /* Hardware */
              OUT_SOUNDCARD,
              /* File creation support */
              OUT_WAV, OUT_AU, OUT_END} OUTPUTS;

// Error and status message numbers.
enum
{
    ERR_SYNTAX = 0,
    ERR_NOT_ENOUGH_MEMORY,
    ERR_SIGHANDLER,
    ERR_FILE_OPEN
};

void displayError (const char *arg0, uint num);


// Grouped global variables
class ConsolePlayer: private Event
{
//private:
// MPi: Made this public so Music Studio can easily access the internals without adding lots of accessor functions.
public:
#ifdef HAVE_LIBRESID_BUILDER
    static const char  RESID_ID[];
#endif
#ifdef HAVE_LIBHARDSID_BUILDER
    static const char  HARDSID_ID[];
#endif
#ifdef HAVE_TSID
    TSID               m_tsid;
#endif

    const char* const  m_name; 
    sidplay2           m_engine;
    sid2_config_t      m_engCfg;
    SidTuneMod         m_tune;
    player_state_t     m_state;
    const char*        m_outfile;
    EventContext      *m_context;
    const char*        m_filename;

    IniConfig          m_iniCfg;
    SidDatabase        m_database;

    // Display parameters
    uint_least8_t      m_quietLevel;
    uint_least8_t      m_verboseLevel;

    uint_least32_t     m_crc;
    bool               m_cpudebug;

    bool    v1mute, v2mute, v3mute;

    struct m_filter_t
    {
        SidFilter      definition;
        bool           enabled;
    } m_filter;

    struct m_driver_t
    {
        OUTPUTS        output;   // Selected output type
        SIDEMUS        sid;      // Sid emulation
        bool           file;     // File based driver
        AudioConfig    cfg;
        AudioBase*     selected; // Selected Output Driver
        AudioBase*     device;   // HW/File Driver
        Audio_Null     null;     // Used for everything
    } m_driver;

    struct m_timer_t
    {   // secs
        uint_least32_t start;
        uint_least32_t current;
        uint_least32_t stop;
        uint_least32_t length;
        bool           valid;
    } m_timer;

    struct m_track_t
    {
        uint_least16_t first;
        uint_least16_t selected;
        uint_least16_t songs;
        bool           loop;
        bool           single;
    } m_track;

    struct m_speed_t
    {
        uint_least8_t current;
        uint_least8_t max;
    } m_speed;

private:
    // Console
    void consoleColour  (player_colour_t colour, bool bold);
    void consoleTable   (player_table_t table);
    void consoleRestore (void);

    // Command line args
    bool parseTime        (const char *str, uint_least32_t &time);
    void displayArgs      (const char *arg = NULL);
    void displayDebugArgs ();

    bool createOutput   (OUTPUTS driver, const SidTuneInfo *tuneInfo);
    bool createSidEmu   (SIDEMUS emu);
    void displayError   (const char *error);
    void displayError   (uint num) { ::displayError (m_name, num); }
    void decodeKeys     (void);
    void event          (void);
    void emuflush       (void);
    void menu           (void);

public:
    ConsolePlayer (const char * const name);
    virtual ~ConsolePlayer() {;}

    int            args  (int argc, const char *argv[]);
    bool           open  (void);
    void           close (void);
    bool           play  (void);
    void           stop  (void);
    player_state_t state (void) { return m_state; }
};

#endif // _player_h_
