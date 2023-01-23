#include "mke.h"


extern USBD_HandleTypeDef hUsbDevice;



mouseHID mousehid = {0,0,0,0};



uint8_t spi_receive_buffer[9]= {0,0,0,0,0,0,0,0,0};


void mke_main(void)
{
	HAL_SPI_Receive(&hspi1,&spi_receive_buffer,sizeof(spi_receive_buffer),100);
	if (spi_receive_buffer[0]==0x01)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	}
}



void mouse_keyboard_test(void)
{
	uint8_t kbdPress[8] = {0,0,0x10,0x0E,0x08,0x28,0,0};
	uint8_t kbdRelease[8] = {0,0,0,0,0,0,0,0};
	mouseHID mousemove[4]={{0,10,0,0},{0,0,-10,0},{0,-10,0,0},{0,0,10,0}};
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
