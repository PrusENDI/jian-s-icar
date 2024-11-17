#include <stdio.h>
#include <math.h>
#include "stm32f10x.h"                  // Device header
#include <stdarg.h>
#include "AD.h"
#include "serial.h"

// ����ӦPID���ƽṹ��
typedef struct {
    float Kp;      // ����ϵ��
    float Ki;      // ����ϵ��
    float Kd;      // ΢��ϵ��
    float integral;
    float last_error;
    float prev_time;
} AdaptivePID;

// ��ʼ������ӦPID�ṹ��
AdaptivePID init_adaptive_pid(float Kp, float Ki, float Kd) {
    AdaptivePID apid = {Kp, Ki, Kd, 0, 0, 0};
    return apid;
}

float output = pid_control(xxx);
// PID���ƺ���
float pid_control(AdaptivePID *apid, float setpoint, float measured_value, float dt) {
    float error = setpoint - measured_value;  // �������
    apid->integral += error * dt;  // ������
    float derivative = (error - apid->last_error) / dt;  // ΢����
    float output = apid->Kp * error + apid->Ki * apid->integral + apid->Kd * derivative;  // PID���
    apid->last_error = error;  // �������
    return output;
}

// ����ӦPID�������º���
void update_pid_params(AdaptivePID *apid, float error, float dt) {
    static const float max_Kp = 2.0;  // Kp�����ֵ
    static const float min_Kp = 0.1;  // Kp����Сֵ
    static const float max_Ki = 0.2;  // Ki�����ֵ
    static const float min_Ki = 0.01; // Ki����Сֵ
    static const float max_Kd = 0.1;  // Kd�����ֵ
    static const float min_Kd = 0.001; // Kd����Сֵ
    static const float learning_rate = 0.1;  // ѧϰ����

    float error_rate = (error - apid->last_error) / dt;  // ���仯��

    // ƽ����������
    float adjustment_factor = 1.0 + learning_rate * error_rate;

    // ��������С�ͱ仯�ʵ�������
    if (fabs(error) > 1.0 || fabs(error_rate) > 0.5) {
        apid->Kp *= adjustment_factor;  // ���ӱ�������
        apid->Ki *= adjustment_factor;  // ���ӻ�������
        apid->Kd *= adjustment_factor;  // ����΢������
    } else {
        apid->Kp /= adjustment_factor;  // ���ٱ�������
        apid->Ki /= adjustment_factor;  // ���ٻ�������
        apid->Kd /= adjustment_factor;  // ����΢������
    }

    // ���Ʋ�����Χ
    apid->Kp = fmax(fmin(apid->Kp, max_Kp), min_Kp);
    apid->Ki = fmax(fmin(apid->Ki, max_Ki), min_Ki);
    apid->Kd = fmax(fmin(apid->Kd, max_Kd), min_Kd);
}

float get_left_sensor_value() {
    double left_sensor_value= AD_Value[1];// ģ���ʵ�ʻ�ȡ�󴫸���ֵ
    return left_sensor_value;  // ʾ������ֵ
}

float get_right_sensor_value() {
    double right_sensor_value= AD_Value[3];// ģ���ʵ�ʻ�ȡ�Ҵ�����ֵ
    return right_sensor_value;  // ʾ������ֵ
}

float get_middle_sensor_value() {
    // ģ���ʵ�ʻ�ȡ�м䴫����ֵ
    return 0.0;  // ʾ������ֵ
}

void calculate_spans(float left_ad, float right_ad, float left_max, float right_max, float *left_span, float *right_span) {
    // ����������ֵ������ֵ
    *left_span = 0.0;  // ʾ������ֵ
    *right_span = 0.0;  // ʾ������ֵ
}

void control_car(float control_output) {
    // ����С���ĺ���
}

