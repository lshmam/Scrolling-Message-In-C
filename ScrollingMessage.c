/*
Name: Aminul Islam
ID: 84166685
Lab Section: L1B
Date: 6/20/19
Purpose: Scrolling Message including the bonus part
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <DAQlib.h>
#include <math.h>

#define NUM_DISPLAYS 8
#define SPACE_CHAR 0

#define SWITCH0 0
#define SWITCH1 1

#define ON 1
#define OFF 0

#define ONE_SECOND 1000

void helloMessage(void);
void blueMessage(void);
void messageWriter(int data[], int size);
void sleepTimer(void);

int main(void)
{
	int setupNum;
	printf("Enter 0 for hardware and 4 DAQ simulator: ");
	scanf("%d", &setupNum);

	if (setupDAQ(setupNum))
	{
		int choice;
		printf("Press 1 to display 'Hello' or 2 to display 'blue': ");
		scanf("%d", &choice);
		
		//Chooses which message to display
		if (choice == 1)
		{
			helloMessage();
		}
		else
		{
			blueMessage();
		}
		
	}

	else
	{
		printf("Cannot initialize DAQ.");
	}

	system("Pause");
	return 0;
}

//If you would like to add a message, put the characters in an array, find its size using displaySize and call messageWriter
void helloMessage(void)
{
	int helloCharacters[] = { 0b01101110, 0b11011110, 0b00001100, 0b00001100, 0b11111100 };
	int displaySize = sizeof(helloCharacters) / sizeof(helloCharacters[0]);

	messageWriter(helloCharacters, displaySize);
}

//Second message
void blueMessage(void)
{
	int blueCharacters[] = { 0b00111110, 0b00001100, 0b00111000, 0b11011110 };
	int displaySize = sizeof(blueCharacters) / sizeof(blueCharacters[0]);

	messageWriter(blueCharacters, displaySize);
}


//Main workfunction
void messageWriter(int data[], int size)
{
	int col = 0, pos = 0, row = 0;
	
	while (continueSuperLoop())
	{
		if (digitalRead(SWITCH0) == ON) //Checks if direction switch is ON/OFF
		{
			while (col - 1 <= row) //Works until blank display
			{
				if (col >= size)
				{
					displayWrite(SPACE_CHAR, pos);//Clears display after message moves
				}
				else
				{
					displayWrite(data[col], pos);
				}
				displayWrite(SPACE_CHAR, row + 1);
				col++;
				pos--;
			}
			sleepTimer();
			row = (row - 1);
			if (row == -2) //Used to wrap around text while going reverse
			{
				Sleep(ONE_SECOND);//Sleeps for a second when there is a blank screen
				row = size + NUM_DISPLAYS;
			}
			pos = row;
			col = 0;
		}
		else
		{
			//Used for moving to the left
			while (pos >= 0)
			{
				if (col >= size)
				{
					displayWrite(SPACE_CHAR, pos); //Displays blanks after message has been displayed
				}
				else
				{
					displayWrite(data[col], pos);
				}
				displayWrite(SPACE_CHAR, pos - size); 	//Displays blank characters
				pos--;
				col++;

			}
			sleepTimer();
			row = (row + 1) % (size + NUM_DISPLAYS); //Wraps around text
			if (row == 0)
			{
				Sleep(ONE_SECOND);//Sleeps for a second with a blank display
			}
			pos = row;
			col = 0;
		}

	}
}

//Used to figure out how long to sleep
void sleepTimer(void)
{
	if (digitalRead(SWITCH1) == ON)
	{
		return Sleep(500);
	}
	else
	{
		return Sleep(1000);
	}
}


