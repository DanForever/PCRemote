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
	Serial.println("Setting up wifi...");

	if (WiFi.isConnected())
		WiFi.disconnect();

	SetMode();

	if (!g_Config.Network().Wifi().Enabled())
	{
		Serial.println("Wifi is disabled");
		return;
	}

	const char* ssid = g_Config.Network().Wifi().SSID();
	const char* password = g_Config.Network().Wifi().Password();

	if (ssid && strlen(ssid) > 0)
	{
		Serial.print("Connecting to SSID '");
		Serial.print(ssid);
		Serial.println("' loaded from config file");
		WiFi.begin(ssid, password);

		switch (WiFi.waitForConnectResult())
		{
		case WL_CONNECTED:
			Serial.println("Connected to wifi!");
			break;

		case WL_NO_SSID_AVAIL:
			Serial.print("Cannot find ");
			Serial.println(ssid);
			break;

		case WL_CONNECT_FAILED:
			Serial.println("Connection to wifi failed");
			break;

		case WL_WRONG_PASSWORD:
			Serial.println("Wifi password is incorrect");
			break;

		case WL_DISCONNECTED:
			Serial.println("Cannot connect to wifi, incorrect station mode set");
			break;
		}
	}
}

void Device::SetupHostname()
{
	if (!g_Config.Network().Wifi().Enabled())
		return;

	const char* hostname = g_Config.Network().Wifi().Hostname();

	if (hostname)
	{
		Serial.print("Restoring hostname '");
		Serial.print(hostname);
		Serial.println("' loaded from config file");

		WiFi.hostname(hostname);
	}
	else
	{
		Serial.println("No hostname found");
	}
}

void Device::SetupAccessPoint()
{
	Serial.println("Setting up Access point...");

	SetMode();

	WiFi.softAPdisconnect();

	if (!g_Config.Network().AccessPoint().Enabled())
	{
		Serial.println("Access point is disabled");
		return;
	}

	const char* ssid = g_Config.Network().AccessPoint().SSID();
	const char* password = g_Config.Network().AccessPoint().Password();

	bool success = WiFi.softAP(ssid, password);

	if (!success)
	{
		Serial.println("Failed to setup access point with:");
		Serial.println(ssid);
		Serial.println(password);
	}
}
