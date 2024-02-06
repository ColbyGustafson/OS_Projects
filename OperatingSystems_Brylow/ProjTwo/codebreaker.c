/**
 * COSC 3250 - Project 2
 * Program to break cypher of encoded message by determining key value
 * @author Colby Gustafson
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu
 */

#include <stdio.h>
#define LINE_MAX 129

void readline(char line[], int size);
void decode(char input[], char output[], int size, int key);

int main()
{
	int bestKey = 0;
	int mostE = -1;
	int secMostE = -1;
	int secBestKey = 0;
	char output[LINE_MAX];
	int testKey;
	int n;
	
	//read input into "inputline[]"
	char inputline[LINE_MAX];
	readline(inputline, LINE_MAX);
		
	for(testKey = 0; testKey < 27; testKey++)
	{
		//decode msg
		decode(inputline, output, LINE_MAX, testKey);
		
		//count # of ‘E’s in decoded msg
		int countE = 0;
		for(n = 0; n<LINE_MAX; n++)
		{
			if(output[n] == 'E')
			{
				countE++;
			}
		}
		//find key with the 2nd highest # of 'E's
		if(countE > mostE)
		{
			secBestKey = bestKey;
			bestKey = testKey;
			secMostE = mostE;
			mostE = countE;
		}
		else if(countE > secMostE)
		{
			secMostE = countE;
			secBestKey = testKey;
		}
	}
	
	// Use key to decode and print msg
	printf("Most probable key is %d", secBestKey);
	decode(inputline, output, LINE_MAX, secBestKey);
	printf("\n%s", output);
	return 0;
}

//decodes message with a specified key
void decode(char input[], char output[], int size, int key)
{
	int i = 0;
	int tran = 0;
	int prev = 0;
	
	for(i=0; i<size && input[i] != '\0'; i++)
	{
		if(input[i] == '\n')
		{
			output[i] = '\n';
		}
		else
		{
			char ch = toupper(input[i]);
			int value =  ch  - ('A' - 1);
			if (ch == ' ') value = 0;
			tran = (((value - prev - key)%27)+27)%27;
			output[i] = tran + 'A' - 1;
			if (tran == 0)
				output[i] = ' ';
			prev = value;	
		}
	}
	output[i] = '\0';
}

//reads in a user's input 
void readline(char line[], int size)
{
	int ch = 0; 
	int i = 0;

	for(i = 0; i<LINE_MAX && (ch = getchar()) != EOF; i++)
	{
		line[i] = ch;
	}
	
	line[i] = '\0';
}
