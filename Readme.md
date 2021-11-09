# 前沿分队团队项目

## 1.利用野火的STM32F405控制ODrive v3.6驱动板
### 功能模块：通过按键控制电机0和电机1的转动

### 具体描述：利用ODrive开发板支持的ASCII串口通信协议，实现了利用野火开发板控制的目的。其中，野火的开发板作为控制板，ODrive的开发板作为驱动板，野火控制板通过USART1通信串口直接发送命令给ODrive驱动板，达到控制电机驱动的效果。

### 硬件接口：

#### 野火STM32F405:

PA9:USART1_TX 端口

PA10:USART1_RX 端口

#### ODrive v3.6驱动板：

GPIO1:TX 端口

GPIO2:RX 端口

硬件连接：PA9接GPIO2,PA10接GPIO1，GND接GND。如图：

 ![野火开发板接线图](./../The%20actual%20demo/野火板子接线图.jpg)

 ![ODrive开发板接线图](./../The%20actual%20demo/ODrive接线图.jpg)

### 核心代码

```C

    int main()
    {
    //位置控制模式下标记电机当前的状态
    int flag0,flag1;
    flag0=0;
    flag1=0;

	//USART1端口初始化
	Debug_USART_Config();
	
	//LED端口初始化
	LED_GPIO_Config();

	//按键端口初始化
	Key_GPIO_Config();

	//printf("这是一个串口通信控制ODrive实验\n");
	
	/*轮询检查按键状态，如果按下按键1就亮紫灯，电机0正转50圈，
	如果按下按键2就亮青灯，电机1正转50圈。*/
	while(1)
	{
		if ( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
		{
			if(flag0==0)
			{
				/* led显示紫色,此时电机0处于状态0 */
				LED_PURPLE;
				/*电机0正转50圈*/
				printf("t 0 50\n");
				/*标记电机0到状态1了*/
				flag0=1;
			}else{
				/*led显示黄色，此时电机0处于状态1*/
				LED_YELLOW;
				/*电机0反转50圈*/
				printf("t 0 0\n");
				/*标记电机0回到状态0了*/
				flag0=0;	
			}
			
		}
		
		if ( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
		{
			if(flag1==0)
			{
				/* led显示青色，此时电机1处于状态0 */
				LED_CYAN;
				/*电机1正转50圈*/
				printf("t 1 50\n");
				/*标记电机1到状态1了*/
				flag1=1;
			}else{
				/*led显示白色，此时电机1处于状态1*/
				LED_WHITE;
				/*电机1反转50圈*/
				printf("t 1 0\n");
				/*标记电机1回到状态1*/
				flag1=0;
			}
		}	
	}
}
```
实际实验效果：

<video id="video" controls="" preload="none" poster="D:\Technics\RealTimeOperateSystem\TeamProject\STM32F407-ODrive-version1\The actual demo\封面.png">
<source id="mp4" src="D:\Technics\RealTimeOperateSystem\TeamProject\STM32F407-ODrive-version1\The actual demo\按键控制电机转动.mp4" type="video/mp4">
</video>

