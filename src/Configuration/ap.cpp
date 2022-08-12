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

#include "ap.h"

#include <ESP8266WiFi.h>

#include "utilities.h"
#include "../Debug/DebugPrint.h"

namespace Key
{
	constexpr auto NETWORK = "network";
	constexpr auto AP = "ap";
	constexpr auto ENABLED = "enabled";
	constexpr auto SSID = "ssid";
	constexpr auto PASSWORD = "password";
}

void ConfigurationImpl::AccessPoint::Enabled(bool enabled)
{
	m_enabled = enabled;
}

bool ConfigurationImpl::AccessPoint::Enabled() const
{
	return m_enabled;
}

void ConfigurationImpl::AccessPoint::SSID(const char* ssid)
{
	m_ssid = ssid;
}

void ConfigurationImpl::AccessPoint::SSID(const String& ssid)
{
	m_ssid = ssid;
}

const char* ConfigurationImpl::AccessPoint::SSID() const
{
	return m_ssid.c_str();
}

void ConfigurationImpl::AccessPoint::Password(const char* password)
{
	m_password = password;
}

void ConfigurationImpl::AccessPoint::Password(const String& password)
{
	m_password = password;
}

const char* ConfigurationImpl::AccessPoint::Password() const
{
	return m_password.c_str();
}

void ConfigurationImpl::AccessPoint::Serialize(JsonDocument& document)
{
	DEBUG_PRINTLN("Saving access point config...");

	Utility::Write(document, m_ssid, Key::NETWORK, Key::AP, Key::SSID);
	Utility::Write(document, m_password, Key::NETWORK, Key::AP, Key::PASSWORD);
	Utility::Write(document, m_enabled, Key::NETWORK, Key::AP, Key::ENABLED);
}

void ConfigurationImpl::AccessPoint::Deserialize(JsonDocument& document)
{
	Utility::ReadDefault(document, m_ssid, GenerateDefaultSSID(), Key::NETWORK, Key::AP, Key::SSID);
	Utility::ReadDefault(document, m_password, {}, Key::NETWORK, Key::AP, Key::PASSWORD);
	Utility::ReadDefault(document, m_enabled, true, Key::NETWORK, Key::AP, Key::ENABLED);
}

String ConfigurationImpl::AccessPoint::GenerateDefaultSSID() const
{
	uint8_t macAddr[6];
	WiFi.softAPmacAddress(macAddr);

	char buffer[64];
	snprintf(buffer, 64, "PCRemote_%02x%02x%02x%02x%02x%02x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);

	return buffer;
}
