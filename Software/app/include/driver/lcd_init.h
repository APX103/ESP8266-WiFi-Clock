#ifndef __LCD_INIT_H
#define __LCD_INIT_H
#include "c_types.h"

#define USE_HORIZONTAL 2  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 	80
#define LCD_H 	160

#else
#define LCD_W 	160
#define LCD_H 	80
#endif

//-----------------LCD�˿ڶ���---------------- 
#define LCD_SCLK_Clr() 	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 0)	//SCL=SCLK
#define LCD_SCLK_Set() 	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 1)

#define LCD_MOSI_Clr() 	GPIO_OUTPUT_SET(GPIO_ID_PIN(4), 0)	//SDA=MOSI
#define LCD_MOSI_Set() 	GPIO_OUTPUT_SET(GPIO_ID_PIN(4), 1)

#define LCD_DC_Clr()   	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 0)	//DC
#define LCD_DC_Set()   	GPIO_OUTPUT_SET(GPIO_ID_PIN(14), 1)

#define LCD_BLK_Clr()   GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 0)	//����
#define LCD_BLK_Set()   GPIO_OUTPUT_SET(GPIO_ID_PIN(12), 1)

#define LCD_RES_Clr()   GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0)	//��λ
#define LCD_RES_Set()   GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 1)

void delay_ms(u32 C_time);
void LCD_GPIO_Init(void);	//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);	//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);	//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);	//д�������ֽ�
void LCD_WR_REG(u8 dat);	//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);	//�������꺯��
void LCD_Init(void);		//LCD��ʼ��

#endif

