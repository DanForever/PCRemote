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

#include "device.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "../Configuration/configuration.h"
#include "../Debug/DebugPrint.h"

Device g_device;

void Device::Initialize()
{
	SetupWifi();
	SetupHostname();
	SetupAccessPoint();
}

void Device::SetMode()
{
	bool wifi = g_Config.Network().Wifi().Enabled();
	bool ap = g_Config.Network().AccessPoint().Enabled();

	if (wifi && ap)
	{
		WiFi.mode(WIFI_AP_STA);
	}
	else if (wifi)
	{
		WiFi.mode(WIFI_STA);
	}
	else if (ap)
	{
		WiFi.mode(WIFI_AP);
	}
}

void Device::SetupWifi()
{
	DEBUG_PRINTLN("Setting up wifi...");

	if (WiFi.isConnected())
		WiFi.disconnect();

	SetMode();

	if (!g_Config.Network().Wifi().Enabled())
	{
		DEBUG_PRINTLN("Wifi is disabled");
		return;
	}

	const char* ssid = g_Config.Network().Wifi().SSID();
	const char* password = g_Config.Network().Wifi().Password();

	if (ssid && strlen(ssid) > 0)
	{
		DEBUG_PRINTLN("Connecting to SSID '{0}' loaded from config file", ssid);
		WiFi.begin(ssid, password);

		switch (WiFi.waitForConnectResult())
		{
		case WL_CONNECTED:
			DEBUG_PRINTLN("Connected to wifi!");
			break;

		case WL_NO_SSID_AVAIL:
			DEBUG_PRINTLN("Cannot find SSID: '{0}'", ssid);
			break;

		case WL_CONNECT_FAILED:
			DEBUG_PRINTLN("Connection to wifi failed");
			break;

		case WL_WRONG_PASSWORD:
			DEBUG_PRINTLN("Wifi password is incorrect");
			break;

		case WL_DISCONNECTED:
			DEBUG_PRINTLN("Cannot connect to wifi, incorrect station mode set");
			break;
		}
	}
}

void Device::SetupHostname()
{
	if (!g_Config.Network().Wifi().Enabled())
		return;

	const char* hostname = g_Config.Network().Wifi().Hostname();

	if (hostname && strlen(hostname) > 0)
	{
		DEBUG_PRINTLN("Restoring hostname '{0}' loaded from config file", hostname);

		WiFi.hostname(hostname);
	}
	else
	{
		DEBUG_PRINTLN("No hostname found");
	}
}

void Device::SetupAccessPoint()
{
	DEBUG_PRINTLN("Setting up Access point...");

	SetMode();

	WiFi.softAPdisconnect();

	if (!g_Config.Network().AccessPoint().Enabled())
	{
		DEBUG_PRINTLN("Access point is disabled");
		return;
	}

	const char* ssid = g_Config.Network().AccessPoint().SSID();
	const char* password = g_Config.Network().AccessPoint().Password();

	bool success = WiFi.softAP(ssid, password);

	if (!success)
	{
		DEBUG_PRINTLN("Failed to setup access point with:");
		DEBUG_PRINTLN("SSID: {0}", ssid);
		DEBUG_PRINTLN("PASSWORD: {0}", password);
	}
}
