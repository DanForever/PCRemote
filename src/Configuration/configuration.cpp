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

#include "configuration.h"

#include <StreamUtils.h>
#include <DF-Print.h>

#include "../Filesystem/filesystem.h"

#define BUFFER_CHUNK_SIZE 64
#define CONFIG_PATH "/config/config.json"

Configuration g_Config;

void Configuration::Initialize()
{
	// Open the config file
	File configFile = Filesystem::Open(CONFIG_PATH);
	
	if (!configFile)
	{
		DEBUG_PRINTLN("No configuration file found, defaults will be used");
	}

	// Read it using a buffered stream
	ReadBufferingStream bufferedConfigFile(configFile, BUFFER_CHUNK_SIZE);

	// Create the document that we'll stream into
	DynamicJsonDocument document(1024);

	// Load the json document
	deserializeJson(document, bufferedConfigFile);

	// Import the values from the json document
	m_network.Deserialize(document);
	m_credentials.Deserialize(document);

	configFile.close();
}

void Configuration::Save()
{
	DynamicJsonDocument document(1024);
	
	m_network.Serialize(document);
	m_credentials.Serialize(document);

	File configFile = Filesystem::Open(CONFIG_PATH, "w");

	// Write it using a buffered stream
	WriteBufferingStream bufferedConfigFile(configFile, BUFFER_CHUNK_SIZE);

	serializeJson(document, bufferedConfigFile);

	bufferedConfigFile.flush();
	configFile.close();
}

bool Configuration::Delete()
{
	DEBUG_PRINTLN("Deleting configuration file...");

	bool success = Filesystem::Delete(CONFIG_PATH);

	if (success)
	{
		DEBUG_PRINTLN("Config file successfully deleted");
	}

	return success;
}
