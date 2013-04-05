/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Fri Jun 2 2000
    copyright            : (C) 2000 by Simon White
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
 *  $Log: main.cpp,v $
 *  Revision 1.32  2004/02/26 18:19:22  s_a_white
 *  Updates for VC7 (use real libstdc++ headers instead of draft ones).
 *
 *  Revision 1.31  2004/02/12 05:58:03  s_a_white
 *  Update argurements and help menu handling.
 *
 *  Revision 1.30  2003/06/27 21:09:37  s_a_white
 *  Constified some of the interfaces.
 *
 *  Revision 1.29  2003/02/22 09:41:17  s_a_white
 *  Removed endl, was in wrong location.
 *
 *  Revision 1.28  2003/02/20 18:50:43  s_a_white
 *  sid2crc support.
 *
 *  Revision 1.27  2002/03/11 18:02:56  s_a_white
 *  Display errors like sidplay1.
 *
 *  Revision 1.26  2002/01/30 00:33:00  s_a_white
 *  Error message now multiline.
 *
 *  Revision 1.25  2001/12/07 18:22:33  s_a_white
 *  Player quit fixes.
 *
 *  Revision 1.24  2001/12/05 22:22:48  s_a_white
 *  Added playerFast flag.
 *
 *  Revision 1.23  2001/12/01 20:15:49  s_a_white
 *  Player changed to ConsolePlayer.
 *
 *  Revision 1.22  2001/11/27 19:10:12  s_a_white
 *  Restructured
 *
 *  Revision 1.21  2001/11/21 18:55:25  s_a_white
 *  Sidplay2 0.8 new frontend interface.
 *
 *  Revision 1.19  2001/09/01 11:48:42  s_a_white
 *  Help moved from cerr to cout.
 *
 *  Revision 1.18  2001/09/01 11:24:22  s_a_white
 *  Renamed configure to config.
 *
 *  Revision 1.17  2001/08/20 18:29:55  s_a_white
 *  SID Model now obtained from info structure.
 *
 *  Revision 1.16  2001/07/25 17:11:32  s_a_white
 *  Support new libsidplay2 configuration interface.
 *
 *  Revision 1.15  2001/07/14 12:38:19  s_a_white
 *  Added sid loop counter, to exit multi-sid tunes.  Added -b to set start
 *  of song.  0xffff songs now get reported as sys.  Support for sidbuilder
 *  classes.  !TODO! must tidy this file, it's getting too big.
 *
 *  Revision 1.14  2001/04/23 17:08:33  s_a_white
 *  Added extended video flag -v<n|p>[f].
 *
 *  Revision 1.13  2001/04/21 13:28:31  s_a_white
 *  Updated help information.
 *
 *  Revision 1.12  2001/04/21 10:28:22  s_a_white
 *  Fix -w flag to take an optional filename.
 *
 *  Revision 1.11  2001/03/27 19:35:33  s_a_white
 *  Moved default record length for wav files from main.cpp to IniConfig.cpp.
 *
 *  Revision 1.10  2001/03/27 19:00:49  s_a_white
 *  Default record and play lengths can now be set in the sidplay2.ini file.
 *
 *  Revision 1.9  2001/03/27 17:14:39  s_a_white
 *  Time length can be made INFINITE by using -t0 on the command line.
 *
 *  Revision 1.8  2001/03/26 18:14:20  s_a_white
 *  Removed debug code.
 *
 *  Revision 1.7  2001/03/21 22:54:55  s_a_white
 *  Support for ini config file and libsidutils tools.
 *
 *  Revision 1.6  2001/03/04 12:58:56  s_a_white
 *  Defualt environment changed to real.  Verbose info now printed correctly.
 *
 *  Revision 1.5  2001/03/01 23:47:00  s_a_white
 *  Support for sample mode to be selected at runtime.
 *
 *  Revision 1.4  2001/02/08 20:58:01  s_a_white
 *  Help screen bug fix for default precision and optimisation, which were
 *  printed as characters.
 *
 *  Revision 1.3  2001/01/23 22:54:24  s_a_white
 *  Prevents timer overwriting paused message.
 *
 *  Revision 1.2  2001/01/23 21:25:15  s_a_white
 *  Only way to load a tune now is by passing in a sidtune object.  This is
 *  required for songlength database support.
 *
 *  Revision 1.1  2001/01/08 16:41:42  s_a_white
 *  App and Library Seperation
 *
 *  Revision 1.20  2000/12/11 18:52:12  s_a_white
 *  Conversion to AC99
 *
 ***************************************************************************/

#include <stdlib.h>
#include <iostream>
using std::cerr;
using std::endl;
#include <signal.h>
#include "player.h"
#include "keyboard.h"
#include "SimpleInterface.h"


// Function prototypes
static void sighandler (int signum);
static ConsolePlayer *g_player;

int main(int argc, char *argv[])
{
    ConsolePlayer player(argv[0]);
    g_player = &player;

    {// Decode the command line args
        int ret = player.args (argc - 1, const_cast<const char**>(argv + 1));
        if (ret < 0)
            goto main_error;
        else if (!ret)
            goto main_exit;
    }

main_restart:
    if (!player.open ())
        goto main_error;

    // Install signal error handlers
    if ((signal (SIGINT,  &sighandler) == SIG_ERR)
     || (signal (SIGABRT, &sighandler) == SIG_ERR)
     || (signal (SIGTERM, &sighandler) == SIG_ERR))
    {
        displayError(argv[0], ERR_SIGHANDLER);
        goto main_error;
    }

#ifdef HAVE_UNIX
    // Configure terminal to allow direct access to key events
    keyboard_enable_raw ();
#endif // HAVE_UNIX

    // Play loop
    for (;;) {
        if (!player.play ())
            break;
    }

#ifdef HAVE_UNIX
    keyboard_disable_raw ();
#endif

    // Restore default signal error handlers
    if ((signal (SIGINT,  SIG_DFL) == SIG_ERR)
     || (signal (SIGABRT, SIG_DFL) == SIG_ERR)
     || (signal (SIGTERM, SIG_DFL) == SIG_ERR))
    {
        displayError(argv[0], ERR_SIGHANDLER);
        goto main_error;
    }

    if ((player.state() & ~playerFast) == playerRestart)
        goto main_restart;
main_exit:
    player.close ();
    return EXIT_SUCCESS;

main_error:
    player.close ();
    return EXIT_FAILURE;
}


void sighandler (int signum)
{
    switch (signum)
    {
    case SIGINT:
    case SIGABRT:
    case SIGTERM:
        // Exit now!
        g_player->stop ();
    break;
    default: break;
    }
}


void displayError (const char *arg0, uint num)
{
    cerr << arg0 << ": ";

    switch (num)
    {
    case ERR_SYNTAX:
        cerr << "command line syntax error" << endl
             << "Try `" << arg0 << " --help' for more information." << endl;
    break;

    case ERR_NOT_ENOUGH_MEMORY:
        cerr << "ERROR: Not enough memory." << endl;
    break;

    case ERR_SIGHANDLER:
        cerr << "ERROR: Could not install signal handler." << endl;
    break;

    case ERR_FILE_OPEN:
        cerr << "ERROR: Could not open file for binary input." << endl;
    break;

    default: break;
    }
}

// MPi: These functions were added by me for Music Studio to use.
extern "C" void *AllocatePlayer(const char *flags,const char *filename)
{
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path),path);
	ConsolePlayer *player = new ConsolePlayer(path);
	player->m_quietLevel = 255;
	player->m_verboseLevel = 0;
	if (flags)
	{
		const char *argv[] = {flags,filename,0};
		player->args(2,argv);
	}
	else
	{
		const char *argv[] = {filename,0};
		player->args(1,argv);
	}
	
	player->open();

	return (void *) player;
}

extern "C" bool PlayerPlay(void *handle)
{
	ConsolePlayer *player = (ConsolePlayer *)handle;
	if (!player)
	{
		return false;
	}

	return player->play();
}

extern "C" unsigned char PlayerGetMemoryByte(void *handle,const unsigned short address)
{
	ConsolePlayer *player = (ConsolePlayer *)handle;
	if (!player)
	{
		return 0;
	}

	unsigned char theByte = player->m_engine.MPiReadMemByte(address);

	return theByte;
}

extern "C" void PlayerSetMemoryByte(void *handle,const unsigned short address,const unsigned char data)
{
	ConsolePlayer *player = (ConsolePlayer *)handle;
	if (!player)
	{
		return;
	}

	player->m_engine.MPiWriteMemByte(address,data);
}

extern "C" void PlayerFree(void *handle)
{
	ConsolePlayer *player = (ConsolePlayer *)handle;
	if (!player)
	{
		return;
	}

	player->close();
	delete player;
}

extern "C" void PlayerMute(void *handle,const int voice,const bool enable)
{
	ConsolePlayer *player = (ConsolePlayer *)handle;
	if (!player)
	{
		return;
	}

	player->m_engine.mute(voice,enable);
}

extern "C" float PlayerGetTime(void *handle)
{
	ConsolePlayer *player = (ConsolePlayer *)handle;
	if (!player)
	{
		return 0.0f;
	}

	return float(player->m_engine.time()) / float(player->m_engine.timebase());
}
