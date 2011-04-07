/*
 * sys_functions.h
 *
 *  Created on: Mar 18, 2011
 *      Author: cj
 *
 *
 *
 * This Code Interfaces with a Nios Softcore Microprocessor intended to be used
 * With the DE2 Hardware FPGA (Field Programmable Gate Array)
 *
 * This is the Header file which links to functions which are tailored for the
 * designed Psychophysics Experiment to be tied in with the
 * Monash University Australia Bionic Eye Project.
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
 *
 *
 */

#ifndef SYS_FUNCTIONS_H_
#define SYS_FUNCTIONS_H_


#endif /* SYS_FUNCTIONS_H_ */

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];
OS_STK    task4_stk[TASK_STACKSIZE];
OS_STK    task5_stk[TASK_STACKSIZE];
OS_STK    task6_stk[TASK_STACKSIZE];
OS_STK    task7_stk[TASK_STACKSIZE];
OS_STK    task8_stk[TASK_STACKSIZE];
OS_STK    task9_stk[TASK_STACKSIZE];
OS_STK    task10_stk[TASK_STACKSIZE];


/* Definition of Semaphore */
OS_EVENT *SD;
OS_EVENT *SD_c;
OS_EVENT *SD_MM;
OS_EVENT *MM_VGAc;
OS_EVENT *MM_Lat_VGA;
OS_EVENT *MM_Elapsed;


/* Definition of Task Priorities */
#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      3
#define TASK4_PRIORITY      4
#define TASK5_PRIORITY      5
#define TASK6_PRIORITY      6
#define TASK7_PRIORITY      7
#define TASK8_PRIORITY      8
#define TASK9_PRIORITY      9
#define TASK10_PRIORITY     10

#define PICTURE_NUMBER 20
/* Function for Handling (ISR) Interrupts */
void simple_irq();
void sw1_irq();
void ps2_irq();
void sw16_irq();
/* Program Initialisation Program*/
void program_init();
/* Functions for handling PS2 Keyboard Signals*/
void wait_ENTER();
int wait_New_or_Current();
void wait_M_or_N();
void wait_SPACE();
void wait_BackSpace();
void wait_Text();
int wait_Main_Menu();
char wait_left_right();
/**/
void wait_PIO_SW();
void wait_PIO_SW_EN_Check(int set);
/* Function to Function PS2 Flags */
void ps2_clear();
/* PS2 Data to Ascii Transformation function */
void PS2Dat2Ascii(char ascii);
/* SD Card Functions */
void SD_open();
void SD_read();
void SD_subread();
void SD_check();
void SD_write(char result[PICTURE_NUMBER]);
void write_buffer(char result);
/*
 * Memory interface Function to write from SDram to VGA Back buffer
 * which is actually the SRAM
*/
void SDram_to_VGA_back_buffer(int set);
/* VGA Interface with PS2 Keyboard Interface Functions*/
int loop();
char image_select();
void text_subject();
int MM();
/* VGA Interface Functions*/
void image_flash();
/* VGA with SD Interface Functions*/
void SD_text_begin();
void SD_text_mid();
void SD_text_end();

