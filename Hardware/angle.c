#include "stm32f10x.h"
#include <math.h>

// 定义滤波器参数
#define Kp 0.02f  // 比例增益
#define Ki 0.001f // 积分增益
#define dt 0.01f  // 时间步长（10 ms）

// 全局变量
static float yaw = 0.0f;      // 当前偏航角
static float yaw_rate = 0.0f; // 偏航角速率
static float bias = 0.0f;     // 偏移量
static float last_yaw = 0.0f; // 上次偏航角
static float integral = 0.0f; // 积分项

/**
 * @brief 计算偏航角
 * @param gyroX X轴角速度 (度/秒)
 * @param gyroY Y轴角速度 (度/秒)
 */
void calculateYaw(float gyroX, float gyroY) {
    static float last_yaw = 0.0f;
    static float integral = 0.0f;

    // 计算偏航角速率
    yaw_rate = gyroY; // 假设 Y 轴角速度为偏航角速率

    // 计算积分项
    integral += yaw_rate * dt;

    // 计算偏航角
    yaw = last_yaw + (yaw_rate - bias) * dt + Kp * (0.0f - yaw) + Ki * integral;

    // 更新上次偏航角
    last_yaw = yaw;

    // 限制偏航角在 -180 到 180 度之间
    if (yaw > 180.0f) {
        yaw -= 360.0f;
    } else if (yaw < -180.0f) {
        yaw += 360.0f;
    }
}

 