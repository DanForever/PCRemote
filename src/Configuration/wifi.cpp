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

#include "wifi.h"

#include "utilities.h"
#include "../Debug/DebugPrint.h"

namespace Key
{
	constexpr auto NETWORK = "network";
	constexpr auto WIFI = "wifi";
	constexpr auto ENABLED = "enabled";
	constexpr auto SSID = "ssid";
	constexpr auto PASSWORD = "password";
	constexpr auto HOSTNAME = "hostname";
}

void ConfigurationImpl::Wifi::Enabled(bool enabled)
{
	m_enabled = enabled;
}

bool ConfigurationImpl::Wifi::Enabled() const
{
	return m_enabled;
}

void ConfigurationImpl::Wifi::SSID(const char* ssid)
{
	m_ssid = ssid;
}

void ConfigurationImpl::Wifi::SSID(const String& ssid)
{
	m_ssid = ssid;
}

const char* ConfigurationImpl::Wifi::SSID() const
{
	return m_ssid.c_str();
}

void ConfigurationImpl::Wifi::Password(const char* password)
{
	m_password = password;
}

void ConfigurationImpl::Wifi::Password(const String& password)
{
	m_password = password;
}

const char* ConfigurationImpl::Wifi::Password() const
{
	return m_password.c_str();
}

void ConfigurationImpl::Wifi::Hostname(const char* hostname)
{
	m_hostname = hostname;
}

void ConfigurationImpl::Wifi::Hostname(const String& hostname)
{
	m_hostname = hostname;
}

const char* ConfigurationImpl::Wifi::Hostname() const
{
	return m_hostname.c_str();
}

void ConfigurationImpl::Wifi::Serialize(JsonDocument& document)
{
	DEBUG_PRINTLN("Saving Wifi config...");

	Utility::Write(document, m_ssid, Key::NETWORK, Key::WIFI, Key::SSID);
	Utility::Write(document, m_password, Key::NETWORK, Key::WIFI, Key::PASSWORD);
	Utility::Write(document, m_enabled, Key::NETWORK, Key::WIFI, Key::ENABLED);
	Utility::Write(document, m_hostname, Key::NETWORK, Key::WIFI, Key::HOSTNAME);
}

void ConfigurationImpl::Wifi::Deserialize(JsonDocument& document)
{
	DEBUG_PRINTLN("Loading Wifi config...");

	Utility::Read(document, m_ssid, Key::NETWORK, Key::WIFI, Key::SSID);
	Utility::Read(document, m_password, Key::NETWORK, Key::WIFI, Key::PASSWORD);
	Utility::ReadDefault(document, m_enabled, false, Key::NETWORK, Key::WIFI, Key::ENABLED);
	Utility::Read(document, m_hostname, Key::NETWORK, Key::WIFI, Key::HOSTNAME);

	DEBUG_PRINTLN("  - SSID: {0}", m_ssid);
	DEBUG_PRINTLN("  - Password: {0}", m_password);
	DEBUG_PRINTLN("  - Enabled: {0}", m_enabled);
	DEBUG_PRINTLN("  - Hostname: {0}", m_hostname);
}
