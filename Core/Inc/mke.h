#include "main.h"
#include "usb.h"
#include "gpio.h"
#include "usbd_def.h"
#include "spi.h"

#define OTG 0b00000100
#define ADB 0b00000000
#define MOUSE 0b00000001
#define KEYBOARD 0b00000010
#define CHECK 0b00001000

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
	uint8_t TARGET;
	mouseHID PACKET;
} mouseSPI;

typedef struct
{
	uint8_t TARGET;
	keyboardHID PACKET;
} keyboardSPI;

typedef struct
{
	uint8_t TARGET;
	uint8_t PACKET;
} switchSPI;

typedef struct
{
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
} bufferSPI;



void mouse_keyboard_test(void);
void mouse_keyboard_test_2(void);
void mke_main(void);
void mke_main_2(void);
int check_state(void);
void button_click(void);
void mousehid_copy(mouseHID *mousehid,bufferSPI *spi_receive_buffer);
void keyboardhid_copy(keyboardHID *mousehid,bufferSPI *spi_receive_buffer);
