#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "system.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
//#include "altera_avalon_pio_regs.h"
#include "sys/alt_sys_init.h"

int main(void)
{
	char buffer_name[20];
	short int handler;
	short int read;
	printf("System starting up\n");

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

//	handler = alt_up_sd_card_find_first("HELLO/.", buffer_name);
	handler = alt_up_sd_card_find_first("", buffer_name);
	printf("%d,  %s \n", handler, buffer_name);
	while ((handler = alt_up_sd_card_find_next(buffer_name)) != -1) printf("%d,  %s \n", handler, buffer_name);

	handler = alt_up_sd_card_fopen("HELLO/HELLO", false);
	while ((read = alt_up_sd_card_read(handler)) != -1) printf("%c ", read);
	printf("\n");
	alt_up_sd_card_fclose(handler);

	int x=0;
	handler = alt_up_sd_card_fopen("BLACK.HEX", false);
	while ((read = alt_up_sd_card_read(handler)) != -1)
		{
		x++;
			printf("%c ", read);
		}
	printf("file count %d \n",x);
	alt_up_sd_card_fclose(handler);
/* writing operations
	char string[20] = "hello world";
	handler = alt_up_sd_card_fopen("text2.txt", true);
	for (x=0 ; x <= 20; x++) alt_up_sd_card_write(handler, string[x]);
	alt_up_sd_card_fclose(handler);

	handler = alt_up_sd_card_fopen("text2.txt", false);
	while ((read = alt_up_sd_card_read(handler)) != -1) printf("%c", read);
	printf("\n");
	alt_up_sd_card_fclose(handler);
*/
return 0;
}

