/***************************************************************************
                          error.h  -  description
                             -------------------
    begin                : Sun Nov 18 2001
    copyright            : (C) 2001 by Paul S.J.Millard
    email                : apex@apexnow.co.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _ERROR_HPP_
#define _ERROR_HPP_

#define ERROR_INITIALISE		1
#define ERROR_ALLEGRO			2
#define ERROR_DATAFILE			3
#define ERROR_LOADLEVEL			4
#define ERROR_INITGRAPHICS		5
#define ERROR_SOUND				6
#define ERROR_RENDER			7
#define ERROR_CLEARBACKBUFFER	8
#define ERROR_LOADSPRITES		9
#define ERROR_LOWMEMORY			10
#define ERROR_SAVEMAZE			11
#define ERROR_SAVEGAME			12
#define ERROR_LOADGAME			13

const char *ErrorText( int nError )
{
	switch( nError )
	{
		case ERROR_INITIALISE:
			return "Initialisation error";

		case ERROR_ALLEGRO:
			return "An internal Allegro error occured";

		case ERROR_DATAFILE:
			return "Failed to load Data file";

		case ERROR_LOADLEVEL:
			return "Failed to load level data";

		case ERROR_INITGRAPHICS:
			return "Unable to initialise graphics mode";

		case ERROR_RENDER:
			return "Error during video rendering";

		case ERROR_CLEARBACKBUFFER:
			return "Failed to clear back buffer";

		case ERROR_LOADSPRITES:
			return "Failed to load sprite data";

		case ERROR_LOWMEMORY:
			return "Low memory";

		default:
			return "Unknown error";
	}
}

#endif
