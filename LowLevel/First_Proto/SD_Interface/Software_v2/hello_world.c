#include <stdio.h>
#include <altera_up_sd_card_avalon_interface.h>
#include "system.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_sys_init.h"

int main(void)
{
	printf("System starting up\n");

	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");
	while(1)
	{
		if (device_reference != NULL)
		{
			while(1)
			{
				if ((connected == 0) && (alt_up_sd_card_is_Present()))
				{
					printf("Card connected.\n");
					//usleep(5000000);
					if (alt_up_sd_card_is_FAT16()) 	printf("FAT16 file system detected.\n");
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
return 0;
}

