# AIRMOUSE
[视频展示请戳这里捏~](https://www.bilibili.com/video/BV1ab4y1N7Ne/?share_source=copy_web&vd_source=a00a1bf65e172d1e96191b4478c5756e)
## Introduction
本项目采用ESP32开发板作为MCU，MPU6050作为姿态传感器，实现了一个无线蓝牙鼠标，实现移动和点击任务。
## Hardware
ESP32\
MPU6050\
按键开关
## Software
采用FreeRTOS，两个Task，`vTask1`用于处理姿态数据，`vTask2`用于处理鼠标按键
### Solution 1：陀螺仪数据
采用`yaw roll pitch`控制，通过计算`vTaskDelay(1)`前后的陀螺仪差值，对其进行限幅处理，得到鼠标的移动速度，从而实现鼠标移动。\
但是这样发现鼠标的移动卡顿感明显，初步判定为`vTaskDelay(1)`的影响，导致鼠标任务执行间隔过长，出现卡顿感。
```C
myimu_update();
vTaskDelay(1);
myimu_err_cal(30);
myimu_limit(90);
bleMouse.move(myimu.yaw_err, myimu.pitch_err, 0, 0);

// 此处imu的数据直接传给update，然后经由其传给正常的，便于差值计算
void myimu_update()
{
    myimu.yaw = myimu.yaw_update;
    myimu.pitch = myimu.pitch_update;
    myimu.roll = myimu.roll_update;
}

void myimu_err_cal(int16_t weight)
{
    myimu.yaw_err = weight * (myimu.yaw_update - myimu.yaw);
    myimu.pitch_err = weight * (myimu.pitch_update - myimu.pitch);
    myimu.roll_err = weight * (myimu.roll_update - myimu.roll);
}

void myimu_limit(int16_t limit)
{
    if (myimu.yaw_err > limit)
    {
        myimu.yaw_err = limit;
    }
    else if (myimu.yaw_err < -limit)
    {
        myimu.yaw_err = -limit;
    }

    if (myimu.pitch_err > limit)
    {
        myimu.pitch_err = limit;
    }
    else if (myimu.pitch_err < -limit)
    {
        myimu.pitch_err = -limit;
    }

    if (myimu.roll_err > limit)
    {
        myimu.roll_err = limit;
    }
    else if (myimu.roll_err < -limit)
    {
        myimu.roll_err = -limit;
    }
}
```
### Solution 2：加速度计数据
采用`x y z`控制，对加速度值进行限幅处理，然后进行比例运算，得到鼠标的移动速度，从而实现鼠标移动。
```C
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

// range为阈值范围，ratio为比例计算
void myimu_dispose(int16_t range, int16_t ratio)
{
    if (aaReal.y > -range && aaReal.y < range)
    {
        myimu.yaw = 0;
    }
    else
    {
        myimu.yaw = -aaReal.y / ratio;
    }

    if (aaReal.z > -range && aaReal.z < range)
    {
        myimu.pitch = 0;
    }
    else
    {
        myimu.pitch = aaReal.z / ratio;
    }
}
```
## Effect
本次实验只实现了移动和点击功能，其他功能在后期都可以实现，比如双击、滚轮等。根据`HID`协议发送相应任务报文即可。