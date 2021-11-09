#include "./bsp_key/bsp_key.h"

//不精确的定时
 
void Key_Delay(__IO u32 nCount)
{
    for(;nCount != 0; nCount--);
}

//配置按键用到的IO口

void Key_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //开启按键GPIO口的时钟
    RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);

    //选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = KEY1_PIN;

    //设置引脚为输入模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

    //设置引脚既不上拉也不下拉
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    //使用上面的结构体初始化指针
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    //选择按键的引脚
    GPIO_InitStructure.GPIO_Pin = KEY2_PIN;

    //使用上面的结构体初始化指针
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

}

//按键扫描函数
uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    //检测是否有按键按下
    if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
    {
        //等待按键释放
        while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
        return KEY_ON;
    }
    else
        return KEY_OFF;
}
