
#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>

//Функция для проверки является ли числом символ

bool is_digit(unsigned char ch)
{
	return ch >= '0' && ch <= '9';
}

//Функция для проверки в пределе ли от -10000 до -10000 число 

bool in_range(int i)
{
	return i > -10000 && i < 10000;
}

//Функция для расчёта примера: "число" "знак" "число"

int count(int a, char d, int b, bool *error)
{
	if (*error == 1)
		return 0;
	switch (d)
	{
	case '-':
		a = a - b;
		break;
	case '+':
		a = a + b;
		break;
	case '*':
		a = a * b;
		break;
	case '/':
		if (b != 0)
		{
			*error = 0;
			return a / b;
		}
		else
		{
			*error = 1;
			return 0;
		}
		break;
	}
	*error = !in_range(a);
	return a;
}

//Функция для перевода числа из char* в int

int my_atoi(char *in, int len)
{
	if (*in != '-')
	{
		int c = 0;
		while (len > 0)
		{
			len--;
			c = c * 10 + *in - '0';
			in++;
		}
		return c;
	}
	else
		return -my_atoi(in + 1, len - 1);
}

//Функция для вычисления длины компонента

int len_part(char *s)
{
	int i;
	if (*s == '(')//если компонент в скобках
	{
		int sko;

		sko = 1;
		i = 1;
		while (sko > 0)
		{
			s++, i++;
			if (*s == '(')
				sko++;
			if (*s == ')')
				sko--;
		}
	}
	else//если компонент число
	{
		i = 0;
		if (*s == '-')
		{
			i++;
			s++;
		}
		while (is_digit(*s))
			s++, i++;
	}
	return i;
}

//Функция для получения символа. 

char my_getch()
{
	char ch;//Код нажатой кнопки.
	do
	{
		ch = _getch();
		if (ch == '\r')
			ch = '\n';
		else
		if (ch == '\0' || ch == -32)
			_getch();
		else
		if (ch == '\t')
			ch = ' ';
	} while (ch >= -128 && ch <= -17 || ch == '\0' || ch == -32);
	return ch;
}

//Функция для ввода ответа y/n

bool input_yn()
{
	char sy;       //Введённый символ
	char ch;       //Нажатая клавиша

	sy = 0;
	do
	{
		ch = my_getch();
		switch (ch)
		{
		case '\b':
			printf("\b \b");
			sy = 0;
			break;
		case 'y':
		case 'Y':
		case 'n':
		case 'N':
			sy = ch;
			printf("\b%c", sy);
			break;
		}
	} while (ch != '\n' || sy == 0);
	printf("\n");
	return sy == 'y' || sy == 'Y';
}

//Функция для проверки символа на возможность быть в примере

bool is_cou(char ch)
{
	if (ch >= '0' && ch <= '9')
		return 1;
	switch (ch)
	{
	case '(':
		return 1;
	case ')':
		return 1;
	case '*':
		return 1;
	case '/':
		return 1;
	case '+':
		return 1;
	case '-':
		return 1;
	default:
		return 0;
	}
}

//Функция для ввода строки

char *input_string(int size)
{
	char *s;       //Указатель на первый символ исходной строки
	int length = 1;//Длина строки
	char ch;       //Введённый символ
	s = NULL;
	do
	{
		ch = my_getch();
		switch (ch)
		{
		case 27:
			break;
		case '\b':
			if (length > 1)
			{
				printf("\b \b");
				length--;
				s = (char *)realloc(s, sizeof(char)*length);
			}
			break;
		case '\n':
			if (length > 1)
			{
				while (s[length - 2] == ' ')
				{
					length--;
					s = (char *)realloc(s, sizeof(char)*length);
				}
				s[length - 1] = 0;
			}
			break;
		default:
			if (length <= size && (ch != ' ' || length > 1) && is_cou(ch))
			{
				s = (char *)realloc(s, sizeof(char)*(length + 1));
				printf("%c", ch);
				s[length - 1] = ch;
				length++;
			}
			break;
		}
	} while (ch != '\n' || length == 1);
	printf("\n");

	return s;
}

//Функция для расчёта примера

int countthis(char* in, int len, bool *error)
{
	if (*error == 1)
		return 0;
	if (len <= 0)
		return 0;

	int i;      //Локальные переменные
	int fri_len;//Длинна первого компонента
	int sec_len;//Длинна второго компонента
	int fri_mno;//Множитель первого компонента

	if (*in == '-')
	{
		in++;
		fri_mno = -1;
		len--;
	}
	else
		fri_mno = 1;

	fri_len = len_part(in);

	if (fri_len + (1 - fri_mno) / 2 < len)//Больше одного компонента
	{
		if (*(in + fri_len) == '+')
			return count(countthis(in, fri_len, error)*fri_mno, *(in + fri_len), countthis(in + fri_len + 1, len - fri_len - 1, error), error);
		else
			if (*(in + fri_len) == '-')
				return count(countthis(in, fri_len, error)*fri_mno, '+', countthis(in + fri_len, len - fri_len, error), error);
			else
			{
				sec_len = len_part(in + fri_len + 1);

				while ((fri_len + sec_len + 1 < len) && *(in + fri_len) != '+' && *(in + fri_len) != '-')
				{
					fri_len += 1 + sec_len;
					sec_len = len_part(in + fri_len + 1);
				};
				if (*(in + fri_len) == '-')
					return count(countthis(in, fri_len, error)*fri_mno, '+', countthis(in + fri_len, len - fri_len, error), error);
				else
					return count(countthis(in, fri_len, error)*fri_mno, *(in + fri_len), countthis(in + fri_len + 1, len - fri_len - 1, error), error);
			}
	}
	else//1 компонент
		if (*in == '(')
			return fri_mno * countthis(in + 1, len - 2, error);
		else
		{
			*error = !in_range(my_atoi(in, len));
			return my_atoi(in, len)*fri_mno;
		}

}

//Функция для проверки правильности ввода примера

bool is_correct(char *s)
{
	int sko;  //Количество незакрытых скобок
	char last;//Предыдущий символ

	last = '-';
	if (*s == '-')
		*s++;
	sko = 0;
	while (*s)
	{
		switch (*s)
		{
		case '(':
			sko++;
			if (is_digit(last) || last == ')')
				return 0;
			break;
		case ')':
			sko--;
			if (!is_digit(last) && last != ')')
				return 0;
			break;
		case '*':
		case '/':
		case '+':
			if (!is_digit(last) && last != ')')
				return 0;
			break;
		case '-':
			if (!is_digit(last) && last != ')' && last != '(')
				return 0;
			break;
		default:
			if (last == ')')
				return 0;
			if (!is_digit(*s))
				return 0;
			break;
		}
		last = *s;
		s++;
	}
	return sko == 0 && last != '-';
}

int main()
{
	setlocale(0, "RU");
	char *buf = NULL;//Указатель на первый символ введённой строки
	int i;           //Вспомогательная переменная
	bool error;      //Флажок ошибки
	bool exit;       //Флажок выхода
	do
	{
		system("cls");
		puts("Это программа-калькулятор");
		puts("Она работает только с * / + -");
		puts("Умножение надо писать даже там, где оно подразумевается.");
		puts("Программа работает только с целыми числами от -9999 до 9999");
		puts("Вводите пример (например 3+5*(-1) )");
		free(buf);
		buf = input_string(79);
		if (is_correct(buf))
		{
			error = 0;
			i = countthis(buf, strlen(buf), &error);
			if (!error)
				printf("Ответ: %i\n", i);
			else
				puts("При вычислении возникла ошибка");
		}
		else
			puts("Пример был введён неверно");
		puts("Продолжить работу?(y/n)");
		exit = !input_yn();
	} while (!exit);
	free(buf);
}



