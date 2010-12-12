/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */

#include <stdio.h>
#include "basic_io.h"
#include "SD_Card.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include "altera_avalon_pio_regs.h"
#include "system.h"

volatile int edge_capture;
void program_init();
int SEG7_EN = 1;

int main()
{

  UINT16 i=0;
  UINT32 j=720;
  BYTE Buffer[512]={0};
  int x=0;

  printf("Hello from Nios II!\n");
  while(SD_card_init())
  usleep(500000);
  printf("SD card initialize complete\n");


  program_init();


  while(1)
  {

    SD_read_lba(Buffer,j,1);
    printf("0x%x , 0x%x \n", Buffer[0], Buffer[1]);
    //IOWR(SEG7_DISPLAY_BASE, 2, 1);
    IOWR_ALTERA_AVALON_PIO_DATA(PIO_SEG7_BASE,x);
    j++;
    //usleep(1000);
    i=0;

    //printf (" x = %d \n", x);
    x += 1;

  }


  return 0;
}
void simple_irq(void* context, alt_u32 id)
{
	printf("interrupt entered\n");
	SEG7_EN = SEG7_EN ^1;
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_SEG7_EN_BASE, SEG7_EN);
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture

}
void program_init()
{
	  IOWR_ALTERA_AVALON_PIO_DATA(PIO_SEG7_EN_BASE, SEG7_EN);

	void* edge_capture_ptr = (void*) &edge_capture;//recast edge capture
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_KEY1_BASE,0x1);//reset edge capture
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_KEY1_BASE,0x1);//enable irq
	alt_irq_register(PIO_KEY1_IRQ,edge_capture_ptr,simple_irq);
}
