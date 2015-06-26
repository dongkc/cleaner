#include <RTL.h>
#include "GUI.H"
#include "DIALOG.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "EDIT.h"

#include  "stm32f4xx_hal.h"
#include  "stm32f4xx_hal_def.h"
#include  "stm32f4xx_hal_rtc.h"
#include  "stm32f4xx_hal_rtc_ex.h"

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

extern  const GUI_FONT GUI_FontHT24;
extern  const GUI_FONT GUI_FontHT33;
extern __task void MEUN_Task(void);
extern OS_MUT mut_GLCD; 

extern  const GUI_FONT GUI_FontYH47; 
extern RTC_HandleTypeDef RtcHandle;


unsigned char timeindex;


__task void Set_SystemTime_Task(void)
{
  int KEY =0;
  timeindex =0;
  
 	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;


  os_mut_wait(mut_GLCD, 0xffff);

  GUI_Clear();
	GUI_SetFont(&GUI_FontHT33);
  GUI_DispStringAt("请输入新的时间:",35,20);

  GUI_SetFont(&GUI_FontYH47);
   
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, FORMAT_BIN);
				/* Get the RTC current Date */
	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);

  GUI_FillRect(80,80,175,125);	
  GUI_SetTextMode(GUI_TM_XOR);

  GUI_DispDecAt(2000 + sdatestructureget.Year,85,80,4);

  GUI_DispStringAt("-",185,80);

  GUI_DispDecAt(sdatestructureget.Month,210,80,2);
  
	GUI_DispStringAt("-",260,80);
  
  GUI_DispDecAt(sdatestructureget.Date,285,80,2);

  GUI_DispDecAt(stimestructureget.Hours,100,150,2);

  GUI_DispStringAt(":",155,150);

  GUI_DispDecAt(stimestructureget.Minutes,177,150,2);
  
	GUI_DispStringAt(":",230,150);
  
  GUI_DispDecAt(stimestructureget.Seconds,245,150,2);
		
	
 
   while (1)
       {	
			  
         KEY=GUI_WaitKey();			
				 switch(KEY)
				{
				    case GUI_KEY_ENTER:
                 if(timeindex==5)
                 {  
                    GUI_ClearRect(245,150,292,192);                
                    GUI_SetTextMode(GUI_TM_NORMAL);                  
                    GUI_DispDecAt(stimestructureget.Seconds,245,150,2);
                    // 保存当前时间起来；
                    sdatestructureget.WeekDay = getxingqi(&sdatestructureget);
 
                    HAL_RTC_SetDate(&RtcHandle,&sdatestructureget,FORMAT_BIN) ;
                    HAL_RTC_SetTime(&RtcHandle,&stimestructureget,FORMAT_BIN) ;
                    setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);  
                    os_mut_release(mut_GLCD);
                    
                    os_tsk_delete_self ();             
                    
           
                 }   
                 if(timeindex==4)
                 {  
                    GUI_ClearRect(173,150,224,192);                
                    GUI_SetTextMode(GUI_TM_NORMAL);                  
                    GUI_DispDecAt(stimestructureget.Minutes,177,150,2);
                    GUI_FillRect(245,150,292,192);	
                    GUI_SetTextMode(GUI_TM_XOR);  
                    GUI_DispDecAt(stimestructureget.Seconds,245,150,2);
                    timeindex = 5;              
                 }   

                 if(timeindex==3)
                 {  
                    GUI_ClearRect(100,150,150,192);                
                    GUI_SetTextMode(GUI_TM_NORMAL);                  
                    GUI_DispDecAt(stimestructureget.Hours,100,150,2);
                    GUI_FillRect(173,150,224,192);	
                    GUI_SetTextMode(GUI_TM_XOR);  
                    GUI_DispDecAt(stimestructureget.Minutes,177,150,2);
                    timeindex = 4;              
                 }  
                 if(timeindex==2)
                 {  
                    GUI_ClearRect(280,80,330,122);                
                    GUI_SetTextMode(GUI_TM_NORMAL);                  
                    GUI_DispDecAt(sdatestructureget.Date,285,80,2);
                    GUI_FillRect(100,150,150,192);	
                    GUI_SetTextMode(GUI_TM_XOR);  
                    GUI_DispDecAt(stimestructureget.Hours,100,150,2);
                    timeindex = 3;              
                 }
                 if(timeindex==1)
                 {  
                    GUI_ClearRect(205,80,260,122);                
                    GUI_SetTextMode(GUI_TM_NORMAL);                  
                    GUI_DispDecAt(sdatestructureget.Month,210,80,2);
                    GUI_FillRect(280,80,330,122);	
                    GUI_SetTextMode(GUI_TM_XOR);  
                    GUI_DispDecAt(sdatestructureget.Date,285,80,2);
                    timeindex = 2;              
                 }
                if(timeindex==0)
                 {  
                 
                    GUI_ClearRect(80,80,175,125);	  
                    GUI_SetTextMode(GUI_TM_NORMAL);                  
                    GUI_DispDecAt(2000 + sdatestructureget.Year,85,80,4);
                    GUI_FillRect(205,80,260,122);	
                    GUI_SetTextMode(GUI_TM_XOR);  
                    GUI_DispDecAt(sdatestructureget.Month,210,80,2);
                    timeindex = 1;              

                 }
                break;
				   case GUI_KEY_ESCAPE:                   
				        setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0); 
                os_mut_release(mut_GLCD);  
                os_tsk_delete_self ();
				        break;				       				       
						
				   case GUI_KEY_UP:
				        if(timeindex==0)
                 { 
                     sdatestructureget.Year++;
                     if(sdatestructureget.Year>225) sdatestructureget.Year=225;
                     GUI_FillRect(80,80,175,122);	
                     GUI_SetTextMode(GUI_TM_XOR);
                     GUI_DispDecAt(2000 + sdatestructureget.Year,85,80,4);
                 }
                if(timeindex==1)
                 { 
                     sdatestructureget.Month++;
                     if(sdatestructureget.Month>=12)  sdatestructureget.Month =12;
                     GUI_FillRect(205,80,260,122);	
                     GUI_SetTextMode(GUI_TM_XOR);  
                     GUI_DispDecAt(sdatestructureget.Month,210,80,2);
                 }
                if(timeindex==2)
                 { 
                     sdatestructureget.Date++;
                     if(sdatestructureget.Date>=31)  sdatestructureget.Date =31;
                     GUI_FillRect(280,80,330,122);	
                     GUI_SetTextMode(GUI_TM_XOR);  
                     
                     GUI_DispDecAt(sdatestructureget.Date,285,80,2);
                 }
                if(timeindex==3)
                 { 
                     stimestructureget.Hours++;
                     if(stimestructureget.Hours>=23)  stimestructureget.Hours =23;
                     GUI_FillRect(100,150,150,192);	
                     GUI_SetTextMode(GUI_TM_XOR);  
                     
                     GUI_DispDecAt(stimestructureget.Hours,100,150,2);
                 }
                if(timeindex==4)
                 { 
                     stimestructureget.Minutes++;
                     if(stimestructureget.Minutes>=59)  stimestructureget.Minutes =59;
                     GUI_FillRect(173,150,224,192);	
                     GUI_SetTextMode(GUI_TM_XOR);                     
                     GUI_DispDecAt(stimestructureget.Minutes,177,150,2);
                 }
                 if(timeindex==5)
                 { 
                     stimestructureget.Seconds++;
                     if(stimestructureget.Seconds>=59)  stimestructureget.Seconds =59;
                     GUI_FillRect(245,150,292,192);	
                     GUI_SetTextMode(GUI_TM_XOR);                       
                     GUI_DispDecAt(stimestructureget.Seconds,245,150,2);
                 }   
 
				        break;
           case GUI_KEY_DOWN:
				        if(timeindex==0)
                 { 
                     sdatestructureget.Year--;
                     if(sdatestructureget.Year<15) sdatestructureget.Year=15;
                     GUI_FillRect(80,80,175,122);	
                     GUI_SetTextMode(GUI_TM_XOR);
                     GUI_DispDecAt(2000 + sdatestructureget.Year,85,80,4);
                 }
                if(timeindex==1)
                 { 
                     sdatestructureget.Month--;
                     if(sdatestructureget.Month<=1)  sdatestructureget.Month =1;
                     GUI_FillRect(205,80,260,122);	
                     GUI_SetTextMode(GUI_TM_XOR);  
                     GUI_DispDecAt(sdatestructureget.Month,210,80,2);
                 }
                if(timeindex==2)
                 { 
                     sdatestructureget.Date--;
                     if(sdatestructureget.Date<=1)  sdatestructureget.Date =1;
                     GUI_FillRect(280,80,330,122);	
                     GUI_SetTextMode(GUI_TM_XOR);  
                     
                     GUI_DispDecAt(sdatestructureget.Date,285,80,2);
                 }
                if(timeindex==3)
                 { 
                     stimestructureget.Hours--;
                     if(stimestructureget.Hours<=0)  stimestructureget.Hours =0;
                     GUI_FillRect(100,150,150,192);	
                     GUI_SetTextMode(GUI_TM_XOR);                      
                     GUI_DispDecAt(stimestructureget.Hours,100,150,2);
                 }
                if(timeindex==4)
                 { 
                     stimestructureget.Minutes--;
                     if(stimestructureget.Minutes<=0)  stimestructureget.Minutes =0;
                     GUI_FillRect(173,150,224,192);	
                     GUI_SetTextMode(GUI_TM_XOR);                     
                     GUI_DispDecAt(stimestructureget.Minutes,177,150,2);
                 }
                if(timeindex==5)
                 { 
                     stimestructureget.Seconds--;
                     if(stimestructureget.Seconds<=0)  stimestructureget.Seconds =0;
                     GUI_FillRect(245,150,292,192);	
                     GUI_SetTextMode(GUI_TM_XOR);                       
                     GUI_DispDecAt(stimestructureget.Seconds,245,150,2);
                 }    
				        break;
           default:  break;
				 }
				
			 }	 
	  
}

__task void Set_LEDTime_Task(void)
{
  int KEY =0;
  unsigned int amiao =0;
  
 	


  os_mut_wait(mut_GLCD, 0xffff);

  GUI_Clear();
	GUI_SetFont(&GUI_FontHT33);
  GUI_DispStringAt("LED自动关闭时间:",35,20);
	

  GUI_SetFont(&GUI_FontYH47);   
  GUI_DispStringAt("秒",255,110);
	
	if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2) ==0)
	{
		 HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,30);  
	}
	
	 amiao  =  HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2);

	
	
   while (1)
       {	
          GUI_SetFont(&GUI_FontYH47);   
			    GUI_DispDecAt(amiao,185,110,2);
				  GUI_SetTextMode(GUI_TM_XOR);   
				  os_dly_wait (200);
				 
         GUI_PollKeyMsg();
		     KEY=GUI_GetKey();
        switch(KEY)
				{
				   case GUI_KEY_ESCAPE:
                os_mut_release(mut_GLCD);
                setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self (); 		
				        break;
				   case GUI_KEY_ENTER: 
                 HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,amiao);  
                GUI_SetFont(&GUI_FontHT33);  
                GUI_SetTextMode(GUI_TM_NORMAL);              
				        GUI_DispStringAt("保存成功",145,220);	       
						    break;
				       	
				   case GUI_KEY_UP:
						    amiao++;
					      if(amiao>99)  amiao =99;
					      HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,amiao);  
				        GUI_SetTextMode(GUI_TM_NORMAL);   
				        break;
           case GUI_KEY_DOWN:
				        amiao--;
					      if(amiao<=0)  amiao =0;
					      HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,amiao);  
					      GUI_SetTextMode(GUI_TM_NORMAL);   
				        break;
           default:  break;
         }				 
				 
				 
				 
			 }
}

__task void Set_SoundEnable_Task(void)
{
  int KEY =0;
  unsigned int amiao =0;	


  os_mut_wait(mut_GLCD, 0xffff);

  GUI_Clear();
	GUI_SetFont(&GUI_FontHT33);
  GUI_DispStringAt("提示音当前状态:",35,110);
	
   while (1)
       {	
          amiao  =  HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR3);
			    
          if(amiao==0)
            {
               GUI_DispStringAt("打开",305,110);
            }else
            {
               GUI_DispStringAt("关闭",305,110);
            }
            
				  GUI_SetTextMode(GUI_TM_XOR);   
				  os_dly_wait (200);
				 
         GUI_PollKeyMsg();
		     KEY=GUI_GetKey();
        switch(KEY)
				{
				   case GUI_KEY_ESCAPE:
                os_mut_release(mut_GLCD);
                setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self (); 		
				        break;
				   case GUI_KEY_ENTER: 
                HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR3,amiao);  
                GUI_SetFont(&GUI_FontHT33);  
                GUI_SetTextMode(GUI_TM_NORMAL);                                                                                                                                  
				        GUI_DispStringAt("保存成功",145,220);	       
						    break;
				       	
				   case GUI_KEY_UP:
           case GUI_KEY_DOWN:
						     if(amiao==0) 
                    {
                      amiao =1;
                    }else
                    {
                      amiao =0; 
                    }
					      
					      HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR3,amiao);  
				        GUI_SetTextMode(GUI_TM_NORMAL);				       
				        break;
           default:  break;
         }				 
			 
		 }
}
/*
__task void Set_YUSHETime_Task(void)
{
  int KEY =0;
  unsigned int amiao =0;

  os_mut_wait(mut_GLCD, 0xffff);

  GUI_Clear();
	GUI_SetFont(&GUI_FontHT24);
  GUI_DispStringAt("星期一:",1,40);
	GUI_DispStringAt("星期二:",1,70);
	GUI_DispStringAt("星期三:",1,100);
	GUI_DispStringAt("星期四:",1,130);
	GUI_DispStringAt("星期五:",1,160);
	GUI_DispStringAt("星期六:",1,190);
	GUI_DispStringAt("星期日:",1,220);
	
	
	GUI_DispStringAt("024681012141618202224:",90,1);
	

	
   while (1)
       {	
          
				 
         GUI_PollKeyMsg();
		     KEY=GUI_GetKey();
        switch(KEY)
				{
				   case GUI_KEY_ESCAPE:
                os_mut_release(mut_GLCD);
                setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self (); 		
				        break;
				   case GUI_KEY_ENTER: 
                 HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,amiao);  
                GUI_SetFont(&GUI_FontHT33);  
                GUI_SetTextMode(GUI_TM_NORMAL);              
				        GUI_DispStringAt("保存成功",145,220);	       
						    break;
				       	
				   case GUI_KEY_UP:
						    amiao++;
					      if(amiao>99)  amiao =99;
					      HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,amiao);  
				        GUI_SetTextMode(GUI_TM_NORMAL);   
				        break;
           case GUI_KEY_DOWN:
				        amiao--;
					      if(amiao<=0)  amiao =0;
					      HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR2,amiao);  
					      GUI_SetTextMode(GUI_TM_NORMAL);   
				        break;
           default:  break;
         }				 
				 
				 
				 
			 }
}
*/
