/***************************************************************************
                          menu.cpp  -  Ansi Console Menu
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
 *  $Log: menu.cpp,v $
 *  Revision 1.14  2004/02/26 18:19:22  s_a_white
 *  Updates for VC7 (use real libstdc++ headers instead of draft ones).
 *
 *  Revision 1.13  2004/02/09 23:42:50  s_a_white
 *  Fixed damaged menu border shown using verbose option.
 *
 *  Revision 1.12  2004/01/31 17:07:45  s_a_white
 *  Support of specifing max sids writes forming sid2crc and experimental
 *  TSID2 library support.
 *
 *  Revision 1.11  2003/10/28 00:26:00  s_a_white
 *  Display power on cycles count for a loaded song.
 *
 *  Revision 1.10  2003/09/14 13:47:41  s_a_white
 *  Show whether the environment was forcefully changed from the default
 *  or user requested.
 *
 *  Revision 1.9  2003/07/16 06:53:12  s_a_white
 *  Only display tune description strings if they have been provided.
 *
 *  Revision 1.8  2003/02/20 18:50:44  s_a_white
 *  sid2crc support.
 *
 *  Revision 1.7  2002/11/22 08:35:16  s_a_white
 *  Renamed "Copyright" to "Released".
 *
 *  Revision 1.6  2002/03/04 19:28:28  s_a_white
 *  Displays more details about sidplay2s psid driver.
 *
 *  Revision 1.5  2002/01/29 21:55:29  s_a_white
 *  Display actual environment mode instead of requested one.
 *
 *  Revision 1.4  2002/01/28 19:40:50  s_a_white
 *  Added TSID support.
 *
 *  Revision 1.3  2002/01/15 19:12:24  s_a_white
 *  PSID2NG update.
 *
 *  Revision 1.2  2001/12/01 20:16:23  s_a_white
 *  Player changed to ConsolePlayer.
 *
 *  Revision 1.1  2001/11/27 19:10:44  s_a_white
 *  Initial Release.
 *
 ***************************************************************************/

#include <ctype.h>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::dec;
using std::hex;
using std::flush;
#include <iomanip>
using std::setw;
using std::setfill;
#include "player.h"


// Display console menu
void ConsolePlayer::menu ()
{
    const sid2_info_t &info     = m_engine.info ();
    const SidTuneInfo &tuneInfo = *info.tuneInfo;

    if (m_quietLevel > 1)
        return;

    // cerr << (char) 12 << '\f'; // New Page
    if ((m_iniCfg.console ()).ansi)
    {
        cerr << '\x1b' << "[40m";  // Background black
        cerr << '\x1b' << "[2J";   // Clear screen
        cerr << '\x1b' << "[0;0H"; // Move cursor to 0,0
    }

    consoleTable (tableStart);
    consoleTable (tableMiddle);
    consoleColour (red, true);
    cerr << "   SID";
    consoleColour (blue, true);
    cerr << "PLAY";
    consoleColour (white, true);
    cerr << " - Music Player and C64 SID Chip Emulator" << endl;
    consoleTable  (tableMiddle);
    consoleColour (white, false);
    {
        cerr << setw(17) << "Sidplay" << " V" << VERSION << ", ";
        cerr << (char) toupper (*info.name);
        cerr << info.name + 1 << " V" << info.version << endl;
    }

    consoleTable (tableSeperator);
    if (tuneInfo.numberOfInfoStrings > 0)
    {
        if (!tuneInfo.musPlayer && (tuneInfo.numberOfInfoStrings == 3))
        {
            consoleTable  (tableMiddle);
            consoleColour (cyan, true);
            cerr << " Title        : ";
            consoleColour (magenta, true);
            cerr << tuneInfo.infoString[0] << endl;
            consoleTable  (tableMiddle);
            consoleColour (cyan, true);
            cerr << " Author       : ";
            consoleColour (magenta, true);
            cerr << tuneInfo.infoString[1] << endl;
            consoleTable  (tableMiddle);
            consoleColour (cyan, true);
            cerr << " Released     : ";
            consoleColour (magenta, true);
            cerr << tuneInfo.infoString[2] << endl;
        }
        else
        {
            for (int i = 0; i < tuneInfo.numberOfInfoStrings; i++)
            {
                consoleTable  (tableMiddle);
                consoleColour (cyan, true);
                cerr << " Description  : ";
                consoleColour (magenta, true);
                cerr << tuneInfo.infoString[i] << endl;
            }
        }
        consoleTable (tableSeperator);
    }

    if (m_verboseLevel)
    {
        consoleTable  (tableMiddle);
        consoleColour (green, true);
        cerr << " File format  : ";
        consoleColour (white, true);
        cerr << tuneInfo.formatString << endl;
        consoleTable  (tableMiddle);
        consoleColour (green, true);
        cerr << " Filename(s)  : ";
        consoleColour (white, true);
        cerr << tuneInfo.dataFileName << endl;
        // Second file is only sometimes present
        if (tuneInfo.infoFileName[0])
        {
            consoleTable  (tableMiddle);
            consoleColour (green, true);
            cerr << "              : ";
            consoleColour (white, true);
            cerr << tuneInfo.infoFileName << endl;
        }
        consoleTable  (tableMiddle);
        consoleColour (green, true);
        cerr << " Condition    : ";
        consoleColour (white, true);
        cerr << tuneInfo.statusString << endl;

#if HAVE_TSID == 1
        if (!m_tsid)
        {
            consoleTable  (tableMiddle);
            consoleColour (green, true);
            cerr << " TSID Error   : ";
            consoleColour (white, true);
            cerr << m_tsid.getError () << endl;
        }
#endif // HAVE_TSID
    }

    consoleTable  (tableMiddle);
    consoleColour (green, true);
    cerr << " Playlist     : ";
    consoleColour (white, true);

    {   // This will be the format used for playlists
        int i = 1;        
        if (!m_track.single)
        {
            i  = m_track.selected;
            i -= (m_track.first - 1);
            if (i < 1)
                i += m_track.songs;
        }
        cerr << i << '/' << m_track.songs;
        cerr << " (tune " << tuneInfo.currentSong << '/'
             << tuneInfo.songs << '['
             << tuneInfo.startSong << "])";
    }

    if (m_track.loop)
        cerr << " [LOOPING]";
    cerr << endl;

    if (m_verboseLevel)
    {
        consoleTable  (tableMiddle);
        consoleColour (green, true);
        cerr << " Song Speed   : ";
        consoleColour (white, true);
        cerr << tuneInfo.speedString << endl;
    }

    consoleTable  (tableMiddle);
    consoleColour (green, true);
    cerr << " Song Length  : ";
    consoleColour (white, true);
    if (m_timer.stop)
        cerr << setw(2) << setfill('0') << ((m_timer.stop / 60) % 100)
             << ':' << setw(2) << setfill('0') << (m_timer.stop % 60);
    else if (m_timer.valid)
        cerr << "FOREVER";
    else
        cerr << "UNKNOWN";
    if (m_timer.start)
    {   // Show offset
        cerr << " (+" << setw(2) << setfill('0') << ((m_timer.start / 60) % 100)
             << ':' << setw(2) << setfill('0') << (m_timer.start % 60) << ")";
    }
    cerr << endl;

    if (m_verboseLevel)
    {
        consoleTable  (tableSeperator);
        consoleTable  (tableMiddle);
        consoleColour (yellow, true);
        cerr << " Addresses    : " << hex;
        cerr.setf(std::ios::uppercase);
        consoleColour (white, false);
        // Display PSID Driver location
        cerr << "DRIVER = ";
        if (info.driverAddr == 0)
            cerr << "NOT PRESENT";
        else
        {
            cerr << "$"  << setw(4) << setfill('0') << info.driverAddr;
            cerr << "-$" << setw(4) << setfill('0') << info.driverAddr +
                (info.driverLength - 1);
        }
        if (tuneInfo.playAddr == 0xffff)
            cerr << ", SYS = $" << setw(4) << setfill('0') << tuneInfo.initAddr;
        else
            cerr << ", INIT = $" << setw(4) << setfill('0') << tuneInfo.initAddr;
        cerr << endl;
        consoleTable  (tableMiddle);
        consoleColour (yellow, true);
        cerr << "              : ";
        consoleColour (white, false);
        cerr << "LOAD   = $" << setw(4) << setfill('0') << tuneInfo.loadAddr;
        cerr << "-$"         << setw(4) << setfill('0') << tuneInfo.loadAddr +
            (tuneInfo.c64dataLen - 1);
        if (tuneInfo.playAddr != 0xffff)
            cerr << ", PLAY = $" << setw(4) << setfill('0') << tuneInfo.playAddr;
        cerr << dec << endl;
        cerr.unsetf(std::ios::uppercase);

        consoleTable  (tableMiddle);
        consoleColour (yellow, true);
        cerr << " SID Details  : ";
        consoleColour (white, false);
        cerr << "Filter = "
             << ((m_filter.enabled == true) ? "Yes" : "No");
        cerr << ", Model = "
             << (info.tuneInfo->sidModel == SID2_MOS8580 ? "8580" : "6581")
             << endl;
        consoleTable  (tableMiddle);
        consoleColour (yellow, true);
        cerr << " Environment  : ";
        consoleColour (white, false);
        switch (info.environment)
        {
        case sid2_envPS:
            cerr << "PlaySID-specific rips";
        break;
        case sid2_envTP:
            cerr << "Transparent ROM";
        break;
        case sid2_envBS:
            cerr << "Bank Switching";
        break;
        case sid2_envR:  // When it happens
            cerr << "Real C64";
        break;
        case sid2_envTR:
            cerr << "Sidusage Tracker Mode";
        break;
        default:
            cerr << "Unknown";
        }

        if (m_engCfg.environment != info.environment)
            cerr << " (forced)";
        cerr << endl;

        if (m_verboseLevel > 1)
        {
            consoleTable  (tableMiddle);
            consoleColour (yellow, true);
            cerr << " Delay        : ";
            consoleColour (white, false);
            cerr << info.powerOnDelay << " (cycles at poweron)" << endl;
        }
    }
    consoleTable (tableEnd);

/*
    cerr << "Credits:\n";
    const char **p;
    const char  *credit;
    p = m_engine.credits ();
    while (*p)
    {
        credit = *p;
        while (*credit)
        {
            cerr << credit << endl;
            credit += strlen (credit) + 1;
        }
        cerr << endl;
        p++;
    }
*/

    if (m_driver.file)
        cerr << "Creating audio file, please wait...";
    else
        cerr << "Playing, press ^C to stop...";

    // Get all the text to the screen so music playback
    // is not disturbed.
    if ( !m_quietLevel )
        cerr << "00:00";
    cerr << flush;
}

// Set colour of text on console
void ConsolePlayer::consoleColour (player_colour_t colour, bool bold)
{
    if ((m_iniCfg.console ()).ansi)
    {
        const char *mode = "";

        switch (colour)
        {
        case black:   mode = "30"; break;
        case red:     mode = "31"; break;
        case green:   mode = "32"; break;
        case yellow:  mode = "33"; break;
        case blue:    mode = "34"; break;
        case magenta: mode = "35"; break;
        case cyan:    mode = "36"; break;
        case white:   mode = "37"; break;
        }

        if (bold)
            cerr << '\x1b' << "[1;40;" << mode << 'm';
        else
            cerr << '\x1b' << "[0;40;" << mode << 'm';
    }
}

// Display menu outline
void ConsolePlayer::consoleTable (player_table_t table)
{
    const uint tableWidth = 54;

    consoleColour (white, true);
    switch (table)
    {
    case tableStart:
        cerr << (m_iniCfg.console ()).topLeft << setw(tableWidth)
             << setfill ((m_iniCfg.console ()).horizontal) << ""
             << (m_iniCfg.console ()).topRight;
    break;

    case tableMiddle:
        cerr << setw(tableWidth + 1) << setfill(' ') << ""
             << (m_iniCfg.console ()).vertical << '\r'
             << (m_iniCfg.console ()).vertical;
    return;

    case tableSeperator:
        cerr << (m_iniCfg.console ()).junctionRight << setw(tableWidth)
             << setfill ((m_iniCfg.console ()).horizontal) << ""
             << (m_iniCfg.console ()).junctionLeft;
    break;

    case tableEnd:
        cerr << (m_iniCfg.console ()).bottomLeft << setw(tableWidth)
             << setfill ((m_iniCfg.console ()).horizontal) << ""
             << (m_iniCfg.console ()).bottomRight;
    break;
    }

    // Move back to begining of row and skip first char
    cerr << "\n";
}

// Restore Ansi Console to defaults
void ConsolePlayer::consoleRestore ()
{
    if ((m_iniCfg.console ()).ansi)
        cerr << '\x1b' << "[0m";
}
