// this program prints out what it sees in the file called thesispipe.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{

FILE *fpin; //input file handler 
int x;

fpin=fopen("/home/cj/Documents/Cdebugging/thesispipe.txt", "r");


            while  ( ( x = fgetc( fpin ) ) != EOF )
            {
		if(x==10)	printf("\n");
		else 		printf( "%d",x);
		
            }
printf("\n");
fclose( fpin );

while(1){}

return 0;

}
