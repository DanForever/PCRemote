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

#ifndef __SECURE_WEBSERVER_H__
#define __SECURE_WEBSERVER_H__

#include <ESP8266WebServer.h>

class SecureWebServer : public ESP8266WebServer
{
public:
	SecureWebServer(int port = 80);

	void OnSecure(const Uri& uri, THandlerFunction handler);
	void OnSecure(const Uri& uri, HTTPMethod method, THandlerFunction fn);
	void OnSecure(const Uri& uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn);

	void SendAccessDeniedPage();

	// @todo; Make a more cryptographically secure version
	static String GetRandomHexString() { return _getRandomHexString(); }

	void Username(const String& username) { m_username = username; }
	void Password(const String& password) { m_password = password; }
	void Realm(const String& realm) { _srealm = realm; }
	void Nonce(const String& nonce) { _snonce = nonce; }
	void Opaque(const String& opaque) { _sopaque = opaque; }

	const String& Username() const { return m_username; }
	const String& Password() const { return m_password; }
	const String& Nonce() const { return _snonce; }
	const String& Realm() const { return _srealm; }
	const String& Opaque() const{ return _sopaque; }

	String ExtractParameter(String& authReq, const String& param, const char delimit = '"') { return _extractParam(authReq, param, delimit); }

private:
	String m_username;
	String m_password;
};

#endif // __SECURE_WEBSERVER_H__
