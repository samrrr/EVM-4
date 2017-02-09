#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>

typedef struct
{
	char *name;//Указатель на первый символ строки, содержащий имя кораблика
	char *type;//Указатель на первый символ строки, содержащий тип кораблика
	int x;     //Положение кораблика по x
	int y;     //Положение кораблика по y
	int z;     //Положение кораблика по z
}DATA;

typedef struct ST1
{
	DATA info;
	struct ST1 *link1;//Ссылка на следующий элемент
} OSHIP;

#define S_input_nt_help "Запрещено:\n  1. Вводить символы кириллицы\n  2. Вводить пробел в начале\n  3. Вводить несколько пробелов подряд\n  4. Вводить какие либо символы, кроме английских букв, пробела и\n     круглых скобок\nВвод запрещённых символов будет игнорироваться.\nПробел в конце будет игнорироваться.\nВводимая строка должна состоять из одного или нескольких слов,\nпервое из которых должно начинаться с строчной буквы.\nВсе строчные буквы в слове после первой будут сделаны прописными.\nВводимая строка может содержать от 1 до 50 символов.\nПри достижении максимального количества символов ввод\nсимволов будет игнорироваться.\nДля окончания ввода нажмите Enter\n"
#define S_input_number "Разрешено вводить только цифры и '-'.\nОбязательно надо ввести число.\n"

/*
Описание: Функция для получения символа.
Возврат: Введённый символ.
*/
char my_getch()
{
	char ch;//Код нажатой кнопки.
	do
	{
		if ((ch = _getch()) == '\r')
			ch = '\n';
		else
			if (ch == '\0' || ch == -32)
			{
				if (kbhit())
					_getch();
			}
			else
				if (ch == '\t')
					ch = ' ';
	} while (ch >= -128 && ch <= -17 || ch == '\0' || ch == -32);
	return ch;
}

/*
Описание: Функция для ввода целого числа.
Возврат: Введённое число.
*/
int input_number()
{
	int number;    //Введённое число	
	char *s;       //Указатель на первый символ исходной строки
	int length = 1;//Длина строки
	char ch;       //Введённый символ
	s = (char*)malloc(sizeof(char));
	do
	{
		switch (ch = my_getch())//Получаем символ нажатой кнопки
		{
		case '\b':
			if (length > 1)
			{
				printf("\b \b");
				length--;
				s = (char *)realloc(s, sizeof(char)*length);
			}
			break;
		case '\n':
			s[length - 1] = '\0';
			break;
		default:
			if ((ch >= '0' && ch <= '9' || ch == '-' && length == 1) && length <= 7)
			{
				s = (char *)realloc(s, sizeof(char)*(length + 1));
				printf("%c", ch);
				s[length - 1] = ch;
				length++;
			}
			break;
		}
	} while (ch != '\n' || length == 1 || length == 2 && *s == '-');
	number = atoi(s);
	free(s);
	printf("\n");
	return number;
}

/*
Описание: Функция для ввода ответа y/n.
Возврат: 1 - 'y', 0 - 'n'.
*/
bool input_yn()
{
	char sy;       //Введённый символ
	char ch;       //Нажатая клавиша

	printf("Ваш выбор(Можно вводить только 'y' и 'n'): ");

	sy = '\0';

	do
	{

		switch (ch = my_getch())//Получаем символ нажатой клавиши
		{
		case '\b':
			if (sy != '\0')
			{
				printf("\b \b");
				sy = '\0';
			}
			break;
		case 'y':
		case 'Y':
		case 'n':
		case 'N':
			if (sy != '\0')
				printf("\b \b");
			printf("%c", sy = ch);
			break;
		}
	} while (ch != '\n' || sy == '\0');
	printf("\n");
	return sy == 'y' || sy == 'Y';
}

/*
Описание: Функция для проверки, является ли разрешённым символ.
Возврат: 1 - является, 0 - не является.
*/
bool is_allowed(char ch, char *not_allowed)
{
	bool b;//Флажок совпадения символа с запрещённым
	for (b = 0; !b && *not_allowed != '\0'; not_allowed++)
		b = ch == *not_allowed;
	return !b;
}

/*
Описание: Функция для ввода строки.
Возврат: Указатель на первый символ введённой строки.
*/
char *input_string(int size, char *not_allowed)
{
	char *s;       //Указатель на первый символ исходной строки
	int length = 1;//Длина строки
	char ch;       //Введённый символ
	s = NULL;
	do
	{
		switch (ch = my_getch())
		{
		case '\b':
			if (length > 1)
			{
				printf("\b \b");//Стираем 1 символ
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
				s[length - 1] = '\0';
			}
			break;
		default:
			if (is_allowed(ch, not_allowed) && length <= size && (ch != ' ' || length > 1 && s[length - 2] != ' '))
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

/*
Описание: Функция для вычисления длины строки.
Возврат: Длина строки.
*/
int str_len(char*s)
{
	int i = 1;//Длина строки
	while (*s)
		i++, s++;
	return i;
}

/*
Описание: Функция для проверки, является ли символ прописной буквой.
Возврат: 1 - является, 0 - не является.
*/
bool is_sm_letter(char ch)
{
	return ch >= 'a' && ch <= 'z';
}

/*
Описание: Функция для проверки, является ли символ строчной буквой.
Возврат: 1 - является, 0 - не является.
*/
bool is_bi_letter(char ch)
{
	return ch >= 'A' && ch <= 'Z';
}

/*
Описание: Функция для коррекции введённой строки.
Возврат: Функция не возвращает значения.
*/
void correct_name(char* s)
{
	if (s == NULL)
		return;
	if (*s == 0)
		return;

	char ra = 'A' - 'a';//Разница в кодах прописных и строчных букв
	bool is_p_let;      //Флажок предыдущего символа(1-предыдущий символ буква 0-предыдущий символ не буква)

	if (is_sm_letter(*s))
	{
		*s += ra;//Делаем из прописной буквы строчную
		is_p_let = 1;
	}
	else
		is_p_let = is_bi_letter(*s);

	for (s = s + 1; *s; s++)
		if (is_p_let)
			if (is_bi_letter(*s))
				*s = *s - ra;//Делаем из строчной буквы прописную
			else
				is_p_let = is_sm_letter(*s);
		else
			is_p_let = is_sm_letter(*s) || is_bi_letter(*s);
}

/*
Описание: Функция для ввода имени.
Возврат: Указатель на первый символ введённой строки.
*/
char* get_nt(char *title, char* name)
{
	char *s;  //Указатель на первый символ имени
	bool exit;//Флажок выхода
	exit = 0;
	do
	{
		system("cls");
		puts(title);
		printf(S_input_nt_help);
		printf("Вводите %s:\n", name);
		s = input_string(50, "\x1b[]{}\\|*/-+,.^%$#@!&_=:;\"'1234567890`~");
		correct_name(s);
		printf("Распознано как:\n%s\n", s);
		puts("Желаете вести ещё раз?('y'-да 'n'-нет)");
		if (input_yn())
		{
			free(s);
			s = NULL;
		}
		else
			exit = 1;
	} while (!exit);
	return s;
}


/*
Описание: Функция для ввода типа кораблика.
Возврат: Указатель на первый символ введённой строки.
*/
char* get_type(char* title)
{
	char *types[] = { "Unknown", "Pinnace", "Rowbarge",
		"Galleon", "Shallop", "Bark", "Galliot", "Brigantine",
		"Galley", "Hoy", "Ketch", "Ship", "Sloop", "Fireship",
		"Ship of the Line", "Bomb Vessel", "Storeship", "Snow",
		"Yacht", "Praam" };
	//Массив указателей на первый символ строки
	int i;    //Вспомогательная переменная
	char *s;  //Указатель на первый символ вводимой строки
	char type;//Тип кораблика
	char len; //Длина строки
	puts("Выберите тип:");
	for (i = 0; i < 20; i++)
		printf("%i-%s\n", i + 1, types[i]);
	puts("другое число-ввод типа вручную");
	printf("Ваш выбор: ");
	type = input_number();
	if (type <= 0 || type > 20)
	{
		system("cls");
		s = get_nt(title, "тип");//Ввод типа вручную
	}
	else
	{
		len = str_len(types[type - 1]);
		s = (char*)malloc(sizeof(char)*len);
		for (i = 0; i < len; i++)
			s[i] = types[type - 1][i];
	}
	return s;
}

/*
Описание: Функция для ввода координат.
Возврат: Введённая координата.
*/
int get_coord(char* title, char ch)
{
	puts(title);
	printf(S_input_number);
	printf("Вводите положение по оси %c в сантиметрах (целое число от -999999 до 9999999):\n", ch);
	return input_number();
}

/*
Описание: Функция получения полей структуры.
Возврат: Функция не возвращает значения.
*/
void get_struct_info(DATA *info)
{
	system("cls");
	info->name = get_nt("Ввод данных кораблика", "имя");
	system("cls");
	info->type = get_type("Ввод данных кораблика");
	system("cls");
	info->x = get_coord("Ввод данных кораблика", 'x');
	system("cls");
	info->y = get_coord("Ввод данных кораблика", 'y');
	system("cls");
	info->z = get_coord("Ввод данных кораблика", 'z');
}

/*
Описание: Функция для вывода информационных полей одной структуры.
Возврат: Функция не возвращает значения.
*/
void O_print_struct(OSHIP *ship)
{
	printf("Имя кораблика: \"%s\"\n", ship->info.name);
	printf("Тип кораблика: \"%s\"\n", ship->info.type);
	printf("Положение кораблика (x,y,z) в сантиметрах: (%i,%i,%i)\n", ship->info.x, ship->info.y, ship->info.z);
}

/*
Описание: Функция для вывода списка.
Возврат: Функция не возвращает значения.
*/
void O_print(OSHIP *ship)
{
	int i;//Номер кораблика
	if (ship != NULL)
	{
		i = 0;
		do
		{
			if (i > 0 && i % 3 == 0)
			{
				system("pause");
				system("cls");
			}
			printf("Кораблик номер %i\n", i + 1);
			O_print_struct(ship);
			puts("");
			ship = ship->link1;//Переходим к следующему символу
			i++;
		} while (ship != NULL);
		puts("Вывод окончен");
	}
	else
		puts("Стек пуст");

}

/*
Описание: Функция для освобождения памяти из-под всего списка.
Возврат: Эта функция возвращает NULL.
*/
OSHIP* O_free_all(OSHIP* ship)
{
	OSHIP *link;//Указатель на элемент, из-под которого освобождается память
	while (ship != NULL)
	{
		link = ship;//Сохраняем указатель на удаляемый элемент
		ship = ship->link1;//Переходим к следующему символу
		free(link->info.name);
		link->info.name = NULL;
		free(link->info.type);
		link->info.type = NULL;
		free(link);
		link = NULL;
	}
	return NULL;
}

/*
Описание: Это функция для ввода одного элемента списка.
Возврат: Указатель на элемент списка.
*/
OSHIP *O_get_struct()
{
	OSHIP* n_sp;//Указатель на создаваемый элемент

	n_sp = (OSHIP*)malloc(sizeof(OSHIP));
	get_struct_info(&(n_sp->info));
	return n_sp;
}

/*
Описание: Это функция для добавления списка в начало исходного списка.
Возврат: Указатель на первый элемент списка.
*/
OSHIP* O_add_frist(OSHIP *sp, OSHIP *add_sp)
{
	if (add_sp != NULL)
	{
		OSHIP *li;//Вспомогательный указатель
		for (li = add_sp; li->link1 != NULL; li = li->link1);
		li->link1 = sp;
		return add_sp;
	}
	else
		return sp;
}

/*
Описание: Это функция для добавления элемента в начало списка.
Возврат: Указатель на первый элемент списка.
*/
OSHIP *O_add(OSHIP *ship)
{
	OSHIP *el;//Указатель на новый элемент
	puts("Вводите данные о добавляемом кораблике");
	el = O_get_struct();
	el->link1 = NULL;
	if (ship != NULL)
		ship = O_add_frist(ship, el);
	else
		ship = el;

	puts("Новый элемент успешно добавлен");
	return ship;
}

/*
Описание: Это функция для удаления одного элемента из списка.
Возврат: Указатель на первый элемент списка.
*/
OSHIP* O_del_frist(OSHIP *ship)
{
	if (ship != NULL)
	{
		OSHIP *l;//Указатель на удаляемый элемент
		l = ship;
		ship = ship->link1;
		free(l->info.name);
		free(l->info.type);
		free(l);
	}
	return ship;
}

/*
Описание: Это функция для взятия элемента из стека.
Возврат: Указатель на первый элемент списка.
*/
OSHIP *O_del(OSHIP *ship)
{
	if (ship != NULL)
	{
		puts("Элемент:");
		O_print_struct(ship);
		puts("Взят из стека");
		ship = O_del_frist(ship);
	}
	else
		puts("Нет элементов в стеке. Невозможно взять элемент из стека");
	return ship;
}

void main()
{
	setlocale(0,"RU");

	int menu;   //Введённый пункт меню
	OSHIP *ship;//Указатель на первый элемент списка

	ship = NULL;

	do
	{
		system("cls");
		puts("Стек на базе односвязного списка");
		puts("1-добавление элемента в стек");
		puts("2-взятие элемента из стека");
		puts("3-отображение стека");
		puts("4-справка");
		puts("0-выход");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			ship = O_add(ship);
			system("pause");
			break;
		case 2:
			ship = O_del(ship);
			system("pause");
			break;
		case 3:
			O_print(ship);
			system("pause");
			break;
		case 4:
			puts("Справка");
			puts("Это программа для ввода элементов в стек, вывода стека и");
			puts("взятия элементов из стека");
			puts("При вводе строки:");
			puts("1. таб конвертируется в пробел");
			puts("2. пробелы сначала вводить запрещено");
			puts("3. пробелы в конце убираются");
			puts("Кораблик имеет пять полей:");
			puts("1. Имя-строка от 1 до 50 символов");
			puts("2. Тип-строка от 1 до 50 символов");
			puts("3. Положение по x-целое число от -999999 до 9999999");
			puts("4. Положение по y-целое число от -999999 до 9999999");
			puts("5. Положение по z-целое число от -999999 до 9999999");
			puts("Символы кириллицы вводить запрещено.");

			system("pause");
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu != 0);
	ship=O_free_all(ship);
}


































































































//**//PF get_nt,name,"Указатель на первый элемент строки с именем того, что вводим"
//**//PF get_type,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF get_coord,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF is_allowed,not_allowed,"Указатель на первый элемент массива данных с запрещёнными символами"
//**//PF input_string,size,"Максимальный размер вводимой строки"
//**//PF input_string,not_allowed,"Указатель на первый символ строки с запрещёнными символами"
//**//PF str_len,s,"Указатель на первый элемент исходной строки"
//**//PF is_sm_letter,ch,"Проверяемый элемент"
//**//PF is_bi_letter,ch,"Проверяемый элемент"
//**//PF get_nt,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF get_coord,ch,"Символ координат, например 'x'"
//**//PF get_struct_info,info,"Указатель на данные о кораблике"


/*END

type:
char void int float bool DATA string OSHIP FILE
*/