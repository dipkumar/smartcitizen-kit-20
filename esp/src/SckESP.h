#pragma once

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <TimeLib.h>
#include "FS.h"
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "RemoteDebug.h"
#include <ArduinoJson.h>
#include <RHReliableDatagram.h>
#include <RH_Serial.h>
#include <PubSubClient.h>

#include <Arduino.h>
#include "Shared.h"
#include "Config.h"

#define NTP_SERVER_NAME "pool.ntp.org"
#define MQTT_SERVER_NAME "mqtt.smartcitizen.me"
#define MQTT_SERVER_PORT 1883
#define MQTT_QOS 1

class SckESP
{
	private:

		// Input/Output
		bool serialDebug = false;		// Interfere with ESP <-> SAM comunnication (use with caution)
		bool telnetDebug = true;
		void SAMbusUpdate();
		void debugOUT(String strOut);

		// SAM communication
		uint8_t netPack[NETPACK_TOTAL_SIZE];
		char netBuff[NETBUFF_SIZE];
		bool sendMessage(SAMMessage wichMessage);
		bool sendMessage(SAMMessage wichMessage, const char *content);
		bool sendMessage();
		void receiveMessage(ESPMessage wichMessage);

		// Notifications
		bool sendToken();
		bool sendCredentials();
		bool sendNetinfo();
		bool sendTime();

		// **** MQTT
		bool mqttConnect();
		bool mqttHellow();
		bool mqttPublish();
		AllSensors sensors;

		// Led control
		const uint8_t pinLED = 4; 	// GPIO5
		uint8_t ledValue = 0;
		Ticker blink;
		uint16_t LED_SLOW = 350;
		uint16_t LED_FAST = 100;
		void ledSet(uint8_t value);
		void ledBlink(float rate);
		void ledOff();

		// Wifi related
		char hostname[20];
		String macAddr;
		String ipAddr;
		int currentWIFIStatus;
		void tryConnection();

		// Config
		Configuration config;
		const char *configFileName = "/config.txt";
		bool saveConfig(Configuration newConfig);
		bool saveConfig();
		bool loadConfig();

		// AP mode
		void startAP();
		void stopAP();
		// void startWebServer();
		bool flashReadFile(String path);
		const byte DNS_PORT = 53;

		// Time
		void sendNTPpacket(IPAddress &address);
		String ISOtime();
		String epoch2iso(uint32_t toConvert);
		String leadingZeros(String original, int decimalNumber);
		WiFiUDP Udp;
		byte packetBuffer[48];


	public:
		void setup();
		void update();

		// External calls
		void _ledToggle();
		time_t getNtpTime();
};

// Static led control
void ledToggle();

// Static webserver handlers
void extSet();

// Time
time_t ntpProvider();