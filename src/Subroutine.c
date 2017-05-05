#include "Subroutine.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



u16 Adc_Teset_Buf[2] = {0};
u16 Adc_DMA_Buf[4] = {0};
u8 MsgRecvQueue[32] ={ 0};
u8 MsgRecv_Count = 0;
u8 MsgRecv_Head = 0;
u8 MsgRecv_Trail = 0;
u8 PrintString[32];
int UART_count ;
//Mode
uint8_t demoMode = 0;
unsigned int  temp ;
unsigned char state = 0x01;
unsigned long Timer_count;
unsigned long TimReload_Value;


void Timer_Routine(void){
	if(Timer_count > 0){
		Timer_count--;
	}
	
}


void sg_90_routine(void){
	static 	unsigned int Period_value = 15;

	if(Timer_count == 0){
	       switch(state){
	             case 0x01:
	                   temp 		=  Period_value;                       
	                   state 		= 0x02;
	                   Timer_count	= 10;
	                    break;
	              case 0x02:
	                    temp		= Period_value  + 30*2;
	                    state 		= 0x01;
	                    Timer_count 	= TimReload_Value;
	                    break;
	                  
	         }
	         
                      TIM3->CCR1 =  temp;
	  
      	}else if( STM_EVAL_PBGetState( BUTTON_USER ) ){
      		Timer_count = 0;
      	}



}

void Get_StringValue(u8 *head_String , u8 *CombineString , u16 InputValue , u16 string_head , u16 string_end){
	int i  =0;   
	u16 temp = InputValue;
	while(head_String[i] != '\0'){
		CombineString[i] = head_String[i];
		i++;
		if(i>5) break ;
	}

	for(i=string_end ; i>=string_head ; i-- ){
		CombineString[i] = (temp%10)+48;
		temp /=10;
	} 
	CombineString[string_end +1] = '\0';


}



void Background_draw(void){
	u8 CCR1_String[12] = {0};
	u8 timer_String[12] = {0};
	u8 Days_String[12] = {0};
	u8 Hours_String[12] = {0};
	u8 Min_String[12] = {0};
	u8 Sec_String[12] = {0};
	long TimerTemp     = Timer_count;
	unsigned int Days         = 0;
	unsigned int Hours 	= 0;
	unsigned int Min	= 0;
	unsigned int Sec 	= 0;
	for(Days=0;(TimerTemp - 86400)> 0;Days++)
	{	
		TimerTemp -= 86400;// 60 * 60 *24 = 86400 = ONE DAY SEC
	}
	for(Hours=0;(TimerTemp - 3600)> 0;Hours++)
	{	
		TimerTemp -= 3600;// 60 * 60 = 3600 = ONE HOUR SEC
	}
	for(Min=0;(TimerTemp - 60)> 0;Min++)
	{	
		TimerTemp -= 60;// 60  = ONE MIN SEC
	}
	Sec = TimerTemp;
	LCD_Clear(0x0000);
	Get_StringValue("CH1:"	,CCR1_String,temp,4,9);
 	Get_StringValue("Tim:"	,timer_String,Timer_count,4,9);
 	//--------------------------------------------------------------------------------
 	Get_StringValue("Day:"	,Days_String,Days,4,9);
 	Get_StringValue("HRs:"	,Hours_String,Hours,4,9);
 	Get_StringValue("Min:"	,Min_String,Min,4,9);
 	Get_StringValue("Sec:"	,Sec_String,Sec,4,9);
	//Get_StringValue("Tim:"	,timer_String,Timer,4,9);

	LCD_SetTextColor( LCD_COLOR_BLUE );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-150,CCR1_String );//LCD_PIXEL_HEIGHT 320 
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-120,timer_String );//LCD_PIXEL_HEIGHT 320

	LCD_SetTextColor( LCD_COLOR_GREEN );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-60,"AutoFeedFish" );//LCD_PIXEL_HEIGHT 320 
	LCD_SetTextColor( LCD_COLOR_GREEN );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT/2-30,"by Soup" );//LCD_PIXEL_HEIGHT 320 

	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-110,Days_String );//LCD_PIXEL_HEIGHT 320 
	LCD_SetTextColor( LCD_COLOR_BLUE2  );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-80,Hours_String );//LCD_PIXEL_HEIGHT 320 
	LCD_SetTextColor( LCD_COLOR_BLUE );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-50,Min_String );//LCD_PIXEL_HEIGHT 320 
	LCD_SetTextColor( LCD_COLOR_MAGENTA );
	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT-20,Sec_String );//LCD_PIXEL_HEIGHT 320 


	vTaskDelay(300);

}