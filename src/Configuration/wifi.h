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

#ifndef __CONFIGURATION_WIFI_H__
#define __CONFIGURATION_WIFI_H__

#include <ArduinoJson.h>

#include "subconfiguration.h"

namespace ConfigurationImpl
{
	class Wifi : public ISubConfiguration
	{
	public:
		Wifi() = default;
		~Wifi() override = default;

		void Enabled(bool enabled);
		bool Enabled() const;

		void SSID(const char* ssid);
		void SSID(const String& ssid);
		const char* SSID() const;

		void Password(const char* password);
		void Password(const String& password);
		const char* Password() const;

		void Hostname(const char* hostname);
		void Hostname(const String& hostname);
		const char* Hostname() const;

		// Inherited via ISubConfiguration
		virtual void Serialize(JsonDocument& document) override;
		virtual void Deserialize(JsonDocument& document) override;

	private:
		// Maximum length of an SSID is 32 characters (+1 for null terminator)
		// https://serverfault.com/a/45509
		//constexpr int MAX_SSID_LENGTH = 33;
		//char m_ssid[MAX_SSID_LENGTH];
		String m_ssid;

		// Maximum length of a wpa2 password is 64 characters
		// https://routersecurity.org/wifi.passwords.php
		//constexpr int MAX_PASSWORD_LENGTH = 64;
		//char m_password[MAX_PASSWORD_LENGTH];
		String m_password;

		// Maximum length of a unix hostname is 255 characters
		// https://routersecurity.org/wifi.passwords.php
		//constexpr int MAX_PASSWORD_LENGTH = 256;
		//char m_hostname[MAX_PASSWORD_LENGTH];
		String m_hostname;

		bool m_enabled;
	};
}

#endif // __CONFIGURATION_WIFI_H__
