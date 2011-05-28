/*
 * sys_sd_functions.c
 *
 *  Created on: Apr 18, 2011
 *      Author: Shee Jia Chin
 *      Revision: 4.3
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
#include "sys_sd_functions.h"
#ifndef SYS_SD_FUNCTIONS_H_
#define SYS_SD_FUNCTIONS_H_

#endif /* SYS_SD_FUNCTIONS_H_ */

alt_up_sd_card_dev *device_reference = NULL;


short int write_handler;
short int read_handler;
short int list_handler;
int sd_count=0;


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
int SD_write_set(short int handler, char text[])
{
	int x = 0 ;
	sd_write(write_handler, "RESULTS.TXT",10);
	for(x = 0 ; x<=40;x++)
		{
			if (text[x] == 0 )break;
			sd_write(handler, "RESULTS.TXT",text[x]);
			printf("%c\n",text[x]);
		}
	sd_write(handler, "RESULTS.TXT",';');
	sd_write(handler, "RESULTS.TXT",10);

	return 0;
}
int SD_read_all(short int handler, char text[])
{
	printf("initiating Picture read\n");
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

	for(picturenumber = PICTURE_START;picturenumber <=PICTURE_NUMBER; picturenumber++)
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
		//OS_ENTER_CRITICAL();
		handler = alt_up_sd_card_fopen(final_text, false);
		//printf("%d",handler);
		while ((read = alt_up_sd_card_read(handler)) != -1)
		{
			//printf("%d\n",read);
			if (row < 159)
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
			if (read_count >=24576)break;

		}
		//OS_EXIT_CRITICAL();
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
