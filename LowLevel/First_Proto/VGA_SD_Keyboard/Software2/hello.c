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
#include "sys_functions.h"


#if OS_CRITICAL_METHOD == 3
OS_CPU_SR cpu_sr;
#endif
/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{

	INT8U err;
	//int x = 0;
  for (;;)
  {
//    OSSemPend(SD,0,&err);
    printf("Hello from task1\n");
    SD_open();
    SD_text_begin();
/*
    OSTaskSuspend(2);
    OSTaskSuspend(3);
    OSTaskSuspend(4);
*/

    SD_subread();
//	IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
    SD_read();
//	IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
/*
    OSTaskResume(2);
    OSTaskResume(3);
    OSTaskResume(4);
*/
    SD_text_mid();
    OS_ENTER_CRITICAL();
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
    SDram_to_VGA_back_buffer();
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
	OS_EXIT_CRITICAL();
    SD_text_end();
    OSSemPost(SD_MM);
    OSSemPost(SD_c);
    OSSemPend(SD,0,&err);
    OSTimeDlyHMSM(0, 0, 1, 0);
  }
}

/* Prints "Hello World" and sleeps for three seconds */
void task2(void* pdata)
{
	int flag1=0;
	int flag2=0;
	int x = 0;
	INT8U err;
	for (;;)
	{
	    if (flag1 ==0)
	    {
	    	OSSemPend(SD_MM,0,&err);
	    	flag1 =1;
	    }
	    else if(flag1 ==1 )
		{
			if (x == 3)
			{
				//printf("Enters Here \n");
				x =0;
				flag2 = 1;
				OSSemPost(MM_VGAc);
			}
			else if(x==0)
			{
				x = MM();
			}
			if (flag2 == 1)
			{
				OSSemPend(MM_VGAc,0,&err);
				flag2= 0;

			}


			//printf("return value %d\n",x);
	    	//printf("Hello from task2\n");




			OSTimeDlyHMSM(0, 0, 0, 50);
		}



	}
}
/* Prints "Hello World" and sleeps for three seconds */
void task3(void* pdata)
{
	int subject_flag = 0;
	int flag=0;
	int x = 0;
	int select =0;
	INT8U err;
	for (;;)
	{
		if (flag ==0)
		{
			OSSemPend(MM_VGAc,0,&err);
			flag =1;
			x=0;
		}
		else if(flag ==1 )
		{
			//printf("Hello from task3\n");
			if (subject_flag == 0)
			{
				subject_flag = 1;
				text_subject();
			}
			image_flash();
			select = image_select();
			if(select == 0) printf("selected right\n");
			else if(select == 1) printf("selected left\n");
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x1);
			SDram_to_VGA_back_buffer();
			IOWR_ALTERA_AVALON_PIO_DATA(PIO_O_EN_BASE, 0x0);
			x = loop();
			if (x == 1)
			{
				subject_flag = 0;
				flag =0;
				OSSemPost(MM_VGAc);
			}
		}
		OSTimeDlyHMSM(0, 0, 0, 50);
	}
}
/* Prints "Hello World" and sleeps for three seconds */
void task4(void* pdata)
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
				//printf("hello from task4 \n");
				SD_check();
				OSTimeDlyHMSM(0, 0, 5, 0);
			}
		}
		//printf("Hello from task4\n");
		OSTimeDlyHMSM(0, 0, 5, 0);
	}
}
/* The main function creates two task and starts multi-tasking */
int main(void)
{
	program_init();
	OSInit();
	SD = OSSemCreate(0);
	SD_MM = OSSemCreate(0);
	MM_VGAc = OSSemCreate(0);
	SD_c = OSSemCreate(0);
  OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);


  OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task3,
                  NULL,
                  (void *)&task3_stk[TASK_STACKSIZE-1],
                  TASK3_PRIORITY,
                  TASK3_PRIORITY,
                  task3_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

  OSTaskCreateExt(task4,
                  NULL,
                  (void *)&task4_stk[TASK_STACKSIZE-1],
                  TASK4_PRIORITY,
                  TASK4_PRIORITY,
                  task4_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);


  OSStart();

  return 0;
}



