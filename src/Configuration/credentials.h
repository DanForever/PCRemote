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

#ifndef __CONFIGURATION_CREDENTIALS_H__
#define __CONFIGURATION_CREDENTIALS_H__

#include <ArduinoJson.h>

#include "wifi.h"
#include "ap.h"
#include "subconfiguration.h"

namespace ConfigurationImpl
{
	class Credentials : public ISubConfiguration
	{
	public:
		Credentials() = default;
		~Credentials() override = default;

		const String& Username() const { return m_username; }
		const String& Password() const { return m_password; }

		void Username(const String& username) { m_username = username; }
		void Password(const String& password) { m_password = password; }

		// Inherited via ISubConfiguration
		virtual void Serialize(JsonDocument& document) override;
		virtual void Deserialize(JsonDocument& document) override;

	private:
		String m_username;
		String m_password;
	};
}

#endif // __CONFIGURATION_NETWORK_H__
