#include "mke.h"
#include"exec_time.h"


extern USBD_HandleTypeDef hUsbDevice;

#ifdef EXEC_TIME
float exec_time;
#endif

mouseHID mousehid = {0,0,0,0};
keyboardHID keyboardhid={0,0,0,0,0,0,0,0};


bufferSPI spi_receive_buffer;
bufferSPI spi_receive_buffer2;

uint8_t current_state = 0xff;
uint8_t target_state = ADB;
uint8_t crc8=0;

uint8_t spi_transmit_buffer=0;
uint8_t spi_transmit_buffer_1=0;
HAL_StatusTypeDef SPIstatus;

void mke_main(void)
{
#ifdef EXEC_TIME
	start_exec_time();
#endif

	if (HAL_GPIO_ReadPin(CS_GPIO_Port,CS_Pin))
			{
				if (HAL_SPI_Receive(&hspi1,&spi_receive_buffer,sizeof(spi_receive_buffer),100)==HAL_OK)
				{
					crc8=CRC_Calculate_software(&spi_receive_buffer,(sizeof(spi_receive_buffer)-1));
					if (spi_receive_buffer.crc==CRC_Calculate_software(&spi_receive_buffer,(sizeof(spi_receive_buffer)-1)))
					{

						if ((spi_receive_buffer.target&CHECK)!=CHECK)
						{
							HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
							switch_state();
#ifdef EXEC_TIME_USB
							start_exec_time();
#endif
							send_to_usb();
							//spi_transmit_buffer=spi_receive_buffer.target; //test
							HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);
#ifdef EXEC_TIME_USB
							exec_time=stop_exec_time_float();
#endif
						}

						else
						{
							spi_transmit_buffer=0x00;
							HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);
						}
					}

					else
					{
						spi_transmit_buffer=0xFF;
						HAL_SPI_Transmit(&hspi1,&spi_transmit_buffer,sizeof(spi_transmit_buffer),10);
						//HAL_Delay(10);

					}
					//spi_transmit_buffer=0;
				}
			}
#ifdef EXEC_TIME
	exec_time=stop_exec_time_float();
#endif
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
	HAL_Delay(45);
	HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_SET);
	//HAL_Delay(100);
}

void switch_state(void)
{
	target_state=spi_receive_buffer.target&OTG;
	current_state=check_state();
	if (target_state!=current_state)
	{
		if (target_state==OTG)
		{
			HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_SET);
			button_click();
		}

		else
		{
			HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_RESET);
			//HAL_Delay(1000);
			button_click();
		}

	}
	spi_transmit_buffer=check_state();
	//spi_transmit_buffer=spi_transmit_buffer|OTG;
}

void send_to_usb(void)
{
	if (target_state==OTG)
	{
		if ((spi_receive_buffer.target&MOUSE)==MOUSE)
		{
			//spi_transmit_buffer=spi_transmit_buffer|MOUSE; //test
			mousehid_copy(&mousehid,&spi_receive_buffer);
			if (USBD_HID_Mouse_SendReport(&hUsbDevice, &mousehid, sizeof (mousehid))==USBD_OK)
			{
				spi_transmit_buffer=spi_transmit_buffer|MOUSE;
			}
		}

		if ((spi_receive_buffer.target&KEYBOARD)==KEYBOARD)
		{
			//spi_transmit_buffer=spi_transmit_buffer|KEYBOARD; //test
			keyboardhid_copy(&keyboardhid,&spi_receive_buffer);
			if (USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof(keyboardhid))==USBD_OK)
			{
#ifndef UNPRESS_ENABLE
				spi_transmit_buffer=spi_transmit_buffer|KEYBOARD;
#endif

#ifdef UNPRESS_ENABLE
				keyboardHID keyboardhid={0,0,0,0,0,0,0,0};
				HAL_Delay(50);
				if (USBD_HID_Keybaord_SendReport(&hUsbDevice, &keyboardhid, sizeof(keyboardhid))==USBD_OK)
				{
					spi_transmit_buffer=spi_transmit_buffer|KEYBOARD;
				}
#endif
			}
		}
	}

}

uint8_t CRC_Calculate_software(uint8_t *Data, uint8_t Buffer_lenght) {
	uint8_t CRC8 = 0x00;
	uint8_t size = (sizeof(*Data));
	while (Buffer_lenght--) {
		CRC8 ^= *Data++;
		for (uint8_t i = 0; i < (sizeof(*Data) * 8); i++) {
			if (CRC8 & 0x80) {
				CRC8 = (CRC8 << 1u) ^ 07;
			} else {
				CRC8 = (CRC8 << 1u);
			}
		}
	}
	return CRC8;
}


