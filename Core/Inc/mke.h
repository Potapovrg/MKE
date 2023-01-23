#include "main.h"
#include "usb.h"
#include "gpio.h"
#include "usbd_def.h"

void SystemClock_Config(void);

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

