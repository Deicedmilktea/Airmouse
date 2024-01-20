#include "myhid.h"
#include <Arduino.h>
#include <Wire.h>
#include "BleMouse.h"

Mouse_ bleMouse;

void myhid_init()
{
    Serial.println("Starting BLE work!");
    bleMouse.begin();
}