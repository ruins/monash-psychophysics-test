/*
 * sys_vga_functions.c
 *
 *  Created on: Apr 17, 2011
 *      Author: cj
 */
#include <io.h>
/* Header Files for VGA interface */
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
/* Header Files for SD Card Interface*/
#include "altera_up_sd_card_avalon_interface.h"//system initiation is incorrect, must change alt_sys_init.c file and rename include section
/* Header Files for Nios Microprocessor Interface with SOPC IP cores */
#include "system.h"
#include "sys/alt_irq.h"//??
#include "sys/alt_sys_init.h"//??
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
/* Header Files for common C functions */
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
//#include <utime.h>
//#include <stdlib.h>
#include <unistd.h>
/* Header Files For Implementing uCOSii Kernal*/
#include "includes.h"
/* Header Files For System Tailored Functions*/
#include "sys_functions.h"
#include "sys_vga_functions.h"
#include "sys_sd_functions.h"
void image_flash()
{

	alt_up_char_buffer_clear(char_buffer_dev);
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	//usleep(500000 + rand()%1000000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(40000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(400000);

}


