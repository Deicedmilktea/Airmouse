#include <Arduino.h>
#include <Wire.h>
#include <freertos/task.h>
#include "mympu6050.h"
#include "myhid.h"
#include "BleMouse.h"

extern myimu_t myimu;
extern Mouse_ bleMouse;
const int buttonPin = 16;   // 将按键连接到ESP32的GPIO16引脚
bool buttonPressed = false; // 用于跟踪按键状态

void vTask1(void *pvParameters)
{
  mympu6050_init();
  // Task code goes here
  for (;;)
  {
    mympu6050_get_data();
    vTaskDelay(1);
  }
}

void vTask2(void *pvParameters)
{
  myhid_init();
  pinMode(buttonPin, INPUT);
  // Task code goes here
  for (;;)
  {
    // myimu_update();
    // vTaskDelay(1);
    // myimu_err_cal(30);
    // // myimu_limit(90);
    // bleMouse.move(myimu.yaw_err, myimu.pitch_err, 0, 0);
    myimu_dispose(200, 120);
    bleMouse.move(myimu.yaw, myimu.pitch, 0, 0);

    int buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && !buttonPressed)
    {
      // 按键被按下
      bleMouse.click(MOUSE_LEFT);
      vTaskDelay(50);       // 可以根据需要调整延迟时间
      buttonPressed = true; // 设置按键状态为按下
    }
    else if (buttonState == HIGH && buttonPressed)
    {
      // 按键被释放
      bleMouse.release(MOUSE_LEFT);
      buttonPressed = false; // 设置按键状态为释放
    }
    vTaskDelay(1);
  }
}

void setup()
{
  Serial.begin(115200);
  xTaskCreate(vTask1, "Task 1", 2048, NULL, 1, NULL);
  xTaskCreate(vTask2, "Task 2", 6144, NULL, 1, NULL);
}

void loop()
{
  delay(1);
}