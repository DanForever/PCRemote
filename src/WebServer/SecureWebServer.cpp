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

#include "SecureWebServer.h"

#include <Crypto.h>

#include "../Filesystem/filesystem.h"
#include "../Debug/DebugPrint.h"

//#define USE_SHA256

#ifdef USE_SHA256
	String(*HashingFunc)(const String&) = experimental::crypto::SHA256::hash;
#else
	String(*HashingFunc)(const String&) = experimental::crypto::MD5::hash;
#endif

class SecureWrapper
{
public:
	SecureWrapper(SecureWebServer* server, SecureWebServer::THandlerFunction function)
		: m_server(server)
		, m_function(function)
	{}

	void operator()()
	{
		if (IsAuthenticationRequired() && !HasPriorAuthentication() && !HasValidAuthenticationHeader())
		{
			DenyAccess();
			return;
		}

		m_function();
	}

private:

	bool IsAuthenticationRequired() const
	{
		return m_server->Username().length() > 0 && m_server->Password().length() > 0;
	}

	bool HasPriorAuthentication()
	{
		// Todo: cookies
		return false;
	}

	String HttpCodeToString(HTTPMethod method)
	{
		switch (method)
		{
		case HTTP_POST:
			return F("POST");

		case HTTP_GET:
			return F("GET");
		}

		return F("");
	}

	bool HasValidAuthenticationHeader()
	{
		if (!m_server->hasHeader("Authorization"))
		{
			DEBUG_PRINTLN("No Authorization header in request, denying access");
			return false;
		}

		String a1 = m_server->Username() + ":" + m_server->Realm() + ":" + m_server->Password();
		String a2 = HttpCodeToString(m_server->method()) + ":" + m_server->uri();

		String ha1 = (*HashingFunc)(a1);
		String ha2 = (*HashingFunc)(a2);

		ha1.toLowerCase();
		ha2.toLowerCase();

		String authorizationHeader = m_server->header(F("Authorization"));

		String expectedResponse;
		String hExpectedResponse;

		// extracting required parameters for RFC 2069 simpler Digest
		String realm = m_server->ExtractParameter(authorizationHeader, F("realm=\""));
		String nonce = m_server->ExtractParameter(authorizationHeader, F("nonce=\""));
		String uri = m_server->ExtractParameter(authorizationHeader, F("uri=\""));
		String response = m_server->ExtractParameter(authorizationHeader, F("response=\""));
		String opaque = m_server->ExtractParameter(authorizationHeader, F("opaque=\""));

		// parameters for the RFC 2617 newer Digest
		String nc, cnonce;
		if (authorizationHeader.indexOf(F("qop=auth")) != -1 || authorizationHeader.indexOf(F("qop=\"auth\"")) != -1)
		{
			nc = m_server->ExtractParameter(authorizationHeader, F("nc="), ',');
			cnonce = m_server->ExtractParameter(authorizationHeader, F("cnonce=\""));

			expectedResponse = ha1 + ":" + m_server->Nonce() + ":" + nc + ":" + cnonce + ":auth:" + ha2;
		}
		else
		{
			expectedResponse = ha1 + ":" + m_server->Nonce() + ":" + ha2;
		}

		hExpectedResponse = (*HashingFunc)(expectedResponse);

		return hExpectedResponse.equalsIgnoreCase(response);
	}

	void DenyAccess()
	{
		m_server->Nonce(m_server->GetRandomHexString());
		m_server->Opaque(m_server->GetRandomHexString());

		const int size = 256;
		char buffer[size];

#ifdef USE_SHA256
		snprintf(buffer, size, F(R"(Digest realm="%s", qop="auth", nonce="%s", opaque="%s", algorithm="SHA-256")"), m_server->Realm().c_str(), m_server->Nonce().c_str(), m_server->Opaque().c_str());
#else
		snprintf(buffer, size, (R"(Digest realm="%s", qop="auth", nonce="%s", opaque="%s")"), m_server->Realm().c_str(), m_server->Nonce().c_str(), m_server->Opaque().c_str());
#endif

		m_server->sendHeader("WWW-Authenticate", buffer);

		m_server->SendAccessDeniedPage();
	}

	SecureWebServer* m_server;
	SecureWebServer::THandlerFunction m_function;
};

SecureWebServer::SecureWebServer(int port)
	: ESP8266WebServer(port)
{
}

void SecureWebServer::OnSecure(const Uri& uri, THandlerFunction fn)
{
	THandlerFunction handler = SecureWrapper(this, fn);
	on(uri, handler);
}

void SecureWebServer::OnSecure(const Uri& uri, HTTPMethod method, THandlerFunction fn)
{
	THandlerFunction handler = SecureWrapper(this, fn);
	on(uri, method, handler);
}

void SecureWebServer::OnSecure(const Uri& uri, HTTPMethod method, THandlerFunction fn, THandlerFunction ufn)
{
	THandlerFunction handler1 = SecureWrapper(this, fn);
	THandlerFunction handler2 = SecureWrapper(this, ufn);
	on(uri, method, handler1, handler2);
}

void SecureWebServer::SendAccessDeniedPage()
{
	File file = Filesystem::Open("/denied.html");

	using namespace mime;
	setContentLength(file.size());
	send(401, "text/html", emptyString);
	file.sendAll(_currentClient);

	file.close();
}
