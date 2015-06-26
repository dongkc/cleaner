/**
  ******************************************************************************
  * @file    STemWin/STemWin_HelloWorld/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    26-February-2014
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
*
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#include <RTL.h>
#include "WM.h"
#include "main.h"
#include  "stm32f4xx_hal.h"
#include  "stm32f4xx_hal_def.h"
#include  "stm32f4xx_hal_rtc.h"
#include  "stm32f4xx_hal_rtc_ex.h"
#include  "stm32f4xx_hal_tim.h"
#include  "stm32f4xx_hal_tim_ex.h"



extern   unsigned char  LEDCURRENTTIME;


extern  const GUI_FONT GUI_FontHZ16;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t GUI_Initialized = 0;
RTC_HandleTypeDef RtcHandle;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void BSP_Pointer_Update(void);
void BSP_Background(void);

void Error_Handler(void);
static void RTC_CalendarConfig(void);
extern  __task void Main_Task(void);
extern  __task void zhuan1(void);
extern  __task void MEUN_Task(void);
static void GPIO_Config(void);
void Time3Enable(unsigned int timvalue);
void Time3Disable(void);
__task void init (void) ;

__task void  KEY_Sound (void);
__task   void phase_KEY (void);

__task void power (void);

extern __task void Server_Task(void);

OS_MUT mut_GLCD;   
OS_MUT mut_Gclock;   

OS_TID zhuantskid=NULL;                     
OS_TID maintskid=NULL; 
OS_TID keytskid=NULL; 
OS_TID servertskid=NULL; 
OS_TID meuntskid=NULL;
OS_TID setsystemtimetskid=NULL;
OS_TID setledlighttskid=NULL;
OS_TID setledtimetskid=NULL;
OS_TID setsoundtskid=NULL;
OS_TID setworkmodetskid=NULL;
OS_TID yushentskid=NULL;


TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Counter Prescaler value */
uint32_t uwPrescalerValue = 0;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main(void)
{  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();  
  
  /* Configure the system clock to have a system clock = 180 Mhz */
  SystemClock_Config();
  
  /* Init the STemWin GUI Library */
 BSP_SDRAM_Init(); /* Initializes the SDRAM device */
 __CRC_CLK_ENABLE(); /* Enable the CRC Module */
	
	
//	__PWR_CLK_ENABLE();
//	HAL_PWR_EnableBkUpAccess();

//	__HAL_RCC_LSE_CONFIG(RCC_LSE_BYPASS);
//	__HAL_RCC_RTC_ENABLE();
	
	/*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;
  
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */ 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
   
  /*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
  /* Read the BackUp Register 0 Data */
  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)
  {  
    /* Configure RTC Calendar */
    RTC_CalendarConfig();
  }
  else
  {
    /* Check if the Power On Reset flag is set */  
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
     
    }
    /* Check if Pin Reset flag is set */
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
     
    }
    
    /* Clear Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
	
  
	
  GUI_Init();  
  //GUI_Initialized = 1;
  GPIO_Config();
	if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) ==0)
	{
		 HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR1,600);  
	}
	Time3Enable(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1));

  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);
    
 os_sys_init (init); 
  
}

void Error_Handler(void)
{
  /* Turn LED3 on */
  //BSP_LED_On(LED3);
  while(1)
  {
  }
}

static void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 0x15;
  sdatestructure.Month = RTC_MONTH_MARCH;
  sdatestructure.Date = 0x02;
  // sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = 0x20;
  stimestructure.Minutes = 0x10;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2);  
}



__task void init (void) {

   os_mut_init(mut_GLCD);
	 
	 os_mut_init(mut_Gclock);
	
   GUI_UC_SetEncodeUTF8();  

  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR6) !=1)
	{
		 
		 zhuantskid = os_tsk_create (zhuan1, 0);
	   maintskid  = os_tsk_create (Main_Task, 0);  
     keytskid   = os_tsk_create (phase_KEY, 0);    
	   servertskid = os_tsk_create (Server_Task, 0);  		
	}else
	{
		                                   Time3Disable();																			                              												 
																			 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);  
																			 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);  
                                   
                                       HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);  
																			 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET);  
		
	}	
  	
	 
	
	 
	 os_tsk_create (power, 0);  
   
 
  // os_tmr_create(500,2);     

  os_tsk_delete_self ();
}

/**
  * @brief  Initializes the STM324x9I-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
	static GPIO_InitTypeDef  GPIO_InitStruct;
	
	__GPIOC_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
  __GPIOE_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();

	// LED背光控制管脚PC8.
	/*
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,GPIO_PIN_SET);
	
	*/
	// 提示音控制脚
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
  //  指示灯：白光  PIN3    黄色光； PIN2
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 	
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET); 
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct); 	
	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET); 
  HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);
 

 

}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Activate the Over Drive feature (available only for STM32F42xxx/43xxx devices)*/
  if(HAL_PWREx_ActivateOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

void Time3Enable(unsigned int timvalue)
{
	 uwPrescalerValue = ((SystemCoreClock ) / 16000000) - 1;
	 TimHandle.Instance = TIM3;
  
  TimHandle.Init.Prescaler     = uwPrescalerValue;
  TimHandle.Init.Period        = 600;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;
  if(HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for all channels */
  sConfig.OCMode     = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	
	/* Set the pulse value for channel 3 */
  sConfig.Pulse = timvalue;
  if(HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
	/* Start channel 3 */
  if(HAL_TIM_PWM_Start(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
	
}

void Time3Disable(void)
{
   if(HAL_TIM_PWM_Stop(&TimHandle, TIM_CHANNEL_3) != HAL_OK)
    {
      /* Starting Error */
       Error_Handler();
    }

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


__task void KEY_Sound (void)
{
   //unsigned int keycount =0;
   //unsigned short BlankLED =0;

  // if(BackLED!=NULL)  os_tmr_kill (BackLED);  

  // BlankLED =  BKP_ReadBackupRegister(BKP_DR34);
     
  // BackLED = os_tmr_create((unsigned int)(BlankLED*100*1.8),1);

   if( HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR3)==0)
   {
	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
	    os_dly_wait (20);
	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
   }
   os_tsk_delete_self ();
		 
  
}

__task   void phase_KEY (void)
{
	  static GPIO_InitTypeDef  GPIO_InitStruct;
	  GPIO_PinState bitstatus;
	
	  __GPIOF_CLK_ENABLE();
	  __GPIOA_CLK_ENABLE();
	
	  while(1)
		{		
			  // 1 TOUCH
	      GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); 	  
	      bitstatus   =   HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6);	      
        if(bitstatus==GPIO_PIN_RESET)
				{			
             os_dly_wait (2);
             bitstatus   =   HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6);
					   if(bitstatus==GPIO_PIN_RESET)
						 {	 
							   do{
							           bitstatus   =   HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_6);
								   }while(bitstatus==GPIO_PIN_RESET); 
								 
							   GUI_StoreKeyMsg(GUI_KEY_ENTER, 1);
                 
	               os_tsk_create (KEY_Sound,0);  	
                 LEDCURRENTTIME =0; // 清除掉关闭背光标识
 								 Time3Enable(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1));
	 
	           }
	  
	      }
				// 2 TOUCH
	      GPIO_InitStruct.Pin = GPIO_PIN_8;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct); 	  
	      bitstatus   =   HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8);	      
        if(bitstatus==GPIO_PIN_RESET)
				{			
             os_dly_wait (2);
             bitstatus   =   HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8);
					   if(bitstatus==GPIO_PIN_RESET)
						 {	 
							   do{
							        bitstatus   =   HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8);
								   }while(bitstatus==GPIO_PIN_RESET); 
								 
							   GUI_StoreKeyMsg(GUI_KEY_DOWN, 1);
	               os_tsk_create (KEY_Sound,0);  	
                 LEDCURRENTTIME =0; // 清除掉关闭背光标识
 								 Time3Enable(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1));
	 
	           }
	  
	      }
			// 3 TOUCH
	      GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 	  
	      bitstatus   =   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);	      
        if(bitstatus==GPIO_PIN_RESET)
				{			
             os_dly_wait (2);
             bitstatus   =   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
					   if(bitstatus==GPIO_PIN_RESET)
						 {	 
							   do{
							        bitstatus   =   HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);
								   }while(bitstatus==GPIO_PIN_RESET); 
								 
							   GUI_StoreKeyMsg(GUI_KEY_UP, 1);
	               os_tsk_create (KEY_Sound,0);  	
                 LEDCURRENTTIME =0; // 清除掉关闭背光标识
 								 Time3Enable(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1));
 
	           }
	  
	      }	
		  // 4 TOUCH
	      GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 	  
	      bitstatus   =   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);	      
        if(bitstatus==GPIO_PIN_RESET)
				{			
             os_dly_wait (2);
             bitstatus   =   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);
					   if(bitstatus==GPIO_PIN_RESET)
						 {	 
							   do{
							        bitstatus   =   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);
								   }while(bitstatus==GPIO_PIN_RESET); 
								 
							   GUI_StoreKeyMsg(GUI_KEY_ESCAPE, 1);
	               os_tsk_create (KEY_Sound,0);  	
                 LEDCURRENTTIME =0; // 清除掉关闭背光标识
 								 Time3Enable(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1));
	 
	           }
	  
	      }			
				
			os_tsk_pass();	
				
	  } 
}	


__task void power (void)
 {
   unsigned int count =0;
   unsigned int temp;
	 static GPIO_InitTypeDef  GPIO_InitStruct;
	 GPIO_PinState bitstatus;
  
   while(1)
     {
	      GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;  
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 	  
	      bitstatus   =   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);	      
        if(bitstatus==GPIO_PIN_RESET)
				{			
             os_dly_wait (200);
             bitstatus   =   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);
					   if(bitstatus==GPIO_PIN_RESET)
						 {	 
							 
							   if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR6)==0)
								 {
												count =0;
											 do{
															 bitstatus   =   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2);
															 count++;
															 if(count>5000000)
															 {       
																			 Time3Disable();
																			 GUI_Clear();                                 												 
																			 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET);  
																			 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_RESET);  
																			 if(zhuantskid  !=NULL)  os_tsk_delete(zhuantskid);   
																			 if(maintskid   !=NULL)  os_tsk_delete(maintskid);   
																			 if(keytskid    !=NULL)  os_tsk_delete(keytskid);  
																			 if(servertskid !=NULL)  os_tsk_delete(servertskid);  
																			 if(meuntskid !=NULL)  os_tsk_delete(meuntskid);  
																			 if(setsystemtimetskid !=NULL)  os_tsk_delete(setsystemtimetskid);  
																			 if(setledlighttskid !=NULL)  os_tsk_delete(setledlighttskid);  
																			 if(setledtimetskid !=NULL)  os_tsk_delete(setledtimetskid);  
																			 if(setsoundtskid !=NULL)  os_tsk_delete(setsoundtskid);   													 
																			 if(setworkmodetskid !=NULL)  os_tsk_delete(setworkmodetskid);  
																			 if(yushentskid !=NULL)  os_tsk_delete(yushentskid);

                                       HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR6,1);  		// 说明是正常关机状态；

                                       														 
                                       HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);  
																			 HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET);  
                                     											 
																	 
															 } 	 
													 
												 }while(bitstatus==GPIO_PIN_RESET); 	
									}else
                  {
                         os_mut_init(mut_GLCD);
	 
												 os_mut_init(mut_Gclock);
												
												 GUI_UC_SetEncodeUTF8();   
												 
												 zhuantskid = os_tsk_create (zhuan1, 0);
												 maintskid  = os_tsk_create (Main_Task, 0);  
												 keytskid   = os_tsk_create (phase_KEY, 0);    
												 servertskid = os_tsk_create (Server_Task, 0);   
                         Time3Enable(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1));
                        
										     HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET);  
												 HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2,GPIO_PIN_SET);  
										
                         HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR6,0);  // 清楚掉关机状态 	
                  }										
					
	           }
	  
	      }
     
	    }    
   
 }