#ifndef SUBROUTINE_H
#define SUBROUTINE_H
#include "stm32f4xx_ltdc.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "stm32f429i_discovery_ioe.h"


/*
#define ADC_Channel  3
#define ADC_Sample  16
#define Vref   3
#define Adc12Bit_Resolution 2047
*/
extern u16 Adc_DMA_Buf[4] ;
extern unsigned long TimReload_Value;
void
GAME_EventHandler1();
/*
void
GAME_EventHandler2();

void
GAME_EventHandler3();

void
GAME_Update();

void
GAME_Render();
*/
void Timer_Routine(void);
void sg_90_routine(void);
void Background_draw(void);
//void Background_draw(void);
//void Get_ADC_StringValue(u8 *X_String , u8 *Y_String , u8 *L_String, u8 *R_String , u16 string_head , u16 string_end);
void Get_StringValue(u8 *head_String , u8 *CombineString , u16 AdcDma_Buf , u16 string_head , u16 string_end);


#endif
