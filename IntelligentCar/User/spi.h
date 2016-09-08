#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x.h"

#define Set_SD_CS          {GPIO_SetBits(GPIOB,GPIO_Pin_7);}
#define Clr_SD_CS          {GPIO_ResetBits(GPIOB,GPIO_Pin_7);}


#define Set_SPI_PCM1770_CS {GPIO_SetBits(GPIOB,GPIO_Pin_11);}
#define Clr_SPI_PCM1770_CS {GPIO_ResetBits(GPIOB,GPIO_Pin_11);}


#define Set_SPI_7843_NSS   {GPIO_SetBits(GPIOA,GPIO_Pin_4);}
#define Clr_SPI_7843_NSS   {GPIO_ResetBits(GPIOA,GPIO_Pin_4);}

#define GPIO_SPI2     GPIOB
#define RCC_SPI2      RCC_APB2Periph_GPIOB
#define SPI2_SCK      GPIO_Pin_13
#define SPI2_MISO     GPIO_Pin_14
#define SPI2_MOSI     GPIO_Pin_15

#define GPIO_SPI3     GPIOC
#define RCC_SPI3      RCC_APB2Periph_GPIOC
#define SPI3_SCK      GPIO_Pin_10
#define SPI3_MISO     GPIO_Pin_11
#define SPI3_MOSI     GPIO_Pin_12

#define SPI_WIRELESS     SPI1
																					  
void WIRELESS_SPI_Init(void);				//初始化SPI口 
u8 SPI_ReadWriteByte(u8 TxData);		//SPI总线读写一个字节
		 
#endif

