#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "system.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
//#include "altera_avalon_pio_regs.h"
#include "sys/alt_sys_init.h"
#define STR_LEN 20
alt_up_sd_card_dev *device_reference = NULL;
/* Interrupt Flag*/
volatile int edge_capture;
short int read_handler;
short int list_handler;
short int write_handler;
char buffer_name[STR_LEN];
char A[20] = "HEY.TXT";
char B[20] = "HEY.TXT";
char string[20] = "hello world";



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
	int match = 0;
	int x = 0 ;
	int ret = 0 ;
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
void key1_irq(void* context, alt_u32 id)
{
	printf("key1 interrupted \n");
	//sd_read(read_handler,"hello.txt");
	sd_read(read_handler,"BLA.TXT");
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
}
void key2_irq(void* context, alt_u32 id)
{
	printf("key2 interrupted \n");

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
}
void key3_irq(void* context, alt_u32 id)
{
	printf("key3 interrupted \n");
	//sd_list(list_handler,0, "");
	//sd_read(read_handler,"EXP/hello.txt");
	sd_write(write_handler, "BLA.TXT",'a');
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY3_BASE,0x1);//reset edge capture
}
void program_init()
{
	void* edge_capture_ptr = (void*) &edge_capture;//recast edge capture

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY2_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY2_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY2_IRQ,edge_capture_ptr,key2_irq);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY1_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY1_IRQ, edge_capture_ptr, key1_irq);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY3_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY3_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY3_IRQ, edge_capture_ptr, key3_irq);

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

}
int main(void)
{


	printf("System starting up\n");

	//SD_open();
	program_init();
	//sd_read(read_handler,"hello.txt");

	for(;;)
	{}

	return 0;
}
