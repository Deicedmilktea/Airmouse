#ifndef _MYMPU6050_H_
#define _MYMPU6050_H_

typedef struct
{
    // 三个轴的度量
    int16_t yaw, pitch, roll;
    // 三个轴的更新值
    int16_t yaw_update, pitch_update, roll_update;
    // 三个轴的误差值
    int16_t yaw_err, pitch_err, roll_err;
} myimu_t;

// 初始化MPU6050
void mympu6050_init();

// 获取MPU6050的数据
void mympu6050_get_data();

// 更新IMU的数据
void myimu_update();

// 计算IMU的变化值
void myimu_err_cal(int16_t weight);

// 限制IMU的差值
void myimu_limit(int16_t limit);

// 处理myimu数据
void myimu_dispose(int16_t range, int16_t ratio);

#endif