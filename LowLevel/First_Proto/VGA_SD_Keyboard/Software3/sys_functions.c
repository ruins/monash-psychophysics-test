/*
 * sys_functions.c
 *
 *  Created on: Mar 30, 2011
 *      Author: Shee Jia Chin
 *      Revision: 4.2
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
#include <stdlib.h>
/* Header Files For Implementing uCOSii Kernal*/
#include "includes.h"
/* Header Files For System Tailored Functions*/
#include "sys_functions.h"
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
alt_up_char_buffer_dev *char_buffer_dev;

alt_up_sd_card_dev *device_reference = NULL;
char buffer_name[STR_LEN];
short int write_handler;
short int read_handler;
short int list_handler;

char oresults[PICTURE_NUMBER];
int sd_count=0;
//int sd_close_set=0;

/* Interrupt Flag*/
volatile int edge_capture;

// ISR Functions
int image_en=0;


/* Parameters for Picture Settings */

short int picture[320][320][PICTURE_NUMBER+1];

int random_picture=0;
/*Parameters for Fixation Point*/
#define X1 318
#define X2 323
#define Y1 238
#define Y2 243
#define FIXATION_COLOUR 100

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
	sd_read(read_handler,"EXP/PICS/SET1/HELLO.TXT");
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
}
void key2_irq(void* context, alt_u32 id)
{
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
}
/* Latency Test of Image Enabling to VGA Output to triggering photoelectric sensor */
/* Yet to be done, confirm latency in physical circuit of sensor is negligible */
void sw1_irq(void* context, alt_u32 id)
{
	printf("sw interrupt executed\n");
	image_en = image_en ^1;

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW1_BASE,0x1);//reset edge capture

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
/**/
void sw16_irq()
{

IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_SW16_BASE,0x1);//reset edge capture

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
/*
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY2_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY2_IRQ,edge_capture_ptr,key2_irq);

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
void SDram_to_VGA_back_buffer(int set)
{
	int xD=160;
	int yD=80;
	if(set == 1)random_picture++;


	if ( random_picture > PICTURE_NUMBER)
	{
		random_picture =1;
		//printf("results: %s\n\n",oresults);
		//sd_write(oresults);
		sd_count =0;
		//alt_up_sd_card_fclose(handler);
		//sd_close_set=1;
	}
	//while ((random_picture = rand()%PICTURE_NUMBER) == 0);
	//printf("%ld\n",tv.tv_usec);
	printf ("random picture : %d\n",random_picture);
//		IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
	while(yD < 399)
	{
		if(xD < 479)
		{
			alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, picture[xD-159][yD-79][random_picture], xD , yD);
			xD++;
		}
		else
		{

			xD=160;
			yD++;
			alt_up_pixel_buffer_dma_draw( pixel_buffer_dev, picture[xD-159][yD-79][random_picture], xD , yD);
		}

	}

}

void VGA_white()
{

}
/**/
void SD_text_begin()
{
	alt_up_char_buffer_string (char_buffer_dev, "Psychophysics Experiment", 20, 10);
	alt_up_char_buffer_string (char_buffer_dev, "Theta-Alpha Prototype", 20, 11);
	alt_up_char_buffer_string (char_buffer_dev, "Testing Phase - Round 2", 20, 12);
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
/**/
int sd_fclose(short int handler, char text[STR_LEN])
{
	int x =0;
	if (handler != -1)	alt_up_sd_card_fclose(handler);
	else	{printf("File: %s",text);printf(" did not close properly\n");x =1;}
	return x;
}
int compare_strings(char A[STR_LEN], char B[STR_LEN])
{
	int match =0 ;
	int mismatch =0;
	int x = 0;
	for(x = 0; x <=STR_LEN; x++)
	{
		//printf("%d ", B[x]);
		if (B[x] == 46) break;
		if(x>0) if(match != 1) mismatch = 1;
		if(A[x] == B[x]) match = 1;
		else match = 0;
	}
	if(mismatch == 1) match =0;
	//printf("\n");
	//if(match == 1) printf("strings match\n");
	return match;
}
int sd_list(short int handler,int check, char text[STR_LEN])
{
	//int match = 0;
	//int x = 0 ;
	//int ret = 0 ;
	int int_set=0;
	handler = alt_up_sd_card_find_first("", buffer_name);
	if(check == 0)
	{
		printf("%s \n", buffer_name);
		while ((handler = alt_up_sd_card_find_next(buffer_name)) != -1) printf("%s \n", buffer_name);
		/* BEWARE!! if you accidently close alt_up_sd_card_fclose(-1), this will lock your sd card
		 * card and bring it into limbo if you try to access it.
		 */

		handler = alt_up_sd_card_find_first("", buffer_name);
		sd_fclose(handler,text);
	}
	else if (check ==1 )
	{
		if(compare_strings(buffer_name , text))
		{
			int_set = 1;
			//printf("match found \n");
		}
		//else int_set = 0;
		//printf("%s %s\n",text, buffer_name);
		while ((handler = alt_up_sd_card_find_next(buffer_name)) != -1)
		{
			if(compare_strings(buffer_name , text))
			{
				int_set = 1;
				//printf("match found \n");
			}
			//else int_set =0;
			//printf("%s %s\n",text, buffer_name);
		}
		//printf("%s \n", buffer_name);
		/* BEWARE!! if you accidently close alt_up_sd_card_fclose(-1), this will lock your sd card
		 * card and bring it into limbo if you try to access it.
		 */
		//sd_fclose(handler,text);

		//if (int_set == 1) x = 1;
		//printf("int_set: %d\n",int_set);
	}

	return int_set;
}
int sd_write(short int handler, char text[STR_LEN], char write_ascii)
{
	/* writing operations*/
	int err_close=0;
	if(sd_list(list_handler, 1 , text) == 0)
	{
		printf("File location does not exist, creating a new file\n");
		handler = alt_up_sd_card_fopen(text, true);
	}
	else if ( sd_list(list_handler, 1 , text) == 1 )
	{
		handler = alt_up_sd_card_fopen(text, false);
	}
	while ((alt_up_sd_card_read(handler)) != -1){}
	//for (x=0 ; x <= 20; x++) alt_up_sd_card_write(handler, string[x]);
	if(alt_up_sd_card_write(handler, write_ascii) == false ) printf("write unsuccessful\n");

	err_close = sd_fclose(handler,text);
	if(err_close == 1) printf("SD_write operation unsuccessful...\n");
	else printf("SD_write operation success!\n");
	//handler = alt_up_sd_card_fopen("text2.txt", false);
	//while ((read = alt_up_sd_card_read(handler)) != -1) printf("%c", read);
	//alt_up_sd_card_fclose(handler);

	return 0;
}

int sd_read(short int handler, char text[STR_LEN])
{
	int x=0;
	short int read;
	handler = alt_up_sd_card_fopen(text, false);
	while ((read = alt_up_sd_card_read(handler)) != -1)
		{
		x++;
			printf("%c", read);
		}
	printf("\n");
	printf("file count %d, handler %d \n",x, handler);
	sd_fclose(handler,text);
	return 0;
}
/**/
int SD_read_all(short int handler, char text[])
{
	char final_text[40];
	unsigned char read;
	//char read;
	int xx = 20;
	int picturenumber=0 ;
	int ref=0;
	char filename[6];
	int x=0;
	int y=0;
	for(x = 0 ; x<=40;x++)
	{
		if(text[x] == 46)break;
		final_text[x] = text[x];
	}

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

		//printf("%s\n", filename);


		for(y = 0 ; y<=6 ; y++ )
		{
			if(filename[y]==0)break;
			final_text[x+y] = filename[y];
		}
		printf("%s\n", final_text);
		//handler = alt_up_sd_card_fopen("1.txt", false);

		int row = 0;
		int col =0;
		int read_count = 0 ;
		OS_ENTER_CRITICAL();
		handler = alt_up_sd_card_fopen(final_text, false);

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
			if (read_count >=98250)break;

		}
		OS_EXIT_CRITICAL();
		alt_up_char_buffer_string (char_buffer_dev, ".", xx, 31);
		xx++;
		alt_up_sd_card_fclose(handler);
	}
	return 0;
}
/**/
void SD_open()
{

	int connected = 0;

	//SD card section
	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");

	//results_handler = alt_up_sd_card_fopen("results.txt", true);
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
/**/
void SD_check()
{
	if ((alt_up_sd_card_is_Present() == false))
	{
		printf("Card disconnected.\n");
	}
}
/**/

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
	sd_write(write_handler, "RESULTS.TXT",10);
	for(x = 0 ; x<=40;x++)
	{
		if (sub_id[x] == 0 )break;
		sd_write(write_handler, "RESULTS.TXT",sub_id[x]);
	}
	sd_write(write_handler, "RESULTS.TXT",';');
	sd_write(write_handler, "RESULTS.TXT",10);
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev, "Instructions:", 10, 16);
	alt_up_char_buffer_string (char_buffer_dev, "You will be shown a image briefly", 10, 17);
	alt_up_char_buffer_string (char_buffer_dev, "Select a category that you think the picture belongs to", 10, 20);
	alt_up_char_buffer_string (char_buffer_dev, "Please press space bar when ready to begin", 20, 30);
	wait_SPACE();
}
/**/
void image_flash()
{

	alt_up_char_buffer_clear(char_buffer_dev);
	//alt_up_pixel_buffer_dma_clear_screen(pixel_buffer_dev, 0);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	//usleep(500000 + rand()%1000000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(50000);
	alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
	usleep(400000);

}
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
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_pixel_buffer_dma_draw_box(pixel_buffer_dev, 315, 235, 325, 245, FIXATION_COLOUR, 0);
	return x;
}
/* */
char image_select()
{
	char x;
	alt_up_char_buffer_clear(char_buffer_dev);
	alt_up_char_buffer_string (char_buffer_dev,"Face or non-Face? (A <-/-> L)" , 20, 20);
	x = wait_left_right();
	alt_up_char_buffer_string (char_buffer_dev,"please wait for a moment" , 30, 49);
	return x;
}

