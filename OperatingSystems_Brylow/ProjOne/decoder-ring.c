#include <stdio.h>

/**
 * COSC 3250 - Project 1
 * Program to decode secret messages using key
 * @author Colby Gustafson
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu
 */

int main(int argc, char **argv)
{
	int inp = 0;
	int enc = 0;
	int key = 0;
	int prev = 0;
	int tran = 0;
		
	if(argc == 2)
	{
		key = atoi(argv[1]);
	}
	else
	{
		key = 0;
	}
	inp = getchar();
	
	while(inp != EOF)
	{	
		if(inp == '\n')
		{
			printf("\n");
		}
		else
		{
			if(!isalpha(inp))
			{
				inp = '@';
			}
			tran = abs(inp - '@');
			enc = (((tran - prev - key)%27)+27)%27;
			enc += '@';
			
			if(!isalpha(enc))
			{
				enc = ' ';
			}
			printf("%c", enc);
			prev = tran;
		}
		inp = getchar();
	}	
	printf("\n");
	return 0;
}
