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
	int m;     //Водоизмещение в мл
	int speed; //Скорость в см/с
}DATA;

typedef struct ST1
{
	DATA info;
	struct ST1 *link1;//Ссылка на следующий элемент
}OSHIP;

typedef struct
{
	char s_name; //Тип сортировки по имени(1-по возрастанию 2-по убыванию)
	char s_type; //Тип сортировки по типу(1-по возрастанию 2-по убыванию)
	char s_x;    //Тип сортировки по x(1-по возрастанию 2-по убыванию)
	char s_y;    //Тип сортировки по y(1-по возрастанию 2-по убыванию)
	char s_z;    //Тип сортировки по z(1-по возрастанию 2-по убыванию)
	char s_m;    //Тип сортировки по водоизмещению(1-по возрастанию 2-по убыванию)
	char s_speed;//Тип сортировки по скорости(1-по возрастанию 2-по убыванию)
	char typ[7]; //Массив размера 7, в котором элемент означает критерий сортировки (1-имя 2-тип 3-x 4-y 6-m 5-speed 0-нет критерия)
} SORTDATA;

typedef struct
{
	char *name;//Указатель на первый символ строки, содержащий имя кораблика
	char *type;//Указатель на первый символ строки, содержащий тип кораблика
	int x;     //Положение кораблика по x
	int y;     //Положение кораблика по y
	int z;     //Положение кораблика по z
	int m;     //Положение кораблика по z
	int speed; //Положение кораблика по z

	char s_name; //Тип поиска по имени(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_type; //Тип поиска по типу(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_x;    //Тип поиска по x(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_y;    //Тип поиска по y(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_z;    //Тип поиска по z(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_m;    //Тип поиска по водоизмещению(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_speed;//Тип поиска по скорости(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
} FINDDATA;

#define S_input_path_help "Запрещено:\n  1. Вводить символы кириллицы\n  2. Вводить пробел в начале\n  3. Вводить несколько пробелов подряд\nВвод запрещённых символов игнорируется.\nПробел в конце пути будет игнорироваться.\nВы также можете добавить расширение \".txt\", для того, чтобы можно\nбыло открыть файл в блокноте.\n\nДля окончания ввода нажмите Enter\n"
#define S_input_nt_help "Запрещено:\n  1. Вводить символы кириллицы\n  2. Вводить пробел в начале\n  3. Вводить несколько пробелов подряд\n  4. Вводить какие либо символы, кроме английских букв, пробела и\n     круглых скобок\nВвод запрещённых символов будет игнорироваться.\nПробел в конце будет игнорироваться.\nВводимая строка должна состоять из одного или нескольких слов,\nпервое из которых должно начинаться с строчной буквы.\nВсе строчные буквы в слове после первой будут сделаны прописными.\nВводимая строка может содержать от 1 до 50 символов.\nПри достижении максимального количества символов ввод\nсимволов будет игнорироваться.\nДля окончания ввода нажмите Enter\n"
#define S_input_number "Разрешено вводить только цифры и '-'.\nОбязательно надо ввести число.\n"
#define S_table_fringe "|----------------------------------------------------|----------------------------------------------------|---------|---------|---------|-------------|--------|\n"

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
char *input_string(int size,char *not_allowed)
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
char* get_nt(char *title,char* name)
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
Описание: Функция для ввода пути к файлу.
Возврат: Указатель на первый символ введённой строки.
*/
char* get_path(char* demo_f)
{
	char* file_name;//Указатель на первый символ строки, содержащей путь к файлу
	system("cls");
	if (demo_f!=NULL)
		printf("Желаете использовать демо-файл: \"%s\"?\n", demo_f);
	if (input_yn()&& demo_f!=NULL)
	{
		file_name = (char*)malloc(8*sizeof(char));
		strcpy(file_name,"txt.txt");
	}
	else
	{
		printf(S_input_path_help);
		puts("Вводите путь к файлу(путь к файлу должен содержать хотя бы 1 символ,");
		puts("но не более 70):");
		file_name = input_string(70, "");
	}

	return file_name;
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
		s = get_nt(title,"тип");//Ввод типа вручную
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
int get_coord(char* title,char ch)
{
	puts(title);
	printf(S_input_number);
	printf("Вводите положение по оси %c в сантиметрах (целое число от -999999 до 9999999):\n", ch);
	return input_number();
}

/*
Описание: Это функция для ввода числа в заданных пределах.
Возврат: Эта функция возвращает введённое число.
*/
int get_sized(char* title,int min, int max, char* name)
{
	int num;//Введённое число
	do
	{
		puts(title);
		printf(S_input_number);
		printf("Вводите %s (целое число от %i до %i):\n", name, min, max);
		num = input_number();
		if (num < min || num > max)
		{
			printf("Ошибка. Пожалуйста введите %s(от %i до %i)\n", name, min, max);
			system("pause");
			system("cls");
		}
	} while (num < min || num > max);
	return num;
}

/*
Описание: Функция получения полей структуры.
Возврат: Функция не возвращает значения.
*/
void get_struct_info(DATA *info)
{
	system("cls");
	info->name = get_nt("Ввод данных кораблика","имя");
	system("cls");
	info->type = get_type("Ввод данных кораблика");
	system("cls");
	info->x = get_coord("Ввод данных кораблика",'x');
	system("cls");
	info->y = get_coord("Ввод данных кораблика",'y');
	system("cls");
	info->z = get_coord("Ввод данных кораблика",'z');
	system("cls");
	info->m = get_sized("Ввод данных кораблика",10, 100000, "водоизмещение");
	system("cls");
	info->speed = get_sized("Ввод данных кораблика", 0, 1000, "скорость в см/с");
}

/*
Описание: Функция для ввода списка.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP *O_get_structs()
{
	OSHIP* n_sp;      //Указатель на первый элемент нового списка
	OSHIP *n_el;      //Указатель на новый элемент
	OSHIP *last_el;   //Указатель на последний элемент списка
	bool is_end_input;//Флажок выхода

	n_sp = (OSHIP*)malloc(sizeof(OSHIP));
	get_struct_info(&(n_sp->info));

	printf("Продолжить ввод?('y'-да 'n'-нет):\n");

	if (input_yn())
	{
		last_el = n_sp;
		do
		{
			n_el = (OSHIP*)malloc(sizeof(OSHIP));
			get_struct_info(&(n_el->info));
			last_el->link1 = n_el;
			last_el = n_el;
			printf("Продолжить ввод?('y'-да 'n'-нет):\n");
			is_end_input = !input_yn();
		} while (!is_end_input);
		last_el->link1 = NULL;
	}
	else
		n_sp->link1 = NULL;
	return n_sp;
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
	printf("Водоизмещение: %i\n", ship->info.m);
	printf("Скорость в см/с: %i\n", ship->info.speed);
}

/*
Описание: Функция для вывода списка.
Возврат: Функция не возвращает значения.
*/
void O_print(OSHIP *ship)
{
	int i;//Номер кораблика
	puts("Вывод с первого элемента");
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
		puts("Картотека пуста");

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
Описание: Это функция для подсчёта количества элементов в списке.
Возврат: Эта функция возвращает количество элементов в списке.
*/
int O_get_len(OSHIP* ship)
{
	int i;      //Количество элементов в списке
	i = 0;
	while (ship != NULL)
	{
		i++;
		ship = ship->link1;
	}
	return i;
}

/*
Описание: Функция для считывания из файла строки.
Возврат: Эта функция возвращает указатель на первый символ считанной строки.
*/
char* F_gets(char end, int maxlen, FILE* f)
{
	if (feof(f))
		return NULL;

	char *s;  //Указатель на первый символ считываемой строки
	bool exit;//Флажок выхода
	int len;  //Длина строки

	s = NULL;
	len = 0;
	exit = 0;
	do
	{
		s = (char*)realloc(s, sizeof(char)*(len + 1));
		exit = !fscanf(f,"%c",s + len);
		if (exit)
		{
			free(s);
			s = NULL;
		}
		else
		{
			if (s[len] == end)
			{
				s[len] = '\0';
				exit = 1;
			}
			len++;
		}
	} while (!exit);
	return s;
}

/*
Описание: Это функция для добавления списка после определённого элемента исходного списка.
Возврат: Функция не возвращает значения.
*/
void O_add_after(OSHIP *sp, OSHIP *add_sp)
{
	if (sp != NULL)
	{
		OSHIP *li;//Вспомогательный указатель
		for (li = add_sp; li->link1 != NULL; li = li->link1);
		li->link1 = sp->link1;
		sp->link1 = add_sp;
	}
}

/*
Описание: Функция для проверки правильности данных в файле.
Возврат: Эта функция возвращает 1 - если данные правильны, 0 - если файл пуст, или его не существует, -1 - если данные в файле повреждены.
*/
int test_file(char*file_name)
{
	FILE* f;     //Указатель на данные о файле
	char* s;     //Указатель на первый символ строки
	int i;       //Вспомогательная переменная
	if ((f = fopen(file_name, "r")) == NULL)
		return 0;
	if (feof(f))
		return 0;

	do
	{
		s = F_gets('\n', 50, f);
		if (s == NULL)
		{
			fclose(f);
			return -1;
		}
		if (s[0] == '\0')
			if (feof(f))
			{
				free(s);
				fclose(f);
				return 1;
			}
			else
			{
				free(s);
				fclose(f);
				return -1;
			}
		free(s);

		s = F_gets('\n', 20, f);
		if (s == NULL || s[0]=='\0')
		{
			free(s);
			fclose(f);
			return -1;
		}

		if (fscanf(f, "%i %i %i %i %i", &i, &i, &i, &i, &i) != 5)
		{
			fclose(f);
			return -1;
		}
		fscanf(f,"\n");
	} while (!feof(f));

	fclose(f);
	return 1;
}

/*
Описание: Функция для вывода информационных полей в файл в "красивом" виде.
Возврат: Эта функция возвращает 1 - если удалось записать, иначе 0.
*/
bool save_good_placed(char*file_name, OSHIP*ship)
{
	if (ship == NULL)
	{
		puts("Нечего записывать");
		return 0;
	}
	FILE* f;     //Указатель на данные о файле

	if ((f = fopen(file_name, "r")) != NULL)
		fclose(f);

	if (f == NULL)
	{
		f = fopen(file_name, "w");
	}
	else
	{
		puts("Файл уже существует. Желаете перезаписать его?");
		if (!input_yn())
		{
			puts("Картотека не была записанна в файл");
			return 0;
		}
		f = fopen(file_name, "w");
	}
	if (f == NULL)
	{
		puts("Ошибка! Не удалось открыть файл");

		return 0;
	}
	if (fputs(S_table_fringe, f) < 0)
	{
		puts("Во время записи произошла ошибка");
		return fclose(f) * 0;
	}

	if (fprintf(f, "|%52s|%52s|%9s|%9s|%9s|%13s|%8s|\n", "Имя кораблика", "Тип кораблика", "x", "y", "z", "Водоизмещение", "Скорость") < 0)
	{
		puts("Во время записи произошла ошибка");
		return fclose(f) * 0;
	}

	if (fputs(S_table_fringe, f) < 0)
	{
		puts("Во время записи произошла ошибка");
		return fclose(f) * 0;
	}

	do
	{
		if( fprintf(f, "|%52s|%52s|%9i|%9i|%9i|%13i|%8i|\n", ship->info.name, ship->info.type, ship->info.x, ship->info.y, ship->info.z, ship->info.m, ship->info.speed) < 0)
		{
			puts("Во время записи произошла ошибка");
			return fclose(f) * 0;
		}

		if (fputs(S_table_fringe, f) < 0)
		{
			puts("Во время записи произошла ошибка");
			return fclose(f) * 0;
		}
		ship = ship->link1;
	} while (ship != NULL);
	fclose(f);
	puts("Данные успешно записаны в файл");

	return 1;
}

/*
Описание: Функция для вывода информационных полей в файл.
Возврат: Эта функция возвращает 1 - если удалось записать, иначе 0.
*/
bool save(char*file_name, OSHIP*ship, bool is_add)
{
	if (ship == NULL)
	{
		puts("Нечего записывать");
		return 0;
	}

	FILE* f;     //Указатель на данные о файле
	OSHIP* link; //Указатель на сохраняемый элемент
	f ;
	if ((f = fopen(file_name, "r")) != NULL)
		fclose(f);

	if (!is_add)
		if (f == NULL)
		{
			f = fopen(file_name, "w");
		}
		else
		{
			puts("Файл уже существует. Желаете перезаписать его?");
			if (!input_yn())
			{
				puts("Картотека не была записанна в файл");
				return 0;
			}
			f = fopen(file_name, "w");
		}
	else
		if (f != NULL)
			f = fopen(file_name, "a");

	if (f == NULL)
	{
		puts("Ошибка! Не удалось открыть файл");
		return 0;
	}

	link = ship;
	do
	{
		if (fprintf(f, "%s\n%s\n%i\n%i\n%i\n%i\n%i\n", link->info.name, link->info.type, link->info.x, link->info.y, link->info.z, link->info.m, link->info.speed) < 0)
		{
			puts("Во время записи произошла ошибка");
			return fclose(f) * 0;
		}
		link = link->link1;
	} while (link != NULL);

	fclose(f);
	puts("Данные успешно записаны в файл");
	return 1;
}

/*
Описание: Функция для ввода данных из файла.
Возврат: Эта функция возвращает указатель на первый элемент загруженного списка.
*/
OSHIP *load(char *file_name)
{
	OSHIP* ship;//Указатель на первый элемент списка
	OSHIP* el;  //Вспомогательный указатель
	OSHIP* last;//Указатель на последний элемент списка
	FILE *f;    //Указатель на данные о файле
	bool err;   //Флажок ошибки
	bool end;   //Флажок конца файла
	char *s;    //Указатель на первый символ считанный элемент

	f = fopen(file_name, "r");

	if (f == NULL)
		return NULL;

	err = 0;
	end = 0;
	ship = NULL;

	do
	{
		s = F_gets('\n', 50, f);
		if (s != NULL)
		{
			if (s[0] != '\0')
			{
				el = (OSHIP*)malloc(sizeof(OSHIP));

				el->link1 = NULL;
				el->info.type = NULL;
				el->info.name = s;

				if (ship != NULL)
				{
					O_add_after(last, el);
					last = last->link1;
				}
				else
					ship = last = el;

				
				if ((s = F_gets('\n', 50, f)) != NULL)
				{
					el->info.type = s;
					err = fscanf(f, "%i %i %i %i %i", &(el->info.x), &(el->info.y), &(el->info.z), &(el->info.m), &(el->info.speed)) != 5;
				}
				else
					err = 1;
				fscanf(f, "\n");
			}
			else
			{
				free(s);
				end = 1;
			}
		}
		else
			if (feof(f))
				end = 1;
			else
				err = 1;
	} while (!err && !end);

	if (err)
		ship = O_free_all(ship);

	fclose(f);
	return ship;
}

/*
Описание: Это функция для выбора типа сортировки.
Возврат: Функция не возвращает значения.
*/
void select_sort(SORTDATA *sort)
{
	bool b;   //Вспомогательный флажок
	int i;    //Вспомогательная переменная
	int len;  //Количество критериев
	int r;    //Вспомогательная переменная
	int o;    //Вспомогательная переменная

	do
	{
		puts("По скольким полям будет проводиться сортировка (от 1 до 7)?");
		len = input_number();
		if (len < 1 || len > 7)
			puts("Ошибка");
	} while (len < 1 || len > 7);
	puts("По какому критерию сортировать в первую очередь?");

	do
	{
		puts("Варианты");
		puts("1-имя");
		puts("2-тип");
		puts("3-x");
		puts("4-y");
		puts("5-z");
		puts("6-водоизмещение");
		puts("7-скорость");

		r = input_number();
		if (r < 1 || r > 7)
			puts("Ошибка");
	} while (r< 1 || r > 7);
	sort->typ[0] = r;
	do
	{
		puts("Сортировать по данному критерию:");
		puts("1-по возрастанию");
		puts("2-по убыванию");
		r = input_number();
		if (r < 1 || r > 7)
			puts("Ошибка");
	} while (r < 1 || r > 7);
	switch (sort->typ[0])
	{
	case 1:
		sort->s_name = r;
		break;
	case 2:
		sort->s_type = r;
		break;
	case 3:
		sort->s_x = r;
		break;
	case 4:
		sort->s_y = r;
		break;
	case 5:
		sort->s_z = r;
		break;
	case 6:
		sort->s_m = r;
		break;
	case 7:
		sort->s_speed = r;
		break;
	}
	for (i = 1; i < len; i++)
	{
		puts("По какому потом?");
		do
		{
			do
			{
				puts("Варианты");
				puts("1-имя");
				puts("2-тип");
				puts("3-x");
				puts("4-y");
				puts("5-z");
				puts("6-водоизмещение");
				puts("7-скорость");

				r = input_number();
				if (r < 1 || r > 7)
				{
					puts("Нет такого пункта меню");
					system("pause");
				}
			} while (r < 1 || r > 7);
			b = 1;
			for (o = 0; o < i && b; o++)
				b = sort->typ[o] != r;
			if (!b)
				puts("Вы уже выбирали данное поле. Выберите теперь другое");
		} while (!b);
		sort->typ[i] = r;
		do
		{
			puts("Сортировать по данному критерию:");
			puts("1-по возрастанию");
			puts("2-по убыванию");
			r = input_number();
			if (r < 1 || r > 7)
				puts("Ошибка");
		} while (r < 1 || r > 7);
		switch (sort->typ[i])
		{
		case 1:
			sort->s_name = r;
			break;
		case 2:
			sort->s_type = r;
			break;
		case 3:
			sort->s_x = r;
			break;
		case 4:
			sort->s_y = r;
			break;
		case 5:
			sort->s_z = r;
			break;
		case 6:
			sort->s_m = r;
			break;
		case 7:
			sort->s_speed = r;
			break;
		}
	}
	if (len < 7)
		sort->typ[len] = 0;
}

//Функция для выбора типа поиска

/*
Описание: Это функция для выбора типа поиска.
Возврат: Функция не возвращает значения.
*/
void select_find(FINDDATA *find)
{
	int i;      //Вспомогательная переменная
	int len;    //Количество полей, по которым будет проводиться поиск
	int r;      //Вспомогательная переменная
	char typ[7];//Массив размера 7 данных о сортировке
	int o;      //Вспомогательная переменная

	for (i = 0; i < 7; i++)
		typ[i] = 0;

	find->s_name = 0;
	find->s_type = 0;
	find->s_x = 0;
	find->s_y = 0;
	find->s_z = 0;

	do
	{
		puts("По скольким полям будет проводиться поиск (от 1 до 7)?");
		len = input_number();
		if (len < 1 || len > 7)
			puts("Ошибка");
	} while (len < 1 || len > 7);

	i = 0;
	do
	{
		do
		{
			do
			{
				puts("Выберите поле, по которому будет проводиться поиск");
				puts("Варианты");
				puts("1-имя");
				puts("2-тип");
				puts("3-x");
				puts("4-y");
				puts("5-z");
				puts("6-водоизмещение");
				puts("7-скорость");

				r = input_number();
				if (r < 1 || r > 7)
					puts("Ошибка");
			} while (r < 1 || r > 7);
			if (typ[r - 1] != 0)
				puts("Вы уже выбирали данное поле. Выберите теперь другое");
		} while (typ[r - 1] != 0);
		typ[r - 1] = 1;
		puts("Задайте значение для сравнения:");
		switch (r)
		{
		case 1:
			find->name = get_nt("Ввод данных для сравнения", "имя кораблика");
			break;
		case 2:
			find->type = get_type("Ввод данных для сравнения");
			break;
		case 3:
			find->x = get_coord("Ввод данных для сравнения", 'x');
			break;
		case 4:
			find->y = get_coord("Ввод данных для сравнения", 'y');
			break;
		case 5:
			find->z = get_coord("Ввод данных для сравнения", 'z');
			break;
		case 6:
			find->m = get_sized("Ввод данных для сравнения", 10, 100000, "водоизмещение");
			break;
		case 7:
			find->speed = get_sized("Ввод данных для сравнения", 0, 1000, "скорость в см/с");
			break;
		}

		do
		{
			puts("Задайте условие для сравнения при котором элемент подходит:");
			puts("1-превышает заданное значение");
			puts("2-меньше заданного значения");
			puts("3-совпадает с заданным значением");
			o = input_number();
			if (o < 1 || o > 3)
				puts("Ошибка");
		} while (o < 1 || o > 3);

		switch (r)
		{
		case 1:
			find->s_name = o;
			break;
		case 2:
			find->s_type = o;
			break;
		case 3:
			find->s_x = o;
			break;
		case 4:
			find->s_y = o;
			break;
		case 5:
			find->s_z = o;
			break;
		case 6:
			find->s_m = o;
			break;
		case 7:
			find->s_speed = o;
			break;
		}
		i++;
	} while (i<len);

}

/*
Описание: Это функция для сравнения двух элементов.
Возврат: Эта функция возвращает 1 если a меньше b, иначе 0.
*/
bool a_sma_b(DATA a, DATA b, SORTDATA * sort_t)
{
	char state;//Значение результата(1 - a < b, 0 - a == b, -1 - a > b)
	int i;     //Вспомогательная переменная

	state = 0;
	for (i = 0; i < 7 && sort_t->typ[i] && !state; i++)
	{
		switch (sort_t->typ[i])
		{
		case 1:
			state = strcmp(a.name, b.name) > 0;
			if (!state)
				state = -(strcmp(a.name, b.name) < 0);
			if (sort_t->s_name == 2)
				state = -state;
			break;
		case 2:
			state = strcmp(a.type, b.type)>0;
			if (!state)
				state = -(strcmp(a.type, b.type)<0);
			if (sort_t->s_type == 2)
				state = -state;
			break;
		case 3:
			state = a.x > b.x;
			if (!state)
				state = -(a.x < b.x);
			if (sort_t->s_x == 2)
				state = -state;
			break;
		case 4:
			state = a.y > b.y;
			if (!state)
				state = -(a.y < b.y);
			if (sort_t->s_y == 2)
				state = -state;
			break;
		case 5:
			state = a.z > b.z;
			if (!state)
				state = -(a.z < b.z);
			if (sort_t->s_z == 2)
				state = -state;
			break;
		case 6:
			state = a.m > b.m;
			if (!state)
				state = -(a.m < b.m);
			if (sort_t->s_m == 2)
				state = -state;
			break;
		case 7:
			state = a.speed > b.speed;
			if (!state)
				state = -(a.speed < b.speed);
			if (sort_t->s_speed == 2)
				state = -state;
			break;
		}
	}
	return state == 1;
}

/*
Описание: Функция для определения, подходит ли элемент критериям поиска.
Возврат: Эта функция возвращает 1 если подходит, иначе 0.
*/
bool is_need(DATA info, FINDDATA *find)
{
	switch (find->s_name)
	{
	case 1:
		if (strcmp(info.name, find->name) <= 0)
			return 0;
		break;
	case 2:
		if (strcmp(info.name, find->name) >= 0)
			return 0;
		break;
	case 3:
		if (strcmp(info.name, find->name) != 0)
			return 0;
		break;
	}
	switch (find->s_type)
	{
	case 1:
		if (strcmp(info.type, find->type) <= 0)
			return 0;
		break;
	case 2:
		if (strcmp(info.type, find->type) >= 0)
			return 0;
		break;
	case 3:
		if (strcmp(info.type, find->type) != 0)
			return 0;
		break;
	}
	switch (find->s_x)
	{
	case 1:
		if (info.x <= find->x)
			return 0;
		break;
	case 2:
		if (info.x >= find->x)
			return 0;
		break;
	case 3:
		if (info.x != find->x)
			return 0;
		break;
	}
	switch (find->s_y)
	{
	case 1:
		if (info.y <= find->y)
			return 0;
		break;
	case 2:
		if (info.y >= find->y)
			return 0;
		break;
	case 3:
		if (info.y != find->y)
			return 0;
		break;
	}
	switch (find->s_z)
	{
	case 1:
		if (info.z <= find->z)
			return 0;
		break;
	case 2:
		if (info.z >= find->z)
			return 0;
		break;
	case 3:
		if (info.z != find->z)
			return 0;
		break;
	}
	switch (find->s_m)
	{
	case 1:
		if (info.m <= find->m)
			return 0;
		break;
	case 2:
		if (info.m >= find->m)
			return 0;
		break;
	case 3:
		if (info.m != find->m)
			return 0;
		break;
	}
	switch (find->s_speed)
	{
	case 1:
		if (info.speed <= find->speed)
			return 0;
		break;
	case 2:
		if (info.speed >= find->speed)
			return 0;
		break;
	case 3:
		if (info.speed != find->speed)
			return 0;
		break;
	}
	return 1;
}

/*
Описание: Это функция для добавления списка в начало исходного списка.
Возврат: Эта функция возвращает указатель на первый элемент списка.
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
Описание: Это функция для добавления списка в указаное место.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP *O_insert_spis(OSHIP *ship, OSHIP *nsp)
{
	int menu;      //Выбранный пункт меню
	int n;         //Номер элемента
	int len;       //Количество элементов в списке
	OSHIP *link;   //Указатель на вставляемый элемент
	char*file_name;//Указатель на первый символ строки, содержащей путь к файлу


	len = O_get_len(ship);
	do
	{
		system("cls");
		puts("Вставка");
		puts("1-Вставить в начало картотеки");
		puts("2-Вставить после n-ного элемента в картотеке");
		puts("3-Вставить в конец картотеки, записанной в файл");
		puts("0-Забыть данные, не вставляя их");
		menu = input_number();
		if (menu<0 || menu>3)
		{
			puts("Нет такого пункта");
			system("pause");
		}
		switch (menu)
		{
		case 1:
			ship = O_add_frist(ship, nsp);
			puts("Новые элементы успешно добавлены");
			menu = 0;
			break;
		case 2:
			if (ship != NULL)
			{
				do
				{
					printf("Вводите номер элемента, после которого будут вставленные\nэлементы (от 1 до %i):", len);
					n = input_number();
					if (n < 1 || n > len)
					{
						puts("Ошибка");
						printf("Ведите число от 1 до %i\n",len);
						system("pause");
					}
				} while (n < 1 || n > len);

				for (link = ship; n > 1; link = link->link1, n--);

				O_add_after(link, nsp);
				puts("Новые элементы успешно добавлены");
				menu = 0;
			}
			else
			{
				puts("Нет элементов в картокеке");
				puts("Желаете добавить данные в начало картотеки?('y'-да 'n'-нет)");
				if (input_yn())
				{
					ship = nsp;
					menu = 0;
					puts("Новые элементы успешно добавлены");
				}

			}
			break;
		case 3:
				
			
			file_name = get_path(NULL);
			
			if (test_file(file_name) != -1)
				if (save(file_name, nsp,1))
				{
					nsp = O_free_all(nsp);
					menu = 0;
				}
				else
					system("pause");
			else
			{
				puts("Выбранный вами файл повреждён или является файлом из другой программы");
				system("pause");
			}
			free(file_name);
			file_name = NULL;
			break;
		case 0:
			nsp = O_free_all(nsp);
			puts("Данные были успешно забыты");
			break;
		}
	} while (menu);

	return ship;
}

/*
Описание: Функция для создания меню ввода.
Возврат: Эта функция возвращает указатель на первый элемент введённого списка.
*/
OSHIP *input_menu(OSHIP *ship)
{
	int menu;       //Выбранный пункт меню
	char *file_name;//Указатель на первый символ строки пути к файлу
	OSHIP* nsp;     //Указатель на первый элемент введённого списка

	do
	{
		system("cls");
		puts("Ввод данных в картотеку");
		puts("1-с клавиатуры");
		puts("2-из файла");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		switch (menu)
		{
		case 1:
			nsp = O_get_structs();
			puts("Данные успешно введены");
			system("pause");
			ship = O_insert_spis(ship, nsp);
			nsp = NULL;
			system("pause");
			break;
		case 2:
			file_name = get_path("txt.txt");

			nsp = load(file_name);
			if (nsp != NULL)
			{
				puts("Данные успешно загружены");
				system("pause");
				ship = O_insert_spis(ship, nsp);
				nsp = NULL;
			}
			else
				puts("Не удалось загрузить данные из файла");
			
			free(file_name);
			
			system("pause");
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu);

	return ship;
}

/*
Описание: Функция для создания меню вывода списка.
Возврат: Функция не возвращает значения.
*/
void output_menu(OSHIP *ship,char* title)
{
	if (ship == NULL)
	{
		puts("Картотека пуста редактировать нечего");
		puts("Для ввода картотеки выберите: Главное меню->ввод данных в картотеку");
		system("pause");
		return;
	}
	int menu;       //Выбранный пункт меню
	char *file_name;//Указатель на первый символ строки пути к файлу

	file_name = (char*)malloc(sizeof(char)* 8);
	strcpy(file_name, "txt.txt");

	do
	{
		system("cls");
		printf("Вывод %s\n",title);
		puts("1-на экран");
		puts("2-в файл");
		puts("3-дополнить существующий файл");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		switch (menu)
		{
		case 1:
			system("cls");
			O_print(ship);
			system("pause");
			break;
		case 2:
			system("cls");
			file_name = get_path("out.txt");

			puts("Желаете записать файл в 'красивом' виде ?");
			puts("При записи в 'красивом' виде файл потом будет невозможно прочитать");
			puts("с помощю данной программы, но будет возможно прочитать с помощью");
			puts("текстового редактора");
			puts("(y-да n-нет):");
			if (input_yn())
				save_good_placed(file_name, ship);
			else
				save(file_name, ship, 0);
			free(file_name);
			system("pause");
			break;
		case 3:
			system("cls");
			file_name = get_path(NULL);
			if (test_file(file_name) != -1)
				save(file_name, ship, 1);
			else
				puts("Выбранный вами файл повреждён или является файлом из другой программы");

			free(file_name);
			file_name = NULL;

			system("pause");
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu);

}

/*
Описание: Функция для создания меню удалений.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP* remove_menu(OSHIP* ship)
{
	int menu;       //Выбранный пункт меню
	char *file_name;//Указатель на первый символ строки пути к файлу
	
	file_name = (char*)malloc(sizeof(char) * 8);
	strcpy(file_name, "txt.txt");

	do
	{
		system("cls");
		puts("Удаление картотеки");
		puts("1-картотеки из оперативной памяти");
		puts("2-файла с картотекой");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		switch (menu)
		{
		case 1:
			system("cls");
			if (ship != NULL)
			{
				ship = O_free_all(ship);
				puts("Удаление картотеки из оперативной памяти прошло успешно");
			}
			else
				puts("Картотека пуста. Удалять из оперативной памяти нечего");
			system("pause");
			break;
		case 2:
			system("cls");
			
			file_name = get_path("out.txt");
			
			if (remove(file_name) == 0)
				puts("Файл с картотекой удалён");
			else
				puts("Не удалось удалить файл с картотекой");

			free(file_name);
			file_name = NULL;

			system("pause");
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu);
	return ship;
}

/*
Описание: Функция для создания меню справки.
Возврат: Функция не возвращает значения.
*/
void help_menu()
{
	int menu;       //Выбранный пункт меню

	do
	{
		system("cls");
		puts("Справка");
		puts("1-по вводу");
		puts("2-по выводу");
		puts("3-по структуре данных");
		puts("4-ограничения");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			system("cls");
			puts("Вывести пустую картотеку невозможно");
			puts("При вводе имени и типа");
			printf(S_input_nt_help);
			puts("");
			puts("При вводе координат");
			puts(S_input_number);
			system("pause");
			break;
		case 2:
			system("cls");
			puts("При выводе на экран:");
			puts("  Элементы выводятся последовательно");
			puts("  Элементы выводятся парциально максимум по 3 шт. за раз");
			puts("");
			puts("При выводе в файл:");
			puts("  Записывается каждый элемент списка в указанный вами файл");
			puts("  Если в файле есть какая-то информация, то предлагается");
			puts("  перезаписать его, или не перезаписывать");
			puts("  Если файла не существует, то создаётся пустой файл и в него");
			puts("  записывается список.");
			puts("  Файл можно записать в 'красивом' и в обычном виде.");
			puts("  При выводе в файл в 'красивом' виде, прочитать его с помощью");
			puts("  этой программы будет невозможно, но будет возможно прочитать");
			puts("  с помощью блокнота.");
			puts("  При записи в обычном виде файл можно будет прочитать только");
			puts("  с помощью данной программы.");
			puts("При добавлении данных в файл:");
			puts("  Данные в файле будут проверены на правильность.");
			puts("  Данные будут добавлены в файл только если данные");
			puts("  в файле правильны.");
			puts("  Добавить данные в пустой файл невозможно.");

			system("pause");
			break;
		case 3:
			system("cls");
			puts("Это программа для создания, загрузки, сохранения картотеки");
			puts("Один элемент картотеки содержит данные об одном кораблике, а именно:");
			puts("  1. Имя кораблика - строка с максимальной длиной 50 символов");
			puts("  2. Тип кораблика - строка с максимальной длиной 50 символов");
			puts("  3. Положение кораблика по оси x");
			puts("  4. Положение кораблика по оси y");
			puts("  5. Положение кораблика по оси z");
			puts("  6. Водоизмещение");
			puts("  7. Скорость кораблика");
			puts("Начало координат вы выбираете сами");
			system("pause");
			break;
		case 4:
			puts("Вывести пустую картотеку невозможно");
			puts("Символы кириллицы вводить запрещено");
			puts("Имя кораблика может максимально иметь 50 символов и состоит только из:");
			puts("  -английских букв");
			puts("  -пробелов");
			puts("  -круглых скобок");
			puts("Имя кораблика должно начинаться с большой буквы");
			puts("Тип кораблика может максимально иметь 50 символов и состоит только из:");
			puts("  -английских букв");
			puts("  -пробелов");
			puts("  -круглых скобок");
			puts("Тип кораблика должен начинаться с большой буквы");
			puts("Водоизмещение кораблика может быть только в мл и только целым числом от");
			puts("10 до 100000");
			puts("Скорость кораблика может быть только в см/с и только целым числом от");
			puts("0 до 1000");
			system("pause");
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu);
}

/*
Описание: Это функция для создания копии элемента списка.
Возврат: Эта функция возвращает указатель на коированный элемент.
*/
OSHIP *O_create_copy(OSHIP* ship)
{
	OSHIP *new_ship;//Указатель на копию элемента
	int i;          //Вспомогательная переменная
	int len;        //Количество элементов в списке
	if (ship != NULL)
	{
		new_ship = (OSHIP*)malloc(sizeof(OSHIP));
		new_ship->info.x = ship->info.x;
		new_ship->info.y = ship->info.y;
		new_ship->info.z = ship->info.z;
		new_ship->info.m = ship->info.m;
		new_ship->info.speed = ship->info.speed;

		len = str_len(ship->info.name);
		new_ship->info.name = (char*)malloc(len*sizeof(char));
		for (i = 0; i < len; i++)
			new_ship->info.name[i] = ship->info.name[i];

		len = str_len(ship->info.type);
		new_ship->info.type = (char*)malloc(len*sizeof(char));
		for (i = 0; i < len; i++)
			new_ship->info.type[i] = ship->info.type[i];
		new_ship->link1 = NULL;
	}
	else
		new_ship = NULL;
	return new_ship;
}

/*
Описание: Это функция для поиска.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP* O_find(FINDDATA*find, OSHIP*ship)
{
	if (ship == NULL)
	{
		puts("Нет элементов в картотеке");
		return NULL;
	}
	OSHIP* ship_res;//Указатель на первый элемент списка с результатами поиска
	OSHIP* link;    //Вспомогательный указатель
	OSHIP* new_el;  //Указатель на копированный элемент

	ship_res = NULL;

	do
	{
		if (is_need(ship->info, find))
		{
			new_el = O_create_copy(ship);
			if (ship_res == NULL)
				ship_res = new_el;
			else
				link->link1 = new_el;
			link = new_el;
		}
		ship = ship->link1;
	} while (ship != NULL);
	if (ship_res != NULL)
	{
		link->link1 = NULL;
		puts("Поиск завершён");
	}
	else
		puts("Не найдено элементов, удовлетворяющих заданным критериям");
	return ship_res;
}

/*
Описание: Это функция для вставления элемента в отсортированный список.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP *O_insert(OSHIP *ship, OSHIP *el, SORTDATA*sort)
{
	if (ship != NULL)
		if (!a_sma_b(ship->info, el->info, sort))
		{
			OSHIP* l;//Вспомогательный указатель
			l = ship;
			while (ship->link1 != NULL)
			{
				if (a_sma_b(ship->link1->info, el->info, sort))
				{
					O_add_after(ship, el);
					return l;
				}
				ship = ship->link1;
			}
			ship->link1 = el;
			return l;
		}
		else
		{
			el->link1 = ship;
			return el;
		}
	else
	{
		el->link1 = NULL;
		return el;
	}
}

/*
Описание: Это функция для сортировки.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP *O_sort(OSHIP *ship, SORTDATA *sort)
{
	if (ship == NULL)
	{
		puts("Нечего сортировать");
		return NULL;
	}
	OSHIP *ship_sorted;//Указатель на первый элемент отсортированного списка
	OSHIP *link;       //Указатель на вставляемый элемент

	ship_sorted = NULL;

	do
	{
		link = ship;
		ship = ship->link1;
		link->link1 = NULL;
		ship_sorted = O_insert(ship_sorted, link, sort);
	} while (ship != NULL);
	puts("Сортировка окончена");
	puts("Для вывода отсортированой картотеки выберите:");
	puts("Главное меню->вывод картотеки");
	return ship_sorted;
}

/*
Описание: Это функция для создания меню обработки списка.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP* calculate_menu(OSHIP* ship)
{
	if (ship == NULL)
	{
		puts("Картотека пуста редактировать нечего");
		puts("Для ввода картотеки выберите: Главное меню->ввод данных в картотеку");
		system("pause");
		return ship;
	}
	SORTDATA sort; //Параметры сортировки
	FINDDATA find; //Параметры поиска
	OSHIP*link;    //Вспомогательный указатель
	OSHIP*ship_res;//Указатель на первый элемент списка с результатами поиска
	int menu;      //Выбранный пункт меню

	find.name = NULL;
	find.type = NULL;

	do
	{
		system("cls");
		puts("Обработка");
		puts("1-сортировка элементов картотеки");
		puts("2-поиск");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			select_sort(&sort);
			ship=O_sort(ship, &sort);
			system("pause");
			break;
		case 2:
			free(find.name);
			find.name = NULL;
			free(find.type);
			find.type = NULL;
			select_find(&find);
			ship_res = O_find(&find, ship);
			system("pause");
			if (ship_res!=NULL)
			{
				output_menu(ship_res, "результатов поиска");
				ship_res = O_free_all(ship_res);
			}
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu);
	free(find.name);
	find.name = NULL;
	free(find.type);
	find.type = NULL;
	return ship;
}

/*
Описание: Это функция для редактирования одного элемента.
Возврат: Функция не возвращает значения.
*/
void edit_one(OSHIP *ship)
{
	int menu;//Выбранный пункт меню

	do
	{
		system("cls");
		puts("Редактируемый элемент:");
		O_print_struct(ship);

		puts("Меню редактирования");
		puts("1-редактировать поле имя");
		puts("2-редактировать поле тип");
		puts("3-редактировать поле x");
		puts("4-редактировать поле y");
		puts("5-редактировать поле z");
		puts("6-редактировать поле водоизмещение");
		puts("7-редактировать поле скорость");
		puts("0-выйти в надменю");
		puts("Ваш выбор:");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			free(ship->info.name);
			ship->info.name = NULL;
			ship->info.name = get_nt("Ввод данных для сравнения", "имя");
			puts("Поле имя успешно отредактировано");
			system("pause");
			break;
		case 2:
			free(ship->info.type);
			ship->info.type = NULL;
			ship->info.type = get_type("Ввод данных для сравнения");
			puts("Поле тип успешно отредактировано");
			system("pause");
			break;
		case 3:
			ship->info.x = get_coord("Ввод данных для сравнения", 'x');
			puts("Поле x успешно отредактировано");
			system("pause");
			break;
		case 4:
			ship->info.y = get_coord("Ввод данных для сравнения", 'y');
			puts("Поле y успешно отредактировано");
			system("pause");
			break;
		case 5:
			ship->info.z = get_coord("Ввод данных для сравнения", 'z');
			puts("Поле z успешно отредактировано");
			system("pause");
			break;
		case 6:
			ship->info.m = get_sized("Ввод данных для сравнения", 10, 100000, "водоизмещение");
			puts("Поле воодоизмещение успешно отредактировано");
			system("pause");
			break;
		case 7:
			ship->info.speed = get_sized("Ввод данных для сравнения", 0, 1000, "скорость в см/с");
			puts("Поле скорость успешно отредактировано");
			system("pause");
			break;
		case 0:
			break;
		}
	} while (menu);
}

/*
Описание: Это функция для создания меню изменения элементов списка.
Возврат: Функция не возвращает значения.
*/
void charg_menu(OSHIP *ship)
{
	if (ship == NULL)
	{
		puts("Картотека пуста обрабатывать нечего");
		puts("Для ввода картотеки выберите: Главное меню->ввод данных в картотеку");
		system("pause");
		return;
	}
	OSHIP* link;              //Указатель на редактируемый элемент
	int len = O_get_len(ship);//Количество элементов в списке
	int n;                    //Номер изменяемого элемента
	if (len != 0)
		do
		{
			puts(S_input_number);
			printf("Вводите номер редактируемого элемента от 1 до %i(0-возврат в надменю):\n",len);
			n = input_number();
			if (n < 0 || n > len)
			{
				puts("Ошибка! Вы ввели число вне заданых значений");
				system("pause");
			}
			else
			{
				if (n > 0)
				{
					link = ship;
					for (; n > 1; n--,link=link->link1);
					edit_one(link);
				}
			}
		} while (n!=0);
}

/*
Описание: Это функция для удаления одного элемента из списка.
Возврат: Функция не возвращает значения.
*/
void O_del_one(OSHIP **link)
{
	if (*link != NULL)
	{
		OSHIP *l;//Вспомогательный указатель
		l = *link;
		*link = (*link)->link1;
		free(l->info.name);
		free(l->info.type);
		free(l);
	}
}

/*
Описание: Это функция для создания меню удаления элементов.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP* del_menu(OSHIP* ship)
{
	int menu;                 //Выбранный пункт меню
	int len = O_get_len(ship);//Количество элементов в списке
	OSHIP *link;              //Вспомогательный указатель
	OSHIP **link_l;           //Указатель на ячейку памяти в которой хранится адрес удаляемого элемента
	int n;                    //Номер первого удалемого элемента
	int m;                    //Номер последнего удалемого элемента
	int i;                    //Вспомогательная переменная

	do
	{
		if (ship != NULL)
		{
			puts("Удаление элементов картотеки");
			puts("1-удалить элементы с n до m");
			puts("2-удалить первый элемент");
			puts("3-удалить последний элемент");
			puts("0-выйти в надменю");
			puts("Ваш выбор:");
			menu = input_number();
			system("cls");
			switch (menu)
			{
			case 1:
				n = get_sized("Удаление элементов картотеки с n до m", 1, len, "n");
				m = get_sized("Удаление элементов картотеки с n до m", n, len, "m");

				for (i = 2, link = ship; i < n; i++, link = link->link1);

				if (n == 1)
					link_l = &ship;
				else
					link_l = &link->link1;

				i = m - n + 1;

				do
				{
					O_del_one(link_l);
					i--;
				} while (i > 0);

				puts("Элементы успешно удалены");
				system("pause");
				len = O_get_len(ship);
				break;
			case 2:
				O_del_one(&ship);

				puts("Элемент успешно удален");
				system("pause");
				len--;
				break;
			case 3:
				link_l = &ship;
				for (; (*link_l)->link1 != NULL; link_l = &(*link_l)->link1);
				O_del_one(link_l);
				puts("Элемент успешно удален");
				system("pause");
				len--;
				break;
			case 0:
				break;
			default:
				puts("Нет такого пункта меню");
				system("pause");
				break;
			}
		}
		else
		{
			puts("В картотеке больше нет элементов.");
			puts("Сейчас вы вернётесь в главное меню");
			system("pause");
			menu = 0;
		}
	} while (menu);
	return ship;
}

/*
Описание: Это функция для создания меню редактирования.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP* edit_menu(OSHIP *ship)
{
	if (ship == NULL)
	{
		puts("Картотека пуста редактировать нечего");
		puts("Для ввода картотеки выберите: Главное меню->ввод данных в картотеку");
		system("pause");
		return ship;
	}
	int menu;       //Выбранный пункт меню

	do
	{
		system("cls");
		puts("Редактирование картотеки");
		puts("1-изменеине одного элемента");
		puts("2-удаление элементов");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			charg_menu(ship);
			break;
		case 2:
			ship = del_menu(ship);
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu&&ship!=NULL);
	return ship;
}

/*
Описание: Это функция для создания меню сохранения картотеки при выводе.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
void exit_save(OSHIP* ship)
{
	bool saved;     //Флажок сохранения в файл
	char *file_name;//Указатель на первый символ строки пути к файлу

	do
	{
		system("cls");
		puts("Сейчас в оперативной памяти находится картотека");
		puts("При выходе она будет удалена");
		puts("Желаете сохранить её в файл?(y-да n-нет)");
		if (input_yn())
		{
			file_name = get_path("out.txt");
			saved = save(file_name, ship, 0);
			free(file_name);
			if (saved)
			{
				puts("Картотека успешно сохранена в файл");
				system("pause");
			}
		}
		else
			saved = 1;
	} while (!saved);
}

/*
Описание: Это функция для создания главного меню.
_________
*/
int main()
{
	setlocale(0, "RU");
	int menu;       //Выбранный пункт меню
	OSHIP *ship;    //Указатель на первый элемент списка

	//Изменение размера шрифта
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



	puts("        II         II                   II         II                          ");
	puts("        I II       I II                 I II       I II                        ");
	puts("        I   II     I   II               I   II     I   II                      ");
	puts("        I     II   I     II             I     II   I     II                    ");
	puts("        I   II     I   II               I   II     I   II                      ");
	puts("        I II       I II                 I II       I II                        ");
	puts("   IIIIIIIIIIIIIIIIIIIIIIIIIIIII  IIIIIIIIIIIIIIIIIIIIIIIIIIIII                ");
	puts("    I                         I    I                         I                 ");
	puts("     III                   III      III                   III                  ");
	puts("        IIIII         IIIII             IIIII         IIIII                    ");
	puts("             IIIIIIIII                       IIIIIIIII                         ");
	puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

	puts("Вас приветствует программа для работы с картотекой.");
	puts("Предметная область кораблики.");
	puts("Один элемент картотеки содержит данные об одном кораблике, а именно:");
	puts("  1. Имя кораблика - строка с максимальной длиной 50 символов");
	puts("  2. Тип кораблика - строка с максимальной длиной 50 символов");
	puts("  3. Положение кораблика по оси x");
	puts("  4. Положение кораблика по оси y");
	puts("  5. Положение кораблика по оси z");
	puts("  6. Водоизмещение");
	puts("  7. Скорость кораблика");


	system("pause");

	ship = NULL;

	do
	{
		system("cls");
		puts("Главное меню");
		puts("1-ввод данных в картотеку");
		puts("2-вывод картотеки");
		puts("3-обработка картотеки");
		puts("4-редактирование картотеки");
		puts("5-удаление картотеки");
		puts("6-справка");
		puts("0-выход");
		printf("Ваш выбор: ");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			ship = input_menu(ship);
			break;
		case 2:
			output_menu(ship,"картотеки");
			break;
		case 3:
			ship=calculate_menu(ship);
			break;
		case 4:
			ship = edit_menu(ship);
			break;
		case 5:
			ship = remove_menu(ship);
			break;
		case 6:
			help_menu();
			break;
		case 0:
			break;
		default:
			puts("Нет такого пункта меню");
			system("pause");
			break;
		}
	} while (menu);
	if (ship != NULL)
		exit_save(ship);

	ship = O_free_all(ship);
}

















































































//**//PF get_sized,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF get_sized,name,"Указатель на первый элемент строки с именем того, что вводим"
//**//PF get_nt,name,"Указатель на первый элемент строки с именем того, что вводим"
//**//PF get_path,demo_f,"Указатель на первый элемент строки с путём к демо файлу"
//**//PF get_type,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF get_coord,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF is_allowed,not_allowed,"Указатель на первый элемент массива данных с запрещёнными символами"
//**//PF input_string,size,"Максимальный размер вводимой строки"
//**//PF input_string,not_allowed,"Указатель на первый символ строки с запрещёнными символами"
//**//PF str_len,s,"Указатель на первый элемент исходной строки"
//**//PF get_nt,title,"Указатель на первый элемент строки с заголовком для ввода"
//**//PF get_coord,ch,"Символ координат, например 'x'"
//**//PF get_struct_info,info,"Указатель на данные о кораблике"
//**//PF F_gets,end,"Символ признака конца строки"
//**//PF F_gets,maxlen,"Максимальная Длина строки"
//**//PF is_sm_letter,ch,"Проверяемый символ"
//**//PF is_bi_letter,ch,"Проверяемый символ"
//**//PF get_sized,min,"Минимальное значение для вводимого числа"
//**//PF get_sized,max,"Минимальное значение для вводимого числа"
//**//PF get_sized,_title,"Указатель на первый элемент вводимого числа"
//**//PF save,is_add,"Флажок записи в режиме добавления"
//**//PF select_sort,sort,"Указатель на структуру с параметрами сортировки"
//**//PF select_find,find,"Указатель на структуру с параметрами поиска"
//**//PF a_sma_b,a,"Данные элемента a"
//**//PF a_sma_b,b,"Данные элемента b"
//**//PF a_sma_b,sort_t,"Указатель на структуру с параметрами сортировки"
//**//PF is_need,info,"Данные элемента"
//**//PF is_need,find,"Указатель на структуру с параметрами поиска"
//**//PF output_menu,title,"Указатель на первый элемент строки с назманием того, что выводим"
//**//PF O_find,find,"Указатель на структуру с параметрами поиска"
//**//PF O_sort,sort,"Указатель на структуру с параметрами сортировки"
//**//PF O_insert,sort,"Указатель на структуру с параметрами сортировки"
/*END

type:
char void int float bool DATA string OSHIP FILE
*/

