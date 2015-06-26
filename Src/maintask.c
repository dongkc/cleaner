#include <RTL.h>
#include "GUI.H"
#include "main.h"
#include "WM.h"

#include  "stm32f4xx_hal.h"
#include  "stm32f4xx_hal_def.h"
#include  "stm32f4xx_hal_rtc.h"
#include  "stm32f4xx_hal_rtc_ex.h"
#include  "Time.h"

#define   num  1

#define  open_jidianqi()        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET) 
#define  close_jidianqi()       HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOG,GPIO_PIN_3,GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOG,GPIO_PIN_2,GPIO_PIN_SET)  

extern __task void MEUN_Task(void);

extern  const GUI_FONT GUI_FontHT33;
extern  const GUI_FONT GUI_FontYH47;
extern  const GUI_FONT GUI_Fontbanhei;
extern  const GUI_FONT GUI_FontYH19;
extern  const GUI_FONT GUI_FontYH25;
extern  const GUI_FONT GUI_Fontfz24;

extern GUI_CONST_STORAGE GUI_BITMAP bmbz;
extern GUI_CONST_STORAGE GUI_BITMAP bmmy;
extern GUI_CONST_STORAGE GUI_BITMAP bmqs;
extern GUI_CONST_STORAGE GUI_BITMAP bmsm;
extern GUI_CONST_STORAGE GUI_BITMAP bmxh;
extern GUI_CONST_STORAGE GUI_BITMAP bmys;

extern GUI_CONST_STORAGE GUI_BITMAP bmTEST;

//gif
//extern  char acbzgif[]; 

#include "tupian\bzgif.c"
#include "tupian\qsgif.c"
#include "tupian\mygif.c"
#include "tupian\smgif.c"
#include "tupian\xhgif.c"
#include "tupian\ysgif.c"


GUI_GIF_INFO InfoGif11;
GUI_GIF_IMAGE_INFO InfoGif12;


GUI_GIF_INFO InfoGif21;
GUI_GIF_IMAGE_INFO InfoGif22;


GUI_GIF_INFO InfoGif31;
GUI_GIF_IMAGE_INFO InfoGif32;

GUI_GIF_INFO InfoGif41;
GUI_GIF_IMAGE_INFO InfoGif42;

GUI_GIF_INFO InfoGif51;
GUI_GIF_IMAGE_INFO InfoGif52;

GUI_GIF_INFO InfoGif61;
GUI_GIF_IMAGE_INFO InfoGif62;


extern RTC_HandleTypeDef RtcHandle;
extern OS_MUT mut_GLCD; 
extern OS_MUT mut_Gclock;   
extern OS_TID zhuantskid;  

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

 
 unsigned char  LEDCURRENTTIME =0;  
 
 unsigned int   ReachTime =0;
 unsigned char jidianqistatus =0;

extern void Time3Disable(void);                         

 uint8_t aShowTime[50] = {0}, aShowTimeStamp[50] = {0};
 uint8_t aShowDate[50] = {0}, aShowDateStamp[50] = {0};
 uint8_t aShowxingqi[50] = {0};

 struct  tm  currenttime;
	   time_t  current_unixtime;
 
 
struct tm Time_ConvUnixToCalendar(time_t t)
{
	struct tm *t_tm;
	t_tm = localtime(&t);
	t_tm->tm_year += 1900;	
	return *t_tm; 
 
}

time_t Time_ConvCalendarToUnix(struct tm t)
{
	t.tm_year -= 1900; 
	return mktime(&t);
}
__task void Server_Task(void)
{
    RTC_DateTypeDef sdatestructureget;
    RTC_TimeTypeDef stimestructureget;   
    HAL_RTC_GetTime(&RtcHandle, &stimestructureget, FORMAT_BIN);
				/* Get the RTC current Date */
		HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);

    currenttime.tm_year = sdatestructureget.Year +2000;
	  currenttime.tm_mon  = sdatestructureget.Month -1;
	  currenttime.tm_yday = sdatestructureget.Date;
    currenttime.tm_hour = stimestructureget.Hours;
    currenttime.tm_min  = stimestructureget.Minutes;
    currenttime.tm_sec  = stimestructureget.Seconds;

	  current_unixtime =  Time_ConvCalendarToUnix(currenttime);
	
     if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==0)
     {
        //GUI_DispStringAt("全时模式",285,230);
			  open_jidianqi();			 
     }
		 
	   if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==1)
     {
        //GUI_DispStringAt("循环模式",285,230);
			  // 开关60分钟间隔；
			  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==0)
	        {
             ReachTime = current_unixtime + num*60;
          }
			 // 开关90分钟间隔；
			  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==1)
	        {
             ReachTime = current_unixtime + num*90;
          }
			 // 开关120分钟间隔；
			  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==2)
	        {
             ReachTime = current_unixtime + num*120;
          }	      
			  jidianqistatus = 1;
			  open_jidianqi();
     }
	
    if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==2)
     {
        //GUI_DispStringAt("标准模式",285,230);
	      ReachTime = current_unixtime + num*45;
			  jidianqistatus = 1;
			  open_jidianqi();
     }
		if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==4)
     {
        //  GUI_DispStringAt("母婴模式",285,230);
			  ReachTime = current_unixtime + num*30;
			  jidianqistatus = 1;
			  open_jidianqi();
			 
     } 
		if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==5)
     {
        //GUI_DispStringAt("睡眠模式",285,230);
			  ReachTime = current_unixtime + num*15;
			  jidianqistatus = 1;
			  open_jidianqi();
			 
     }  
		 
		 
	
	while(1)
	{
		os_mut_wait(mut_Gclock, 0xffff);
	  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, FORMAT_BIN);
				/* Get the RTC current Date */
		HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);
    os_mut_release(mut_Gclock);
		
    currenttime.tm_year = sdatestructureget.Year +2000;
	  currenttime.tm_mon  = sdatestructureget.Month -1;
	  currenttime.tm_yday = sdatestructureget.Date;
    currenttime.tm_hour = stimestructureget.Hours;
    currenttime.tm_min  = stimestructureget.Minutes;
    currenttime.tm_sec  = stimestructureget.Seconds;

	  current_unixtime =  Time_ConvCalendarToUnix(currenttime);
		
		
		
		 
		if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==1)
     {
        //GUI_DispStringAt("循环模式",285,230);		
	      if((current_unixtime>=ReachTime)&&(jidianqistatus==1))
				{
					  jidianqistatus = 0;
					  close_jidianqi();
					  // 开关60分钟间隔；
						if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==0)
							{
								 ReachTime = current_unixtime + num*60;
							}
					 // 开关90分钟间隔；
						if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==1)
							{
								 ReachTime = current_unixtime + num*90;
							}
					 // 开关120分钟间隔；
						if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==2)
							{
								 ReachTime = current_unixtime + num*120;
							}	      		
				}
				if((current_unixtime>=ReachTime)&&(jidianqistatus==0))
				{
					  jidianqistatus = 1;
					  open_jidianqi();
					  // 开关60分钟间隔；
						if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==0)
							{
								 ReachTime = current_unixtime + num*60;
							}
					 // 开关90分钟间隔；
						if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==1)
							{
								 ReachTime = current_unixtime + num*90;
							}
					 // 开关120分钟间隔；
						if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==2)
							{
								 ReachTime = current_unixtime + num*120;
							}	      				
				}
			 
			 
			 
     }
    
    if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==2)
     {
        //GUI_DispStringAt("标准模式",285,230);
	      if((current_unixtime>=ReachTime)&&(jidianqistatus==1))
				{
					  jidianqistatus = 0;
					  close_jidianqi();
					  ReachTime = current_unixtime + num*15;					
				}
				if((current_unixtime>=ReachTime)&&(jidianqistatus==0))
				{
					  jidianqistatus = 1;
					  open_jidianqi();
					  ReachTime = current_unixtime + num*45;					
				}
				
     }
		 if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==4)
     {
        //  GUI_DispStringAt("母婴模式",285,230);
	      if((current_unixtime>=ReachTime)&&(jidianqistatus==1))
				{
					  jidianqistatus = 0;
					  close_jidianqi();
					  ReachTime = current_unixtime + num*30;					
				}
				if((current_unixtime>=ReachTime)&&(jidianqistatus==0))
				{
					  jidianqistatus = 1;
					  open_jidianqi();
					  ReachTime = current_unixtime + num*30;					
				}
				
     }
		 if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR4)==5)
     {
        //  GUI_DispStringAt("睡眠模式",285,230);
	      if((current_unixtime>=ReachTime)&&(jidianqistatus==1))
				{
					  jidianqistatus = 0;
					  close_jidianqi();
					  ReachTime = current_unixtime + num*45;					
				}
				if((current_unixtime>=ReachTime)&&(jidianqistatus==0))
				{
					  jidianqistatus = 1;
					  open_jidianqi();
					  ReachTime = current_unixtime + num*15;					
				}
				
     }
	
	}
}

unsigned char  getxingqi(RTC_DateTypeDef* my)
{
  
  int y = my->Year + 2000;
  int m = my->Month;
  int d = my->Date;
  
  if(m==1||m==2) {
        m+=12;
        y--;
    }
    return(((d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7)+1);  

}

unsigned char myindex =0;
__task void Main_Task(void)
{
	RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

    int KEY =0; 
    int i =0;

	GUI_SetFont(&GUI_Fontfz24);   
  os_mut_wait(mut_GLCD, 0xffff);


 

  GUI_Clear();
  /*
  GUI_SetColor(JH_QIANLAN);
  GUI_FillRect(1,1,375,272);	

  GUI_SetColor(JH_QIANLAN1);
  GUI_FillRect(380,1,480,272);  

  GUI_SetColor(GUI_WHITE);  
  GUI_FillRect(375,1,380,272);	

  GUI_FillRect(380,93,480,95);	
  GUI_FillRect(380,188,480,190);	 

  GUI_SetTextMode(GUI_TM_TRANS|GUI_TM_REV); 
	
  GUI_DispStringAt("桃园仙居智能通风净化系统",80,240);  
  */
//	GUI_FillRect(10,48,470,49);	
		
//  GUI_FillRect(10,210,470,211);

    GUI_SetColor(GUI_WHITE);

    GUI_FillRect(0,0,480,272);	
    
 //   GUI_DrawBitmap(&bmqs, 30, 5);
  //  GUI_DrawBitmap(&bmbz, 145, 5);
	//	GUI_DrawBitmap(&bmmy, 260, 5);
	//	GUI_DrawBitmap(&bmsm, 30, 127);
	//	GUI_DrawBitmap(&bmxh, 145, 127);
	//	GUI_DrawBitmap(&bmys, 260, 127); 

     GUI_DrawBitmap(&bmTEST, 0, 0); 
 
	  GUI_GIF_GetInfo(acqsgif, sizeof(acqsgif), &InfoGif11); 
    GUI_GIF_GetInfo(acbzgif, sizeof(acbzgif), &InfoGif21); 
    GUI_GIF_GetInfo(acmygif, sizeof(acmygif), &InfoGif31); 
    GUI_GIF_GetInfo(acsmgif, sizeof(acsmgif), &InfoGif41); 
    GUI_GIF_GetInfo(acxhgif, sizeof(acxhgif), &InfoGif51); 
    GUI_GIF_GetInfo(acysgif, sizeof(acysgif), &InfoGif61); 
	 
  os_mut_release(mut_GLCD);
	
  myindex = 10;
	
	while(1)
  {      
        os_mut_wait(mut_GLCD, 0xffff);		
						/* Get the RTC current Time */
    //     GUI_SetBkColor(JH_QIANLAN);
       if(myindex ==0)
       {
        // GUI_SetColor(GUI_WHITE);  
        // GUI_FillRect(27,3,150,125);

         if(i < InfoGif11.NumImages-1)
         {
              GUI_GIF_GetImageInfo(acqsgif, sizeof(acqsgif), &InfoGif12, i);
              GUI_GIF_DrawSub(acqsgif,
                            sizeof(acqsgif),
                            30,
                            5,
                            i++);
         
         }
         else
         {
              i = 0;
         }
        }
       if(myindex ==1)
       {

        // GUI_SetColor(JH_QIANLAN);
       //  GUI_SetColor(GUI_WHITE);  
       //  GUI_FillRect(142,3,265,125);

         if(i < InfoGif21.NumImages-1)
         {
              GUI_GIF_GetImageInfo(acbzgif, sizeof(acbzgif), &InfoGif22, i);
              GUI_GIF_DrawSub(acbzgif,
                            sizeof(acbzgif),
                            145,
                            5,
                            i++);
           
         }
         else
         {
              i = 0;
         }
        }
       if(myindex ==2)
       {

        // GUI_SetColor(JH_QIANLAN);
       //  GUI_SetColor(GUI_WHITE);  
       //  GUI_FillRect(142,3,265,125);

         if(i < InfoGif31.NumImages-1)
         {
              GUI_GIF_GetImageInfo(acmygif, sizeof(acmygif), &InfoGif32, i);
              GUI_GIF_DrawSub(acmygif,
                            sizeof(acmygif),
                            260,
                            5,
                            i++);
           
         }
         else
         {
              i = 0;
         }
        } 
       if(myindex ==3)
       {

        // GUI_SetColor(JH_QIANLAN);
       //  GUI_SetColor(GUI_WHITE);  
       //  GUI_FillRect(142,3,265,125);

         if(i < InfoGif41.NumImages-1)
         {
              GUI_GIF_GetImageInfo(acsmgif, sizeof(acsmgif), &InfoGif42, i);
              GUI_GIF_DrawSub(acsmgif,
                            sizeof(acsmgif),
                            30,
                            127,
                            i++);
           
         }
         else
         {
              i = 0;
         }
        } 
       if(myindex ==4)
       {

        // GUI_SetColor(JH_QIANLAN);
       //  GUI_SetColor(GUI_WHITE);  
       //  GUI_FillRect(142,3,265,125);

         if(i < InfoGif51.NumImages-1)
         {
              GUI_GIF_GetImageInfo(acxhgif, sizeof(acxhgif), &InfoGif52, i);
              GUI_GIF_DrawSub(acxhgif,
                            sizeof(acxhgif),
                            145,
                            127,
                            i++);
           
         }
         else
         {
              i = 0;
         }
        } 
       if(myindex ==5)
       {

        // GUI_SetColor(JH_QIANLAN);
       //  GUI_SetColor(GUI_WHITE);  
       //  GUI_FillRect(142,3,265,125);

         if(i < InfoGif61.NumImages-1)
         {
              GUI_GIF_GetImageInfo(acysgif, sizeof(acysgif), &InfoGif62, i);
              GUI_GIF_DrawSub(acysgif,
                            sizeof(acysgif),
                            260,
                            127,
                            i++);
           
         }
         else
         {
              i = 0;
         }
        } 
        GUI_SetBkColor(GUI_BLACK); 
		  
		    os_mut_wait(mut_Gclock, 0xffff);
				HAL_RTC_GetTime(&RtcHandle, &stimestructureget, FORMAT_BIN);
				/* Get the RTC current Date */
				HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);
				os_mut_release(mut_Gclock);
				/* Display date Format : mm-dd-yy */
				sprintf((char*)aShowDate,"%0.2d-%0.2d-%0.2d",  2000 + sdatestructureget.Year,sdatestructureget.Month, sdatestructureget.Date); 
				
				/* Display time Format : hh:mm:ss */
			//	sprintf((char*)aShowTime,"%0.2d:%0.2d:%0.2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
				
				sprintf((char*)aShowTime,"%0.2d:%0.2d", stimestructureget.Hours, stimestructureget.Minutes);

        

        if(sdatestructureget.WeekDay==1)  sprintf((char*)aShowxingqi,"星期一");
        if(sdatestructureget.WeekDay==2)  sprintf((char*)aShowxingqi,"星期二");
        if(sdatestructureget.WeekDay==3)  sprintf((char*)aShowxingqi,"星期三");
        if(sdatestructureget.WeekDay==4)  sprintf((char*)aShowxingqi,"星期四");
        if(sdatestructureget.WeekDay==5)  sprintf((char*)aShowxingqi,"星期五");
        if(sdatestructureget.WeekDay==6)  sprintf((char*)aShowxingqi,"星期六");
        if(sdatestructureget.WeekDay==7)  sprintf((char*)aShowxingqi,"星期日");  
				  
		   /*
        GUI_SetFont(&GUI_Fontbanhei);

        GUI_SetColor(JH_QIANLAN1);
        GUI_FillRect(381,1,480,90);
        GUI_SetTextMode(GUI_TM_TRANS|GUI_TM_REV); 
				GUI_DispStringAt((char*)aShowTime, 384,25);	

        GUI_SetFont(&GUI_FontYH19);
        GUI_SetTextMode(GUI_TM_TRANS|GUI_TM_REV); 
				GUI_DispStringAt((char*)aShowDate, 394,218);

        GUI_SetFont(&GUI_FontYH25);
        GUI_SetTextMode(GUI_TM_TRANS|GUI_TM_REV); 
				GUI_DispStringAt((char*)aShowxingqi, 403,130);
	
		    */ 
        
        GUI_SetTextMode(GUI_TM_NORMAL);   
  	
				os_mut_release(mut_GLCD);
		    os_dly_wait (125);
        //以下为重要的计算时间到的方法；
        // 以下为计算的背光关闭时间设置方法；
        LEDCURRENTTIME++;
        if(LEDCURRENTTIME>4*HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR2))
        {
           Time3Disable();
        }
	
         GUI_PollKeyMsg();
		     KEY=GUI_GetKey();

        switch(KEY)
				{
				   case GUI_KEY_ESCAPE:
                
                
				        break;
				   case GUI_KEY_ENTER: 

                if(zhuantskid !=NULL) 
                    os_tsk_delete(zhuantskid);   
                 
				        setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self (); 			       
						    break;
				       	
				   case GUI_KEY_UP:
				        myindex++;
				        break;
           case GUI_KEY_DOWN:
				        myindex--;
				        break;
           default:  break;
         }
	
	}
	
}


