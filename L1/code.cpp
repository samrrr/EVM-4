#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <string>
#include <vector>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);



class TYPE_DATA
{
public:
	string name,print_st;
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
			res = (char)(buf[i]&(1<<r)) + res;
		}

		for (int i = 0; i < res.length(); i++)
		{
			SetConsoleTextAttribute(hConsole, (WORD)(5));
			if (i <= p_bytes)
				SetConsoleTextAttribute(hConsole, (WORD)(4));
			if (i == 0)
				SetConsoleTextAttribute(hConsole, (WORD)(7));
			printf("%c", res[i]?'1':'0');
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
			en = - 1;
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
		if (i>0 && i == str.length()-1 && str[i] == ' ')
		{
			str.erase(i, 1);
			i-=2;
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
	SetConsoleTextAttribute(hConsole, (WORD)(15));


	int i;     //Введённое число
	int i_res; //Результат обработки числа
	bool is_input; //Флажок ввода
	bool is_res;   //Флажок результата
	int menu;      //Переменная выбранного пункта меню

	is_input = 0;
	is_res = 0;
	i = 0;
	i_res = 0;

	char buf[1024];
	bool is_exit = 0;

	do
	{
		system("cls");
		printf("Число:");
		number.print_base();
		printf("\n");
		printf("Битовое представление:\n");
		number.print_bytes();
		printf("\n");
		printf("Тип:");
		printf(number.name.data());
		printf("\n");
		puts("Комманды:");
		puts("chnt-сменить тип переменной");
		puts("set <n>-Сменить значение на <n>");
		puts("setb <bits>-Сменить биты на <bits>");
		puts("inv <en> <co>-инверсия <co> битов заканчивая битом <en>");
		puts("exit-выход");

		gets_s(buf, 1024);

		auto arr_com = split_command(buf);

		if (arr_com.size() > 0)
		{
			if (arr_com[0] == "exit")
				is_exit = 1;
			if (arr_com[0] == "chnt")
			{
				if (number.name == "double")
				{
					number.name = "short int";
					number.bytes = sizeof(short int);
					number.buf = new byte[sizeof(short int)];
					for (int i = 0; i < sizeof(short int);i++)
						number.buf[i] = -1;
					number.p_bytes = 0;
					number.print_st = "%hd";
				}
				else
				{
					number.name = "double";
					number.bytes = sizeof(double);
					number.buf = new byte[sizeof(double)];
					for (int i = 0; i < sizeof(double); i++)
						number.buf[i] = 0;
					number.p_bytes = 11;
					number.print_st = "%lf";


				}
			}
		}
		if (arr_com.size() > 1)
		{
			if (arr_com[0] == "set")
			{
				sscanf(arr_com[1].data(), number.print_st.data(), number.buf);

			}
			if (arr_com[0] == "setb")
			{
				for (int i = number.bytes * 8 - 1; i >= 0; i--)
					number.buf[i / 8] = 0;


				for (int i = number.bytes * 8 - 1, r = 0; i >= 0 && r < arr_com[1].size(); i--, r++)
				{
					if (arr_com[1][r] == '1')
						number.buf[i / 8] ^= (1 << (i % 8));
				}

			}
		}
		if (arr_com.size() > 2)
		{
			int pos = atoi(arr_com[1].data());
			int num = atoi(arr_com[2].data());
			
			number.invert_bits(pos, num);

		}

	} while (!is_exit);
}


