/*
 * sys_functions.c
 *
 *  Created on: Apr 18, 2011
 *      Author: Shee Jia Chin
 *      Revision: 4.3
 *
 * This Code Interfaces with a Nios Softcore Microprocessor intended to be used
 * With the DE2 Hardware FPGA (Field Programmable Gate Array)
 * In Here, There are tailored functions designed for the purpose of the
 * Psychophysics Experiment to be tied in with the Monash University Australia
 * Bionic Eye Project.
 *
 * There are:
 * @@@@@@@@@@  Input interrupt functions: Testing, Interfacing and Reseting
 * @@@@@@@@@@  Program initialisation Function
 * @@@@@@@@@@  PS2 Keyboard Interface Handling Functions
 * @@@@@@@@@@  SD Card Interface Functions
 * $$$$$$$$$$$$$$$$$$$$   SD Card Functions:
 * $$$$$$$$$$$$$$$$$$$$    SD_open()
 * $$$$$$$$$$$$$$$$$$$$    SD_read()
 * $$$$$$$$$$$$$$$$$$$$    SD_subread()
 * $$$$$$$$$$$$$$$$$$$$    SD_text_begin()
 * $$$$$$$$$$$$$$$$$$$$    SD_text_mid()
 * $$$$$$$$$$$$$$$$$$$$    SD_text_end()
 * @@@@@@@@@@  VGA Interface Functions
 */

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
/* For Ultilizing OS_ENTER_CRITICAL() & OS_EXIT CRITICAL*/
#if OS_CRITICAL_METHOD == 3
OS_CPU_SR cpu_sr;
#endif


/* Definitions for PS2 variables */
alt_u8 ps2_data=0;
alt_u8 ps2_used=0;
volatile int keyboard_flag = 0;

/* Translation for Text from PS2 to Ascii Variables*/
char subject_id[40];
char sub_id[10];
int text_x = 10;
int text_y = 17;
int value=0;

/* Definition of Buffer DMA Devices*/
alt_up_pixel_buffer_dma_dev *pixel_buffer_dev;


short int write_handler;

char oresults[PICTURE_NUMBER];
//int sd_close_set=0;

/* Interrupt Flag*/
volatile int edge_capture;

// ISR Functions
int image_en=0;


/* Parameters for Picture Settings */



int random_picture=0;
/*Parameters for Fixation Point*/
#define X1 318
#define X2 323
#define Y1 238
#define Y2 243



/* Testing and Debugging ISR Interrupt Service Routine */
/*
void key1_irq(void* context, alt_u32 id)
{
	printf("Testing and Debugging ISR\n");
	printf("Pixel buffer Slave address %d start address %d back buffer address %d\n",pixel_buffer_dev->base,pixel_buffer_dev->buffer_start_address,pixel_buffer_dev->back_buffer_start_address);
	printf("Addressing mode %d, Color Mode  %d X resolution  %d  Y Resolution  %d\n ",pixel_buffer_dev->addressing_mode,pixel_buffer_dev->color_mode,pixel_buffer_dev->x_resolution,pixel_buffer_dev->y_resolution);
	printf("X Coordinate Offset%d,  X Coord Mask%d,  Y Coord Offset %d, Y Coord Mask%d\n",pixel_buffer_dev->x_coord_offset,pixel_buffer_dev->x_coord_mask,pixel_buffer_dev->y_coord_offset,pixel_buffer_dev->y_coord_mask);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
}
*/
void key1_irq(void* context, alt_u32 id)
{
	printf("key1 interrupted \n");
	//sd_read(read_handler,"hello.txt");
	//sd_read(read_handler,"EXP/PICS/SET1/HELLO.TXT");
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
}
void key2_irq(void* context, alt_u32 id)
{
	printf("key2 interrupted \n");
	SDram_to_VGA_back_buffer(1);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
}
void key3_irq(void* context, alt_u32 id)
{
	printf("key3 interrupted \n");
	image_en = image_en ^1;
	if(image_en == 1) alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 480);
	else alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 960);
	printf("Pixel buffer Slave address %d start address %d back buffer address %d\n",pixel_buffer_dev->base,pixel_buffer_dev->buffer_start_address,pixel_buffer_dev->back_buffer_start_address);
	//printf("X Coordinate Offset%d,  X Coord Mask%d,  Y Coord Offset %d, Y Coord Mask%d\n",pixel_buffer_dev->x_coord_offset,pixel_buffer_dev->x_coord_mask,pixel_buffer_dev->y_coord_offset,pixel_buffer_dev->y_coord_mask);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY3_BASE,0x1);//reset edge capture
}
/* Latency Test of Image Enabling to VGA Output to triggering photoelectric sensor */
/* Yet to be done, confirm latency in physical circuit of sensor is negligible */
void sw1_irq(void* context, alt_u32 id)
{
/*
	printf("sw interrupt executed\n");
	image_en = image_en ^1;
*/

	//alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW1_BASE,0x1);//reset edge capture

}
/**/
void sw16_irq()
{
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 0);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW16_BASE,0x1);//reset edge capture

}

/* ISR for handling all PS2 Interrupts */
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

/* Program Initialisation */
void program_init()
{

	/* initialize the pixel buffer HAL */
	pixel_buffer_dev = alt_up_pixel_buffer_dma_open_dev ("/dev/Pixel_Buffer_DMA");
	//if (pixel_buffer_dev->buffer_start_address == 480)alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	//printf("pixel buffer current address %d start address %d back buffer address %d\n",pixel_buffer_dev->base,pixel_buffer_dev->buffer_start_address,pixel_buffer_dev->back_buffer_start_address);
	printf("hello from program init function\n");
	/* clear the graphics screen */
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);

	char_buffer_dev = alt_up_char_buffer_open_dev ("/dev/Char_Buffer_with_DMA");
	alt_up_char_buffer_clear(char_buffer_dev);



	void* edge_capture_ptr = (void*) &edge_capture;//recast edge capture

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY2_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY2_IRQ,edge_capture_ptr,key2_irq);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY3_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY3_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY3_IRQ,edge_capture_ptr,key3_irq);

/*
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY1_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY1_IRQ, edge_capture_ptr, key1_irq);
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
/* Polling Wait for PS2 Backspace Key */
void wait_BackSpace()
{
	ps2_clear();
	while(ps2_used != 102){}
	ps2_clear();
}
/* Polling Wait for Enter Key*/
void wait_ENTER()
{
	ps2_clear();
	while(ps2_used != 90){}
	ps2_clear();
}
/**/
void wait_PIO_SW_EN_Check(int set)
{
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Checking Switch Status17", 5, 5);
	usleep(100000);
	alt_up_char_buffer_clear(char_buffer_dev);
	if(set == 0)
	{
		while(IORD_ALTERA_AVALON_PIO_DATA(PIO_SW17_BASE) == 1)
		{
			alt_up_char_buffer_string (char_buffer_dev, "Switch17 is currently enabled, Please disable it", 5, 6);
		}

		alt_up_char_buffer_clear(char_buffer_dev);
		alt_up_char_buffer_string (char_buffer_dev, "Switch17 is disabled, Proceed with Measurement", 5, 6);
		alt_up_char_buffer_string (char_buffer_dev, "The Segment 7 LED devices show the time in hexadecimal", 5, 7);
		alt_up_char_buffer_string (char_buffer_dev, "Press Space when done", 5, 8);

	}
	else if(set==1)
	{
		while(IORD_ALTERA_AVALON_PIO_DATA(PIO_SW17_BASE) == 0)
		{
			alt_up_char_buffer_string (char_buffer_dev, "Switch17 is currently disabled, Please enable it", 5, 6);
		}

		alt_up_char_buffer_clear(char_buffer_dev);
		alt_up_char_buffer_string (char_buffer_dev, "Switch17 is enabled, Measurement will begin soon", 5, 6);
		alt_up_char_buffer_string (char_buffer_dev, "The Segment 7 LED devices show the time in hexadecimal", 5, 7);
		alt_up_char_buffer_string (char_buffer_dev, "Press Space when done", 5, 8);
	}
}
/* Polling function for latency measurement*/
void wait_PIO_SW()
{
	wait_PIO_SW_EN_Check(0);
	ps2_clear();
	while(1)
	{
		if ( image_en == 1)
		{
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
			alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 0);
		}
		else if( image_en == 0)
		{
			alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
			alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 480);
		}

		if(ps2_used == 41) break;
	}
	ps2_clear();
}
/* Function for flushing the registers of PS2 */
void ps2_clear()
{
	keyboard_flag=0;
	ps2_used=0;
	ps2_data=0;
}
/* Polling Wait for Text followed by Enter */
void wait_Text()
{

	text_x = 10;
	text_y = 17;
	value=0;
    char ascii;
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
			    	PS2Dat2Ascii('0');
			    	break;
			    case 22 ://1
			        //printf("entered 1\n");
			    	PS2Dat2Ascii('1');
			    	break;
			    case 30 ://2
			        //printf("entered 2\n");
			    	PS2Dat2Ascii('2');
			        break;
			    case 38://3
					//printf("entered 3\n");
			    	PS2Dat2Ascii('3');
					break;
				case 37 ://4
					//printf("entered 4\n");
					PS2Dat2Ascii('4');
					break;
				case 46 ://5
					//printf("entered 5\n");
					PS2Dat2Ascii('5');
					break;
			    case 54 ://6
			        //printf("entered 6\n");
			    	PS2Dat2Ascii('6');
			    	break;
			    case 61 ://7
			        //printf("entered 7\n");
			    	PS2Dat2Ascii('7');
			    	break;
			    case 62://8
					//printf("entered 8\n");
			    	PS2Dat2Ascii('8');
			    	break;
				case 70 ://9
					//printf("entered 9\n");
					PS2Dat2Ascii('9');
					break;
				case 102 ://Backspace
					//printf("entered 9\n");
					if(text_x>10)text_x--;
					ascii = ' ';
					subject_id[value] = ascii;
					sub_id[text_x - 10] = ascii;
					alt_up_char_buffer_string (char_buffer_dev, subject_id, text_x, text_y);
					ps2_clear();
					break;
				case 240 ://break
					break;
				default:
					ps2_clear();
					break;


			}
		}

	}

	ps2_clear();
}
/* Conversion from PS2 Dat to VGA output which uses Ascii*/
void PS2Dat2Ascii(char ascii)
{
	subject_id[value] = ascii;
	sub_id[text_x - 10] = ascii;
	alt_up_char_buffer_string (char_buffer_dev, subject_id, text_x, text_y);
	text_x++;
	ps2_clear();
}
/* Polling Wait for Text followed by Space*/
void wait_SPACE()
{
	ps2_clear();
	while(ps2_used != 41){}
	ps2_clear();
}

/* Polling Wait for selection of category */
char wait_left_right()
{
	ps2_clear();
	while(1)
	{
		if (ps2_used == 28)//left
		{
			ps2_clear();
			return '1';
		}
		else if (ps2_used == 75)//right
		{
			ps2_clear();
			return '0';
		}
	}

}
/* Polling Wait Selection between New or Current */
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
/* */
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
/* Polling Wait for Selection at Main Menu */
int wait_Main_Menu()
{
	ps2_clear();
	while(1)
	{
		if (ps2_used == 22 )
		{
			ps2_clear();
			return 1;
		}
		else if (ps2_used == 30)
		{
			ps2_clear();
			return 2;
		}
		else if (ps2_used == 38)
		{
			ps2_clear();
			return 3;
		}
	}
}
/*SDram function*/
/*
 * Function to write Image stored in Main Memory (SDRam) to Pixel Buffer which is
 * actually the Sram
*/
int pixel_buffer_dma_draw(alt_up_pixel_buffer_dma_dev *pixel_buffer, unsigned int color, unsigned int x, unsigned int y)
/* This function draws a pixel to the back buffer.
 */
{
	// boundary check
	unsigned int addr = 0;
		addr |= ((x & pixel_buffer->x_coord_mask) << pixel_buffer->x_coord_offset);
		addr |= ((y & pixel_buffer->y_coord_mask) << pixel_buffer->y_coord_offset);
	/* Now, depending on the color depth, write the pixel color to the specified memory location. */
		IOWR_8DIRECT(pixel_buffer->back_buffer_start_address, addr, color);

	return 0;
}

int SDram_to_VGA_back_buffer(int set)
{
	int ret = 0;
	int xD=240;
	int yD=160;
	if(set == 1)random_picture++;


	if ( random_picture > PICTURE_NUMBER)
	{
		random_picture =1;
		ret = 1;
		//printf("results: %s\n\n",oresults);
		//sd_write(oresults);
		//sd_count =0;
		//alt_up_sd_card_fclose(handler);
		//sd_close_set=1;
	}
	//while ((random_picture = rand()%PICTURE_NUMBER) == 0);
	//printf("%ld\n",tv.tv_usec);
	printf ("random picture : %d\n",random_picture);
//		IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
	while(yD < 319)
	{
		if(xD < 399)
		{
			alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, picture[xD-240][yD-160][random_picture], xD , yD);
			xD++;
		}
		else
		{

			xD=240;
			yD++;
			alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, picture[xD-240][yD-160][random_picture], xD , yD);
		}

	}
	return ret;
}

void VGA_white()
{

}
/**/
void SD_text_begin()
{
	alt_up_char_buffer_string (char_buffer_dev, "Psychophysics Experiment", 20, 10);
	alt_up_char_buffer_string (char_buffer_dev, "Theta-Beta Prototype", 20, 11);
	alt_up_char_buffer_string (char_buffer_dev, "Testing Phase - Round 3", 20, 12);
	alt_up_char_buffer_string (char_buffer_dev, "Processing Information from SD Card...", 20, 30);
}
/**/
void SD_text_mid()
{
	printf("read complete\n");
	alt_up_char_buffer_string (char_buffer_dev, "Complete!", 20, 32);
	alt_up_char_buffer_string (char_buffer_dev, "Buffering Pixels...", 20, 33);
	alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer_dev, 480);
}
/**/
void SD_text_end()
{
	printf("frame to back buffer complete\n");
	alt_up_char_buffer_string (char_buffer_dev, "Ready!", 20, 34);
	usleep(600000);
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
	//alt_up_char_buffer_string (char_buffer_dev, "1        | Latency Test    : Time to LCD ", 5, 8);
	//alt_up_char_buffer_string (char_buffer_dev, "2        | Processing Test : Time from Blank -> Image -> Blank", 5, 9);
	alt_up_char_buffer_string (char_buffer_dev, "3        | Experiment", 5, 8);
	main_menu = wait_Main_Menu();
	return main_menu;
}
/**/
void text_subject()
{
	int x=0;
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Please enter your Subject Number:", 10, 16);
	wait_Text();
	//sd_write(write_handler, "RESULTS.TXT",10);
	for(x = 0 ; x<=40;x++)
	{
		if (sub_id[x] == 0 )break;
		sd_write(write_handler, "RESULTS.TXT",sub_id[x]);
	}
	sd_write(write_handler, "RESULTS.TXT",';');
	sd_write(write_handler, "RESULTS.TXT",10);
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Instructions:", 10, 12);
	alt_up_char_buffer_string (char_buffer_dev, "You will be shown a image briefly", 10, 13);
	alt_up_char_buffer_string (char_buffer_dev, "Select whether you think it is a human face or a non-face", 10, 16);
	alt_up_char_buffer_string (char_buffer_dev, "Please press space bar when ready to begin", 20, 20);
	wait_SPACE();

	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Pay attention to the Centre Point ", 10, 12);
	alt_up_char_buffer_string (char_buffer_dev, "The Experiment is about to start ", 10, 13);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	usleep(3000000);

}

void exp_complete()
{
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	alt_up_char_buffer_string (char_buffer_dev, "Experiment Complete!! ", 10, 18);
	alt_up_char_buffer_string (char_buffer_dev, "Thank you for participating... ", 10, 19);
	alt_up_char_buffer_string (char_buffer_dev, "Hope to See you again soon...", 10, 20);
	usleep(3000000);
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
}
/**/

/* */
int loop()
{
	int x=0;
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, X1, Y1, X2, Y2, FIXATION_COLOUR, 0);
	alt_up_char_buffer_string (char_buffer_dev,"Data Collected!" , 30, 50);
	//alt_up_char_buffer_string (char_buffer_dev,"Press Space Bar to Continue" , 30, 51);
	//alt_up_char_buffer_string (char_buffer_dev,"as Current User or press N for New User" , 25, 52);
	//x = wait_New_or_Current();
	//x  =
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	return x;
}
/* */
char image_select()
{
	char x;
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev,"Face or non-Face? (A <-/-> L)" , 20, 15);
	x = wait_left_right();
	alt_up_char_buffer_string (char_buffer_dev,"please wait for a moment" , 30, 49);
	return x;
}
