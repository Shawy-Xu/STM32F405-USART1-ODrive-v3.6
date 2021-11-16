# 前沿分队团队项目

---

## 1. 利用野火的STM32F405控制ODrive v3.6驱动板
### 功能模块：`通过按键控制电机0和电机1的转动`

### 具体描述：`利用ODrive开发板支持的ASCII串口通信协议，实现了通过野火开发板控制的目的。其中，野火的开发板作为控制板，ODrive的开发板作为驱动板，野火控制板通过USART1通信串口直接发送命令给ODrive驱动板，达到控制电机驱动的效果。`

### 硬件接口：

#### 野火STM32F405:

`PA9`:USART1_TX 端口

`PA10`:USART1_RX 端口

#### ODrive v3.6驱动板：

`GPIO1`:TX 端口

`GPIO2`:RX 端口

硬件连接：
- `PA9接GPIO2 `
- `PA10接GPIO1` 
- `GND接GND`

如图：

![野火接线图](\Actual-demo\野火接线图.jpg "野火开发板接线图")

![ODrive接线](\Actual-demo\ODrive接线.jpg "ODrive接线图")

**注意:**`原来的野火STM32开发板上USART1串口的PA9和PA10接口直接用跳帽连接了RX和TX接口（打印输出到电脑上位机的串口调试助手），连接时需要拔掉。 `
### 核心代码（主函数）

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

<video id="video" controls="" preload="none" poster="D:\Technics\RealTimeOperateSystem\TeamProject\STM32F407-ODrive-v1.0\Actual-demo\第一图.png">
<source id="mp4" src="D:\Technics\RealTimeOperateSystem\TeamProject\STM32F407-ODrive-v1.0\Actual-demo\按键控制.mp4" 
type="video/mp4">
</video>

---

## 2.功能实现过程详细描述

### 文件结构

**Actual-demo** 文件夹：存放实际演示的结果
- 实际效果演示视频。
- 两个开发板实际接线的图。

**Doc/RefersData** 文件夹：存放用到的参考书籍
- 野火STM32F407开发板的入门教程。
- ODrive ASCII通信协议的中文文档（有部分错误）。
- STM32F407的官方英文手册（包含引脚和GPIO接口对应关系）。

**Libraries** 文件夹：存放开发依赖的各种库文件和头文件
- ARM芯片的启动文件，核心文件，依赖文件。
- STM32F407的官方库文件。

**Project** 文件夹：存放keil 5工程文件

**User** 文件夹：存放用户的各种功能文件
- `main.c` 主函数
- `stm32f4xx_conf.h` 头文件
- `stm32f4xx_it.h` 中断头文件
- `stm32f4xx_it.c` 中断主文件
- `/bsp_key` 按键功能模块配置文件夹
- `/bsp_led` LED指示灯模块配置文件夹
- `/bsp_usart` USART1串口通信配置文件夹

**.gitignore** 文件：git版本管理忽略配置文件

**Readme.md** 文件：说明文件

### 主函数 main.c
`Debug_USART_Config();` USART1串口初始化

`LED_GPIO_Config();` LED端口初始化

`Key_GPIO_Config();` 按键端口初始化

初始化完成之后就进入主循环

**电机0**和**电机1**分别拥有两个状态：**状态0** 和 **状态1**

因为电机采用`位置控制模式`，所以发送的命令为**位置指令**，

指令具体命令含义请参考：`ASCII格式协议中文版.pdf`（位于 **/Doc/RefersData** 文件夹）

**电机0状态0**：LED显示`紫色`，`电机0`处于`0`位置

**电机0状态1**：LED显示`黄色`，`电机0`处于`50`位置

**电机1状态0**：LED显示`青色`，`电机1`处于`0`位置

**电机1状态1**：LED显示`白色`，`电机1`处于`50`位置

轮询扫描按键变化

`按键1`负责控制`电机0`状态转换

`按键2`负责控制`电机1`状态转换

### 按键配置 bsp_key
#### bsp_key.h
按键配置头文件

**GPIO分组**：`GPIOA`组（按键1），`GPIOC`组（按键2）

**GPIO引脚**：`GPIO_Pin_0` 按键1引脚，`GPIO_Pin_13` 按键2引脚

**使能时钟**：`RCC_AHB1Periph_GPIOA`（按键1时钟使能），   `RCC_AHB1Periph_GPIOC`（按键2时钟使能）

**按键引脚相关请参考**：`[野火EmbedFire]《STM32库开发实战指南——基于野火霸天虎开发板》—20210122.pdf` **第13章 GPIO输入——按键检测**，

以及`2-STM32F407英文数据手册（没有中文）.pdf` 第3节：**3 Pinouts and pin description**

#### bsp_key.c
按键配置具体实现程序
调用STM32F407的官方库函数来**初始化按键（Key_GPIO_Config）**，并定义了**按键扫描函数（Key_Scan）**。
详细内容参考源代码

### LED配置 bsp_led
#### bsp_led.h
LED配置头文件

**GPIO分组**：`GPIOF`组

**GPIO引脚**：`GPIO_Pin_6` LED1引脚，`GPIO_Pin_7` LED2引脚，`GPIO_Pin_8` LED3引脚

**使能时钟**：`RCC_AHB1Periph_GPIOF`

**LED引脚相关请参考**：`[野火EmbedFire]《STM32库开发实战指南——基于野火霸天虎开发板》—20210122.pdf` **第12章 GPIO输出——使用固件库点亮LED**，

以及`2-STM32F407英文数据手册（没有中文）.pdf` 第3节：**3 Pinouts and pin description**

头文件利用直接操作寄存器的方式控制LED，利用三个LED灯的亮灭来显示不同的颜色，并给每个LED灯的颜色一个宏定义，方便直接调用。

#### bsp_led.c
LED配置具体实现程序
调用STM32F407的官方库函数来**初始化LED灯（LED_GPIO_Config）**。

详细内容参考源代码

### USART串口通信配置 bsp_debug_usart
#### bsp_debug_usart.h
USART1串口通信配置头文件

**GPIO分组**：`GPIOA`组

**GPIO引脚**：`GPIO_Pin_10` USART1接收端口引脚， `GPIO_Pin_9` UASART1发送端口引脚

**GPIO引脚源**：`GPIO_PinSource10` USART1接收端口引脚源，`GPIO_PinSource9` USART1发送端口引脚源

**使能时钟**：`RCC_APB2Periph_USART1`（接收端口和发送端口都为此时钟源）

**USART传输速率**：`115200`

**USART串口通信相关请参考**：`[野火EmbedFire]《STM32库开发实战指南——基于野火霸天虎开发板》—20210122.pdf` **第21章 USART——串口通信**，

以及`2-STM32F407英文数据手册（没有中文）.pdf` 第3节：**3 Pinouts and pin description**

头文件定义了USART1串口的发送端端口和接收端端口的基本参数，并规定了传输的速率，方便后续的调试移植。

### bsp_debug_usart.c
USART1串口通信配置实现函数

USART1串口通信使用中断方式来传输数据

`NVIC_Configuration()`函数：向量中断配置函数。配置USART为中断源，抢断优先级为1，子优先级为1

`Debug_USART_Config()`函数：初始化USART串口通信。配置GPIO串口的速度为`50MHz`（高速），通信的波特率为`115200`，数据位加校验位的字长为`8`，停止位为`1`（1个停止位），不使用校验，硬件流控制，同时能接收和发送。

`Usart_SendByte()`函数：利用USART通信发送一个字符。

`Usart_SendString()`函数：利用USART通信发送字符串。

`Usart_SendHalfWord()`函数：利用USART通信发送一个16位数。

`fputc()`函数：重定向c库函数printf到串口，重定向后可使用printf函数直接发送数据给ODrive。

`fgetc()`函数：重定向c库函数scanf到串口，重定向后可使用scanf、getchar等函数。

具体配置过程以及详细内容参考源代码。

### stm32f4xx_it.c 中断配置文件
`DEBUG_USART_IRQHandler()`函数：USART串口通信中断服务函数。
