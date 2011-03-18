/*
 * sys_functions.c
 *
 *  Created on: Mar 18, 2011
 *      Author: CJ
 *      Revision: 1.0
 */
#include "altera_up_avalon_video_pixel_buffer_dma.h"
#include "altera_up_avalon_video_character_buffer_with_dma.h"
#include "altera_up_sd_card_avalon_interface.h"//system initiation is incorrect, must change alt_sys_init.c file and rename include section

#include "system.h"
#include "sys/alt_irq.h"//??
#include "sys/alt_sys_init.h"//??
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include "includes.h"


#if OS_CRITICAL_METHOD == 3
OS_CPU_SR cpu_sr;
#endif


/* Definitions for PS2 variables */
alt_u8 ps2_data=0;
alt_u8 ps2_used=0;
volatile int keyboard_flag = 0;

/* Definition of Buffer DMA Devices*/
alt_up_pixel_buffer_dma_dev *pixel_buffer_dev;
alt_up_char_buffer_dev *char_buffer_dev;



volatile int edge_capture;

// ISR Functions
int count=0;
/* Parameters for Picture Settings */
#define PICTURE_NUMBER 50
short int picture[320][240][PICTURE_NUMBER];
/*Parameters for Fixation Point*/
#define X1 318
#define X2 323
#define Y1 238
#define Y2 243
#define FIXATION_COLOUR 100


void simple_irq(void* context, alt_u32 id)
{
	printf("im in an interrupt\n");
	//alt_up_char_buffer_clear(char_buffer_dev);
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	/*
	if(count==0)
	{
		alt_up_char_buffer_string (char_buffer_dev, "Psychophysics Experiment", 10, 0);
		alt_up_char_buffer_string (char_buffer_dev, "First Prototype", 5, 1);
		alt_up_char_buffer_string (char_buffer_dev, "Press again to continue...", 30, 30);
		alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 0, 0, 640, 480, 200, 0);
	}
	else if (count==1)
	{
		alt_up_char_buffer_string (char_buffer_dev, "============================Main Menu===========================", 5, 5);
		alt_up_char_buffer_string (char_buffer_dev, "================================================================", 5, 6);
		alt_up_char_buffer_string (char_buffer_dev, "~//////////Controls | Destination\\\\\\\\\\~", 5, 7);
		alt_up_char_buffer_string (char_buffer_dev, "~//////////KEY3     | Start\\\\\\\\\\~", 5, 7);
		alt_up_char_buffer_string (char_buffer_dev, "~//////////KEY2     | Stop\\\\\\\\\\~", 5, 8);
	}
	else
	{

	}*/

	if(count == 0)
	{
		alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 0);
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	}
	else if (count == 1)
	{
		alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 480);
		alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	}
	count = count ^1;
/*
	count = count+100;
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, count);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
*/
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture

}


void sw1_irq(void* context, alt_u32 id)
{
	printf("sw interrupt executed\n");
	//flag1 = flag1 ^1;

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW1_BASE,0x1);//reset edge capture

}
/*
void simple2_irq(void* context, alt_u32 id)
{

	alt_up_char_buffer_clear(char_buffer_dev);
	//alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 480);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 160, 120, 479, 359, 255, 0);
	//usleep(30000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(200000);
	//alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 160, 120, 479, 359, 255, 0);
	//usleep(60000);
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 160, 120, 479, 359, 0, 0);
	usleep(3000000);
	alt_up_char_buffer_string (char_buffer_dev, "Timeout", 35, 50);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
}
*/

void ps2_irq(void* context, alt_u32 id)
{

	ps2_data = IORD_ALTERA_AVALON_PIO_DATA(PIO_PS2_DATA_BASE);
	if (ps2_data == 240) keyboard_flag = 1;
	else ps2_used =0;
	if (keyboard_flag ==1)
		{

			ps2_used = ps2_data;
			//keyboard_flag =0;
		}
	printf("ps2 interrupt executed, data : %d, used data %d, keyboard flag: %d\n", ps2_data, ps2_used, keyboard_flag);

	//printf("Hello World \n");
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_PS2_INTERRUPT_BASE,0x1);//reset edge capture

}

void sw16_irq()
{

IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW16_BASE,0x1);//reset edge capture

}

/* Program Initialisation */
void program_init()
{

	/* initialize the pixel buffer HAL */
	pixel_buffer_dev = alt_up_pixel_buffer_dma_open_dev ("/dev/Pixel_Buffer_DMA");
	/* clear the graphics screen */
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);

	char_buffer_dev = alt_up_char_buffer_open_dev ("/dev/Char_Buffer_with_DMA");
	alt_up_char_buffer_clear(char_buffer_dev);

	void* edge_capture_ptr = (void*) &edge_capture;//recast edge capture
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY2_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY2_IRQ,edge_capture_ptr,simple_irq);

	/*
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY1_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY1_IRQ,edge_capture_ptr,simple2_irq);
	*/

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW1_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW1_BASE,0x1);//enable irq
	alt_irq_register(PIO_SW1_IRQ,edge_capture_ptr,sw1_irq);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW16_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_SW16_BASE,0x1);//enable irq
	alt_irq_register(PIO_SW16_IRQ,edge_capture_ptr,sw16_irq);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_PS2_INTERRUPT_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_PS2_INTERRUPT_BASE,0x1);//enable irq
	alt_irq_register(PIO_PS2_INTERRUPT_IRQ,edge_capture_ptr,ps2_irq);

}

// PS2 Keyboard Functions

void wait_BackSpace()
{
	ps2_clear();
	while(ps2_used != 102){}
	ps2_clear();
}
void wait_ENTER()
{
	ps2_clear();
	while(ps2_used != 90){}
	ps2_clear();
}
void ps2_clear()
{
	keyboard_flag=0;
	ps2_used=0;
	ps2_data=0;
}
void wait_Text()
{
	char text[40];
	int x = 10;
	int y = 17;
	int value=0;
	char ascii=0;
	ps2_clear();
	while((ps2_used != 90))
	{
		if(keyboard_flag == 1)
		{

			//printf("Entered wait text \n");
			if (ps2_used == 90)break;
			switch( ps2_used )
			{
			    case 69://0
			        //printf("entered 0\n");
			    	ascii = '0';
			    	text[value] = ascii;
			    	alt_up_char_buffer_string (char_buffer_dev, text, x, y);
			    	x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
			        break;
			    case 22 ://1
			        //printf("entered 1\n");
			    	ascii = '1';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
			    	keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
			        break;
			    case 30 ://2
			        //printf("entered 2\n");
			    	ascii = '2';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
			        break;
			    case 38://3
					//printf("entered 3\n");
			    	ascii = '3';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;
				case 37 ://4
					//printf("entered 4\n");
			    	ascii = '4';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;
				case 46 ://5
					//printf("entered 5\n");
			    	ascii = '5';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;
			    case 54 ://6
			        //printf("entered 6\n");
			    	ascii = '6';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
			        break;
			    case 61 ://7
			        //printf("entered 7\n");
			    	ascii = '7';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
			        break;
			    case 62://8
					//printf("entered 8\n");
			    	ascii = '8';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;
				case 70 ://9
					//printf("entered 9\n");
			    	ascii = '9';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					x++;
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;
				case 102 ://Backspace
					//printf("entered 9\n");
					x--;
					ascii = ' ';
					text[value] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, text, x, y);
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;
				case 240 ://break
					break;
				default:
					keyboard_flag=0;
					ps2_used=0;
					ps2_data=0;
					break;


			}
		}

	}

	ps2_clear();
}

void wait_SPACE()
{
	ps2_clear();
	while(ps2_used != 41){}
	ps2_clear();
}


int wait_left_right()
{
	ps2_clear();
	while(1)
	{
		if (ps2_used == 107)//left
		{
			ps2_clear();
			return 1;
		}
		else if (ps2_used == 116)//right
		{
			ps2_clear();
			return 0;
		}
	}

}

int wait_New_or_Current()
{
	ps2_clear();
	while(1)
	{
		if (ps2_used == 49)
		{
			ps2_clear();
			return 1;
		}
		else if (ps2_used == 41)
		{
			ps2_clear();
			return 0;
		}
	}

}
void wait_M_or_N()
{
	ps2_clear();
	while(1)
	{
		if (ps2_used == 58) break;
		else if (ps2_used == 43) break;
	}
	ps2_clear();
}
int wait_Main_Menu()
{
	ps2_clear();
	while(1)
	{
		if (ps2_used == 22 )
		{
			keyboard_flag=0;
			ps2_used=0;
			ps2_data=0;
			return 1;
		}
		else if (ps2_used == 30)
		{
			keyboard_flag=0;
			ps2_used=0;
			ps2_data=0;
			return 2;
		}
		else if (ps2_used == 38)
		{
			keyboard_flag=0;
			ps2_used=0;
			ps2_data=0;
			return 3;
		}
	}
}
/*SDram function*/

void SDram_to_VGA_back_buffer()
{
	int xD=160;
	int yD=120;
	int random_picture =0;

	//random_picture++;
	//if ( random_picture == 10)random_picture ==0;
	while ((random_picture = rand()%PICTURE_NUMBER) == 0);
	//printf("%ld\n",tv.tv_usec);
	printf ("random picture : %d\n",random_picture);
//		IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
	while(yD < 359)
	{
		if(xD < 479)
		{
			alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, picture[xD-160][yD-120][random_picture], xD , yD);
			xD++;
		}
		else
		{

			xD=160;
			yD++;
			alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, picture[xD-160][yD-120][random_picture], xD , yD);
		}

	}

}
/* SD Card Functions*/
/*
 * SD_open()
 * SD_read()
 * SD_subread()
 * SD_text_begin()
 * SD_text_mid()
 * SD_text_end()
 */
void SD_text_begin()
{
	alt_up_char_buffer_string (char_buffer_dev, "Psychophysics Experiment", 20, 10);
	alt_up_char_buffer_string (char_buffer_dev, "BETA Prototype", 20, 11);
	alt_up_char_buffer_string (char_buffer_dev, "Processing Information from SD Card...", 20, 30);
}
void SD_text_mid()
{
	printf("read complete\n");
	alt_up_char_buffer_string (char_buffer_dev, "Complete!", 20, 32);
	alt_up_char_buffer_string (char_buffer_dev, "Buffering Pixels...", 20, 33);
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 480);
}
void SD_text_end()
{
	printf("frame to back buffer complete\n");
	alt_up_char_buffer_string (char_buffer_dev, "Ready!", 20, 34);
	usleep(600000);
}
void SD_subread()
{
	short int handler;

	int mode = 0;
	int read_flag = 0;
	char sys_read;
	handler = alt_up_sd_card_fopen("sys.txt", false);
	while ((sys_read = alt_up_sd_card_read(handler)) != -1)
	{
		if (read_flag == 1 ) printf("%c", sys_read);
		//printf("%c", sys_read,sys_read);

		if(sys_read == 10) // new line
		{
			//printf("\n");
		}
		else if (sys_read == 59) // seperator ;
		{

		}
		else if (sys_read == 126) // seperator ~
		{
			//printf("here Here HERE!!!\n");
			mode = 1;
		}
		else if (sys_read == 92) // seperator
		{
			read_flag = 1;
			//printf("here Here HERE!!!\n");
		}
	}
	alt_up_sd_card_fclose(handler);
}

void SD_read()
{
	short int handler;
	unsigned char read;
	int xx = 20;
	int picturenumber=0 ;
	int ref=0;
	char filename[6];
/*
	handler = alt_up_sd_card_find_first("", buffer_name);
	printf("%d,  %s \n", handler, buffer_name);

	while ((handler = alt_up_sd_card_find_next(buffer_name)) != -1) printf("%d,  %s \n", handler, buffer_name);
*/

	for(picturenumber = 1;picturenumber <=PICTURE_NUMBER; picturenumber++)
	{
		ref = picturenumber;
		sprintf(filename, "%d", ref);

		if(picturenumber >=10)
		{
			filename[2]='.';
			filename[3]='t';
			filename[4]='x';
			filename[5]='t';
			filename[6]=0;
		}
		else
		{
			filename[1]='.';
			filename[2]='t';
			filename[3]='x';
			filename[4]='t';
			filename[5]=0;
			//filename[6]
		}

		printf("%s\n", filename);



		//handler = alt_up_sd_card_fopen("1.txt", false);

		int row = 0;
		int col =0;
		int read_count = 0 ;
		OS_ENTER_CRITICAL();
		handler = alt_up_sd_card_fopen(filename, false);
		while ((read = alt_up_sd_card_read(handler)) != -1)
		{
			//printf("%d\n",read);
			if (row < 319)
			{
				row++;
				picture [row][col][picturenumber] =read;
			}
			else
			{
				row=0;
				col++;
				picture [row][col][picturenumber] =read;
			}
			read_count++;
			if (read_count >=72958)break;

		}
		OS_EXIT_CRITICAL();
		alt_up_char_buffer_string (char_buffer_dev, ".", xx, 31);
		xx++;
		alt_up_sd_card_fclose(handler);
	}
}
void SD_open()
{
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;

	//SD card section
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

}
void SD_check()
{
	if ((alt_up_sd_card_is_Present() == false))
	{
		printf("Card disconnected.\n");
	}
}
/* Main Menu Functions  */
int MM()
{
	int main_menu = 0;
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "=================================Main Menu===================================", 0, 5);
	alt_up_char_buffer_string (char_buffer_dev, "=============================================================================", 0, 6);
	alt_up_char_buffer_string (char_buffer_dev, "Controls | Destination", 5, 7);
	alt_up_char_buffer_string (char_buffer_dev, "1        | Latency Test    : Time to LCD ", 5, 8);
	alt_up_char_buffer_string (char_buffer_dev, "2        | Processing Test : Time from Blank -> Image -> Blank", 5, 9);
	alt_up_char_buffer_string (char_buffer_dev, "3        | Experiment", 5, 10);
	main_menu = wait_Main_Menu();
	return main_menu;
}

void text_subject()
{
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Please enter your Subject Number:", 10, 16);
	wait_Text();
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Instructions:", 10, 16);
	alt_up_char_buffer_string (char_buffer_dev, "You will be shown a picture briefly", 10, 17);
	alt_up_char_buffer_string (char_buffer_dev, "Select a category that you think the picture belongs to", 10, 20);
	alt_up_char_buffer_string (char_buffer_dev, "Please press space bar when ready to begin", 20, 30);
	wait_SPACE();
}

void image_flash()
{

	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	usleep(500000 + rand()%1000000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(20000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(400000);

}

int loop()
{
	int x=0;
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, X1, Y1, X2, Y2, FIXATION_COLOUR, 0);
	alt_up_char_buffer_string (char_buffer_dev,"Data Collected" , 30, 50);
	alt_up_char_buffer_string (char_buffer_dev,"Press Space Bar to Continue" , 30, 51);
	alt_up_char_buffer_string (char_buffer_dev,"as Current User or press N for New User" , 25, 52);
	x = wait_New_or_Current();
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	return x;
}

int image_select()
{
	int x=0;
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev,"Face or non-Face? (<-/->)" , 20, 20);
	x = wait_left_right();
	alt_up_char_buffer_string (char_buffer_dev,"please wait for a moment" , 30, 49);
	return x;
}

