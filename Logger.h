/*
	Logger - der Apache Library Log4j nachempfunden. Verschiedene
	LogLevel definieren, welche Meldungen angezeigt werden sollen.

	Copyright (c) 2013 Marcel Bernet.  All right reserved.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

/////////////////////////////////////////////////////////////////////////////////
// Meldungen
#define	OFF_LEVEL		0		// keine Meldungen
#define	ERROR_LEVEL		1 		// Fehler in der Programmausfuehrung
#define	WARN_LEVEL		2		// Warnungen
#define	INFO_LEVEL		3		// Informationen, Start, Initialisierung i.o. etc.
#define	DEBUG_LEVEL		4		// Allgemeine Debug Infos
#define	TRACE_LEVEL		5		// Fein Detalierte Meldungen, nur in Ausnahmefaellen zu aktivieren

#ifndef	LOGGER_LEVEL			// Eclipse kann Symbols Global setzen = 1. Prioritaet
#if ( __AVR_ATmega1280__ || __AVR_ATmega2560__ )
#define	LOGGER_LEVEL	INFO_LEVEL
#else
#define LOGGER_LEVEL 	OFF_LEVEL
#endif
#endif

/**
 * Allgemeine Initialisierungen
 */
#if ( LOGGER_LEVEL > OFF_LEVEL)

#if ( __AVR_ATmega1280__ || __AVR_ATmega2560__ )
#define Logger	Serial3
#elif ( __AVR_ATmega328P__ )
extern	SoftwareSerial Logger;
#endif

#define BEGIN(x) 	Logger.begin( x );
#else
#define BEGIN(x)
#endif

/**
 * Error
 */
#if ( LOGGER_LEVEL >= ERROR_LEVEL )
#define ERROR(str) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("E: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(' ');      \
		Logger.print(__FILE__);     \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"');
#else
#define ERROR(str)
#endif

/**
 * Warning
 */
#if ( LOGGER_LEVEL >= WARN_LEVEL )
#define WARN(str) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("W: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"');
#else
#define WARN(str)
#endif

/**
 * Info
 */
#if ( LOGGER_LEVEL >= INFO_LEVEL )
#define INFO(str) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("I: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"');
#else
#define INFO(str)
#endif

#if ( LOGGER_LEVEL >= INFO_LEVEL )
#define INFO2(str, v) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("I: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"'); \
		Logger.print( ", " ); \
		Logger.print( v );
#else
#define INFO2(str, v)
#endif

#if ( LOGGER_LEVEL >= INFO_LEVEL )
#define INFO3(str, v, x ) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("I: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"'); \
		Logger.print( ", " ); \
		Logger.print( v ); \
		Logger.print( ", " ); \
		Logger.print( x );
#else
#define INFO3(str, v, x)
#endif

#if ( LOGGER_LEVEL >= INFO_LEVEL )
#define INFO_DEVICES() \
		int values[6]; \
		DeviceManager.getDescription( 0, 0, srcp::LAN, values ); \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("I: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print("Devices: "); \
		Logger.print('\"'); \
		Logger.print( ", FB "); \
		Logger.print( values[0] ); \
		Logger.print( "-" ); \
		Logger.print( values[1] ); \
		Logger.print( ", GA "); \
		Logger.print( values[2] ); \
		Logger.print( "-" ); \
		Logger.print( values[3] ); \
		Logger.print( ", GL "); \
		Logger.print( values[4] ); \
		Logger.print( "-" ); \
		Logger.print( values[5] );
#else
#define INFO_DEVICES(str)
#endif
/**
 * Debug
 */
#if ( LOGGER_LEVEL >= DEBUG_LEVEL )
#define DEBUG(str) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("D: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"');
#else
#define DEBUG(str)
#endif

#if ( LOGGER_LEVEL >= DEBUG_LEVEL )
#define DEBUG2(str, v) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("D: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"'); \
		Logger.print( ", " ); \
		Logger.print( v );
#else
#define DEBUG2(str, v)
#endif

#if ( LOGGER_LEVEL >= DEBUG_LEVEL )
#define DEBUG3(str, v, x ) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("D: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"'); \
		Logger.print( ", " ); \
		Logger.print( v ); \
		Logger.print( ", " ); \
		Logger.print( x );
#else
#define DEBUG3(str, v, x)
#endif

/**
 * Trace
 */
#if ( LOGGER_LEVEL >= TRACE_LEVEL )
#define TRACE(str) \
		Logger.println(); \
		Logger.print(millis());     \
		Logger.print("T: ");    \
		Logger.print(__PRETTY_FUNCTION__); \
		Logger.print(':');      \
		Logger.print(__LINE__);     \
		Logger.print(" \"");      \
		Logger.print(str); \
		Logger.print('\"');
#else
#define TRACE(str)
#endif


#endif /* LOGGER_H_ */
