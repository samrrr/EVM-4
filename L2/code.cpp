#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

#include <signal.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


char*col_names[] = {
	"black",
	"blue",
	"green",
	"cyan",
	"red",
	"magnetta",
	"brown",
	"lightgray",
	"darkgray",
	"lightblue",
	"lightgreen",
	"lightcyan",
	"lightred",
	"lightmagnetta",
	"yellow",
	"white"
};
void gotoxy(int x, int y)
{
	COORD scrn;
	scrn.X = x; scrn.Y = y;
	SetConsoleCursorPosition(hConsole, scrn);
}
void textcol(int t, int b)
{
	SetConsoleTextAttribute(hConsole, WORD(t | (b << 4)));
}

void ob_sgnint(int a)
{
	printf("ok");
	signal(SIGINT, ob_sgnint);
}

void main()
{

	setlocale(0, "RU");

	signal(SIGINT,ob_sgnint);

	HANDLE hCon = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hCon != INVALID_HANDLE_VALUE) {
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;
		cfi.dwFontSize.Y = 24;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = 400;
		wcscpy(cfi.FaceName, L"Lucida Console");
		SetCurrentConsoleFontEx(hCon, FALSE, &cfi);
	}
	
	int x, y,xx;

	x = 10;
	y = 10;
	xx = 0;


	while (1)
	{

		if (_kbhit())
		{
			char ch = getch();

			if (ch == -32)
			{
				ch = getch();
				if (ch == 72)
				{
					xx = -1;
				}
				if (ch == 80)
				{
					xx = 1;
				}
			}

		}
		gotoxy(y, x);
		cout << " ";
		x = x + xx;
		if (x < 0)
			x = 0;
		if (x >= 16)
			x=15;
		gotoxy(y, x);
		cout << "*";



		Sleep(300);

	}




	system("pause");

}
