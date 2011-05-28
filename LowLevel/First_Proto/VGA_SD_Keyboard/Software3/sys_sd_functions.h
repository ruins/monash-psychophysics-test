/*
 * sys_sd_functions.h
 *
 *  Created on: Apr 18, 2011
 *      Author: Shee Jia Chin
 *      Revision: 4.3
 *
 * This Code Interfaces with the SD socket component Nios Softcore Microprocessor intended to be used
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

#define STR_LEN 20
char buffer_name[STR_LEN];

void SD_check();
void SD_open();
int sd_fclose(short int handler, char text[STR_LEN]);
int compare_strings(char A[STR_LEN], char B[STR_LEN]);
int sd_list(short int handler,int check, char text[STR_LEN]);
int sd_write(short int handler, char text[STR_LEN], char write_ascii);
int sd_read(short int handler, char text[STR_LEN]);
int SD_read_all(short int handler,char text[]);
int SD_write_set(short int handler,char text[] );
