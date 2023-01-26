#include "mke.h"


extern USBD_HandleTypeDef hUsbDevice;



mouseHID mousehid = {0,0,0,0};

keyboardHID keyboardhid={0,0,0,0,0,0,0,0};


uint8_t spi_receive_buffer[9]= {0,0,0,0,0,0,0,0,0};

bufferSPI spi_receive_buffer2;

void mke_main(void)
{
	HAL_SPI_Receive(&hspi1,&spi_receive_buffer,sizeof(spi_receive_buffer),100);
	if (spi_receive_buffer[0]==0x01)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	}

	else if (spi_receive_buffer[0]==0x02)
	{
		keyboardhid.MODIFIER=spi_receive_buffer[1];
		keyboardhid.RESERVED=spi_receive_buffer[2];
		keyboardhid.KEYCODE1=spi_receive_buffer[3];
		keyboardhid.KEYCODE2=spi_receive_buffer[4];
		keyboardhid.KEYCODE3=spi_receive_buffer[5];
		keyboardhid.KEYCODE4=spi_receive_buffer[6];
		keyboardhid.KEYCODE5=spi_receive_buffer[7];
		keyboardhid.KEYCODE6=spi_receive_buffer[8];

		USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof(keyboardhid));
	}

	else if (spi_receive_buffer[0]==0x03)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	}

	for (int i = 0; spi_receive_buffer[i] != NULL; i++);
}





void mke_main_2(void)
{
	HAL_SPI_Receive(&hspi1,&spi_receive_buffer2,sizeof(spi_receive_buffer2),100);

	if ((spi_receive_buffer2.target&0b10000000)==0b10000000)
	{
		mousehid.button=spi_receive_buffer2.button;
		mousehid.mouse_x=spi_receive_buffer2.mouse_x;
		mousehid.mouse_y=spi_receive_buffer2.mouse_y;
		mousehid.wheel=spi_receive_buffer2.wheel;

		USBD_HID_Mouse_SendReport(&hUsbDevice, &mousehid, sizeof (mousehid));
	}

	if ((spi_receive_buffer2.target&0b01000000)==0b01000000)
	{
		keyboardhid.MODIFIER=spi_receive_buffer2.modifier;
		keyboardhid.RESERVED=spi_receive_buffer2.reserved;
		keyboardhid.KEYCODE1=spi_receive_buffer2.keycode1;
		keyboardhid.KEYCODE2=spi_receive_buffer2.keycode2;
		keyboardhid.KEYCODE3=spi_receive_buffer2.keycode3;
		keyboardhid.KEYCODE4=spi_receive_buffer2.keycode4;
		keyboardhid.KEYCODE5=spi_receive_buffer2.keycode5;
		keyboardhid.KEYCODE6=spi_receive_buffer2.keycode6;

		USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof(keyboardhid));
	}

	if ((spi_receive_buffer2.target&0b00100000)==0b00100000)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
	}

	spi_receive_buffer2.target=0;
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
