//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Main program, simply calls D_DoomMain high level loop.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"

#include "doomtype.h"
#include "i_system.h"
#include "m_argv.h"
#include "m_misc.h"

#ifdef __SWITCH__
#include <arpa/inet.h>
#include <switch.h>
#endif

//
// D_DoomMain()
// Not a globally visible function, just included for source reference,
// calls all startup code, parses command line options.
//

void D_DoomMain(void);

int main(int argc, char **argv)
{
#ifdef __SWITCH__
    // Initialize console. Using NULL as the second argument tells the console library to use the internal console structure as current one.
    //consoleInit(NULL);

    // Configure supported input layout: a single player with standard controller.
    //padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    //PadState pad;
    //padInitializeDefault(&pad);

    // Initialize nxlink sockets
    socketInitializeDefault();

    printf("nxlink host is %s\n", inet_ntoa(__nxlink_host));

    // Redirect stdout and stderr to nxlink server
    nxlinkStdio();

    printf("Chocolate Doom NX v%s\n", PACKAGE_VERSION);
#endif

    // save arguments
    myargc = argc;
    myargv = malloc(argc * sizeof(char *));
    assert(myargv != NULL);

    for (int i = 0; i < argc; i++)
    {
        myargv[i] = M_StringDuplicate(argv[i]);
    }

    //!
    // Print the program version and exit.
    //
    if (M_ParmExists("-version") || M_ParmExists("--version"))
    {
        puts(PACKAGE_STRING);
        exit(0);
    }

#if defined(_WIN32)
    // compose a proper command line from loose file paths passed as arguments
    // to allow for loading WADs and DEHACKED patches by drag-and-drop
    M_AddLooseFiles();
#endif

    M_FindResponseFile();
    M_SetExeDir();

#ifdef SDL_HINT_NO_SIGNAL_HANDLERS
    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
#endif

    // start doom

    D_DoomMain();

#ifdef __SWITCH__
    socketExit();
    //consoleExit(NULL);
#endif

    return 0;
}
