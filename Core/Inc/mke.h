#include "main.h"
#include "usb.h"
#include "gpio.h"
#include "usbd_def.h"
#include "spi.h"
#include "iwdg.h"
#include "tim.h"

#define	SPI_STOP

//target defines
#define MOUSE    0b00000001
#define KEYBOARD 0b00000010
#define CONSUMER 0b00000100
#define OTG      0b00001000
#define ADB      0b00010000
#define CHECK    0b00100000
#define RESET    0b01000000

//control defines
#define BYPASS   0b00000001
#define WRITE 	 0b00000010
#define READ     0b00000100
#define STOP     0b00001000



//Errors

#define SPI_ERROR 0xF0
#define CRC_ERROR 0xF1



//#define UNPRESS_ENABLE
#define ERR_RESET
//#define EXEC_TIME
//#define EXEC_TIME_USB
typedef struct
{
	uint8_t button;
	int8_t mouse_x;
	int8_t mouse_y;
	int8_t wheel;
} mouseHID;

typedef struct
{
	uint8_t MODIFIER;
	uint8_t RESERVED;
	uint8_t KEYCODE1;
	uint8_t KEYCODE2;
	uint8_t KEYCODE3;
	uint8_t KEYCODE4;
	uint8_t KEYCODE5;
	uint8_t KEYCODE6;
} keyboardHID;

typedef struct
{
	uint16_t KEYCODE1;
	uint16_t KEYCODE2;
	uint16_t KEYCODE3;
	uint16_t KEYCODE4;
} consumerHID;

typedef struct
{
	uint8_t control;
	uint8_t target;
	uint8_t button;
	int8_t mouse_x;
	int8_t mouse_y;
	int8_t wheel;
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode1;
	uint8_t keycode2;
	uint8_t keycode3;
	uint8_t keycode4;
	uint8_t keycode5;
	uint8_t keycode6;
	uint8_t c_keycode1;
	uint8_t c_keycode2;
	uint8_t c_keycode3;
	uint8_t c_keycode4;
	uint8_t delay_hb;
	uint8_t delay_lb;
	uint8_t crc;
} bufferSPI;

typedef struct
{
	uint16_t delay;
	uint8_t target;
	uint8_t button;
	int8_t mouse_x;
	int8_t mouse_y;
	int8_t wheel;
	uint8_t modifier;
	uint8_t reserved;
	uint8_t keycode1;
	uint8_t keycode2;
	uint8_t keycode3;
	uint8_t keycode4;
	uint8_t keycode5;
	uint8_t keycode6;
	uint8_t c_keycode1;
	uint8_t c_keycode2;
	uint8_t c_keycode3;
	uint8_t c_keycode4;
	uint8_t result;
} bufferMEM;

typedef enum
{
	CLICK_OK=0,
	CLICK_BUSY=1

} click_statusTypeDef;

click_statusTypeDef click_status;

void mouse_keyboard_test(void);
void mouse_keyboard_test_2(void);
void mke_init(void);
void mke_main(void);
void mke_main_2(void);
int check_state(void);
void switch_state(void);
void write_to_transmit_buffer(void);
void button_click_IT(void);
void send_to_usb(void);
void bypass_mode(void);
void write_mode(void);
void read_mode(void);

void mousehid_copy(mouseHID *mousehid,bufferSPI *spi_receive_buffer);
void keyboardhid_copy(keyboardHID *keyboardhid,bufferSPI *spi_receive_buffer);
void consumerhid_copy(consumerHID *consumerhid,bufferSPI *spi_receive_buffer);

void mousehid_copy_mem(mouseHID *mousehid,bufferMEM *buffermem);
void keyboardhid_copy_mem(keyboardHID *keyboardhid,bufferMEM *buffermem);
void consumerhid_copy_mem(consumerHID *consumerhid,bufferMEM *buffermem);

uint8_t CRC_Calculate_software(uint8_t *Data, uint8_t Buffer_lenght);
void force_spi_reset(void);
void spi_stop(void);
void spi_start(void);
