#include "defines.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* -------------------------- VARIABLES -------------------------- */
RCC_ClocksTypeDef RCC_Clock;
int lux;
extern __IO uint8_t Receive_Buffer[64];
extern __IO  uint32_t Receive_length ;
extern __IO  uint32_t length ;
uint8_t Send_Buffer[64];
uint32_t packet_sent=1;
uint32_t packet_receive=1;

/* -------------------------- FUNCTIONS -------------------------- */
void init_CLK(void);
void init_TIM(void);
char *sendInt(int data);

int main(void)
{
	init_CLK();
	init_TIM();
	//BH1750_Init();
	/*0->9999*/
	
	
	int testint = 3456;
	
	unsigned char digit1, digit2, digit3, digit4;
	while(1)
	{	
		//digit1 = (char) ((testint % 10) & 0xff);
			//TIM_SetCompare4(TIM4, i);
		
		//lux = BH1750_ReadLux();
		if (bDeviceState == CONFIGURED)
    {
      CDC_Receive_DATA();
      /*Check to see if we have data yet */
      if (Receive_length  != 0)
      {
        if (packet_sent == 1)
          CDC_Send_DATA((unsigned char*)Receive_Buffer,Receive_length);
        Receive_length = 0;
      }
    }
		
		//CDC_Send_DATA((unsigned char*)"abc",Receive_length);
		// delay
	}
}

void init_CLK(void)
{
	RCC_GetClocksFreq(&RCC_Clock);
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();
}

void init_TIM(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 5000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	TIM_Cmd(TIM4,ENABLE);
	// turn off LED
	TIM_SetCompare4(TIM4, 9998);
}

