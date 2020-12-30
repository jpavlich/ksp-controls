#include <Arduino.h>
#include <vector>

#include "conn.h"
#include <krpc.h>
#include <krpc/services/krpc.h>
#include <krpc/services/space_center.h>
#define LED_BUILTIN PB12

krpc_connection_t conn;
krpc_connection_config_t conf;
bool connected = false;
krpc_SpaceCenter_AutoPilot_t autopilot;
krpc_SpaceCenter_Vessel_t vessel;

void blink()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}

bool connect()
{
  blink();
  // Set up communication with the server
  if (!connected && krpc_connect(conn, "Arduino Example") == KRPC_OK)
  {
    // Indicate succesful connection by lighting the on-board LED
    digitalWrite(LED_BUILTIN, LOW);
    krpc_SpaceCenter_Vessel_t vessel;
    krpc_SpaceCenter_ActiveVessel(conn, &vessel);

    krpc_SpaceCenter_Vessel_set_Name(conn, vessel, "My Vessel");
    connected = true;
    krpc_SpaceCenter_ActiveVessel(conn, &vessel);
    krpc_SpaceCenter_Vessel_AutoPilot(conn, &autopilot, vessel);
    return true;
  }
  else
  {
    return false;
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PA0, INPUT_PULLUP);
  pinMode(PA1, INPUT_PULLUP);
  pinMode(PA2, INPUT_PULLUP);
  pinMode(PA3, INPUT_PULLUP);
  pinMode(PA4, INPUT_PULLUP);
  pinMode(PA5, INPUT_PULLUP);
  pinMode(PA6, INPUT_PULLUP);
  pinMode(PA7, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, HIGH);
  conn = &Serial;
  Serial.begin(115200);
  delay(5000);
  while (!connect())
    ;
}

void loop()
{
  if (connected)
  {

    if (digitalRead(PA0) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_STABILITYASSIST);
    }
    else 
    if (digitalRead(PA1) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_MANEUVER);
    }
    else 
    if (digitalRead(PA2) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_PROGRADE);
    }
    else 
    if (digitalRead(PA3) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_RETROGRADE);
    }
    else 
    if (digitalRead(PA4) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_NORMAL);
    }
    else 
    if (digitalRead(PA5) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_ANTINORMAL);
    }
    else 
    if (digitalRead(PA6) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_RADIAL);
    }
    else 
    if (digitalRead(PA7) == LOW)
    {
      krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
      krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, KRPC_SPACECENTER_SASMODE_ANTIRADIAL);
    }
  }
  delay(10);
}
