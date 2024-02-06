#include <stdio.h>

/**
 * COSC 3250 - Project 1
 * Program to encode a secret message using a key
 * @author Colby Gustafson
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu
 */

int main(int argc, char **argv)
{
	int sum = 0;
	int key;
	if(argc == 2)
	{
		key = atoi(argv[1]);
	}
	else
	{
		key = 0;
	}
	int inp = getchar();
	//printf("%d", sum);
	
	while(inp != EOF)
	{
		if(inp == '\n')
		{
			printf("\n");
		}
		else if(!isalpha(inp))
		{
			sum += key;
			printf("%c",sum%27+'@');
		}
		else
		{
			int increment = toupper(inp)-'@';
			sum += increment + key;
			if(sum%27 == 0)
			{
				printf("%c", ' ');
			}
			else
			{
				printf("%c", sum%27+'@');
			}
		}
		inp = getchar();
	}
	
	printf("\n");
	return 0;
}
