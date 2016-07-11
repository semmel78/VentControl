#ifndef USER_H
#define USER_H

typedef struct _UserConfig {
	// put your user data here
  boolean AutoTurnOff;
  boolean AutoTurnOn;
  byte TurnOffHour;
  byte TurnOffMinute;
  byte TurnOnHour;
  byte TurnOnMinute;
  byte LED_R;
  byte LED_G;
  byte LED_B;
  byte VentilationInMin;
  byte VentilationInCom;
  byte VentilationInMax;
  byte VentilationOutMin;
  byte VentilationOutCom;
  byte VentilationOutMax;
} UserConfig;

// Pin definition
const int ledPin = 15;
const int buttonPin = 2;
const int dataPin  = 14;
const int clockPin = 16;
const int resetPin = 0;

const int VentilationInPin = 12;
const int VentilationOutPin = 13;

const byte MODE_MIN = 0;
const byte MODE_COM = 1;
const byte MODE_MAX = 2;
const byte MODE_EXT = 3;

extern byte VentilationInActual;
extern byte VentilationOutActual;
extern byte VentilationMode;
extern boolean Refresh;                      // For Main Loop, to refresh things like GPIO / WS2812
//byte Minute_Old;                        // Helpvariable for checking, when a new Minute comes up (for Auto Turn On / Off)
extern unsigned long upTime;                     // Seconds since start of ESP8266

extern void userInit();
extern void userLoop();
extern void userInitConfig();
extern void userLoadConfig(JsonObject&);
extern void userSaveConfig(JsonObject&);

#endif
