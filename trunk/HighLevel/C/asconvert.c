//this program, takes in input of a file and converts each number into its corresponding ascii value 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{

FILE *fpin; //input file handler 
FILE *fpout;//ouput file handler 
int x;//buffer for reading text
char y;

int j; //for loop variables

char text[10]; 
int count=0;
char atextcvt;
int textcvt;

printf("Programming starting...\n");

fpin=fopen("/home/cj/Documents/Cdebugging/thesis.txt", "r");
fpout=fopen("/home/cj/Documents/Cdebugging/thesispipe.txt", "w");


            while  ( ( x = fgetc( fpin ) ) != EOF )
            {
		
		y=x;
		if (x == 10)
		{
			//printf("current text is : %s\n", text);
			textcvt = atoi(text);
			atextcvt = textcvt;
			fprintf(fpout, "%c\n", atextcvt);
			count=0; //reset counter
			for (j=0;j<10;j++)text[j]=0;
		}		
		else 
		{
			text[count]=y;
			count++;
		}
		                

		//printf( "%c\n",x);
		
            }
/*
            while  ( ( x = fgetc( fpin ) ) != EOF )
            {
		printf( "%c\n",x);
		
            }
*/
printf("\n");
printf("task finished \n\n");

fclose( fpin );
fclose( fpout );

while(1)
{}

return 0;
}

