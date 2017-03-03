#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

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
void main()
{

	setlocale(0, "RU");


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
	textcol(4, 3);
	gotoxy(10, 10);


	string text = "setInterval(function() {players[0].x += Math.random() * 10 - Math.random() * 10;players[0].y += Math.random() * 10 - Math.random() * 10;if (players[0].x< 10)players[0].x = 10;if (players[0].x>100)players[0].x = 100;if (players[0].y< 10)players[0].y = 10;if (players[0].y>100)players[0].y = 100;socket.broadcast.json.send({ 'event': 'players', 'text' : players });//alert('Прошла 1 секунда');}";

	int outk = 0;
	int x1, x2, y1, y2;

	x1 = 10;
	y1 = 12;
	x2 = 70;

	y2 = 23;

	for (int r = 0; r < 16; r++)
	{
		textcol((r+3)%16, r);
		cout << col_names[r] << endl;
	}
	system("pause");

	int str_out = y1;

	for (int i = 0; i < 8; i++)
	{
		for (int r = 0; r < 16; r++)
		{
			//system("cls");
			textcol(15, 0);
			gotoxy(0, 0);
			cout << "                                           ";
			gotoxy(0, 1);
			cout << "                                           ";
			gotoxy(0, 0);
			cout << "text color:" << col_names[i];
			gotoxy(0, 1);
			cout << "text color:" << col_names[r];
			int y = str_out;
				for (int x = x1; x <= x2; x++)
				{
					gotoxy(x, y);
					textcol(i,r);
					cout << text[outk];
					outk++;
					if (outk >= text.length())
						outk=0;
				}
			str_out--;
			if (str_out < y1)
				str_out = y2;
			Sleep(300);
		}
	}



	cout << "ok";

	system("pause");

}
