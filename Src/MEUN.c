#include <RTL.h>
#include "GUI.H"
#include "main.h"
#include "WM.h"
#include "LISTBOX.h"
#include  "stm32f4xx_hal.h"
#include  "stm32f4xx_hal_def.h"
#include  "stm32f4xx_hal_rtc.h"
#include  "stm32f4xx_hal_rtc_ex.h"

extern __task void Main_Task(void);
extern __task void zhuan1(void);
extern __task void Set_SystemTime_Task(void);
extern __task  void SetBlackLight_Task(void);
extern __task void Set_LEDTime_Task(void);
extern __task void Set_SoundEnable_Task(void);
extern  const GUI_FONT GUI_FontHT24;

__task void WorkMode_Task(void);
__task void XUNHUANTIMEMode_Task(void);
extern __task void Set_YUSHETime_Task(void);

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







extern  const GUI_FONT GUI_Fontst24;

extern RTC_HandleTypeDef RtcHandle;
extern OS_MUT mut_GLCD;                        /* Mutex to controll GLCD access     */
int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo);
 
__task void MEUN_Task(void)
{
	
  int KEY =0;

   static const GUI_ConstString _aListBox[] = {
				"---时间日期调整",
        "---屏幕背光亮度调整",	
        "---屏幕背光时间调整", 	
    		"---提示音设置",
        "---工作模式选择",	
        "---循环模式时间间隔选择", 
        "---预设模式设置",       
				NULL
  };
  static LISTBOX_Handle hListBox;     

   hListBox = LISTBOX_Create(_aListBox, 0, 0, 480, 272, WM_CF_SHOW);    
	
	 SCROLLBAR_SetDefaultWidth(8); 
   SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);


   LISTBOX_SetBackColor(hListBox,0,GUI_BLACK);
   LISTBOX_SetTextColor(hListBox,0,GUI_WHITE);
  
   LISTBOX_SetBackColor(hListBox,1,GUI_WHITE);
   LISTBOX_SetTextColor(hListBox,1,GUI_BLACK);  
   

   LISTBOX_SetFont(hListBox,&GUI_Fontst24);
   
   
   WM_SetFocus(hListBox);  
   
   LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
   GUI_ClearKeyBuffer();

  while (1)
       {	
			    KEY=GUI_WaitKey();
		
				switch(KEY)
				{
				    case GUI_KEY_ENTER:
                 switch(LISTBOX_GetSel(hListBox))
                 {
                    case 0:
                       
                       LISTBOX_Delete(hListBox);
                   		 os_tsk_create(Set_SystemTime_Task,0);                        
                       os_tsk_delete_self ();
                    break;
										case 1:                       
                       LISTBOX_Delete(hListBox);
                   		 setledlighttskid =  os_tsk_create(SetBlackLight_Task,0);                        
                       os_tsk_delete_self ();
                    break;
										case 2:                       
                       LISTBOX_Delete(hListBox);
                   		 setledtimetskid  =  os_tsk_create(Set_LEDTime_Task,0);                        
                       os_tsk_delete_self ();
                    break;
                    case 3:                       
                       LISTBOX_Delete(hListBox);
                   		 setsoundtskid  =  os_tsk_create(Set_SoundEnable_Task,0);                        
                       os_tsk_delete_self ();
                    break;                     
                    case 4:                       
                       LISTBOX_Delete(hListBox);
                   		 setworkmodetskid  =  os_tsk_create(WorkMode_Task,0);                        
                       os_tsk_delete_self ();
                    break;
                    case 5:                       
                       LISTBOX_Delete(hListBox);
                   		 yushentskid  =   os_tsk_create(XUNHUANTIMEMode_Task,0);                        
                       os_tsk_delete_self ();
                    break;
									  case 6:                       
                       LISTBOX_Delete(hListBox);
                   		 yushentskid  =   os_tsk_create(Set_YUSHETime_Task,0);                        
                       os_tsk_delete_self ();
                    break;

                 }
                break;
				   case GUI_KEY_ESCAPE: 
                LISTBOX_Delete(hListBox);
						    hListBox = NULL;
                zhuantskid = os_tsk_create (zhuan1, 0);   
				        maintskid =  os_tsk_create (Main_Task, 0);   
                os_tsk_delete_self ();
				        break; 

				   case GUI_KEY_UP:
				       
				        break;
           case GUI_KEY_DOWN:
				       
				        break;
           default:  break;
				 }
				
			 }	 
	
}

__task void WorkMode_Task(void)
{
	
  int KEY =0;

   static const GUI_ConstString _aListBox[] = {
				"---全时模式",
        "---循环模式",	
        "---标准模式", 	
    		"---预设模式",
        "---母婴模式",	
        "---睡眠模式",         
				NULL
  };
  static LISTBOX_Handle hListBox;     

   hListBox = LISTBOX_Create(_aListBox, 0, 0, 480, 272, WM_CF_SHOW);    
	
	 SCROLLBAR_SetDefaultWidth(8); 
   SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);


   LISTBOX_SetBackColor(hListBox,0,GUI_BLACK);
   LISTBOX_SetTextColor(hListBox,0,GUI_WHITE);
  
   LISTBOX_SetBackColor(hListBox,1,GUI_WHITE);
   LISTBOX_SetTextColor(hListBox,1,GUI_BLACK);  
   

   LISTBOX_SetFont(hListBox,&GUI_Fontst24);
   
   
   WM_SetFocus(hListBox);  
   
   LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
   GUI_ClearKeyBuffer();

  while (1)
       {	
			    KEY=GUI_WaitKey();
		
				switch(KEY)
				{
				    case GUI_KEY_ENTER:
                 
                 		
                 HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR4,LISTBOX_GetSel(hListBox));  
                 LISTBOX_Delete(hListBox);
                 setsystemtimetskid =  os_tsk_create (MEUN_Task, 0);                 
                 os_tsk_delete_self ();

                break;
				   case GUI_KEY_ESCAPE: 
                LISTBOX_Delete(hListBox);						                  
				        setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self ();
				        break; 

				   case GUI_KEY_UP:
				       
				        break;
           case GUI_KEY_DOWN:
				       
				        break;
           default:  break;
				 }
				
			 }	 
	
}

void drawbiao(unsigned char index)
{
	  GUI_FillRect(110,1,470,4);
    GUI_FillRect(110,30,470,31);
	
	  GUI_FillRect(110,60,470,64);
    GUI_FillRect(110,90,470,91);
	
	  GUI_FillRect(110,120,470,124);
    GUI_FillRect(110,150,470,151);
	
	  GUI_FillRect(110,180,470,184);	
	  GUI_FillRect(110,210,470,211);	
	
	  GUI_FillRect(110,240,470,244);	
	
	   
	  GUI_FillRect(110,1,114,244); 
	  GUI_FillRect(470,1,474,244);
	  GUI_SetTextMode(GUI_TM_XOR);   
	  GUI_InvertRect(170,1,171,244); 
	  GUI_InvertRect(230,1,231,244);  
	  GUI_InvertRect(290,1,291,244);  
	  GUI_InvertRect(350,1,351,244);  
	  GUI_InvertRect(410,1,411,244);  
		
		GUI_DispStringAt("0",130,5);   GUI_DispStringAt("关闭",115,32); 
	  GUI_DispStringAt("1",190,5);   GUI_DispStringAt("关闭",175,32); 
		GUI_DispStringAt("2",250,5);   GUI_DispStringAt("关闭",235,32); 
		GUI_DispStringAt("3",310,5);   GUI_DispStringAt("关闭",295,32); 
		GUI_DispStringAt("4",370,5);   GUI_DispStringAt("关闭",355,32); 
		GUI_DispStringAt("5",430,5);   GUI_DispStringAt("关闭",415,32); 
		
		GUI_DispStringAt("6",130,65);  GUI_DispStringAt("运行",115,92);
	  GUI_DispStringAt("7",190,65);  GUI_DispStringAt("关闭",175,92);
		GUI_DispStringAt("8",250,65);  GUI_DispStringAt("运行",235,92);
		GUI_DispStringAt("9",310,65); GUI_DispStringAt("关闭",295,92);
		GUI_DispStringAt("10",370,65); GUI_DispStringAt("运行",355,92);
		GUI_DispStringAt("11",430,65); GUI_DispStringAt("关闭",415,92);

    GUI_DispStringAt("12",130,125);  GUI_DispStringAt("运行",115,152);
	  GUI_DispStringAt("13",190,125);  GUI_DispStringAt("运行",175,152);
		GUI_DispStringAt("14",250,125);  GUI_DispStringAt("运行",235,152);
		GUI_DispStringAt("15",310,125);  GUI_DispStringAt("运行",295,152);
		GUI_DispStringAt("16",370,125);  GUI_DispStringAt("运行",355,152);
		GUI_DispStringAt("17",430,125);  GUI_DispStringAt("运行",415,152);

    GUI_DispStringAt("18",130,185);  GUI_DispStringAt("运行",115,212); 
	  GUI_DispStringAt("19",190,185);  GUI_DispStringAt("运行",175,212);
		GUI_DispStringAt("20",250,185);  GUI_DispStringAt("运行",235,212);
		GUI_DispStringAt("21",310,185);  GUI_DispStringAt("运行",295,212);
		GUI_DispStringAt("22",370,185);  GUI_DispStringAt("运行",355,212);
		GUI_DispStringAt("23",430,185);  GUI_DispStringAt("关闭",415,212);
		
		
		
    GUI_DispStringAt("当天各时段运行图",200,248);

}
__task void Set_YUSHETime_Task(void)
{
	
  int KEY =0;

   static const GUI_ConstString _aListBox[] = {
				"星期一",
        "星期二",	
        "星期三", 	
    		"星期四",
        "星期五",	
        "星期六", 
        "星期日", 		 
				NULL
  };
  static LISTBOX_Handle hListBox;    

   os_mut_wait(mut_GLCD, 0xffff);	

   hListBox = LISTBOX_Create(_aListBox, 0, 0, 100, 272, WM_CF_SHOW);    
	
	 SCROLLBAR_SetDefaultWidth(8); 
   SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);


   LISTBOX_SetBackColor(hListBox,0,GUI_BLACK);
   LISTBOX_SetTextColor(hListBox,0,GUI_WHITE);
  
   LISTBOX_SetBackColor(hListBox,1,GUI_WHITE);
   LISTBOX_SetTextColor(hListBox,1,GUI_BLACK);  
   

   LISTBOX_SetFont(hListBox,&GUI_Fontst24);
   
   
   WM_SetFocus(hListBox);  
	
	 GUI_Clear();
   
   LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
   GUI_ClearKeyBuffer();

  while (1)
       {	
			    KEY=GUI_WaitKey();
		
				switch(KEY)
				{
				    case GUI_KEY_ENTER:                 
                 
                 
                break;
				   case GUI_KEY_ESCAPE: 
						    os_mut_release(mut_GLCD);
                LISTBOX_Delete(hListBox);						                  
				        setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self ();
				        break; 
				   case GUI_KEY_UP:
				        GUI_SetFont(&GUI_FontHT24);  
						    drawbiao(LISTBOX_GetSel(hListBox));
				        break;
           case GUI_KEY_DOWN:
						    GUI_SetFont(&GUI_FontHT24);  
						    drawbiao(LISTBOX_GetSel(hListBox));
				       
				        break;
           default:  break;
				 }
				
			 }	 
	
}
__task void XUNHUANTIMEMode_Task(void)
{
	
  int KEY =0;

   static const GUI_ConstString _aListBox[] = {
		    "---A:开60分钟关60分钟",                      
		    "---B:开90分钟关90分钟",	
		    "---C:开120分钟关120分钟",     	     
				NULL
  };
  static LISTBOX_Handle hListBox;     

   hListBox = LISTBOX_Create(_aListBox, 0, 0, 480, 200, WM_CF_SHOW);    
	
	 SCROLLBAR_SetDefaultWidth(8); 
   SCROLLBAR_CreateAttached(hListBox, SCROLLBAR_CF_VERTICAL);


   LISTBOX_SetBackColor(hListBox,0,GUI_BLACK);
   LISTBOX_SetTextColor(hListBox,0,GUI_WHITE);
  
   LISTBOX_SetBackColor(hListBox,1,GUI_WHITE);
   LISTBOX_SetTextColor(hListBox,1,GUI_BLACK);  
   

   LISTBOX_SetFont(hListBox,&GUI_Fontst24);
   
   
   WM_SetFocus(hListBox);  
   
   LISTBOX_SetOwnerDraw(hListBox, _OwnerDraw);
   GUI_ClearKeyBuffer();
   GUI_SetFont(&GUI_FontHT24);  

   if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==0)
	 {
     GUI_DispStringAt("当前状态: A-开60分钟关60分钟 ",65,220); 
   }
	 if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==1)
	 {
     GUI_DispStringAt("当前状态: B-开90分钟关90分钟 ",65,220); 
   }
	 if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR5)==2)
	 {
     GUI_DispStringAt("当前状态: C-开120分钟关120分钟 ",65,220); 
   }
	 
	 
  while (1)
       {	
			    KEY=GUI_WaitKey();
		
				switch(KEY)
				{
				    case GUI_KEY_ENTER:
                 
                 		
                 HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR5,LISTBOX_GetSel(hListBox));  
                 LISTBOX_Delete(hListBox);
                 setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);                 
                 os_tsk_delete_self ();

                break;
				   case GUI_KEY_ESCAPE: 
                LISTBOX_Delete(hListBox);						                  
				        setsystemtimetskid  =  os_tsk_create (MEUN_Task, 0);   
                os_tsk_delete_self ();
				        break; 

				   case GUI_KEY_UP:
				       
				        break;
           case GUI_KEY_DOWN:
				       
				        break;
           default:  break;
				 }
				
			 }	 
	
}

/*********************************************************************
*
*       _GetItemSizeX
*/
static int _GetItemSizeX(WM_HWIN hWin, int ItemIndex) {
  char acBuffer[100];
  int  DistX;
  LISTBOX_GetItemText(hWin, ItemIndex, acBuffer, sizeof(acBuffer));
  DistX = GUI_GetStringDistX(acBuffer);
  return DistX + 0 + 0;
}

/*********************************************************************
*
*       _GetItemSizeY
*/
static int _GetItemSizeY(WM_HWIN hWin, int ItemIndex) {
  int DistY;
  DistY = GUI_GetFontDistY() ;
  if (LISTBOX_GetMulti(hWin)) {
    if (LISTBOX_GetItemSel(hWin, ItemIndex)) {
      DistY += 0;
    }
  } else if (LISTBOX_GetSel(hWin) == ItemIndex) {
    DistY += 0;
  }
  return DistY;
}
 /*****************************************************************/
int _OwnerDraw(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) {
 WM_HWIN hWin;
  int Index;
  hWin     = pDrawItemInfo->hWin;
  Index    = pDrawItemInfo->ItemIndex;
  switch (pDrawItemInfo->Cmd) {
  case WIDGET_ITEM_GET_XSIZE:
    return _GetItemSizeX(hWin, Index);
  case WIDGET_ITEM_GET_YSIZE:
    return _GetItemSizeY(hWin, Index);
  case WIDGET_ITEM_DRAW:
    {
	  int Sel, YSize,FontDistY;
      int ColorIndex = 0;
      char acBuffer[100];
      const GUI_FONT* pOldFont = 0;
      GUI_COLOR aColor[4] = {0xffffff, GUI_GREEN, 0x0000000, GUI_GRAY};
      GUI_COLOR aBkColor[4] = {GUI_BLACK, GUI_BLUE, GUI_WHITE, 0xC0C0C0};

       Sel        = LISTBOX_GetSel(hWin);
      YSize       = _GetItemSizeY(hWin, Index);
	 
        
          if (pDrawItemInfo->ItemIndex == Sel) {
            ColorIndex = WM_HasFocus(pDrawItemInfo->hWin) ? 2 : 1;
          } else {
            ColorIndex = 0;
          }

	   /* Draw item */
      GUI_SetBkColor(aBkColor[ColorIndex]);
      GUI_SetColor  (aColor[ColorIndex]);
      LISTBOX_GetItemText(pDrawItemInfo->hWin, pDrawItemInfo->ItemIndex, acBuffer, sizeof(acBuffer));
      //GUI_SetTextMode(GUI_TM_NORMAL);
	    GUI_Clear();
      //if ((ColorIndex == 1) || (ColorIndex == 2)) {
        //pOldFont = GUI_SetFont(&GUI_Font13HB_ASCII);
      //}
      FontDistY  = GUI_GetFontDistY();
      GUI_DispStringAt(acBuffer, pDrawItemInfo->x0 + 0 + 8, pDrawItemInfo->y0 + (YSize - FontDistY) / 2);
      if (pOldFont) {
        GUI_SetFont(pOldFont);
      }
      GUI_DispCEOL();
	 
	 // pBm = MultiSel?IsSelected ? &bmSmilie1 : &bmSmilie0 : (pDrawItemInfo->ItemIndex == Sel) ? &bmSmilie1 : &bmSmilie0;
     // GUI_DrawBitmap(pBm, pDrawItemInfo->x0 + 7, pDrawItemInfo->y0+1 );//+ (YSize - pBm->YSize) / 2);
	}
	GUI_SetTextMode(GUI_TM_TRANS);
 //   GUI_DispDecAt(LISTBOX_GetSel(hWin),130,8,1); 
	break;
  default:
    return LISTBOX_OwnerDraw(pDrawItemInfo);

 }
  return 0;
}
