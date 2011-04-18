/*
 * hello_ucosii.c
 *
 *  Created on: Apr 18, 2011
 *      Author: Shee Jia Chin
 *      Revision: 4.3
 *
 * This code is the Main Program Flow of the System to be implemented on a
 * Nios Softcore Microprocessor intended to be used with the Altera DE2 Hardware
 * FPGA (Field Programmable Gate Array),
 * This section shows the system flow of the Experiment controlled by an
 * Embedded Real Time Kernal.
 *
 * This System is Tailored to be used in the Psychophysics Experiment to be
 * tied in with the Monash University Australia Bionic Eye Project.
 *
 * Note ( Caution ) ( Mandatory Action): At BSP Settings->Main->Settings->Common->
 * ->hal->hal.linker->exception stack memory region name->select onchip_memory
 *                  ->interrupt stack memory region name->select onchip_memory
 * By doing so, there will be a memory hierachy which efficiently organises memory
 * To ensure good response from system design.
 * Addtionally this is the only bypass currently available from a fatal crash to
 * the system
 *
 * Functions: Provides Interfacing with major components which are
 * SD Card, PS2 Keyboard, VGA output and various other peripherals.
 *
 * Features:
 *           -Psychophysics Experiment
 *           -Hardware Latency Measurement for DC Baising against possible results
 *           collected used to measure reaction times
 *           -Hardware Time Measurement of anywhere in program flow,
 *           -Measurement Resolution of 50MHz
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
#include "sys_sd_functions.h"
#include "sys_vga_functions.h"
// Required for Enabling and Disabling Interrupts
#if OS_CRITICAL_METHOD == 3
OS_CPU_SR cpu_sr;
#endif

int reload_image = 0;

short int write_handler;
short int read_handler;
/* Establish Flow of Task 2 Thread, Main Menu Execution */
void task1(void* pdata)
{
	//OS_ENTER_CRITICAL();
	//program_init();
	//OS_EXIT_CRITICAL();
	//For Semaphore
	INT8U err;
	//int x = 0;
  for (;;)
  {
//    OSSemPend(SD,0,&err);
    printf("Hello from task1\n");
    if( pixel_buffer_dev->buffer_start_address== 480) alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer_dev);
    SD_text_begin();
    SD_open();

/*
    OSTaskSuspend(2);
    OSTaskSuspend(3);
    OSTaskSuspend(4);
*/

 //   SD_subread();
	//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
    //OS_ENTER_CRITICAL();

    SD_write_set(write_handler,"SET4");
    SD_read_all(read_handler, "EXP/PICS/SET4/.");
    //sd_read(read_handler,"EXP/PICS/Set1/HELLO.TXT");
   //OS_EXIT_CRITICAL();
	//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
/*
    OSTaskResume(2);
    OSTaskResume(3);
    OSTaskResume(4);
*/
    SD_text_mid();
    //OS_ENTER_CRITICAL();
	//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
    //SDram_to_VGA_back_buffer(0);
	//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
	//OS_EXIT_CRITICAL();
    SD_text_end();
    OSSemPost(SD_MM);
    //OSSemPost(SD_c);
    OSSemPend(SD,0,&err);
    OSTimeDlyHMSM(0, 0, 0, 50);
  }
}
/* Establish Flow of Task 2 Thread, Main Menu Execution */
void task2(void* pdata)
{
	int IIK=0;   //Input Internal Key
	int OIK=0; //Output Internal Key
	int x = 0;
	INT8U err;
	for (;;)
	{
	    if (IIK ==0)
	    {
	    	OSSemPend(SD_MM,0,&err);
	    	IIK =1;
	    }
	    else if(IIK ==1 )
		{

			if (OIK == 1)
			{
				OSSemPend(MM_Lat_VGA,0,&err);
				OIK= 0;
			}
			else if (OIK == 2)
			{
				OSSemPend(MM_Elapsed,0,&err);
				OIK= 0;
			}
			else if (OIK == 3)
			{
				OSSemPend(MM_VGAc,0,&err);
				OIK= 0;
			}

			if (x == 1)
			{
				printf("Enters 1\n");
				x =0;
				OIK = 1;
				OSSemPost(MM_Lat_VGA);
			}
			else if (x == 2)
			{
				printf("Enters 2\n");
				x =0;
				OIK = 2;
				OSSemPost(MM_Elapsed);
			}
			else if (x == 3)
			{
				//printf("Enters Here \n");
				x =0;
				OIK = 3;
				OSSemPost(MM_VGAc);
			}
			else if(x==0)
			{
				x = MM();
			}


			//printf("return value %d\n",x);
	    	//printf("Hello from task2\n");




			OSTimeDlyHMSM(0, 0, 0, 60);
		}



	}
}
/* Psychophysic Experiment Thread */
void task3(void* pdata)
{

	int subject_flag = 0;
	int IK=0; // Internal Key
	int x = 0;
	char select;
	INT8U err;
	for (;;)
	{
		if (IK ==0)
		{
			OSSemPend(MM_VGAc,0,&err);
			//text_wait();
			text_subject();
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
			OS_ENTER_CRITICAL();
			SDram_to_VGA_back_buffer(1);
			OS_EXIT_CRITICAL();
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
			//text_wait_end();
			if(reload_image == 1 )
			{
				reload_image = 0;
				//SDram_to_VGA_back_buffer(0);
			}
			IK =1;
			x=0;
		}
		else if(IK ==1 )
		{
			//printf("Hello from task3\n");
			if (subject_flag == 0)
			{
				subject_flag = 1;
				//text_subject();
			}
			//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
			OS_ENTER_CRITICAL();
			image_flash();
			OS_EXIT_CRITICAL();
			//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
			select = image_select();
			if(select == '0')
			{
				printf("selected right\n");
				//SD_write(select);
				sd_write(write_handler,"RESULTS.TXT",select);
			}
			else if(select == '1')
			{
				printf("selected left\n");
				//SD_write(select);
				sd_write(write_handler,"RESULTS.TXT",select);
			}

			//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
			x =SDram_to_VGA_back_buffer(1);
			//IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
			loop();
			// x = loop();
			//SD_write_en = 1;
			if (x == 1)
			{
				sd_write(write_handler, "RESULTS.TXT",10);//new line
				exp_complete();
				subject_flag = 0;
				IK =0;
				OSSemPost(MM_VGAc);
			}

		}
		OSTimeDlyHMSM(0, 0, 0, 70);
	}
}
/* SD write Thread */
/*
void task6(void* pdata)
{
	INT8U err;
	for(;;)
	{
		if(SD_write_en ==1 )
		{
			SD_write_en =0;
		}
		else
		{

		}
		OSTimeDlyHMSM(0, 0, 0, 50);
	}
}
*/
/* Latency Measurement Thread*/
void task4(void* pdata)
{
	INT8U err;
	int IK=0; // Internal Key
	int x =0;


	for (;;)
	{
		if (IK ==0)
		{
			OSSemPend(MM_Lat_VGA,0,&err);
			printf("latency measurement entered\n");
			IK =1;
			x=0;
		}
		else if(IK ==1 )
		{
			if (x == 1)
			{
				IK =0;
				OSSemPost(MM_Lat_VGA);
			}
			wait_PIO_SW();
			x=1;
		}
		OSTimeDlyHMSM(0, 0, 0, 80);
	}
}
/* Elasped Time Measurement*/
void task5(void* pdata)
{
	INT8U err;
	int IK=0; // Internal Key
	int x=0;

	for (;;)
	{
		if (IK ==0)
		{

			OSSemPend(MM_Elapsed,0,&err);
			printf("elapsed time entered\n");
			IK =1;
			x=0;
		}
		else if(IK ==1 )
		{
			if (x == 1)
			{
				IK =0;
				OSSemPost(MM_Elapsed);
			}
			wait_PIO_SW_EN_Check(1);
			usleep(1000000);
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
			image_flash();
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
			wait_SPACE();
			x=1;
		}
		OSTimeDlyHMSM(0, 0, 0, 80);
	}
}

/* SD card existence check*/
/*
void task10(void* pdata)
{
	INT8U err;
	int flag = 0;
	for(;;)
	{
		if(flag ==0)
		{
			OSSemPend(SD_c,0,&err);
			flag=1;
		}
		else if(flag == 1)
		{

			for(;;)
			{
				printf("hello from task10 \n");
				SD_check();
				OSTimeDlyHMSM(0, 0, 5, 0);
			}
		}
		//printf("Hello from task4\n");
		OSTimeDlyHMSM(0, 0, 5, 0);
	}
}
*/
/* The main function creates four tasks and starts multi-tasking */
int main(void)
{

	program_init();
	//OSInit();
	SD = OSSemCreate(0);
	SD_MM = OSSemCreate(0);
	MM_VGAc = OSSemCreate(0);
	//SD_c = OSSemCreate(0);
	MM_Lat_VGA = OSSemCreate(0);
	MM_Elapsed = OSSemCreate(0);

	//printf("Hello world from startup \n");
							/* Creation of Startup Thread */
  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

							/* Creation of Main Menu Thread */
  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
							/* Creation of Experiment Thread */
  OSTaskCreateExt(task3,
                  NULL,
                  (void *)&task3_stk[TASK_STACKSIZE-1],
                  TASK3_PRIORITY,
                  TASK3_PRIORITY,
                  task3_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
						    /* Creation of Latency Measurement Thread */
  OSTaskCreateExt(task4,
                  NULL,
                  (void *)&task4_stk[TASK_STACKSIZE-1],
                  TASK4_PRIORITY,
                  TASK4_PRIORITY,
                  task4_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
						   /* Creation of Elapsed Time Measurement Thread */

  OSTaskCreateExt(task5,
                  NULL,
                  (void *)&task5_stk[TASK_STACKSIZE-1],
                  TASK5_PRIORITY,
                  TASK5_PRIORITY,
                  task5_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  /*
  OSTaskCreateExt(task6,
                  NULL,
                  (void *)&task6_stk[TASK_STACKSIZE-1],
                  TASK6_PRIORITY,
                  TASK6_PRIORITY,
                  task6_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSTaskCreateExt(task7,
                  NULL,
                  (void *)&task7_stk[TASK_STACKSIZE-1],
                  TASK7_PRIORITY,
                  TASK7_PRIORITY,
                  task7_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSTaskCreateExt(task8,
                  NULL,
                  (void *)&task8_stk[TASK_STACKSIZE-1],
                  TASK8_PRIORITY,
                  TASK8_PRIORITY,
                  task8_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
  OSTaskCreateExt(task9,
                  NULL,
                  (void *)&task9_stk[TASK_STACKSIZE-1],
                  TASK9_PRIORITY,
                  TASK9_PRIORITY,
                  task9_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
*/
  /* Creation of SD Detection Thread */
  /*
  OSTaskCreateExt(task10,
                  NULL,
                  (void *)&task10_stk[TASK_STACKSIZE-1],
                  TASK10_PRIORITY,
                  TASK10_PRIORITY,
                  task10_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);
*/

  OSStart();

  return 0;
}



