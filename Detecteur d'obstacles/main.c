#include "stm32f4xx.h"
/* Include my libraries here */
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_servo.h"
#define SR04_OFFSET 0.8
void TIM3_Init(void) {
  uint16_t PrescalerValue = 0;

  TIM_TimeBaseInitTypeDef   timerInitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;
  timerInitStructure.TIM_Prescaler = PrescalerValue;
  timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  timerInitStructure.TIM_Period = 65535 - 1;
  timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  timerInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM3, &timerInitStructure);
  TIM_Cmd(TIM3, ENABLE);
}
float SR04read(void) {
  TIM_SetCounter(TIM3, 0);
  GPIO_SetBits(GPIOC, GPIO_Pin_10);
  while(TIM_GetCounter(TIM3) < 15);
  GPIO_ResetBits(GPIOC, GPIO_Pin_10);
  TIM_SetCounter(TIM3, 0);
  while(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11));// & (TIM_GetCounter(TIM2) < 50000));
  TIM_SetCounter(TIM3, 0);
  while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11));// & (TIM_GetCounter(TIM2) < 50000));
  return ((float)TIM_GetCounter(TIM3) * 0.01715 + SR04_OFFSET);
}
int main(void) {
	float dist=0;
	int i=0;
		SystemCoreClockUpdate();
		TIM3_Init();
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		GPIO_InitTypeDef ultratrig ;
		ultratrig.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12 |GPIO_Pin_12 | GPIO_Pin_14 ;
		ultratrig.GPIO_Speed = GPIO_Speed_50MHz;
		ultratrig.GPIO_Mode = GPIO_Mode_OUT ;
		ultratrig.GPIO_OType = GPIO_OType_PP ;
		ultratrig.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOC,&ultratrig);
		GPIO_Init(GPIOD,&ultratrig);
		GPIO_InitTypeDef ultraecho ;
		ultraecho.GPIO_Pin = GPIO_Pin_11 ;
		ultraecho.GPIO_Speed = GPIO_Speed_50MHz;
		ultraecho.GPIO_Mode = GPIO_Mode_IN ;
		ultraecho.GPIO_OType = GPIO_OType_PP ;
		ultraecho.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOC,&ultraecho);


		/* Initialize system */
		SystemInit();

		/* Initialize delay */
		TM_DELAY_Init();

    while (1) {
        dist=SR04read();
        if(dist<50)
        {
        GPIO_SetBits(GPIOD,GPIO_Pin_14);
        GPIO_ResetBits(GPIOD,GPIO_Pin_12);
        }
        else
        {
        	GPIO_SetBits(GPIOD,GPIO_Pin_12);
        	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
        }
        Delayms(300);
    }
}
