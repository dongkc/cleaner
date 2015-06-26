#include <RTL.h>
#include "GUI.H"
#include "main.h"
#include "WM.h"
#include "LISTBOX.h"
#include  "stm32f4xx_hal.h"
#include  "stm32f4xx_hal_def.h"
#include  "stm32f4xx_hal_rtc.h"
#include  "stm32f4xx_hal_rtc_ex.h"

extern GUI_CONST_STORAGE GUI_BITMAP bm1;
extern GUI_CONST_STORAGE GUI_BITMAP bm2;
extern  const GUI_FONT GUI_FontHT24;
extern OS_TID zhuantskid;
extern __task void MEUN_Task(void);
extern void Time3Enable(unsigned int timvalue);


extern OS_TID zhuantskid;
extern OS_TID zhuantskid;                     
extern OS_TID maintskid; 
extern OS_TID keytskid; 
extern OS_TID servertskid; 
extern OS_TID meuntskid;
extern OS_TID setsystemtimetskid;
extern OS_TID setledlighttskid;
extern OS_TID setledtimetskid;
extern OS_TID setsoundtskid;
extern OS_TID setworkmodetskid;
extern OS_TID yushentskid;



extern RTC_HandleTypeDef RtcHandle;
extern OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */

__task void zhuan1(void)
{
	
	
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetColor(GUI_WHITE);
	GUI_SetTextMode(GUI_TM_NORMAL);           
	//GUI_SetBkColor(GUI_WHITE);
	//GUI_SetColor(GUI_BLACK);
	
	while(1)
  {      
     
   
     os_mut_wait(mut_GLCD, 0xffff);
		 
	//	 GUI_SetColor(GUI_BLACK);
	//	 GUI_SetBkColor(GUI_WHITE);
	
	//   GUI_DrawBitmap(&bm1,30,220);	
	
	   os_mut_release(mut_GLCD);
		
		 os_dly_wait (100);
		
		
		 os_mut_wait(mut_GLCD, 0xffff);
		 
	//	 GUI_SetColor(GUI_BLACK);
	//	 GUI_SetBkColor(GUI_WHITE);
	
	//   GUI_DrawBitmap(&bm2,30,220);	

     GUI_SetFont(&GUI_FontHT24); 
     
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==0)
     {
       // GUI_DispStringAt("全时模式",285,230);
     }
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==1)
     {
       // GUI_DispStringAt("循环模式",285,230);
     }
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==2)
     {
       // GUI_DispStringAt("标准模式",285,230);
     }
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==3)
     {
       // GUI_DispStringAt("预设模式",285,230);
     }
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==4)
     {
       // GUI_DispStringAt("母婴模式",285,230);
     }
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==5)
     {
       // GUI_DispStringAt("睡眠模式",285,230);
     }     
	
	   os_mut_release(mut_GLCD);
		
		 os_dly_wait (100);
		
	   // zhuantskid = NULL;
	}
	
	
	
	
	
	
}
void drawLight(unsigned int level)
{
   	switch(level)
	 {
		 case 0:
		    GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(31*4,38*4,34*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(20*4,41*4,25*4,45*4);
			  break;
	     case 1:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(41*4,35*4,44*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(30*4,38*4,35*4,45*4);
			  break;
		  case 2:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(51*4,32*4,54*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(40*4,35*4,45*4,45*4);
			  break;
		  case 3:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(61*4,29*4,64*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(50*4,32*4,55*4,45*4);
			  break;
		 case 4:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(71*4,26*4,74*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(60*4,29*4,65*4,45*4);
			  break;
		 case 5:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(81*4,23*4,84*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(70*4,26*4,75*4,45*4);
			  break;
		 case 6:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(91*4,20*4,94*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(80*4,23*4,85*4,45*4);
			  break;
		 case 7:
		      GUI_SetBkColor(GUI_WHITE);	 
			  GUI_SetColor(GUI_BLACK);
			  GUI_FillRect(101*4,17*4,104*4,44*4);
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(90*4,20*4,95*4,45*4);
			  break;
		 case 8:
		      
			  GUI_SetBkColor(GUI_BLACK);
			  GUI_SetColor(GUI_WHITE);
			  GUI_FillRect(100*4,17*4,105*4,45*4);
			  break;
		default:break;
	 }
	 
	 if(level==0) level = 10;
	 if(level==1) level = 30;
	 if(level==2) level = 60;
	 if(level==3) level = 80;
	 if(level==4) level = 100;
	 if(level==5) level = 200;
	 if(level==6) level = 300;
	 if(level==7) level = 500;
	 if(level==8) level = 600;
	 
	 
    Time3Enable(level);
    HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR1,level);  
	
}
__task  void SetBlackLight_Task(void)
{
    int KEY=0;
	int i=0;
	unsigned short  mylevel;

            GUI_SetBkColor(GUI_BLACK);
						GUI_SetColor(GUI_WHITE);
						GUI_Clear();
				
						GUI_DrawLine(0,0,0,270);
						GUI_DrawLine(0,270,478,270);
				    GUI_DrawLine(478,0,478,270);
						GUI_DrawLine(0,0,478,0);
							
					  GUI_FillRect(20*4,40*4,25*4,45*4);

						GUI_FillRect(30*4,37*4,35*4,45*4);

						GUI_FillRect(40*4,34*4,45*4,45*4);

						GUI_FillRect(50*4,31*4,55*4,45*4);

						GUI_FillRect(60*4,28*4,65*4,45*4);

						GUI_FillRect(70*4,25*4,75*4,45*4);

						GUI_FillRect(80*4,22*4,85*4,45*4);

						GUI_FillRect(90*4,19*4,95*4,45*4);

						GUI_FillRect(100*4,16*4,105*4,45*4);

						GUI_SetBkColor(GUI_WHITE);	 
						GUI_SetColor(GUI_BLACK);

						mylevel = 3;

						
															    		
					  if(mylevel<0) GUI_FillRect(21*4,41*4,24*4,44*4); 

						if(mylevel<1) GUI_FillRect(31*4,38*4,34*4,44*4);

						if(mylevel<2) GUI_FillRect(41*4,35*4,44*4,44*4);

						if(mylevel<3) GUI_FillRect(51*4,32*4,54*4,44*4);

						if(mylevel<4) GUI_FillRect(61*4,29*4,64*4,44*4);

						if(mylevel<5) GUI_FillRect(71*4,26*4,74*4,44*4);

						if(mylevel<6) GUI_FillRect(81*4,23*4,84*4,44*4);

						if(mylevel<7) GUI_FillRect(91*4,20*4,94*4,44*4);

						if(mylevel<8) GUI_FillRect(101*4,17*4,104*4,44*4);

         while(1)
          {
	      
		    KEY=GUI_WaitKey();
		
			 switch(KEY)
				{
				   case GUI_KEY_LEFT:
				        			 
				        break;
				   case GUI_KEY_RIGHT: 
				       	
						    break;
				   case GUI_KEY_UP:
				        if(mylevel>0)  mylevel--; 
						    drawLight(mylevel);			
				        break;
				   case GUI_KEY_DOWN:
						    if(mylevel<8)  mylevel++; 
						    drawLight(mylevel);						 
						break;

				   case GUI_KEY_ESCAPE:
				   case GUI_KEY_ENTER:
						    setsystemtimetskid  =   os_tsk_create (MEUN_Task,0);
						    os_tsk_delete_self ();
				        break;
				  
				   default:  break;
                 }
		 }

}