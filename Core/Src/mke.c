#include "mke.h"


extern USBD_HandleTypeDef hUsbDevice;

uint8_t kbdPress[8] = {0,0,0x10,0x0E,0x08,0x28,0,0};
uint8_t kbdRelease[8] = {0,0,0,0,0,0,0,0};


mouseHID mousehid = {0,0,0,0};
mouseHID mousemove[4]={{0,10,0,0},{0,0,-10,0},{0,-10,0,0},{0,0,10,0}};

void mouse_keyboard_test(void)
{
	  for (int y=0; y<4; y++)
	 	  {

	 		  for (int i=0; i < 30; i++)
	 		  {
	 			  HAL_Delay(50);
	 			  mousehid=mousemove[y];
	 			  USBD_HID_Mouse_SendReport(&hUsbDevice, &mousehid, sizeof (mousehid));
	 		  }
	 		  USBD_HID_Keybaord_SendReport(&hUsbDevice, &kbdPress, 8);
	 		  HAL_Delay(50);
	 		  USBD_HID_Keybaord_SendReport(&hUsbDevice, &kbdRelease, 8);
	 	  }
}
