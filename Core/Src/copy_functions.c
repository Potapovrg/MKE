#include "copy_functions.h"

void mem_buffer_copy(const bufferSPI* spi_receive_buffer, bufferMEM* mem_buffer)
{
    mem_buffer->target = spi_receive_buffer->target;
    mem_buffer->button = spi_receive_buffer->button;
    mem_buffer->mouse_x = spi_receive_buffer->mouse_x;
    mem_buffer->mouse_y = spi_receive_buffer->mouse_y;
    mem_buffer->wheel = spi_receive_buffer->wheel;
    mem_buffer->modifier = spi_receive_buffer->modifier;
    mem_buffer->reserved = spi_receive_buffer->reserved;
    mem_buffer->keycode1 = spi_receive_buffer->keycode1;
    mem_buffer->keycode2 = spi_receive_buffer->keycode2;
    mem_buffer->keycode3 = spi_receive_buffer->keycode3;
    mem_buffer->keycode4 = spi_receive_buffer->keycode4;
    mem_buffer->keycode5 = spi_receive_buffer->keycode5;
    mem_buffer->keycode6 = spi_receive_buffer->keycode6;
    mem_buffer->c_keycode1 = spi_receive_buffer->c_keycode1;
    mem_buffer->c_keycode2 = spi_receive_buffer->c_keycode2;
    mem_buffer->c_keycode3 = spi_receive_buffer->c_keycode3;
    mem_buffer->c_keycode4 = spi_receive_buffer->c_keycode4;
    mem_buffer->delay_hb = spi_receive_buffer->delay_hb;
    mem_buffer->delay_lb = spi_receive_buffer->delay_lb;
    mem_buffer->result = 0;
}
