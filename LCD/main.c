/**
 *	Keil project example for HD44780 LCD driver
 *
 *	@author 	Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 */
#include <stdio.h>
#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_keypad.h"
int a=0,cons=200,nb,nb_pieces=0;
char ch[5];
TM_RE_t RE1_Data;
TM_KEYPAD_Button_t Keypad_Button;
uint8_t customChar[] = {
        0x1F,    // xxx 11111
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x1F    // xxx 11111
    };
void initialisations()
{
		SystemInit();
		TM_HD44780_CreateChar(0, &customChar[0]);
		TM_DELAY_Init();
		TM_HD44780_Init(16, 2);
		TM_DISCO_LedInit();
		TM_DISCO_ButtonInit();
		TM_KEYPAD_Init(TM_KEYPAD_Type_Large);
		TM_HD44780_Puts(2, 0, "-XPERT-MECA-");
		TM_HD44780_Puts(1, 1, "Rotary Encoder");
		Delayms(500);
		TM_HD44780_Clear();
}
int lireConsigne()
{		int j=0,tab[4];
		tab[0]=0;
		tab[1]=0;
		tab[2]=0;
		tab[3]=0;
		TM_HD44780_Puts(0, 0, "CONSIGNE :");
		TM_HD44780_Puts(0, 1, "0000 Cm");
		TM_HD44780_CursorOn();
		TM_HD44780_BlinkOn();
		TM_HD44780_CursorSet(0,1);
		  while(!(TM_DISCO_ButtonPressed()))
		 {
		 Keypad_Button = TM_KEYPAD_Read();
		 if (Keypad_Button != TM_KEYPAD_Button_NOPRESSED){
		 switch(Keypad_Button){
		 case TM_KEYPAD_Button_0:
			 cons=0;
		 break;
		 case TM_KEYPAD_Button_1:
			 cons=1;
		 break;
		 case TM_KEYPAD_Button_2:
			 cons=2;
		 break;
		 case TM_KEYPAD_Button_3:
			 cons=3;
		 break;
		 case TM_KEYPAD_Button_4:
			 cons=4;
		 break;
		 case TM_KEYPAD_Button_5:
			 cons=5;
		 break;
		 case TM_KEYPAD_Button_6:
			 cons=6;
		 break;
		 case TM_KEYPAD_Button_7:
			 cons=7;
		 break;
		 case TM_KEYPAD_Button_8:
			 cons=8;
		 break;
		 case TM_KEYPAD_Button_9:
			 cons=9;
		 break;
		 case TM_KEYPAD_Button_STAR:
		 { if (j!=0)
		 	 j--;
		 else {
			j=3;
		}
		 	 goto final;
		 }
		 case TM_KEYPAD_Button_HASH:
		 { if (j!=3)
		 	 j++;
		 else
			 j=0;
		 	 goto final;
		 }
		default:
		break;
		}
		 TM_HD44780_CursorOff();
		 TM_HD44780_BlinkOff();
		 tab[j]=cons ;
		 sprintf(ch,"%d",tab[j]);
		 TM_HD44780_Puts(j, 1,ch);
		 TM_HD44780_CursorOn();
		 TM_HD44780_BlinkOn();
		 final:
		 TM_HD44780_CursorSet(j,1);
		 }}

		TM_HD44780_CursorOff();
		TM_HD44780_BlinkOff();
		Delayms(200);
		TM_HD44780_Clear();
		Delayms(200);
		return(((tab[0]*100)+(tab[1]*100)+(tab[2]*10)+tab[3]));
}
int main(void) {
	initialisations();
	consi:
	TM_HD44780_Clear();
	nb=lireConsigne();
	back :
	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Nombre de tours:");
	a=0;
	TM_RE_Init(&RE1_Data, GPIOD, GPIO_PIN_8, GPIOD, GPIO_PIN_7);
	TM_RE_SetMode(&RE1_Data, TM_RE_Mode_One);
	TM_DISCO_LedOn(LED_GREEN);
	TM_DISCO_LedOff(LED_RED);
	while((a/60)<nb)
		{
		Keypad_Button = TM_KEYPAD_Read();
		switch (Keypad_Button) {
		case TM_KEYPAD_Button_A:goto consi;break;
		case TM_KEYPAD_Button_B:goto back;break;
		}
		TM_RE_Get(&RE1_Data);
		a=RE1_Data.Absolute;
		sprintf(ch,"%d",a/60);
		TM_HD44780_Puts(6, 1, ch);
		Delayms(10);
	}
	nb_pieces++;
	TM_DISCO_LedOff(LED_GREEN);
	TM_DISCO_LedOn(LED_RED);
	Keypad_Button = TM_KEYPAD_Read();
	boucle:
	while(Keypad_Button!=TM_KEYPAD_Button_D)
	{
		Keypad_Button = TM_KEYPAD_Read();
	}
	if(Keypad_Button==TM_KEYPAD_Button_D)
		{
		TM_HD44780_Clear();
		TM_HD44780_Puts(0, 0, "Nombre de Pieces");
		sprintf(ch,"%d",nb_pieces);
		TM_HD44780_Puts(6, 1, ch);
		Delayms(300);
		goto back;
		}

	Delayms(3000);
	while(1);

}
/* TM EXTI Handler for all EXTI lines */
void TM_EXTI_Handler(uint16_t GPIO_Pin) {
    /* Check RE pin 1 */
    if (GPIO_Pin == RE1_Data.GPIO_PIN_A) {
        /* Process data */
        TM_RE_Process(&RE1_Data);
    }

}

void TM_DELAY_1msHandler(void) {

    TM_KEYPAD_Update();
}
