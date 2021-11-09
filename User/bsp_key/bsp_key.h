#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"

//引脚定义

#define KEY1_PIN            GPIO_Pin_0
#define KEY1_GPIO_PORT      GPIOA
#define KEY1_GPIO_CLK       RCC_AHB1Periph_GPIOA

#define KEY2_PIN            GPIO_Pin_13
#define KEY2_GPIO_PORT      GPIOC
#define KEY2_GPIO_CLK       RCC_AHB1Periph_GPIOC

//按键按下设置宏

#define KEY_ON  1
#define KEY_OFF 0

void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, u16 GPIO_Pin);

#endif /*__LED_H*/
