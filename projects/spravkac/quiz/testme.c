#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#include "rngs.h"
#include <math.h>
#define MAX_LENGTH 6

char inputChar()
{
    // this returns ascii characters that covers just what we need to hit each state
    return floor(Random()*94+32);
}

char *inputString()
{
	static char array[MAX_LENGTH];
	// let's just fuzz with the characters we are looking for, otherwise this will take
	// a very very long time
	char testChars[5] = {'r', 'e', 's', 't', '\0'};
   
	int i=0;
	for (i=0; i< MAX_LENGTH; i++) {
		// just pick from our little set
		array[i] = testChars[(int)floor(Random()*5)];
		// for debugging
		/*
		if (i==MAX_LENGTH-1) {
			array[i] ='\0';
		}
		*/
	}
    return array;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
	//set up random number generator
	SelectStream(1);
	PutSeed((long)time(NULL));
	// PUT test section
	testme();
    return 0;
}
