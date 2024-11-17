#include "C:\\weizijian\\documents\\stm32\\stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
#include "serial.h"
#include "MPU6050.h"
#include "angle.h"
#include <math.h>

static float yaw = 0.0f;      // 当前偏航角
static float yaw_rate = 0.0f; // 偏航角速率
static float bias = 0.0f;     // 偏移量
static float last_yaw = 0.0f; // 上次偏航角
static float integral = 0.0f; // 积分项

#ifndef M_PI
#define M_PI 3.14159265358979323846     //定义pi
#endif

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量

int main(void)
{
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	AD_Init();					//AD初始化
	Serial_Init();              //UART初始化
    MPU6050_Init();		//MPU6050初始化
    
	/*显示静态字符串*/
////	OLED_ShowString(1, 1, "AD0:");
//	OLED_ShowString(2, 1, "AD1:");
//	OLED_ShowString(3, 1, "AD2:");
//	OLED_ShowString(4, 1, "AD3:");
    /*显示ID号*/
	OLED_ShowString(1, 1, "ID:");		//显示静态字符串
	ID = MPU6050_GetID();				//获取MPU6050的ID号
	OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号

//	while (1)
//	{
////		OLED_ShowNum(1, 5, AD_Value[0], 4);		//显示转换结果第0个数据
//		OLED_ShowNum(2, 5, AD_Value[1], 4);		//显示转换结果第1个数据
//		OLED_ShowNum(3, 5, AD_Value[2], 4);		//显示转换结果第2个数据
//		OLED_ShowNum(4, 5, AD_Value[3], 4);		//显示转换结果第3个数据
//		send_sensor_data();
//		Delay_ms(100);							//延时100ms，手动增加一些转换的间隔时间
//	}
    
    
    while (1) {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
//		OLED_ShowSignedNum(2, 1, AX, 5);					//OLED显示数据
//		OLED_ShowSignedNum(3, 1, AY, 5);
//		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
        // 读取传感器数据
//        int16_t ax, ay, az;
//        int16_t gx, gy, gz;
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);

        // 将角速度数据转换为浮点数
        float wx = (float)GX;
        float wy = (float)GY;

        // 计算夹角
        calculateYaw(wx,wy);


//        if (angle == -1.0) {
//           continue;
//        } 
//        else {
//        float newangle = angle * 180.0 / M_PI ;
        OLED_ShowSignedNum(2, 1, yaw, 5);
//            // 打印结果
//            printf("水平面产生的夹角（弧度）: %.2f\n", angle);
//            printf("水平面产生的夹角（度）: %.2f\n", angle * 180.0 / M_PI);
//        }

        // 延时100ms
//        Delay_ms(100);
    }
}
  
