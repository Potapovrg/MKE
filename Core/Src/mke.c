#include "mke.h"


extern USBD_HandleTypeDef hUsbDevice;



mouseHID mousehid = {0,0,0,0};
keyboardHID keyboardhid={0,0,0,0,0,0,0,0};


bufferSPI spi_receive_buffer;
bufferSPI spi_receive_buffer2;

uint8_t current_state = 0xff;
uint8_t target_state = ADB;

uint8_t spi_transmit_buffer=0;
uint8_t spi_transmit_buffer_1=0;
HAL_StatusTypeDef SPIstatus;

void mke_main(void)
{

	if (HAL_GPIO_ReadPin(CS_GPIO_Port,CS_Pin))
			{
				SPIstatus=HAL_SPI_Receive(&hspi1,&spi_receive_buffer,sizeof(spi_receive_buffer),10);

				if ((SPIstatus==HAL_OK)&&((spi_receive_buffer.target&CHECK)==0))
				{
					//spi_transmit_buffer=spi_receive_buffer.target;
					HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);

					HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
					target_state=spi_receive_buffer.target&OTG;
					current_state=check_state();

					if (target_state!=current_state)
					{
						if (target_state==OTG)
						{
							HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_SET);
							//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
							button_click();
						}

						else
							{
								//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
								HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_RESET);
								HAL_Delay(1000);
								button_click();
							}

					}

					spi_transmit_buffer=check_state();

					if (target_state==OTG)
					{
						if ((spi_receive_buffer.target&MOUSE)==MOUSE)
						{
							mousehid_copy(&mousehid,&spi_receive_buffer);
							if (USBD_HID_Mouse_SendReport(&hUsbDevice, &mousehid, sizeof (mousehid))==USBD_OK)
							{
								spi_transmit_buffer=spi_transmit_buffer|MOUSE;
							}
						}

						if ((spi_receive_buffer.target&KEYBOARD)==KEYBOARD)
						{
							keyboardhid_copy(&keyboardhid,&spi_receive_buffer);
							if (USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof(keyboardhid))==USBD_OK)
							{
								spi_transmit_buffer=spi_transmit_buffer|KEYBOARD;
							}
						}
					}

				}
			}
}


void mke_main_2(void)
{

	if (HAL_GPIO_ReadPin(CS_GPIO_Port,CS_Pin))
			{
				SPIstatus=HAL_SPI_Receive(&hspi1,&spi_receive_buffer,sizeof(spi_receive_buffer),10);
				if (SPIstatus==HAL_OK)
				//if (HAL_SPI_Receive(&hspi1,&spi_receive_buffer,sizeof(spi_receive_buffer),100)==HAL_OK)
				{
					//spi_transmit_buffer=0xff;
					//HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);
					HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
					target_state=spi_receive_buffer.target&OTG;
					current_state=check_state();

					if (target_state!=current_state)
					{
						if (target_state==OTG)
						{
							HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_SET);
							//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
							button_click();
						}

						else
							{
								//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
								HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_RESET);
								HAL_Delay(1000);
								button_click();
							}

					}

					spi_transmit_buffer=check_state();

					if (target_state==OTG)
					{
						if ((spi_receive_buffer.target&MOUSE)==MOUSE)
						{
							mousehid_copy(&mousehid,&spi_receive_buffer);
							if (USBD_HID_Mouse_SendReport(&hUsbDevice, &mousehid, sizeof (mousehid))==USBD_OK)
							{
								spi_transmit_buffer=spi_transmit_buffer|MOUSE;
							}
						}

						if ((spi_receive_buffer.target&KEYBOARD)==KEYBOARD)
						{
							keyboardhid_copy(&keyboardhid,&spi_receive_buffer);
							if (USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof(keyboardhid))==USBD_OK)
							{
								spi_transmit_buffer=spi_transmit_buffer|KEYBOARD;
							}
						}
					}

					//spi_transmit_buffer=0xff;
					//HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);

					if ((spi_receive_buffer.target&CHECK)==CHECK)
					{
						//spi_transmit_buffer=spi_transmit_buffer|CHECK;
						spi_transmit_buffer=spi_receive_buffer.target;
						HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);
					}
				}
			}
}


int check_state(void)
{
	if (HAL_GPIO_ReadPin(SWITCH_FEEDBACK_GPIO_Port,SWITCH_FEEDBACK_Pin)) return OTG;
	else return ADB;
}

void mousehid_copy(mouseHID *mousehid,bufferSPI *spi_receive_buffer)
{
	mousehid->button=spi_receive_buffer->button;
	mousehid->mouse_x=spi_receive_buffer->mouse_x;
	mousehid->mouse_y=spi_receive_buffer->mouse_y;
	mousehid->wheel=spi_receive_buffer->wheel;
}

void keyboardhid_copy(keyboardHID *keyboardhid,bufferSPI *spi_receive_buffer)
{
	keyboardhid->MODIFIER=spi_receive_buffer->modifier;
	keyboardhid->RESERVED=spi_receive_buffer->reserved;
	keyboardhid->KEYCODE1=spi_receive_buffer->keycode1;
	keyboardhid->KEYCODE2=spi_receive_buffer->keycode2;
	keyboardhid->KEYCODE3=spi_receive_buffer->keycode3;
	keyboardhid->KEYCODE4=spi_receive_buffer->keycode4;
	keyboardhid->KEYCODE5=spi_receive_buffer->keycode5;
	keyboardhid->KEYCODE6=spi_receive_buffer->keycode6;
}

void button_click()
{
	HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_SET);
	HAL_Delay(100);
}


void mouse_keyboard_test_2(void)
{
	keyboardHID print_ar[]=
	{
			{0b00000100,0,0x62,0x00,0x00,0x00,0,0},
			{0b00000100,0,0x00,0x00,0x00,0x00,0,0},
			{0b00000100,0,0x5A,0x00,0x00,0x00,0,0},
			{0b00000100,0,0x00,0x00,0x00,0x00,0,0},
			{0b00000100,0,0x5A,0x00,0x00,0x00,0,0},
			{0b00000100,0,0x00,0x00,0x00,0x00,0,0},
			{0b00000100,0,0x5D,0x00,0x00,0x00,0,0},
			{0b00000000,0,0x00,0x00,0x00,0x00,0,0},
	};

	keyboardHID print_al[]=
		{
				{0b00000100,0,0x62,0x00,0x00,0x00,0,0},
				{0b00000100,0,0x00,0x00,0x00,0x00,0,0},
				{0b00000100,0,0x5A,0x00,0x00,0x00,0,0},
				{0b00000100,0,0x00,0x00,0x00,0x00,0,0},
				{0b00000100,0,0x5C,0x00,0x00,0x00,0,0},
				{0b00000100,0,0x00,0x00,0x00,0x00,0,0},
				{0b00000100,0,0x5C,0x00,0x00,0x00,0,0},
				{0b00000000,0,0x00,0x00,0x00,0x00,0,0},
		};

	keyboardHID print_al1[]=
		{
				{0,0,0x10,0x0E,0x08,0x28,0,0},
				{0,0,0,0,0,0,0,0}

		};

	uint8_t kbdRelease[8] = {0,0,0,0,0,0,0,0};

	mouseHID mousemove[4]={{0,10,0,0},{0,0,-10,0},{0,-10,0,0},{0,0,10,0}};

	 for (int i=0; i < 8; i++)
		 		  {
		 	 	 	 USBD_HID_Keybaord_SendReport(&hUsbDevice, &print_al[i], sizeof(print_al[i]));
		 	 	 	 HAL_Delay(50);
		 	 		 //USBD_HID_Keybaord_SendReport(&hUsbDevice, &kbdRelease, 8);
		 		  }

	 for (int i=0; i < 8; i++)
	 		 		  {
	 		 	 	 	 USBD_HID_Keybaord_SendReport(&hUsbDevice, &print_ar[i], sizeof(print_ar[i]));
	 		 	 	 	 HAL_Delay(50);
	 		 	 		 //USBD_HID_Keybaord_SendReport(&hUsbDevice, &kbdRelease, 8);
	 		 		  }

	 for (int y=0; y<4; y++)
	 	 	  {

	 	 		  for (int i=0; i < 30; i++)
	 	 		  {
	 	 			  HAL_Delay(50);
	 	 			  mousehid=mousemove[y];
	 	 			  USBD_HID_Mouse_SendReport(&hUsbDevice, &mousehid, sizeof (mousehid));
	 	 		  }

	 	 	  }

}
