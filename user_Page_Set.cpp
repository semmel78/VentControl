#include "global.h"

void set_active_values() {
  // /set?mode=[0,1,2]
  // /set?in=[0 ... 100]
  // /set?out=[0 ... 100]
  // /set?oled="String"

  if (server.args() > 0) {
    if (server.arg("mode").length() > 0) {
      if (server.arg("mode") == "MIN") {
        VentilationMode = MODE_MIN;
      } else if (server.arg("mode") == "COM") {
        VentilationMode = MODE_COM;
      } else if (server.arg("mode") == "MAX") {
        VentilationMode = MODE_MAX;
      } else {
        // FEHLERMELDUNG 404/500
        server.send ( 400, "text/plain", "ERROR - parameter [mode] must be one of [MIN, MAX, COM]!" );
        return;
      }
    }
    if (server.arg("in").length() > 0) {
      byte temp;
      temp = byte(server.arg("in").toInt());
      if ( temp >= 0 && temp <= 100 ) {
        VentilationInActual = temp;
        VentilationMode = MODE_EXT;
      } else {
        //FEHLERMELDUNG value out of range
        server.send ( 400, "text/plain", "ERROR - parameter [in] must be in the range of [0 ... 100]!" );
        return;
      }
    }
    if (server.arg("out").length() > 0) {
      byte temp;
      temp = byte(server.arg("out").toInt());
      if ( temp >= 0 && temp <= 100 ) {
        VentilationOutActual = temp;
        VentilationMode = MODE_EXT;
      } else {
        //FEHLERMELDUNG value out of range
        server.send ( 400, "text/plain", "ERROR - parameter [out] must be in the range of [0 ... 100]!" );
        return;
      }
    }
    if (server.arg("oled").length() > 0) {
      // todo: OLED integration
      server.send ( 501, "text/plain", "ERROR - Not yet implemented!" );
      return;
    }
    server.send ( 200, "text/plain", "OK" );
    Refresh = true;
    return;
  } else {
    // no arguments passed -> Error 501
    server.send ( 501, "text/plain", "ERROR - No valid GET arguments passed to server!" );
    return;
  }
}

