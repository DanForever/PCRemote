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

#include "computer.h"

#include <Arduino.h>

#include "../Debug/DebugPrint.h"

#define MICROCONTROLLER_LED_PIN 16

#define POWER_LED_PIN 2
#define POWER_SWITCH_PIN 4

void Computer::Initialize()
{
	pinMode(MICROCONTROLLER_LED_PIN, OUTPUT);

	pinMode(POWER_SWITCH_PIN, OUTPUT);
	digitalWrite(POWER_SWITCH_PIN, LOW);

	pinMode(POWER_LED_PIN, INPUT_PULLUP);
}

void Computer::PressPowerButton(unsigned long duration)
{
	DEBUG_PRINTLN("Power button pressed");

	m_pressRequested = true;
	m_targetPressDuration = duration;
	m_pressTimestamp = millis();
	digitalWrite(POWER_SWITCH_PIN, HIGH);
}

void Computer::Update()
{
	UpdatePowerButton();
	UpdatePowerCheck();
}

void Computer::UpdatePowerButton()
{
	if (m_pressRequested)
	{
		const unsigned long now = millis();
		const unsigned long currentPressDuration = now - m_pressTimestamp;

		if (currentPressDuration > m_targetPressDuration)
		{
			DEBUG_PRINTLN("Power button released");
			digitalWrite(POWER_SWITCH_PIN, LOW);
			m_pressRequested = false;
		}
	}
}

void Computer::UpdatePowerCheck()
{
	m_isPowerOn = digitalRead(POWER_LED_PIN) == LOW;

	if (m_isPowerOn)
		digitalWrite(MICROCONTROLLER_LED_PIN, HIGH);
	else
		digitalWrite(MICROCONTROLLER_LED_PIN, LOW);
}

Computer g_Computer;
