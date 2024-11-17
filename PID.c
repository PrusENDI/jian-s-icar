#include <stdio.h>
#include <math.h>
#include "stm32f10x.h"                  // Device header
#include <stdarg.h>
#include "AD.h"
#include "serial.h"

// 自适应PID控制结构体
typedef struct {
    float Kp;      // 比例系数
    float Ki;      // 积分系数
    float Kd;      // 微分系数
    float integral;
    float last_error;
    float prev_time;
} AdaptivePID;

// 初始化自适应PID结构体
AdaptivePID init_adaptive_pid(float Kp, float Ki, float Kd) {
    AdaptivePID apid = {Kp, Ki, Kd, 0, 0, 0};
    return apid;
}

float output = pid_control(xxx);
// PID控制函数
float pid_control(AdaptivePID *apid, float setpoint, float measured_value, float dt) {
    float error = setpoint - measured_value;  // 计算误差
    apid->integral += error * dt;  // 积分项
    float derivative = (error - apid->last_error) / dt;  // 微分项
    float output = apid->Kp * error + apid->Ki * apid->integral + apid->Kd * derivative;  // PID输出
    apid->last_error = error;  // 更新误差
    return output;
}

// 自适应PID参数更新函数
void update_pid_params(AdaptivePID *apid, float error, float dt) {
    static const float max_Kp = 2.0;  // Kp的最大值
    static const float min_Kp = 0.1;  // Kp的最小值
    static const float max_Ki = 0.2;  // Ki的最大值
    static const float min_Ki = 0.01; // Ki的最小值
    static const float max_Kd = 0.1;  // Kd的最大值
    static const float min_Kd = 0.001; // Kd的最小值
    static const float learning_rate = 0.1;  // 学习因子

    float error_rate = (error - apid->last_error) / dt;  // 误差变化率

    // 平滑参数调整
    float adjustment_factor = 1.0 + learning_rate * error_rate;

    // 根据误差大小和变化率调整参数
    if (fabs(error) > 1.0 || fabs(error_rate) > 0.5) {
        apid->Kp *= adjustment_factor;  // 增加比例增益
        apid->Ki *= adjustment_factor;  // 增加积分增益
        apid->Kd *= adjustment_factor;  // 增加微分增益
    } else {
        apid->Kp /= adjustment_factor;  // 减少比例增益
        apid->Ki /= adjustment_factor;  // 减少积分增益
        apid->Kd /= adjustment_factor;  // 减少微分增益
    }

    // 限制参数范围
    apid->Kp = fmax(fmin(apid->Kp, max_Kp), min_Kp);
    apid->Ki = fmax(fmin(apid->Ki, max_Ki), min_Ki);
    apid->Kd = fmax(fmin(apid->Kd, max_Kd), min_Kd);
}

float get_left_sensor_value() {
    double left_sensor_value= AD_Value[1];// 模拟或实际获取左传感器值
    return left_sensor_value;  // 示例返回值
}

float get_right_sensor_value() {
    double right_sensor_value= AD_Value[3];// 模拟或实际获取右传感器值
    return right_sensor_value;  // 示例返回值
}

float get_middle_sensor_value() {
    // 模拟或实际获取中间传感器值
    return 0.0;  // 示例返回值
}

void calculate_spans(float left_ad, float right_ad, float left_max, float right_max, float *left_span, float *right_span) {
    // 计算左右阈值点修正值
    *left_span = 0.0;  // 示例返回值
    *right_span = 0.0;  // 示例返回值
}

void control_car(float control_output) {
    // 控制小车的函数
}

