/**
 * COSC 3250 - Project 3
 * Program to break cypher of encoded message by determining key value
 * @authors Colby Gustafson & Noah Bayer
 * Instructor Dennis Brylow
 * TA-BOT:MAILTO colby.gustafson@marquette.edu & noah.bayer@marquette.edu
 */

#include <xinu.h>
#define LINE_MAX 129

void readline(char line[], int size);
void decode(char input[], char output[], int size, int key);

process main(void)
{
	int bestKey = 0;
	int mostE = -1;
	int secMostE = -1;
	int secBestKey = 0;
	char output[LINE_MAX];
	int testKey;
	int n;
	kprintf("Hello Xinu World!\n");
	kprintf("Enter your codebreaker text:\n");

    // TODO: Put your main program for codebreaker here.
    //       Replace any calls to printf() with kprintf().
    //       Replace any calls to getchar() with kgetc().

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
	kprintf("Most probable key is %d", secBestKey);
	decode(inputline, output, LINE_MAX, secBestKey);
	kprintf("\n%s", output);

	kprintf("===TEST END===");
	while (1)
	    ;
	
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

	for(i = 0; i<LINE_MAX && (ch = kgetc()) != EOF; i++)
	{
		line[i] = ch;
	}
	
	line[i] = '\0';
}
