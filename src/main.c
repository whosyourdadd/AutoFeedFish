/**
  ******************************************************************************
  * @file    Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

  #include "Subroutine.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "stm32f4xx_tim.h"
#include "stm32f4xx_ltdc.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_l3gd20.h"
#include "stm32f429i_discovery_ioe.h"

/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t demoMode;





void
prvInit()
{
	//LCD init
	LCD_Init();
	IOE_Config();
	LTDC_Cmd( ENABLE );

	LCD_LayerInit();
	LCD_SetLayer( LCD_FOREGROUND_LAYER );
	LCD_Clear( LCD_COLOR_BLACK );
	LCD_SetTextColor( LCD_COLOR_WHITE );
      
	//Button
	STM_EVAL_PBInit( BUTTON_USER, BUTTON_MODE_GPIO );

	//LED
	STM_EVAL_LEDInit( LED3 );
}


static void SG_90Ctrl_Task( void *pvParameters )
{
	while( 1 ){
              sg_90_routine();
              vTaskDelay( 100 );
	}
}

static void TimerCountTask( void *pvParameters )
{
	while( 1 ){
            Timer_Routine();
            //vTaskDelay( 1000 );
            vTaskDelay( 333 );

	}
}


  static void BackgroundTask( void *pvParameters )
{
	while( 1 ){
		Background_draw();
		vTaskDelay( 50 );
	}
}


void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(  RCC_AHB1Periph_GPIOB  , ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);//PB3  TIM3 CH1
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
      
}

void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;
   
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );

    TIM_TimeBaseInitStruct.TIM_Period = (u16)( 500-1); 
    TIM_TimeBaseInitStruct.TIM_Prescaler = (1000 -1);
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct );
    
    TIM_OCStructInit( &TIM_OCInitStruct );
     TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;      // when Count < PWM_MOTOR_MIN => Highw; 
    TIM_OC1Init( TIM3, &TIM_OCInitStruct ); // Channel 1  FOR PB4 PWM

   
  
    TIM_Cmd( TIM3, ENABLE );
}





//Main Function
int main(void)
{	

      //init 
	prvInit();
      GPIO_Configuration();
      TIM_Configuration();
      TimReload_Value = 86400 ;// time cycle input;

      //task create
      xTaskCreate( BackgroundTask, (signed char*) "BackgroundTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( SG_90Ctrl_Task, (signed char*) "SG_90Ctrl_Task", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( TimerCountTask, (signed char*) "TimerCountTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL );

	//Call Scheduler
	vTaskStartScheduler();
}




