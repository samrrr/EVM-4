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



class TYPE_DATA
{
public:
	string name, print_st;
	int bytes;
	int p_bytes;
	byte *buf;

	TYPE_DATA()
	{
		buf = nullptr;
	}

	~TYPE_DATA()
	{
		delete[]buf;
		buf = nullptr;
	}

	void print_bytes()
	{
		string res;

		for (int i = 0; i < bytes; i++)
			for (int r = 0; r < 8; r++)
			{
				res = (char)(buf[i] & (1 << r)) + res;
			}

		for (int i = 0; i < res.length(); i++)
		{
			SetConsoleTextAttribute(hConsole, (WORD)(5));
			if (i <= p_bytes)
				SetConsoleTextAttribute(hConsole, (WORD)(4));
			if (i == 0)
				SetConsoleTextAttribute(hConsole, (WORD)(7));
			printf("%c", res[i] ? '1' : '0');
		}
		SetConsoleTextAttribute(hConsole, (WORD)(15));
	}
	void print_base()
	{
		if (print_st.length()>0)
		{
			if (bytes == 1)
				printf(print_st.data(), *(byte*)(buf));
			if (bytes == 2)
				printf(print_st.data(), *(short*)(buf));
			if (bytes == 4)
				printf(print_st.data(), *(int*)(buf));
			if (bytes == 8)
				printf(print_st.data(), *(long long*)(buf));
		}
	}
	void invert_bits(int en, int count)
	{
		if (en >= bytes * 8)
			en = -1;
		for (int i = en; i > en - count && i >= 0; i--)
		{
			buf[i / 8] ^= (1 << (i % 8));
		}

	}
};


vector < string > split_command(string str)
{
	vector < string > res;
	for (int i = 0; i < str.length(); i++)
	{
		if (i == 0 && str[i] == ' ')
		{
			str.erase(i, 1);
			i--;
		}
		if (i>0 && i == str.length() - 1 && str[i] == ' ')
		{
			str.erase(i, 1);
			i -= 2;
		}
		if (i > 0 && str[i] == ' ' && str[i - 1] == ' ')
		{
			str.erase(i, 1);
			i--;
		}
	}

	str += ' ';

	string resres;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ')
		{
			res.push_back(resres);
			resres = "";
		}
		else
		{
			resres += str[i];
		}
	}


	return res;
}

class PIX
{
public:
	char sym;
	char color;
	char back_color;
	char is_mig;
	PIX()
	{
		sym =  '#';
		color = 15;
		back_color = 0;
		is_mig = 1;
	}
};

class WIND
{
public:
	vector<PIX> arr;
	int sizex;
	int sizey;
	WIND()
	{
		sizex = 0;
		sizey = 0;
	}
	WIND(int x,int y)
	{
		sizex = x;
		sizey = y;

		arr.resize(x*y);
	}
	
	void resize(int x, int y)
	{
		vector < PIX > newarr;
		newarr.resize(x*y);
		for (int i = 0; i < sizex&&i<x; i++)
			for (int r = 0; r < sizey&&r < y; r++)
			{
				newarr[i + r*x] = arr[i + r*sizex];
			}
		sizex = x;
		sizey = y;
		arr = newarr;
	}

	void put(int x, int y,int offsetx,int offsety)
	{
		
	}
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

	TYPE_DATA number;
	number.bytes = 0;
	number.name = "none";
	number.print_st = "";
	number.buf = nullptr;

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
	cout << "ok";

	system("pause");

}
