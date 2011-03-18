/*
 * sys_functions.h
 *
 *  Created on: Mar 18, 2011
 *      Author: cj
 */

#ifndef SYS_FUNCTIONS_H_
#define SYS_FUNCTIONS_H_


#endif /* SYS_FUNCTIONS_H_ */

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       4096
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];
OS_STK    task4_stk[TASK_STACKSIZE];

/* Definition of Semaphore */
OS_EVENT *SD;
OS_EVENT *SD_c;
OS_EVENT *SD_MM;
OS_EVENT *MM_VGAc;


/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
#define TASK4_PRIORITY      4

void simple_irq();
void sw1_irq();
void ps2_irq();
void sw16_irq();
void program_init();

void wait_ENTER();
int wait_New_or_Current();
void wait_M_or_N();
void wait_SPACE();
void wait_BackSpace();
void wait_Text();
int wait_Main_Menu();
int wait_left_right();

void ps2_clear();

void SD_open();
void SD_read();
void SD_subread();
void SDram_to_VGA_back_buffer();

void SD_check();
void text_subject();
int MM();
void image_flash();
int loop();
int image_select();
void SD_text_begin();
void SD_text_mid();
void SD_text_end();

