#include "stm32f10x.h"
#include <math.h>

// �����˲�������
#define Kp 0.02f  // ��������
#define Ki 0.001f // ��������
#define dt 0.01f  // ʱ�䲽����10 ms��

// ȫ�ֱ���
static float yaw = 0.0f;      // ��ǰƫ����
static float yaw_rate = 0.0f; // ƫ��������
static float bias = 0.0f;     // ƫ����
static float last_yaw = 0.0f; // �ϴ�ƫ����
static float integral = 0.0f; // ������

/**
 * @brief ����ƫ����
 * @param gyroX X����ٶ� (��/��)
 * @param gyroY Y����ٶ� (��/��)
 */
void calculateYaw(float gyroX, float gyroY) {
    static float last_yaw = 0.0f;
    static float integral = 0.0f;

    // ����ƫ��������
    yaw_rate = gyroY; // ���� Y ����ٶ�Ϊƫ��������

    // ���������
    integral += yaw_rate * dt;

    // ����ƫ����
    yaw = last_yaw + (yaw_rate - bias) * dt + Kp * (0.0f - yaw) + Ki * integral;

    // �����ϴ�ƫ����
    last_yaw = yaw;

    // ����ƫ������ -180 �� 180 ��֮��
    if (yaw > 180.0f) {
        yaw -= 360.0f;
    } else if (yaw < -180.0f) {
        yaw += 360.0f;
    }
}

 