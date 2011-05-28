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

int picturenumber;
char open[45] = "/home/cj/Documents/Cdebugging/";
char close[45] = "/home/cj/Documents/Cdebugging/result/";
char filename[45];
char extension[4] = ".txt";
printf("Programming starting...\n");
int counting =0;

for ( picturenumber = 1; picturenumber<=100; picturenumber++)
{ 
sprintf(filename, "%s%d%s", open, picturenumber, extension);
printf("in: %s\n",filename);
fpin=fopen(filename, "r");
sprintf(filename, "%s%d%s", close, picturenumber, extension);
fpout=fopen(filename, "w");
printf("out: %s\n",filename);

//fpin=fopen("/home/cj/Documents/Cdebugging/testA.txt", "r");
//fpout=fopen("/home/cj/Documents/Cdebugging/testB.txt", "w");


            while  ( ( x = fgetc( fpin ) ) != EOF )
            {
		y=x;
		if (x == 10)
		{			
			counting++;
			textcvt = atoi(text);
			//printf("textcvt : %d\n", textcvt);			
			atextcvt = textcvt;
			fprintf(fpout, "%c", textcvt);
			//fprintf(fpout, 10, textcvt);
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
}
printf("%d", counting);
printf("task finished \n\n");

fclose( fpin );
fclose( fpout );

while(1)
{}

return 0;
}

