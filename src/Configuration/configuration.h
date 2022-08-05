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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <ArduinoJson.h>

#include "Network.h"

#define CONFIG_DOC_BUFFERSIZE (1024 * 4)

class Configuration
{
public:
	void Initialize();
	void Save();
	bool Delete();

	ConfigurationImpl::Network& Network() { return m_network; }
	const ConfigurationImpl::Network& Network() const { return m_network; }

private:
	ConfigurationImpl::Network m_network;
};

extern Configuration g_Config;

#endif // __CONFIGURATION_H__
