#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	/*
		On the ASCII table, 32 = Space and 125 = }
		Values above and below this are not desired for this code.
		We will use rand() to select a number to add to 32 (including 0) to select
		a random character in this range of ASCII 32 - 125.
	*/
	int range = 125 - 32 + 1; // +1 ensures 32 and 125 are both valid.
	char c = 32 + (rand() % range);
	
    return c;
}

char *inputString()
{
	static char str[6];
	str[5] = '\0';
	
	/*
		On the ASCII table, 97 = a and 122 = z
		We will only use this range of alphabet characters. 
		Using the full ASCII range may cause a very long time for the string "reset\0" to generate randomly.
		The final position will be set to '\0' manually.		
	*/
	/*
		Update: After letting this run per the above description for over 7.5 million iterations, "reset\0"
		never occurred. I will now refine this to only include letters 'e' through 't' and test again.
	*/
	int range = 116 - 101 + 1; // +1 ensures 'e' and 't' are both valid.
	char c;
	
	for (int i = 0; i < 5; i++) {
		c = 101 + (rand() % range);
		str[i] = c;
	}
	return str;
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
    srand(time(NULL));
    testme();
    return 0;
}
