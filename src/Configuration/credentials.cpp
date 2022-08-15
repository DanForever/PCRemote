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

#include "credentials.h"

#include "utilities.h"
#include <DF-Print.h>

namespace Key
{
	constexpr auto CREDENTIALS = "credentials";
	constexpr auto USERNAME = "username";
	constexpr auto PASSWORD = "password";
}

void ConfigurationImpl::Credentials::Serialize(JsonDocument& document)
{
	DEBUG_PRINTLN("Saving credentials config...");

	Utility::Write(document, m_username, Key::CREDENTIALS, Key::USERNAME);
	Utility::Write(document, m_password, Key::CREDENTIALS, Key::PASSWORD);
}

void ConfigurationImpl::Credentials::Deserialize(JsonDocument& document)
{
	Utility::Read(document, m_username, Key::CREDENTIALS, Key::USERNAME);
	Utility::Read(document, m_password, Key::CREDENTIALS, Key::PASSWORD);
}
