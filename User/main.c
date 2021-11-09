#include "stm32f4xx.h"
#include "./bsp_usart/bsp_debug_usart.h"
#include "./bsp_key/bsp_key.h"
#include "./bsp_led/bsp_led.h"

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
