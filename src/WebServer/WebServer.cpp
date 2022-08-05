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

#include "WebServer.h"

#include <arduino.h>
#include <ESP8266WiFi.h>

#include <uri/UriBraces.h>

#include "../Filesystem/filesystem.h"
#include "../Configuration/configuration.h"
#include "../Computer/Computer.h"
#include "../Device/device.h"
#include "../Version/version.h"

#include "../Configuration/utilities.h"

WebServer::WebServer()
	: m_server(80)
{
	m_server.on("/", HTTP_GET, [this] { HandlerHome(); });

	m_server.on("/network", HTTP_GET, [this] { HandlerNetwork(); });
	m_server.on("/network/rssi", HTTP_GET, [this] { HandlerRSSI(); });
	m_server.on("/network/wifi", HTTP_POST, [this] { HandlerSetWifi(); });
	m_server.on("/network/wifi", HTTP_GET, [this] { HandlerGetWifi(); });
	m_server.on("/network/hostname", HTTP_POST, [this] { HandlerSetHostname(); });
	m_server.on("/network/hostname", HTTP_GET, [this] { HandlerGetHostname(); });
	m_server.on("/network/ap", HTTP_POST, [this] { HandlerSetAccessPoint(); });
	m_server.on("/network/ap", HTTP_GET, [this] { HandlerGetAccessPoint(); });

	m_server.on("/settings", HTTP_GET, [this] { HandlerSettings(); });
	m_server.on("/settings/reset", HTTP_POST, [this] { HandlerFactoryReset(); });

	m_server.on(UriBraces("/style/{}"), HTTP_GET, [this] { HandlerStyle(); });
	m_server.on(UriBraces("/images/{}"), HTTP_GET, [this] { HandlerImages(); });
	m_server.on(UriBraces("/js/{}"), HTTP_GET, [this] { HandlerScripts(); });
	m_server.on("/computer/press", HTTP_GET, [this] { HandlerPowerPressed(); });
	m_server.on("/computer/status", HTTP_GET, [this] { HandlerPowerStatus(); });
	m_server.on("/update", HTTP_POST, [this] { HandlerUpdateComplete(); }, [this] { HandlerUpdateInProgress(); });

	m_server.on("/version/firmware", HTTP_GET, [this] { HandlerVersionFirmware(); });
	m_server.on("/version/filesystem", HTTP_GET, [this] { HandlerVersionFilesystem(); });
}

void WebServer::Start()
{
	m_server.begin();
}

void WebServer::Update()
{
	m_server.handleClient();

	if (m_triggerRestart)
	{
		ESP.restart();
	}
}

void WebServer::HandlerHome()
{
	File file = Filesystem::Open("/home.html");
	m_server.streamFile(file, "text/html");
	file.close();
}

void WebServer::HandlerStyle()
{
	String path = "/style/";
	path += m_server.pathArg(0);

	if (!Filesystem::Exists(path.c_str()))
		return;

	File file = Filesystem::Open(path.c_str());
	m_server.streamFile(file, "text/css");
	file.close();
}

void WebServer::HandlerImages()
{
	String path = "/images/" + m_server.pathArg(0);

	if (!Filesystem::Exists(path.c_str()))
		return;

	File file = Filesystem::Open(path.c_str());
	m_server.streamFile(file, "image/png");
	file.close();
}

void WebServer::HandlerScripts()
{
	Serial.println("Serving script...");

	String path = "/js/" + m_server.pathArg(0) + ".js";

	if (!Filesystem::Exists(path.c_str()))
		return;

	Serial.print(" - ");
	Serial.println(path);

	File file = Filesystem::Open(path.c_str());
	m_server.streamFile(file, "text/javascript");
	file.close();
}

void WebServer::HandlerNetwork()
{
	File file = Filesystem::Open("/network.html");
	m_server.streamFile(file, "text/html");
	file.close();
}

void WebServer::HandlerSettings()
{
	File file = Filesystem::Open("/settings.html");
	m_server.streamFile(file, "text/html");
	file.close();
}

void WebServer::HandlerFactoryReset()
{
	bool success = g_Config.Delete();

	StaticJsonDocument<128> document;
	document["success"] = success;

	char buffer[128];
	serializeJson(document, buffer);

	m_server.send(200, "application/json", buffer);

	m_triggerRestart = true;
}

void WebServer::HandlerRSSI()
{
	int32_t rssi = WiFi.RSSI();

	StaticJsonDocument<128> document;
	document["rssi"] = rssi;

	char buffer[128];
	serializeJson(document, buffer);

	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerSetWifi()
{
	// Retrieve the parameters from the http post data
	const bool enabled = m_server.hasArg("enabled");
	const String& ssid = m_server.arg("ssid");
	const String& password = m_server.arg("password");

	// Print them to the serial for debugging
	Serial.print("Wifi Enabled: ");
	Serial.println(enabled);

	Serial.print("SSID: ");
	Serial.println(ssid);

	Serial.print("Password: ");
	Serial.println(password);

	// Save them to the config
	g_Config.Network().Wifi().Enabled(enabled);
	g_Config.Network().Wifi().SSID(ssid);
	g_Config.Network().Wifi().Password(password);
	g_Config.Save();

	// Re-setup the wifi (the parameters will be taken directly from the config)
	g_device.SetupWifi();

	// Send output to the user
	HandlerNetwork();
}

void WebServer::HandlerGetWifi()
{
	StaticJsonDocument<128> document;
	document["network"]["wifi"]["enabled"] = g_Config.Network().Wifi().Enabled();
	document["network"]["wifi"]["ssid"] = g_Config.Network().Wifi().SSID();

	char buffer[128];
	serializeJson(document, buffer);
	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerSetHostname()
{
	const String hostname = m_server.arg("hostname");

	// Store the hostname in our configuration so it will be automatically set after a restart
	g_Config.Network().Wifi().Hostname(hostname);
	g_Config.Save();

	g_device.SetupHostname();

	// Send output to the user
	Serial.println("HandlerNetwork();");
	HandlerNetwork();
}

void WebServer::HandlerGetHostname()
{
	StaticJsonDocument<512> document;
	document["network"]["wifi"]["hostname"] = g_Config.Network().Wifi().Hostname();

	char buffer[512];
	serializeJson(document, buffer);
	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerSetAccessPoint()
{
	// Retrieve the parameters from the http post data
	const bool enabled = m_server.hasArg("enabled");
	const String ssid = m_server.arg("ssid");
	const String password = m_server.arg("password");

	// Print them to the serial for debugging
	Serial.print("AP Enabled: ");
	Serial.println(enabled);

	Serial.print("SSID: ");
	Serial.println(ssid);

	Serial.print("Password: ");
	Serial.println(password);

	// Save them to the config
	g_Config.Network().AccessPoint().Enabled(enabled);
	g_Config.Network().AccessPoint().SSID(ssid);
	g_Config.Network().AccessPoint().Password(password);
	g_Config.Save();

	// Re-setup the wifi (the parameters will be taken directly from the config)
	g_device.SetupAccessPoint();

	// Send output to the user
	HandlerNetwork();
}

void WebServer::HandlerGetAccessPoint()
{
	StaticJsonDocument<128> document;
	document["network"]["ap"]["enabled"] = g_Config.Network().AccessPoint().Enabled();
	document["network"]["ap"]["ssid"] = g_Config.Network().AccessPoint().SSID();

	char buffer[128];
	serializeJson(document, buffer);
	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerPowerPressed()
{
	g_Computer.PressPowerButton();

	StaticJsonDocument<128> document;
	document["power"] = "ok";

	char buffer[128];
	serializeJson(document, buffer);

	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerPowerStatus()
{
	bool isPowerOn = g_Computer.IsPowerOn();

	StaticJsonDocument<128> document;
	document["power"] = isPowerOn;

	char buffer[128];
	serializeJson(document, buffer);

	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerVersionFirmware()
{
	StaticJsonDocument<128> document;
	document["version"]["major"] = VERSION_MAJOR;
	document["version"]["minor"] = VERSION_MINOR;
	document["version"]["patch"] = VERSION_PATCH;

	char buffer[128];
	serializeJson(document, buffer);

	m_server.send(200, "application/json", buffer);
}

void WebServer::HandlerVersionFilesystem()
{
	File file = Filesystem::Open("/version.json");
	m_server.streamFile(file, "application/json");
	file.close();
}

void WebServer::HandlerUpdateComplete()
{
	Serial.println("Update finished (Final handler)");
	m_server.send(200, "text/plain", (::Update.hasError()) ? "FAIL" : "OK");
	m_triggerRestart = true;
}

void WebServer::HandlerUpdateInProgress()
{
	HTTPUpload& incomingFile = m_server.upload();

	switch (incomingFile.status)
	{
	case UPLOAD_FILE_START:
		StartOTA(incomingFile);
		break;

	case UPLOAD_FILE_WRITE:
		UpdateOTA(incomingFile);
		break;

	case UPLOAD_FILE_END:
		FinishOTA(incomingFile);
		break;
	}
	yield();
}

void WebServer::StartOTA(HTTPUpload& incomingFile)
{
	int command = incomingFile.filename.endsWith(".fs.bin") ? U_FS : U_FLASH;

	const uint32_t freeSketchSpace = ESP.getFreeSketchSpace();
	const uint32_t maxSketchSpace = (freeSketchSpace - 0x1000) & 0xFFFFF000;

	Serial.println("Starting OTA Update...");

	Serial.print("Free sketch space: ");
	Serial.println(freeSketchSpace);

	Serial.print("Estimated max sketch space: ");
	Serial.println(maxSketchSpace);

	Serial.print("Incoming reported filesize: ");
	Serial.println(incomingFile.totalSize);

	File file = Filesystem::Open("/loading.html");
	m_server.streamFile(file, "text/html");
	file.close();

	if (!::Update.begin(maxSketchSpace, command))
	{
		// start with max available size
		::Update.printError(Serial);
	}
}

void WebServer::UpdateOTA(HTTPUpload& incomingFile)
{
	Serial.print("Incoming total filesize: ");
	Serial.println(incomingFile.totalSize);

	Serial.print("Incoming current filesize: ");
	Serial.println(incomingFile.currentSize);

	if (::Update.write(incomingFile.buf, incomingFile.currentSize) != incomingFile.currentSize)
	{
		::Update.printError(Serial);
	}
}

void WebServer::FinishOTA(HTTPUpload& incomingFile)
{
	if (::Update.end(true))
	{
		// true to set the size to the current progress
		Serial.printf("Update Success: %u\nRebooting...\n", incomingFile.totalSize);
	}
	else
	{
		::Update.printError(Serial);
	}
}
