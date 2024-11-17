#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "AD.h"
#include "serial.h"

/**
  * 函数：串口初始化
  * 参数：无
  * 返回值：无
  */
void Serial_Init(void)
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  // 开启USART1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   // 开启GPIOA的时钟

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // 将PA9引脚初始化为复用推挽输出

    /* USART初始化 */
    USART_InitTypeDef USART_InitStructure;  // 定义结构体变量
    USART_InitStructure.USART_BaudRate = 9600;  // 波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  // 硬件流控制，不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx;  // 模式，选择为发送模式
    USART_InitStructure.USART_Parity = USART_Parity_No;  // 奇偶校验，不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1;  // 停止位，选择1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;  // 字长，选择8位
    USART_Init(USART1, &USART_InitStructure);  // 将结构体变量交给USART_Init，配置USART1

    /* USART使能 */
    USART_Cmd(USART1, ENABLE);  // 使能USART1，串口开始运行
}

/**
  * 函数：串口发送一个字节
  * 参数：Byte 要发送的一个字节
  * 返回值：无
  */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);  // 将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  // 等待发送完成
    /* 下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位 */
}

/**
  * 函数：串口发送一个数组
  * 参数：Array 要发送数组的首地址
  * 参数：Length 要发送数组的长度
  * 返回值：无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)  // 遍历数组
    {
        Serial_SendByte(Array[i]);  // 依次调用Serial_SendByte发送每个字节数据
    }
}

/**
  * 函数：串口发送一个字符串
  * 参数：String 要发送字符串的首地址
  * 返回值：无
  */
void Serial_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)  // 遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        Serial_SendByte(String[i]);  // 依次调用Serial_SendByte发送每个字节数据
    }
}

/**
  * 函数：次方函数（内部使用）
  * 返回值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;  // 设置结果初值为1
    while (Y--)  // 执行Y次
    {
        Result *= X;  // 将X累乘到结果
    }
    return Result;
}

/**
  * 函数：串口发送数字
  * 参数：Number 要发送的数字，范围：0~4294967295
  * 参数：Length 要发送数字的长度，范围：0~10
  * 返回值：无
  */
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++)  // 根据数字长度遍历数字的每一位
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');  // 依次调用Serial_SendByte发送每位数字
    }
}

/**
  * 函数：使用printf需要重定向的底层函数
  * 参数：保持原始格式即可，无需变动
  * 返回值：保持原始格式即可，无需变动
  */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);  // 将printf的底层重定向到自己的发送字节函数
    return ch;
}

/**
  * 函数：自己封装的prinf函数
  * 参数：format 格式化字符串
  * 参数：... 可变的参数列表
  * 返回值：无
  */
void Serial_Printf(char *format, ...)
{
    char String[100];  // 定义字符数组
    va_list arg;  // 定义可变参数列表数据类型的变量arg
    va_start(arg, format);  // 从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);  // 使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);  // 结束变量arg
    Serial_SendString(String);  // 串口发送字符数组（字符串）
}

void send_sensor_data(void)
{
    uint8_t buffer[50];
    int left_ad = AD_Value[1];  // 获取左传感器值
    int right_ad = AD_Value[2]; // 获取右传感器值
    int measured_value=AD_Value[3]; //获取实际值
    // 格式化数据为字符串
    sprintf((char *)buffer, "%d,%d，%d\r\n", left_ad , measured_value, right_ad);

    // 发送数据
    Serial_SendString((char *)buffer);
}

