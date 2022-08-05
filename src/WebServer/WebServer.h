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

#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include <ESP8266WebServer.h>

class WebServer
{
public:
	WebServer();

	void Start();
	void Update();

private:
	void HandlerHome();
	void HandlerStyle();
	void HandlerImages();
	void HandlerScripts();
	void HandlerNetwork();
	void HandlerSettings();
	void HandlerFactoryReset();
	void HandlerRSSI();
	void HandlerSetWifi();
	void HandlerGetWifi();
	void HandlerSetHostname();
	void HandlerGetHostname();
	void HandlerSetAccessPoint();
	void HandlerGetAccessPoint();
	void HandlerPowerPressed();
	void HandlerPowerStatus();
	void HandlerVersionFirmware();
	void HandlerVersionFilesystem();

	void HandlerUpdateComplete();
	void HandlerUpdateInProgress();
	void StartOTA(HTTPUpload& incomingFile);
	void UpdateOTA(HTTPUpload& incomingFile);
	void FinishOTA(HTTPUpload& incomingFile);

	ESP8266WebServer m_server;
	bool m_triggerRestart = false;
};

#endif // __WEBSERVER_H__
