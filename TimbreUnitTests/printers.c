// MQX printers  Robert Chapman III  Jun 27, 2012

#include <stdio.h>
#include "printers.h"
 
void print(const char *message)
{
	printf("%s", message);
}

void printHex(unsigned int hex)
{
	printf("%8X ", hex);
}

void printnHex(unsigned int n, unsigned int hex)
{
	if (n)
		printf("%*X ", n, hex);
	else
		printf("%X ", hex);
}

void printDec(unsigned int dec)
{
	printf("%d ", (int)dec);
}

void printDec0(unsigned int dec)
{
	printf("%d", (int)dec);
}

void printBin(unsigned int bin)
{
	int i;

	for(i=0x80; i != 0; i>>=1)
		printf("%1i",(((bin)&i) != 0));
	printf(" ");
}

void printChar(unsigned char ch)
{
	printf("%c",ch);
}

void printHex2(unsigned int hex)
{
	printf(" %02X",hex);
}

void flush(void)
{
	fflush(stdout);
}

void printFloat(float f, int n)
{
	printDec0((int)f);
	print(".");
	while (n--) {
		f *= 10;
		printDec0(((unsigned int)f)%10);
	}
	print(" ");
}
			
