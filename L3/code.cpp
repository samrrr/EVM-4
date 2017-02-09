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
}OSHIP;

#define S_input_path_help "Запрещено:\n  1. Вводить символы кириллицы\n  2. Вводить пробел в начале\n  3. Вводить несколько пробелов подряд\nВвод запрещённых символов игнорируется.\nПробел в конце пути будет игнорироваться.\nВы также можете добавить расширение \".txt\", для того, чтобы можно\nбыло открыть файл в блокноте.\n\nДля окончания ввода нажмите Enter\n"
#define S_input_nt_help "Запрещено:\n  1. Вводить символы кириллицы\n  2. Вводить пробел в начале\n  3. Вводить несколько пробелов подряд\n  4. Вводить какие либо символы, кроме английских букв, пробела и\n     круглых скобок\nВвод запрещённых символов будет игнорироваться.\nПробел в конце будет игнорироваться.\nВводимая строка должна состоять из одного или нескольких слов,\nпервое из которых должно начинаться с строчной буквы.\nВсе строчные буквы в слове после первой будут сделаны прописными.\nВводимая строка может содержать от 1 до 50 символов.\nПри достижении максимального количества символов ввод\nсимволов будет игнорироваться.\nДля окончания ввода нажмите Enter\n"
char *S_input_number = "Разрешено вводить только цифры и '-'.\nВвод других символов игнорируется.\nОбязательно надо ввести число.\n";

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
Описание: Функция для ввода пути к файлу.
Возврат: Указатель на первый символ введённой строки.
*/
char* get_path(char* demo_f)
{
	char* file_name;//Указатель на первый символ строки, содержащей путь к файлу
	system("cls");
	if (demo_f != NULL)
		printf("Желаете использовать демо-файл: \"%s\"?\n", demo_f);
	if (input_yn() && demo_f != NULL)
	{
		file_name = (char*)malloc(8 * sizeof(char));
		strcpy(file_name, "txt.txt");
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
	printf("Положение кораблика по оси x в сантиметрах: %i\n", ship->info.x);
	printf("Положение кораблика по оси y в сантиметрах: %i\n", ship->info.y);
	printf("Положение кораблика по оси z в сантиметрах: %i\n", ship->info.z);
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
		puts("Список пуст");

}

/*
Описание: Функция для освобождения памяти из-под всего списка.
Возврат: Эта функция возвращает NULL.
*/
OSHIP* O_free_all(OSHIP* ship)
{
	OSHIP *link;//Вспомогательный указатель
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
Описание: Функция для вывода информационных полей в файл.
Возврат: Эта функция возвращает 1 - если удалось записать, иначе 0.
*/
bool save(char *file_name, OSHIP*ship)
{
	FILE *f;   //Указатель на данные о файле
	OSHIP*link;//Указатель на сохраняемый элемент
	bool err;  //Флажок ошибки
	if (ship != NULL)
	{
		f = fopen(file_name, "w");
		if (f != NULL)
		{
			link = ship;
			do
			{
				err = fprintf(f, "%s\n%s\n%i\n%i\n%i\n", link->info.name, link->info.type, link->info.x, link->info.y, link->info.z) < 0;
				if (err)
					return fclose(f) * 0;
				link = link->link1;
			} while (link != NULL);
		}
		else
			return 0;

		fclose(f);
	}
	else
		return 0;
	return 1;
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
Описание: Функция для ввода данных из файла.
Возврат: Эта функция возвращает указатель на первый элемент загруженного списка.
*/
OSHIP *load(char *file_name)
{
	OSHIP* ship;//Указатель на первый элемент списка
	OSHIP* el;  //Указатель на новый элемент
	OSHIP* last;//Указатель на последний элемент списка
	FILE *f;    //Указатель на данные о файле
	bool err;   //Флажок ошибки
	bool end;   //Флажок конца файла
	char *s;    //Указатель на первый символ считанный элемент


	if ((f = fopen(file_name, "r")) == NULL)
		return NULL;

	err = 0;
	end = 0;
	ship = NULL;

	do
	{
		if ((s = F_gets('\n', 50, f)) != NULL)
		{
			if (s[0] != '\0')
			{
				el = (OSHIP*)malloc(sizeof(OSHIP));

				el->link1 = NULL;
				el->info.type = NULL;
				if (ship != NULL)
				{
					O_add_after(last, el);
					last = last->link1;
				}
				else
					ship = last = el;

				el->info.name = s;
				
				if ((s = F_gets('\n', 50, f)) != NULL)
				{
					el->info.type = s;
					err = fscanf(f, "%i %i %i", &(el->info.x), &(el->info.y), &(el->info.z)) != 3;
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
Описание: Функция для создания меню ввода.
Возврат: Эта функция возвращает указатель на первый элемент введённого списка.
*/
OSHIP *input_menu(OSHIP *ship)
{
	int menu;       //Выбранный пункт меню
	char *file_name;//Указатель на первый символ строки пути к файлу


	do
	{
		system("cls");
		puts("Ввод данных");
		if (ship != NULL)
			puts("Внимание при вводе старый список будет стёрт");
		puts("1-с клавиатуры");
		puts("2-из файла");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		switch (menu)
		{
		case 1:
			ship = O_free_all(ship);
			ship = O_get_structs();
			puts("Новый список успешно создан");
			system("pause");
			break;
		case 2:
			file_name = get_path("txt.txt");
			ship = O_free_all(ship);
			if ((ship = load(file_name)) != NULL)
				puts("Список успешно загружен");
			else
				puts("Не удалось загрузить список");
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
Описание: Функция для создания меню вывода.
Возврат: Функция не возвращает значения.
*/
void output_menu(OSHIP *ship)
{
	if (ship == NULL)
	{
		puts("Нет данных для вывода");
		system("pause");
		return;
	}
	int menu;       //Выбранный пункт меню
	char *file_name;//Указатель на первый символ строки пути к файлу


	do
	{
		system("cls");
		puts("Вывод данных");
		puts("1-на экран");
		puts("2-в файл");
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
			if (save(file_name, ship))
				puts("Список успешно сохранён");
			else
				puts("Не удалось сохранить список");
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
}

/*
Описание: Функция для создания меню удаления.
Возврат: Эта функция возвращает указатель на первый элемент списка.
*/
OSHIP* remove_menu(OSHIP* ship)
{
	int menu;       //Выбранный пункт меню
	char *file_name;//Указатель на первый символ строки пути к файлу
	

	do
	{
		system("cls");
		puts("Удаление");
		puts("1-списка из оперативной памяти");
		puts("2-файла");
		puts("0-возврат в надменю");
		printf("Ваш выбор: ");
		menu = input_number();
		switch (menu)
		{
		case 1:
			system("cls");
			if (ship != NULL)
				ship = O_free_all(ship);
			else
				puts("Список пуст. Удалять нечего");
			system("pause");
			break;
		case 2:
			system("cls");
			file_name = get_path("out.txt");
			if (remove(file_name) == 0)
				puts("Файл удалён");
			else
				puts("Не удалось удалить файл");
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
Описание: Функция для создания меню справки.
Возврат: Функция не возвращает значения.
*/
void help_menu()
{
	int menu;       //Выбранный пункт меню

	do
	{
		system("cls");
		puts("Это программа для создания, загрузки и сохранения списков");
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
			puts("При вводе нового списка старый список стирается");
			puts("При вводе имени и типа");
			printf(S_input_nt_help);
			puts("");
			puts("При вводе координат, водоизмещения и скорости");
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
			puts("  Если в файле есть какая-то информация, то она стирается");
			puts("  Если файла не существует, то создаётся пустой файл и в него");
			puts("  записывается информационные поля списка.");
			system("pause");
			break;
		case 3:
			system("cls");
			puts("Один элемент списка содержит данные об одном кораблике кораблике, а именно:");
			puts("  1. Имя кораблика - строка с максимальной длинной 50 символов");
			puts("  2. Тип кораблика - строка с максимальной длинной 50 символов");
			puts("  3. Положение кораблика по оси x");
			puts("  4. Положение кораблика по оси y");
			puts("  5. Положение кораблика по оси z");
			puts("Начало координат вы выбираете сами");
			system("pause");
			break;
		case 4:
			puts("Вывести пустой список невозможно");
			puts("Символы кириллицы вводить запрещено");
			puts("Имя кораблика может максимально иметь 50 символов и состоит только из:");
			puts("  -английских букв");
			puts("  -пробелов");
			puts("  -круглых скобок");
			puts("Имя кораблика должно начинаться с строчной буквы");

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

int main()
{
	setlocale(0, "RU");
	int menu;       //Выбранный пункт меню
	OSHIP *ship;    //Указатель на первый элемент списка

	ship = NULL;

	do
	{
		system("cls");
		puts("Работа с файлами");
		puts("Главное меню");
		puts("1-ввод данных");
		puts("2-вывод данных");
		puts("3-удаление");
		puts("4-справка");
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
			output_menu(ship);
			break;
		case 3:
			ship = remove_menu(ship);
			break;
		case 4:
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
	ship = O_free_all(ship);
}





























































































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
//**//PF F_gets,maxlen,"Максимальная длинна строки"
//**//PF is_sm_letter,ch,"Проверяемый символ"
//**//PF is_bi_letter,ch,"Проверяемый символ"

/*END

type:
char void int float bool DATA string OSHIP FILE
*/

