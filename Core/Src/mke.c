#include "mke.h"
#include"exec_time.h"


extern USBD_HandleTypeDef hUsbDevice;

#ifdef EXEC_TIME
float exec_time;
#endif

float exec_time;
mouseHID mousehid = {0,0,0,0};
mouseHID customhid = {0x30,0,0,0}; //CONSUMER_POWER	= 0x30, MEDIA_VOLUME_UP	= 0xE9, MEDIA_VOLUME_DOWN	= 0xEA
mouseHID customhid_r = {0,0,0,0};
keyboardHID keyboardhid={0,0,0,0,0,0,0,0};
consumerHID consumerhid = {0,0,0,0};
click_statusTypeDef click_status;

bufferSPI spi_receive_buffer;
bufferSPI spi_transmit_buffer_crc;

uint8_t current_state = 0xff;
uint8_t target_state = ADB;
uint8_t crc8=0;
uint8_t btn_state = 0;
mke_stateTypeDef mke_state = ADB_S;

uint8_t spi_transmit_buffer=0;
uint8_t spi_transmit_buffer_1=0;
HAL_StatusTypeDef SPIstatus;



#ifdef ERR_RESET
uint8_t error_counter=0;
uint8_t crc_error_counter=0;
uint8_t timeout_error_counter=0;
#endif

void mke_init(void)
{
	/*
	if (check_state()!=ADB)
	{
		button_click();
	}
    */

	//spi_transmit_buffer=check_state();
	adb_state();
	current_state=ADB;
	spi_transmit_buffer=current_state;
	spi_transmit_buffer_crc.target=spi_transmit_buffer;
	crc8=CRC_Calculate_software(&spi_transmit_buffer,1);
	spi_transmit_buffer_crc.button=crc8;

#ifdef SPI_STOP
	spi_stop();
#endif


	while(1)
	{
		//mke_main_2();
		//mke_main();
		if (HAL_GPIO_ReadPin(CS_GPIO_Port,CS_Pin)) mke_main();


	}
}

void mke_main_2(void)
{
	if (!HAL_GPIO_ReadPin(BTN_GPIO_Port,BTN_Pin)&(btn_state==0))
		{
		//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
		if (mke_state==ADB_S)
		{
			//otg_state();
			//mke_state=OTG_S;
			chrg_state();
			mke_state=CHRG_S;
		}
		else if (mke_state==OTG_S)
		{
			adb_state();
			mke_state=ADB_S;
		}
		else if (mke_state=CHRG_S)
		{
			adb_state();
			mke_state=ADB_S;
		}

		btn_state=1;
		HAL_Delay(1000);

		}
	else btn_state=0;
}

void chrg_state(void)
{
	HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OTG_HUB_GPIO_Port,OTG_HUB_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(CHRG_ON_GPIO_Port,CHRG_ON_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEL_HUB_GPIO_Port,SEL_HUB_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEL_PC_GPIO_Port,SEL_PC_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
}

void otg_state(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(CHRG_ON_GPIO_Port,CHRG_ON_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(OTG_HUB_GPIO_Port,OTG_HUB_Pin,GPIO_PIN_SET);
	//HAL_Delay(50);
	HAL_GPIO_WritePin(SEL_PC_GPIO_Port,SEL_PC_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SEL_HUB_GPIO_Port,SEL_HUB_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
}

void adb_state(void)
{
	HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(OTG_HUB_GPIO_Port,OTG_HUB_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(CHRG_ON_GPIO_Port,CHRG_ON_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEL_HUB_GPIO_Port,SEL_HUB_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SEL_PC_GPIO_Port,SEL_PC_Pin,GPIO_PIN_SET);
	//HAL_Delay(1000);
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);

}

void mke_main(void)
{
#ifdef SPI_STOP
	spi_start();
#endif
	if (HAL_SPI_TransmitReceive(&hspi1,&spi_transmit_buffer_crc,&spi_receive_buffer,sizeof(spi_receive_buffer),100)==HAL_OK)
	{
		crc8=CRC_Calculate_software(&spi_receive_buffer,(sizeof(spi_receive_buffer)-1));
		if (spi_receive_buffer.crc==CRC_Calculate_software(&spi_receive_buffer,(sizeof(spi_receive_buffer)-1)))
		{
			error_counter=0;
			if ((spi_receive_buffer.target&RESET)==RESET) NVIC_SystemReset();
			else if ((spi_receive_buffer.target&CHECK)==CHECK) spi_transmit_buffer=check_state();
			else if ((spi_receive_buffer.target&ADB)==ADB)
			{
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				target_state=ADB;
				switch_state();
			}
			else if ((spi_receive_buffer.target&OTG)==OTG)
			{
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				target_state=OTG;
				switch_state();
				send_to_usb();
			}

		}
		else
		{
				spi_transmit_buffer=CRC_ERROR;
				//crc_error_counter++;
				error_counter++;
		}
	}
	else
	{
		spi_transmit_buffer=SPI_ERROR;
		//timeout_error_counter++;
		error_counter++;
	}

	if (error_counter>2)
	{
		//NVIC_SystemReset();
		force_spi_reset();
	}

	spi_transmit_buffer_crc.target=spi_transmit_buffer;
	//crc8=0xFA;
	crc8=CRC_Calculate_software(&spi_transmit_buffer,1);
	spi_transmit_buffer_crc.button=crc8;

#ifdef	SPI_STOP
	spi_stop();
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

void consumerhid_copy(consumerHID *consumerhid,bufferSPI *spi_receive_buffer)
{
	consumerhid->KEYCODE1=spi_receive_buffer->c_keycode1;
	consumerhid->KEYCODE2=spi_receive_buffer->c_keycode2;
	consumerhid->KEYCODE3=spi_receive_buffer->c_keycode3;
	consumerhid->KEYCODE4=spi_receive_buffer->c_keycode4;
}

void button_click()
{
	HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_RESET);
	HAL_Delay(45);
	HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_SET);
	//HAL_Delay(100);
}

void button_click_IT()
{
	if (click_status==CLICK_OK)
	{
		HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_RESET);
		click_status=CLICK_BUSY;
		//TIM2->CNT = 0;
		__HAL_TIM_CLEAR_IT(&htim2 ,TIM_IT_UPDATE);
		HAL_TIM_Base_Start_IT(&htim2);

	}
}

void switch_state(void)
{
	//target_state=spi_receive_buffer.target&OTG;
	//current_state=check_state();
	if (target_state!=current_state)
	{
		if (target_state==ADB)
		{
			/*
			HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(OTG_HUB_GPIO_Port,OTG_HUB_Pin,GPIO_PIN_RESET);
			//button_click();
			button_click_IT();
			*/
			adb_state();
			current_state=ADB;
		}

		else if (target_state==OTG)
		{
			/*
			HAL_GPIO_WritePin(OTG_GPIO_Port,OTG_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(OTG_HUB_GPIO_Port,OTG_HUB_Pin,GPIO_PIN_SET);
			//HAL_Delay(1000);
			//button_click();
			button_click_IT();
			*/
			otg_state();
			current_state=OTG;
		}

	}
	//spi_transmit_buffer=check_state();
	spi_transmit_buffer=current_state;
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
				spi_transmit_buffer=spi_transmit_buffer|KEYBOARD;
			}
		}

		if ((spi_receive_buffer.target&CONSUMER)==CONSUMER)
				{
					//spi_transmit_buffer=spi_transmit_buffer|KEYBOARD; //test
					consumerhid_copy(&consumerhid,&spi_receive_buffer);
					if (USBD_CUSTOM_HID_SendReport(&hUsbDevice, &consumerhid, sizeof(consumerhid))==USBD_OK)
					{
						spi_transmit_buffer=spi_transmit_buffer|CONSUMER;
					}
				}


	}

}

uint8_t CRC_Calculate_software(uint8_t *Data, uint8_t Buffer_lenght)
{
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

void force_spi_reset(void)
{
	__HAL_RCC_SPI1_FORCE_RESET();
	while(hspi1.Instance->SR & SPI_SR_BSY);
	__HAL_RCC_SPI1_RELEASE_RESET();
	while(hspi1.Instance->SR & SPI_SR_BSY);
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
		{
			//NVIC_SystemReset();
		}
}

void spi_stop(void)
{
	__HAL_RCC_SPI1_FORCE_RESET();
	while(hspi1.Instance->SR & SPI_SR_BSY);
}

void spi_start(void)
{
	__HAL_RCC_SPI1_RELEASE_RESET();
	while(hspi1.Instance->SR & SPI_SR_BSY);
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
			{
				//NVIC_SystemReset();
			}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	  HAL_GPIO_WritePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin,GPIO_PIN_SET);
	  //HAL_GPIO_TogglePin(SWITCH_CONTROL_GPIO_Port,SWITCH_CONTROL_Pin);
	  click_status=CLICK_OK;
	  spi_transmit_buffer=check_state();
	  spi_transmit_buffer_crc.target=spi_transmit_buffer;
	  crc8=CRC_Calculate_software(&spi_transmit_buffer,1);
	  spi_transmit_buffer_crc.button=crc8;
	  HAL_TIM_Base_Stop_IT(&htim2);
}

