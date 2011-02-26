#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_sd_card_avalon_interface.h"//system initiation is incorrect, must change alt_sys_init.c file and rename include section

#include "system.h"
#include "sys/alt_irq.h"//??
#include "sys/alt_sys_init.h"//??
#include "alt_types.h"

#include "stdio.h"
void draw_big_A(alt_up_pixel_buffer_dma_dev *);

int main(void)
{
	//Lets get started
	printf("hello world!\nNios is now starting up.......\n\n");

	//SD section initialisation
	char buffer_name[20];
	short int handler;
	short int read;

	//VGA Section initialisation
	alt_up_pixel_buffer_dma_dev *pixel_buffer_dev;
	alt_up_char_buffer_dev *char_buffer_dev;
	int xD=0;
	int yD=0;

	/* create a message to be displayed on the VGA display */
	char text_top_row[40] = "Hello\0";
	char text_bottom_row[40] = "Testing\0";

	/* initialize the pixel buffer HAL */
	pixel_buffer_dev = alt_up_pixel_buffer_dma_open_dev ("/dev/Pixel_Buffer_DMA");
	/* clear the graphics screen */

	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 1);
/*
	if (alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev) == 0)
	{
		printf("backbuffer swap success\n");
		printf("check status \n");
	}
	else printf ("backbuffer swap unsuccessfull");

	while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer_dev) == 1) printf("..");
*/
	/* output text message in the middle of the VGA monitor */

	char_buffer_dev = alt_up_char_buffer_open_dev ("/dev/Char_Buffer_with_DMA");
	alt_up_char_buffer_clear(char_buffer_dev);
//	alt_up_char_buffer_string (char_buffer_dev, text_top_row, 35, 29);
//	alt_up_char_buffer_string (char_buffer_dev, text_bottom_row, 35, 30);

	/* now draw a background box for the text */
//	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 34*8, 28*8, 50*8, 32*8, 200, 0);

	//printf("hello world\n");
/*
while(1)
{
    for(xcount=0;xcount<640;xcount++)
    	alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, 255, xcount, 100);
	//alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, 100, 300, 200);
}
*/
	/* now draw a big A for ALTERA */
//	draw_big_A (pixel_buffer_dev);

	//SD card section
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

		if (device_reference != NULL)
		{
			while(1)
			{
				if ((connected == 0) && (alt_up_sd_card_is_Present()))
				{
					printf("Card connected.\n");
					//usleep(5000000);
					if (alt_up_sd_card_is_FAT16())
					{
						printf("FAT16 file system detected.\n");
						break;
					}
					else 							printf("Unknown file system.\n");
					connected = 1;
				}
				else if ((connected == 1) && (alt_up_sd_card_is_Present() == false))
				{
					printf("Card disconnected.\n");
					connected = 0;
				}
				//printf("polling, connected %d\n", connected);
			}
		}


		handler = alt_up_sd_card_find_first("", buffer_name);
		printf("%d,  %s \n", handler, buffer_name);

		while ((handler = alt_up_sd_card_find_next(buffer_name)) != -1) printf("%d,  %s \n", handler, buffer_name);

		handler = alt_up_sd_card_fopen("THESIS.TXT", false);

		while ((read = alt_up_sd_card_read(handler)) != -1)
		{
			if(yD < 479)
			{
				alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, read, xD , yD);
				yD++;
			}
			else
			{

				yD=0;
				xD++;
				alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, read, xD , yD);
			}

		}

//		while ((read = alt_up_sd_card_read(handler)) != -1){}
		printf("read complete\n");
		alt_up_sd_card_fclose(handler);

return 0;
}


/* draws a big letter A on the screen */
void draw_big_A(alt_up_pixel_buffer_dma_dev *pixel_buffer_dev )
{
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 10, 88, 44, 10, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 44, 10, 72, 10, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 72, 10, 106, 88, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 106, 88, 81, 88, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 81, 88, 75, 77, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 75, 77, 41, 77, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 41, 77, 35, 88, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 35, 88, 10, 88, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 47, 60, 58, 32, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 58, 32, 69, 60, 0xff, 0);
	alt_up_pixel_buffer_dma_draw_line(pixel_buffer_dev, 69, 60, 47, 60, 0xff, 0);
}

