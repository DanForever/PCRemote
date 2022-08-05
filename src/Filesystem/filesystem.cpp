/*
* PC Remote
* Firmware for an IOT-device that can remotely press the power switch
* of an ATX style PC as well as monitor its current power state (on/off)
* https://github.com/DanForever/PCRemote
* Copyright (C) 2022 Daniel Neve
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "filesystem.h"
//#define USE_SPIFFS
#define USE_LITTLEFS

#if defined(USE_SPIFFS)
#	include <FS.h>
#	define FS_CONFIG_TYPE SPIFFSConfig
#	define FS_OBJECT SPIFFS
#elif defined(USE_LITTLEFS)
#	include <LittleFS.h>
#	define FS_CONFIG_TYPE LittleFSConfig
#	define FS_OBJECT LittleFS
#else
#	error No filesystem defined
#endif

void Filesystem::Initialize()
{
	// By default, SPIFFS and LittleFS will format the filesystem
	// if they are unable to mount it when you call begin()
	FS_CONFIG_TYPE config;
	config.setAutoFormat(false);
	FS_OBJECT.setConfig(config);

	FS_OBJECT.begin();
}

void Filesystem::Deinitialize()
{
	FS_OBJECT.end();
}

File Filesystem::Open(const char* path)
{
	return FS_OBJECT.open(path, "r");
}

File Filesystem::Open(const char* path, const char* mode)
{
	return FS_OBJECT.open(path, mode);
}

bool Filesystem::Exists(const char* path)
{
	return FS_OBJECT.exists(path);
}

bool Filesystem::Delete(const char* path)
{
	return FS_OBJECT.remove(path);
}
