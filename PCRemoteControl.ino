#include "src/WebServer/WebServer.h"
#include "src/Filesystem/filesystem.h"
#include "src/Configuration/configuration.h"
#include "src/Computer/Computer.h"
#include "src/Device/device.h"
#include "src/Debug/DebugPrint.h"

WebServer webServer;

void setup()
{
	Serial.begin(115200);
	//Serial.setDebugOutput(true);

	DEBUG_PRINTLN("");
	DEBUG_PRINTLN("Initializing...");

	g_Computer.Initialize();
	Filesystem::Initialize();
	g_Config.Initialize();
	g_device.Initialize();
	webServer.Start();

	DEBUG_PRINTLN("Ready");
}

void loop()
{
	webServer.Update();
	g_Computer.Update();
}
