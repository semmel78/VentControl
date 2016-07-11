#include "global.h"
#include "user_pages.h"
#include <Bounce2.h>

Bounce debouncer = Bounce();                  // Instantiate a Bounce object
byte VentilationInActual;
byte VentilationOutActual;
byte VentilationMode = MODE_COM;
boolean Refresh = false;                      // For Main Loop, to refresh things like GPIO / WS2812
unsigned long upTime = 0;                     // Seconds since start of ESP8266


void userInit() {
	// user init code here
  server.on( "/", userPage);
  server.on( "/filldynamicdata", filldynamicdata);
  server.on ( "/set", set_active_values );
  server.on ( "/get", get_active_values );
  
  //	server.on( "/style.css", sendCSS);
pinMode(LED_BUILTIN, OUTPUT);
analogWrite(LED_BUILTIN, 220);
}

void userLoop() {
	// user code here
  yield();
  // Button press
  debouncer.update();
   // Call code if Bounce fell (transition from HIGH to LOW) :
  if ( debouncer.fell() ) {

    VentilationMode++;
    if (VentilationMode > 2) {
      VentilationMode = 0;
    }
    Refresh = true;
    Serial.println("BUTTON pressed");
    Serial.print("VentilationMode changed to: ");
    Serial.println(VentilationMode);
  }

  if (Refresh)
  {
    Refresh = false;
    Serial.println("Refreshing values");
    Serial.println(VentilationMode);
    switch (VentilationMode) {
      case MODE_MIN:
        // set air volume
        VentilationInActual = config.userConfig.VentilationInMin;
        VentilationOutActual = config.userConfig.VentilationOutMin;
        analogWriteFreq(1);
/*
        // set LED color
        StatusLED.R = 255; // R
        StatusLED.G = 255; // G
        StatusLED.B = 0;   // B
*/
        // display text
        //lcdMiddleLine = "MINIMUM";
        break;
      case MODE_COM:
        // set air volume
        VentilationInActual = config.userConfig.VentilationInCom;
        VentilationOutActual = config.userConfig.VentilationOutCom;
        analogWriteFreq(4);
/*
        // set LED color
        StatusLED.R = 0;   // R
        StatusLED.G = 255; // G
        StatusLED.B = 0;   // B
*/
        // display text
        //lcdMiddleLine = "MINIMUM";
        break;
      case MODE_MAX:
        // set air volume
        VentilationInActual = config.userConfig.VentilationInMax;
        VentilationOutActual = config.userConfig.VentilationOutMax;
        analogWriteFreq(10);
/*
        // set LED color
        StatusLED.R = 255; // R
        StatusLED.G = 0;   // G
        StatusLED.B = 0;   // B
*/
        // display text
        //lcdMiddleLine = "MINIMUM";
        break;
      case MODE_EXT:
        // set air volume
        //pwmZuluftRate = 5;
        //pwmAbluftRate = 5;
        analogWriteFreq(20);
/*
        // set LED color
        StatusLED.R = 0;   // R
        StatusLED.G = 0;   // G
        StatusLED.B = 255; // B
*/
        // display text
        //lcdMiddleLine = "EXTERNAL CONTROL";
        break;
      default:
        // Use comfort settings as above
        VentilationMode = MODE_COM;
    }

    // set output PWM
    analogWrite(VentilationInPin, 205 + ((1024 - 205) / 100) * VentilationInActual);
    analogWrite(VentilationOutPin, 205 + ((1024 - 205) / 100) * VentilationOutActual);
    Serial.print("ZuluftRate: ");
    Serial.print(VentilationInActual);
    Serial.print("\t");
    Serial.print("Zuluft PWM value: ");
    Serial.println(205 + ((1024 - 205) / 100) * VentilationInActual);
    Serial.print("AbluftRate: ");
    Serial.print(VentilationOutActual);
    Serial.print("\t");
    Serial.print("Abluft PWM value: ");
    Serial.println(205 + (1024 - 205) / 100 * VentilationOutActual);
    yield();

    // Set LED
    //strip.SetPixelColor(1, StatusLED.R, StatusLED.G, StatusLED.B);
    //strip.Show();

    ///Serial.println("Refreshing...");
    //Serial.printf("FreeMem:%d %d:%d:%d %d.%d.%d \n",ESP.getFreeHeap() , DateTime.hour,DateTime.minute, DateTime.second, DateTime.year, DateTime.month, DateTime.day);
  }
}


// config
void userInitConfig() {
  config.userConfig.AutoTurnOff = false;
  config.userConfig.AutoTurnOn = false;
  config.userConfig.TurnOffHour = 0;
  config.userConfig.TurnOffMinute = 0;
  config.userConfig.TurnOnHour = 0;
  config.userConfig.TurnOnMinute = 0;
  config.userConfig.VentilationInMin = 5;
  config.userConfig.VentilationInCom =  60;
  config.userConfig.VentilationInMax =  100;
  config.userConfig.VentilationOutMin = 5;
  config.userConfig.VentilationOutCom = 60;
  config.userConfig.VentilationOutMax = 100;
}

void userLoadConfig(JsonObject& json) {
  config.daylight = json["AutoTurnOff"] == 1 ?  true : false;
  config.daylight = json["AutoTurnOn"] == 1 ?  true : false;
  config.ntpUpdatePeriod = json["TurnOffHour"];
  config.ntpUpdatePeriod = json["TurnOffMinute"];
  config.ntpUpdatePeriod = json["TurnOnHour"];
  config.ntpUpdatePeriod = json["TurnOnMinute"];
  config.ntpUpdatePeriod = json["VentilationInMin"];
  config.ntpUpdatePeriod = json["VentilationInCom"];
  config.ntpUpdatePeriod = json["VentilationInMax"];
  config.ntpUpdatePeriod = json["VentilationOutMin"];
  config.ntpUpdatePeriod = json["VentilationOutCom"];
  config.ntpUpdatePeriod = json["VentilationOutMax"];
}

void userSaveConfig(JsonObject& json) {
  json["AutoTurnOff"] = config.userConfig.AutoTurnOff;
  json["AutoTurnOnTurnOff"] = config.userConfig.AutoTurnOn;
  json["TurnOffHour"] = config.userConfig.TurnOffHour;
  json["TurnOffMinute"] = config.userConfig.TurnOffMinute;
  json["TurnOnHour"] = config.userConfig.TurnOnHour;
  json["TurnOnMinute"] = config.userConfig.TurnOnMinute;
  json["VentilationInMin"] = config.userConfig.VentilationInMin;
  json["VentilationInCom"] = config.userConfig.VentilationInCom;
  json["VentilationInMax"] = config.userConfig.VentilationInMax;
  json["VentilationOutMin"] = config.userConfig.VentilationOutMin;
  json["VentilationOutCom"] = config.userConfig.VentilationOutCom;
  json["VentilationOutMax"] = config.userConfig.VentilationOutMax;
}
