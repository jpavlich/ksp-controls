#pragma once
#include "conn.h"
#include <krpc.h>
#include <krpc/services/krpc.h>
#include <krpc/services/space_center.h>
#include "button.h"
#define LED_BUILTIN PB12

class Kspsas
{
private:
    krpc_connection_t conn;
    std::vector<Button> buttons;

    bool connected = false;
    krpc_SpaceCenter_AutoPilot_t autopilot;
    krpc_SpaceCenter_Vessel_t vessel;
    krpc_SpaceCenter_SASMode_t sas_mode;
    bool sas = false;

public:
    Kspsas(krpc_connection_t conn, std::vector<Button> &&buttons) : conn(conn), buttons(buttons) {}

    void blink()
    {
        for (auto but : buttons)
        {
            but.light();
        }
        delay(200);
        for (auto but : buttons)
        {
            but.light(false);
        }
        delay(200);
    }

    bool connect_to_server()
    {
        blink();
        // Set up communication with the server
        if (!connected && krpc_connect(conn, "KSP SAS") == KRPC_OK)
        {
            // Indicate succesful connection by lighting the on-board LED
            digitalWrite(LED_BUILTIN, LOW);
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

    void wait_for_server()
    {
        for (size_t i = 0; i < 10; i++)
        {
            blink();
        }

        while (!connect_to_server())
            ;
    }

    void setup()
    {
        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        for (size_t i = 0; i < buttons.size(); i++)
        {
            buttons[i].setup();
        }
        conn->begin(115200);
        wait_for_server();
    }

    void loop()
    {
        for (size_t i = 0; i < buttons.size(); i++)
        {
            if (buttons[i].pushed())
            {

                krpc_SpaceCenter_AutoPilot_set_SAS(conn, autopilot, true);
                krpc_SpaceCenter_AutoPilot_set_SASMode(conn, autopilot, (krpc_SpaceCenter_SASMode_t)i);
            }
        }
        krpc_SpaceCenter_AutoPilot_SASMode(conn, &sas_mode, autopilot);
        krpc_SpaceCenter_AutoPilot_SAS(conn, &sas, autopilot);
        for (size_t i = 0; i < buttons.size(); i++)
        {
            buttons[i].light(sas && (krpc_SpaceCenter_SASMode_t)i == sas_mode);
        }
    }
};