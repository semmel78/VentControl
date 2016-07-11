#include "global.h"

void get_active_values() {
  // /get?mode  -> [MIN, COM, MAX, EXT]
  // /get?in    -> [0 ... 100]
  // /get?out   -> [0 ... 100]
  // /get?heap  -> 123456
  // /get?chipid  -> 123456
  // /get?flashid  -> 123456
  // /get?vcc   -> 123456
  // /get?temp  -> 20.4
  // /get?uptime -> dd hh:mm:ss
Serial.println("DEBUG HTTP:");
Serial.print("HTTPMethod:");
Serial.println(server.method());
Serial.print("WiFiClient:");
Serial.println(server.client());

Serial.println("Headers:");
for (int i=0; i<server.headers(); i++) {
  Serial.print(i);
  Serial.print(": ");
  Serial.print(server.headerName(i));
  Serial.print(" - ");
  Serial.print(server.header(i));
}
Serial.println("");
Serial.println("Hostheader:");
Serial.println(server.hostHeader());
Serial.println("");
Serial.println("Arguments:");
for (int i=0; i<server.args(); i++) {
  Serial.print(i);
  Serial.print(": ");
  Serial.print(server.argName(i));
  Serial.print(" - ");
  Serial.print(server.arg(i));
}

  if (server.args() == 1) {
    String message;
    if (server.argName(0) == "mode") {
      switch (VentilationMode) {
        case MODE_MIN:
          message = "MIN";
          break;
        case MODE_COM:
          message = "COM";
          break;
        case MODE_MAX:
          message = "MAX";
          break;
        case MODE_EXT:
          message = "EXT";
          break;
      }
    } else if (server.argName(0) == "in") {
      message = String(VentilationInActual);
      
    } else if (server.argName(0) == "out") {
      message = String(VentilationOutActual);
      
    } else if (server.argName(0) == "heap") {
      message = String(ESP.getFreeHeap());
      
    } else if (server.argName(0) == "chipid") {
      message = String(ESP.getChipId());
      
    } else if (server.argName(0) == "flashid") {
      message = String(ESP.getFlashChipId());
      
    } else if (server.argName(0) == "temp") {
      server.send ( 501, "text/plain", "ERROR - Service not implemented!" );
      return;
      
    } else if (server.argName(0) == "uptime") {
      const int cseconds_in_day = 86400;
      const int cseconds_in_hour = 3600;
      const int cseconds_in_minute = 60;
      const int cseconds = 1;

      int days = upTime / cseconds_in_day;
      int hours = (upTime % cseconds_in_day) / cseconds_in_hour;
      int minutes = ((upTime % cseconds_in_day) % cseconds_in_hour) / cseconds_in_minute;
      int seconds = (((upTime % cseconds_in_day) % cseconds_in_hour) % cseconds_in_minute) / cseconds;

      message = (String) days + " " + (String) hours + ":" + (String) minutes + ":" + (String) seconds;
      //server.send ( 501, "text/plain", "ERROR - Service not implemented!" );
      //return;
      
    } else if (server.argName(0) == "time") {
      //message = (String) DateTime.hour + ":" + (String) + DateTime.minute +  ":"  + (String)  DateTime.second + " " + (String)   DateTime.year + "-" + (String)  DateTime.month + "-" + (String)  DateTime.day;
    
    } else {       // FEHLERMELDUNG 404/500
      server.send ( 501, "text/plain", "ERROR - Service not implemented!" );
      return;
    }
    //send message
    server.send ( 200, "text/plain", message );
  } else {
    // too much or too less arguments -> ERROR
    server.send ( 400, "text/plain", "ERROR - Wrong syntax!" );
    return;
  }
}

