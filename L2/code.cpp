#include <conio.h>
#include <malloc.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>

typedef struct
{
	char s_name;//Тип сортировки по имени(1-по возрастанию 2-по убыванию)
	char s_type;//Тип сортировки по типу(1-по возрастанию 2-по убыванию)
	char s_x;   //Тип сортировки по x(1-по возрастанию 2-по убыванию)
	char s_y;   //Тип сортировки по y(1-по возрастанию 2-по убыванию)
	char s_z;   //Тип сортировки по z(1-по возрастанию 2-по убыванию)
	char typ[5];//Массив размера 5, в котором элемент означает критерий сортировки (1-имя 2-тип 3-x 4-y 5-z 0-нет критерия)
} SORTDATA;

typedef struct
{
	char *name;//Указатель на первый символ строки, содержащий имя кораблика
	char *type;//Указатель на первый символ строки, содержащий тип кораблика
	int x;     //Положение кораблика по x
	int y;     //Положение кораблика по y
	int z;     //Положение кораблика по z

	char s_name;//Тип поиска по имени(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_type;//Тип поиска по типу(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_x;   //Тип поиска по x(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_y;   //Тип поиска по y(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
	char s_z;   //Тип поиска по z(0-не важно 1-те, что больше 2-те, что меньше 3-полное совпадение)
} FINDDATA;

typedef struct
{
	char *name;//Указатель на первый символ строки, содержащий имя кораблика
	char *type;//Указатель на первый символ строки, содержащий тип кораблика
	int x;     //Положение кораблика по x
	int y;     //Положение кораблика по y
	int z;     //Положение кораблика по z
}DATA;

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

//Функция для ввода целого числа

int input_number()
{
	int number;    //Введённое число	
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
			if (length>1)
				s[length - 1] = 0;
			break;
		default:
			if ((ch >= '0' && ch <= '9' || ch == '-' && length == 1) && length <= 7)
			{
				s = (char *)realloc(s, sizeof(char)*(length+1));
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
			printf("\b%c",sy);
			break;
		}
	} while (ch != '\n' || sy == 0);
	printf("\n");
	return sy == 'y' || sy == 'Y';
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
			if (length <= size && (ch != ' ' || length > 1))
			{
				s = (char *)realloc(s, sizeof(char)*(length+1));
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

//Функция для вычисления длинны строки

int str_len(char*s)
{
	int i = 1;//Длина строки
	while (*s)
		i++, s++;
	return i;
}

//Функция для ввода типа кораблика

char* get_type()
{
	char *types[4] = { "battleship", "cruiser", "boat", "steelship" };
              //Массив указателей на первый символ строки
	int i;    //Вспомогательная переменная
	char *s;  //Указатель на первый символ вводимой строки
	char type;//Тип ораблика
	char len; //Длина строки
	puts("Выберите тип:");
	for (i = 0; i < 4; i++)
		printf("%i-%s\n", i + 1, types[i]);
	puts("другое число-ввод типа вручную\n");
	type = input_number();
	if (type <= 0 || type >= 5)
	{
		puts("Вводите тип (от 1 до 20 символов):");
		s = input_string(20);
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

//Функция для ввода имени

char* get_name()
{
	char *s;  //Указатель на первый символ введённой строки
	bool exit;//Флажок выхода
	exit = 0;
	do{
		puts("Введите имя (имя должно содержать хотя бы один символ");
		puts("и не более 30 символов):");
		s = input_string(30);
		if (*s < 'A' || *s > 'Z')
		{
			puts("Вниание! Имя корабля начинается не с большой буквы.");
			puts("Вы хотите ввести другое имя? (y/n)");
			if (input_yn())
				free(s);
			else
				exit = 1;
		}
		else
			exit = 1;
	} while (!exit);
	return s;
}

//Функция для ввода координат

int get_coord(char ch)
{
	printf("Вводите положение по %c в сантиметрах (целое число от -999999 до 9999999):\n", ch);
	return input_number();
}

//Функция получения полей структуры

void get_struct_info(DATA *info)
{
	info->name = get_name();
	info->type = get_type();
	info->x = get_coord('x');
	info->y = get_coord('y');
	info->z = get_coord('z');
}

//@@

typedef struct ST1
{
	DATA info;        //Данные о кораблике
	struct ST1 *link1;//Указатель на следующий элемент
} OSHIP;

//Функция для ввода списка

OSHIP *O_get_structs()
{
	OSHIP* n_sp;      //Указатель на первый элемент списка
	OSHIP *n_el;      //Указатель на элемент списка
	OSHIP *last_el;   //Указатель на последний элемент списка
	bool is_end_input;//Вспомогательная переменная

	n_sp = (OSHIP*)malloc(sizeof(OSHIP));
	get_struct_info(&(n_sp->info));

	printf("Продолжить ввод?('y'-да 'n'-нет):\n");

	if (input_yn())
	{
		is_end_input = false;
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

//Функция для добавления списка в начало исходного списка

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

//Функция для добавления списка в конец исходного списка

OSHIP* O_add_last(OSHIP *sp, OSHIP *add_sp)
{
	if (sp != NULL)
	{
		OSHIP *li;//Вспомогательный указатель
		for (li = sp; li->link1 != NULL; li = li->link1);
		li->link1 = add_sp;
	}
	else
		sp = add_sp;
	return sp;
}

//Функция для добавления списка после определённого элемента исходного списка

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

//Функция для добавления новых элементов в список

OSHIP *O_add(OSHIP *ship)
{
	int menu;     //Выбранный пункт меню
	int n;        //Номер элемента
	int len;      //Количество элементов в списке
	OSHIP *new_sp;//Указатель на первый элемент списка
	OSHIP *link;  //Вспомогательный указатель
	puts("Вводите последовательность элементов");
	if (ship != NULL)
	{
		new_sp = O_get_structs();
		do
		{
			system("cls");
			puts("Вставка");
			puts("1-Вставить в начало");
			puts("2-Вставить в конец");
			puts("3-Вставить после n-ного элемента");
			menu = input_number();
			if (menu<1 || menu>4)
			{
				puts("Нет такого пункта");
				system("pause");
			}
		} while (menu<1 || menu>4);
		switch (menu)
		{
		case 1:
			ship = O_add_frist(ship, new_sp);
			break;
		case 2:
			ship = O_add_last(ship, new_sp);
			break;
		case 3:
			for (len = 1, link = ship; link->link1 != NULL; link = link->link1, len++);			
			do
			{
				printf("Вводите номер элемента после которого будут вставленные\nэлементы (от 1 до %i)", len);
				n = input_number();
				if (n < 1 || n > len)
					puts("Ошибка");
			} while (n < 1 || n > len);

			for (link = ship; n > 1; link = link->link1, n--);
			
			O_add_after(link, new_sp);
			break;
		}
	}
	else
		ship = O_get_structs();

	puts("Новые элементы успешно добавлены");
	return ship;
}

//Функция для удаления одного элемента из списка 

void O_del_one(OSHIP **link)
{
	if (*link != NULL)
	{
		OSHIP *l;//
		l = *link;
		*link = (*link)->link1;
		free(l->info.name);
		free(l->info.type);
		free(l);
	}
}

//Функция для удаления элементов из списка

OSHIP *O_del(OSHIP *ship)
{
	int len;       //Номер первого удаляемгог элемента
	int m;         //Номер последнего удаляемого элемента
	int n;         //Номер первого удаляемого элемента
	int i;         //Вспомогательная переменная
	OSHIP *link;   //Вспомогательный указатель
	OSHIP **link_l;//Вспомогательный указатель
	if (ship != NULL)
	{
		for (link = ship, len = 1; link->link1 != NULL; link = link->link1, len++);
		puts("Удаление элементов с n до m");
		do
		{
			printf("Вводите n(от 1 до %i)\n", len);
			n = input_number();
			if (n<1 || n>len)
				puts("Неправильный ввод");
		} while (n<1 || n>len);

		do
		{
			printf("Вводите m(от %i до %i)\n", n,len);
			m = input_number();
			if (m<n || m>len)
				puts("Неправильный ввод");
		} while (m<n || m>len);
		
		for (i = 2, link = ship; i < n; i++, link = link->link1);

		if (n==1)
			link_l = &ship;
		else
			link_l = &link->link1;
		
		i = m - n + 1;

		do
		{
			O_del_one(link_l);
			i--;
		} while (i > 0);
		puts("Элемент(-ы) успешно удалены");
	}
	else
		puts("Список пуст, удалять нечего");

	return ship;
}

//Функция для создания копии элемента списка

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

//Функция для поиска

OSHIP* O_find(OSHIP* ship)
{
	FINDDATA find;  //Указатель на копию элемента
	OSHIP *ship_res;//Указатель на первый элемент обработанного списка
	OSHIP *link;    //Вспомогательный указатель
	OSHIP *new_el;  //Указатель на копию элемента
	select_find(&find);
	ship_res = NULL;

	do
	{
		if (is_need(ship->info,&find))
			{
				new_el = O_create_copy(ship);
				if (ship_res == NULL)
					ship_res = new_el;
				else
					link->link1 = new_el;
				link = new_el;
			}
		ship = ship->link1;
	} while (ship!=NULL);
	if (ship_res != NULL)
		link->link1 = NULL;
	if (ship_res != NULL)
		puts("Поиск завершён");
	else
		puts("Не найдено элементов, удовлетворяющих заданным критериям");

	return ship_res;
}

//Функция для вставления элемента в отсортированный список.

OSHIP *O_insert(OSHIP *ship, OSHIP *el, SORTDATA*sort)
{
	if (ship!=NULL)
		if (!a_sma_b(ship->info, el->info, sort))
		{
			OSHIP* l;//Вспомогательный указатель
			l = ship;
			while (ship->link1 != NULL)
			{
				if (a_sma_b(ship->link1->info, el->info, sort))
				{
					O_add_after(ship,el);
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

//Функция для сортировки

OSHIP *O_sort(OSHIP *ship)
{
	if (ship == NULL)
	{
		puts("Нечего сортировать");
		return NULL;
	}
	OSHIP *ship_sorted;//Указатель на первый элемент отсортированного списка
	OSHIP *link;       //Вспомогательный указатель
	SORTDATA sort;     //Данные о сортировке

	select_sort(&sort);
	ship_sorted = NULL;

	do
	{
		link = ship;
		ship = ship->link1;
		link->link1 = NULL;
		ship_sorted = O_insert(ship_sorted,link,&sort);
	} while (ship != NULL);
	puts("Сортировка окончена");
	return ship_sorted;
}

//Функция для вывода информационных полей одной структуры

void O_print_struct(OSHIP *ship)
{
	printf("Имя кораблика: \"%s\"\n", ship->info.name);
	printf("Тип кораблика: \"%s\"\n", ship->info.type);
	printf("Положение кораблика по x в сантиметрах:%i\n", ship->info.x);
	printf("Положение кораблика по y в сантиметрах:%i\n", ship->info.y);
	printf("Положение кораблика по z в сантиметрах:%i\n", ship->info.z);
}

//Функция для вывода списка

void O_print(OSHIP *ship)
{
	int i;//Вспомогательная переменная
	puts("Вывод с первого элемента");
	if (ship != NULL)
	{
		i = 0;
		do
		{
			if (i > 0 && i % 3 == 0)
				system("pause");
			printf("\nКораблик номер: %i\n", i + 1);
			O_print_struct(ship); 
			ship = ship->link1;
			i++;
		}	
		while (ship != NULL);
		printf("\nВывод окончен\n");
	}
	else
		puts("Список пуст");

}

//Функция для освобождения памяти из под списка

OSHIP* O_free_all(OSHIP* ship)
{
	OSHIP *link;//
	while(ship != NULL)
	{
		link = ship;
		ship = ship->link1;
		free(link->info.name);
		free(link->info.type);
		free(link);
	}
	return NULL;
}

//Функция для создания меню

void O_menu()
{
	int menu;       //Выбранный пункт меню
	int i;          //Вспомогательная переменная
	OSHIP *ship;    //Указатель на первый элемент списка исходных данных
	OSHIP *ship_res;//Указатель на первый элемент списка результатов поиска
	bool is_find;   //Флажок поиска

	ship = NULL;
	ship_res = NULL;
	is_find = false;

	do
	{
		system("cls");
		puts("Односвязные списки");
		puts("1-добавление элементов в список");
		puts("2-удаление элементов из списка");
		puts("3-вывод элементов списка");
		puts("4-обработка списка");
		puts("5-вывод результатов последнего поиска");
		puts("6-справка");
		puts("0-возврат в надменю");
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
			if (ship != NULL)
			{
				do
				{
					system("cls");
					puts("Выберите тип обработки");
					puts("1-поиск (при этом формируется новый список)");
					puts("2-сортировка элементов исходного списка");
					puts("0-возврат в надменю");
					i = input_number();
					if (i<0 || i>2)
					{
						puts("Вы неправильно ввели пункт меню");
						system("pause");
					}
				} while (i<0 || i>2);
				switch (i)
				{
				case 1:
						is_find = TRUE;
						ship_res = O_free_all(ship_res);
						ship_res = O_find(ship);
						system("pause");
						break;
				case 2:
						ship = O_sort(ship);
						system("pause");
						break;
				}
			}
			else
			{
				puts("Список пуст, обрабатывать нечего");
				system("pause");
			}
			break;
		case 5:
			if (is_find)
				if (ship_res != NULL)
					O_print(ship_res);
				else
					puts("При последнем поиске не найдено элементов,\nсоответствующих заданным критериям");
			else
				puts("Вы ещё ни разу не проводили поиск");
			system("pause");
			break;
		case 6:
			puts("Справка");
			puts("При вводе строки:");
			puts("1. таб конвертируется в пробел");
			puts("2. пробелы сначала вводить запрещено");
			puts("3. пробелы в конце убираются");
			puts("Кораблик имеет пять полей:");
			puts("1. Имя-строка от 1 до 30 символов");
			puts("2. Тип-строка от 1 до 20 символов");
			puts("3. Положение по x-целое число от -999999 до 9999999");
			puts("4. Положение по y-целое число от -999999 до 9999999");
			puts("5. Положение по z-целое число от -999999 до 9999999");
			puts("Результат обработки - новый массив данных состоящий из элементов исходного");
			puts("массива, которые соответствуют заданным критериям.");
			puts("Русские буквы вводить запрещено.");
			puts("Для двух связных списков возможен вывод с начала");
			puts("и с конца");

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
	ship_res=O_free_all(ship_res);
}

//@@

typedef struct ST2
{
	DATA info;        //Данные о кораблике
	struct ST2 *link1;//Указатель на следующий элемент
	struct ST2 *link2;//Указатель на предыдующий элемент
} DSHIP;

DSHIP *D_get_structs()
{
	DSHIP* n_sp;      //Указатель на первый элемент списка
	DSHIP *last_el;   //Указатель на последний элемент списка
	bool is_end_input;//Вспомогательная переменная

	n_sp = (DSHIP*)malloc(sizeof(DSHIP));
	get_struct_info(&(n_sp->info));
	n_sp->link2 = NULL;

	printf("Продолжить ввод?('y'-да 'n'-нет):\n");

	if (input_yn())
	{
		is_end_input = false;
		last_el = n_sp;
		do
		{
			last_el->link1 = (DSHIP*)malloc(sizeof(DSHIP));
			get_struct_info(&(last_el->link1->info));
			last_el->link1->link2 = last_el;
			last_el = last_el->link1;
			printf("Продолжить ввод?('y'-да 'n'-нет):\n");
			is_end_input = !input_yn();
		} while (!is_end_input);
		last_el->link1 = NULL;
	}
	else
		n_sp->link1 = NULL;
	return n_sp;
}

//Функция для добавления списка в начало исходного списка

DSHIP *D_add_frist(DSHIP *ship, DSHIP *sp)
{
	if (ship != NULL)
	{
		DSHIP* next;//Вспомогательный указатель
		for (next = sp; next->link1 != NULL; next = next->link1);
		next->link1 = ship;
		ship->link2 = next;
	}
	return sp;
}

//Функция для добавления списка в начало исходного списка.

DSHIP *D_add_last(DSHIP *ship, DSHIP *sp)
{
	if (ship != NULL)
	{
		DSHIP* next;
		for (next = ship; next->link1 != NULL; next = next->link1);
		next->link1 = sp;
		sp->link2 = next;
	}
	else
		ship = sp;
	return ship;
}

//Функция для добавления списка в начало исходного списка.

DSHIP *D_add_after(DSHIP *ship, DSHIP *sp)
{
	if (ship != NULL)
	{
		DSHIP* next;
		for (next = sp; next->link1 != NULL; next = next->link1);
		next->link1 = ship->link1;
		if (next->link1 != NULL)
			next->link1->link2 = next;
		ship->link1 = sp;
		sp->link2 = ship;
	}
	else
		ship = sp;
	return ship;
}

//Функция для добавления списка в начало исходного списка

DSHIP *D_add(DSHIP *ship)
{
	int menu;     //Выбранный пункт меню
	int n;        //Вспомогательная переменная
	int len;      //Количество элементов в списке
	DSHIP *new_sp;//Указатель на первый элемент списка
	DSHIP *link;  //Указатель на первый элемент списка
	puts("Вводите последовательность элементов");
	if (ship != NULL)
	{
		new_sp = D_get_structs();
		do
		{
			system("cls");
			puts("Вставка");
			puts("1-Вставить в начало");
			puts("2-Вставить в конец");
			puts("3-Вставить после n-ного элемента");
			menu = input_number();
			if (menu<1 || menu>4)
			{
				puts("Нет такого пункта");
				system("pause");
			}
		} while (menu<1 || menu>4);
		switch (menu)
		{
		case 1:
			ship = D_add_frist(ship, new_sp);
			break;
		case 2:
			ship = D_add_last(ship, new_sp);
			break;
		case 3:
			for (len = 1, link = ship; link->link1 != NULL; link = link->link1, len++);
			do
			{
				printf("Вводите номер элемента после которого будут вставленные\nэлементы (от 1 до %i)", len);
				n = input_number();
				if (n < 1 || n > len)
					puts("Ошибка");
			} while (n < 1 || n > len);

			for (link = ship; n > 1; link = link->link1, n--);

			D_add_after(link, new_sp);
			break;
		}
	}
	else
		ship = D_get_structs();

	puts("Новые элементы успешно добавлены");
	return ship;
}

//Функция для удаления одного элемента из списка 

void D_del_one(DSHIP **link)
{
	if (*link != NULL)
	{
		DSHIP *l;//Вспомогательный указатель
		l = *link;
		if ((*link)->link1 != NULL)
			(*link)->link1->link2 = (*link)->link2;
		*link = (*link)->link1;
		free(l->info.name);
		free(l->info.type);
		free(l);
	}
}

//Функция для удаления элементов из списка

DSHIP *D_del(DSHIP *ship)
{
	int len;       //Количество элементов в списке
	int m;         //Номер последнего удаляемого элемента
	int n;         //Номер первого удаляемого элемента
	int i;         //Вспомогательная переменная
	DSHIP *link;   //Вспомогательный указатель
	DSHIP **link_l;//Вспомогательный указатель
	if (ship != NULL)
	{
		for (link = ship, len = 1; link->link1 != NULL; link = link->link1, len++);
		puts("Удаление элементов с n до m");
		do
		{
			printf("Вводите n(от 1 до %i)\n", len);
			n = input_number();
			if (n<1 || n>len)
				puts("Неправильный ввод");
		} while (n<1 || n>len);

		do
		{
			printf("Вводите m(от %i до %i)\n", n, len);
			m = input_number();
			if (m<n || m>len)
				puts("Неправильный ввод");
		} while (m<n || m>len);

		for (i = 2, link = ship; i < n; i++, link = link->link1);

		if (n == 1)
			link_l = &ship;
		else
			link_l = &link->link1;

		i = m - n + 1;

		do
		{
			D_del_one(link_l);
			i--;
		} while (i > 0);
		puts("Элемент(-ы) успешно удалены");
	}
	else
		puts("Список пуст, удалять нечего");

	return ship;
}

//Функция для создания копии элемента списка

DSHIP *D_create_copy(DSHIP* ship)
{
	int i;          //Вспомогательная переменная
	int len;        //Количество элементов в списке
	DSHIP *new_ship;//Указатель на копию элемента
	if (ship != NULL)
	{
		new_ship = (DSHIP*)malloc(sizeof(DSHIP));
		new_ship->info.x = ship->info.x;
		new_ship->info.y = ship->info.y;
		new_ship->info.z = ship->info.z;

		len = str_len(ship->info.name);
		new_ship->info.name = (char*)malloc(len*sizeof(char));
		for (i = 0; i < len; i++)
			new_ship->info.name[i] = ship->info.name[i];

		len = str_len(ship->info.type);
		new_ship->info.type = (char*)malloc(len*sizeof(char));
		for (i = 0; i < len; i++)
			new_ship->info.type[i] = ship->info.type[i];
		new_ship->link1 = NULL;
		new_ship->link2 = NULL;
	}
	else
		new_ship = NULL;
	return new_ship;
}

//Функция для поиска

DSHIP* D_find(DSHIP* ship)
{
	FINDDATA find;  //Данные о поиске
	DSHIP *ship_res;//Указатель на первый элемент обработанного списка
	DSHIP *link;    //Вспомогательный указатель
	DSHIP *new_el;  //Указатель на копию элемента

	select_find(&find);
	ship_res = NULL;
	link = NULL;

	do
	{
		if (is_need(ship->info, &find))
		{
			new_el = D_create_copy(ship);
			if (ship_res == NULL)
				ship_res = new_el;
			else
				link->link1 = new_el;
			new_el->link2 = link;
			link = new_el;
		}
		ship = ship->link1;
	} while (ship != NULL);
	if (ship_res != NULL)
		link->link1 = NULL;
	if (ship_res != NULL)
		puts("Поиск завершён");
	else
		puts("Не найдено элементов, удовлетворяющих заданным критериям");

	return ship_res;
}

//Функция для вставки элемента в отсортированный список

DSHIP *D_insert(DSHIP *ship, DSHIP *el, SORTDATA*sort)
{
	if (ship != NULL)
	if (!a_sma_b(ship->info, el->info, sort))
	{
		DSHIP *l;
		l = ship;//Вспомогательный указатель
		while (ship->link1 != NULL)
		{
			if (a_sma_b(ship->link1->info, el->info, sort))
			{
				D_add_after(ship, el);
				return l;
			}
			ship = ship->link1;
		}
		ship->link1 = el;
		ship->link1->link2 = ship;
		return l;
	}
	else
	{
		el->link1 = ship;
		el->link1->link2 = el;
		return el;
	}
	else
	{
		el->link1 = NULL;
		el->link2 = NULL;
		return el;
	}
}

//Функция для сортировки

DSHIP *D_sort(DSHIP *ship)
{
	if (ship == NULL)
	{
		puts("Нечего сортировать");
		return NULL;
	}
	DSHIP *ship_sorted;//
	DSHIP *link;       //
	SORTDATA sort;     //

	select_sort(&sort);
	ship_sorted = NULL;

	do
	{
		link = ship;
		ship = ship->link1;
		link->link1 = NULL;
		link->link2 = NULL;
		ship_sorted = D_insert(ship_sorted, link, &sort);
	} while (ship != NULL);
	puts("Сортировка окончена");
	return ship_sorted;
}

//Функция для вывода информационных полей одной структуры

void D_print_struct(DSHIP *ship)
{
	printf("Имя кораблика: \"%s\"\n", ship->info.name);
	printf("Тип кораблика: \"%s\"\n", ship->info.type);
	printf("Положение кораблика по x в сантиметрах:%i\n", ship->info.x);
	printf("Положение кораблика по y в сантиметрах:%i\n", ship->info.y);
	printf("Положение кораблика по z в сантиметрах:%i\n", ship->info.z);
}

//Функция для вывода списка

void D_print(DSHIP *ship)
{
	int i;   //Вспомогательная переменная
	int r;   //Вспомогательная переменная
	int type;//Тип вывода


	if (ship != NULL)
	{
		do
		{
			puts("1-Вывод с первого элемента");
			puts("2-Вывод с последнего элемента");
			type = input_number();
			if (type < 1 || type > 2)
				puts("Нет такого пункта");
		} while (type < 1 || type > 2);

		if (type == 1)
		{
			i = 0;
			do
			{
				if (i > 0 && i % 3 == 0)
					system("pause");
				i++;
				printf("\nКораблик номер: %i\n", i);
				D_print_struct(ship);
				ship = ship->link1;
			} while (ship != NULL);
		}
		else
		{
			i = 1;
			for (; ship->link1!=NULL; ship = ship->link1,i++);
			r = i % 3;
			do
			{
				if (ship->link1!=NULL && i % 3 == r)
					system("pause");
				printf("\nКораблик номер: %i\n", i);
				D_print_struct(ship);
				ship = ship->link2;
				i--;
			} while (ship != NULL);
		}
		printf("\nВывод окончен\n");
	}
	else
		puts("Список пуст");
}

//Функция для освобождения памяти из под списка

DSHIP* D_free_all(DSHIP* ship)
{
	DSHIP *link;//Вспомогательный указатель
	while (ship != NULL)
	{
		link = ship;
		ship = ship->link1;
		free(link->info.name);
		free(link->info.type);
		free(link);
	}
	return NULL;
}

//Функция для создания меню

void D_menu()
{
	DSHIP *ship;    //Указатель на первый элемент списка исходных данных
	DSHIP *ship_res;//Указатель на первый элемент списка результатов поиска
	int menu;       //Выбранный пункт меню
	int i;          //Вспомогательная переменная
	bool is_find;   //Флажок поиска

	is_find = false;
	ship = NULL;
	ship_res = NULL;

	do
	{
		system("cls");
		puts("Двухсвязные списки");
		puts("1-добавление элементов в список");
		puts("2-удаление элементов из списка");
		puts("3-вывод элементов списка");
		puts("4-обработка списка");
		puts("5-вывод результатов последнего поиска");
		puts("6-справка");
		puts("0-возврат в надменю");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			ship=D_add(ship);
			system("pause");
			break;
		case 2:
			ship = D_del(ship);
			system("pause");
			break;
		case 3:
			D_print(ship);
			system("pause");
			break;
		case 4:
			if (ship != NULL)
			{
				do
				{
					system("cls");
					puts("Выберите тип обработки");
					puts("1-поиск (при этом формируется новый список)");
					puts("2-сортировка элементов исходного списка");
					puts("0-возврат в надменю");
					i = input_number();
					if (i<0 || i>2)
					{
						puts("Вы неправильно ввели пункт меню");
						system("pause");
					}
				} while (i<0 || i>2);
				switch (i)
				{
				case 1:
					is_find = TRUE;
					ship_res = D_free_all(ship_res);
					ship_res = D_find(ship);
					system("pause");
					break;
				case 2:
					ship = D_sort(ship);
					system("pause");
					break;
				}
			}
			else
			{
				puts("Список пуст, обрабатывать нечего");
				system("pause");
			}
			break;
		case 5:
			if (is_find)
				if (ship_res != NULL)
					D_print(ship_res);
				else
					puts("При последнем поиске не найдено элементов,\nсоответствующих заданным критериям");
			else
				puts("Вы ещё ни разу не проводили поиск");
			system("pause");
			break;
		case 6:
			puts("Справка");
			puts("При вводе строки:");
			puts("1. таб конвертируется в пробел");
			puts("2. пробелы сначала вводить запрещено");
			puts("3. пробелы в конце убираются");
			puts("Кораблик имеет пять полей:");
			puts("1. Имя-строка от 1 до 30 символов");
			puts("2. Тип-строка от 1 до 20 символов");
			puts("3. Положение по x-целое число от -999999 до 9999999");
			puts("4. Положение по y-целое число от -999999 до 9999999");
			puts("5. Положение по z-целое число от -999999 до 9999999");
			puts("Результат обработки - новый массив данных состоящий из элементов исходного");
			puts("массива, которые соответствуют заданным критериям.");
			puts("Русские буквы вводить запрещено.");
			puts("Для двухсвязных списков возможен вывод с начала");
			puts("и с конца");

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
	ship = D_free_all(ship);
	ship_res = D_free_all(ship_res);
}

//@@

typedef struct ST3
{
	DATA info;        //Данные о кораблике
	struct ST3 *link1;//Указатель на следующий элемент
	struct ST3 *link2;//Указатель на предыдующий элемент
} CSHIP;

//Функция для добавления списка в начало исходного списка

CSHIP *C_add_frist(CSHIP *ship, CSHIP *sp)
{
	if (ship != NULL)
	{
		CSHIP* l;//Вспомогательный указатель
		ship->link2->link1 = sp;
		sp->link2->link1 = ship;
		l = sp->link2;
		sp->link2 = ship->link2;
		ship->link2 = l;
	}
	return sp;
}

//Функция для добавления списка в начало исходного списка

CSHIP *C_add_last(CSHIP *ship, CSHIP *sp)
{
	if (ship != NULL)
	{
		CSHIP* l;//Вспомогательный указатель
		ship->link2->link1 = sp;
		sp->link2->link1 = ship;
		l = sp->link2;
		sp->link2 = ship->link2;
		ship->link2 = l;
	}
	else
		ship = sp;
	return ship;
}

//Функция для добавления списка в начало исходного списка

CSHIP *C_add_after(CSHIP *ship, CSHIP *sp)
{
	if (ship != NULL)
		ship = C_add_frist(ship->link1,sp)->link2;
	else
		ship = sp;
	return ship;
}

//Функция возвращает указатель на первый элемент списка

CSHIP *C_get_structs()
{
	CSHIP *n_sp;      //Указатель на первый элемент списка
	CSHIP *last_el;   //Указатель на последний элемент списка
	CSHIP *new_el;    //Указатель на введённую структуру
	bool is_end_input;//Указатель на введённую структуру

	n_sp = (CSHIP*)malloc(sizeof(CSHIP));
	get_struct_info(&(n_sp->info));
	
	n_sp->link1 = n_sp;
	n_sp->link2 = n_sp;

	printf("Продолжить ввод?('y'-да 'n'-нет):\n");

	if (input_yn())
	{
		is_end_input = false;
		last_el = n_sp;
		do
		{
			new_el = (CSHIP*)malloc(sizeof(CSHIP));
			get_struct_info(&(new_el->info));
			new_el->link1 = new_el;
			new_el->link2 = new_el;
			C_add_after(last_el,new_el);
			last_el = new_el;
			printf("Продолжить ввод?('y'-да 'n'-нет):\n");
			is_end_input = !input_yn();
		} while (!is_end_input);
	}
	return n_sp;
}

//Функция для вычисления длинны списка

int C_get_len(CSHIP*ship)
{
	int i;      //Вспомогательная переменная
	CSHIP *link;//Вспомогательная переменная
	i = 0;
	if (ship != NULL)
		for (i = 1, link = ship->link1; link != ship; link = link->link1, i++);
	return i;
}

//Функция для добавления элементов в список

CSHIP *C_add(CSHIP *ship)
{
	int menu;     //Выбранный пункт меню
	int n;        //Вспомогательная переменная
	int len;      //Количество элементов в списке
	CSHIP *new_sp;//Указатель на первый элемент списка
	CSHIP *link;  //Вспомогательный указатель
	puts("Вводите последовательность элементов");
	if (ship != NULL)
	{
		new_sp = C_get_structs();
		do
		{
			system("cls");
			puts("Вставка");
			puts("1-Вставить в начало");
			puts("2-Вставить в конец");
			puts("3-Вставить после n-ного элемента");
			menu = input_number();
			if (menu<1 || menu>4)
			{
				puts("Нет такого пункта");
				system("pause");
			}
		} while (menu<1 || menu>4);
		switch (menu)
		{
		case 1:
			ship = C_add_frist(ship, new_sp);
			break;
		case 2:
			ship = C_add_last(ship, new_sp);
			break;
		case 3:
			len = C_get_len(ship);
			do
			{
				printf("Вводите номер элемента после которого будут вставленные\nэлементы (от 1 до %i)", len);
				n = input_number();
				if (n < 1 || n > len)
					puts("Ошибка");
			} while (n < 1 || n > len);

			for (link = ship; n > 1; link = link->link1, n--);

			C_add_after(link, new_sp);
			break;
		}
	}
	else
		ship = C_get_structs();

	puts("Новые элементы успешно добавлены");
	return ship;
}

//Функция для удаления одного элемента из списка 

CSHIP* C_del_one(CSHIP *link)
{
	if (link != NULL)
	if (link->link1 != link)
	{
		link->link1->link2 = link->link2;
		link = link->link1;
		free(link->link2->link1->info.name);
		free(link->link2->link1->info.type);
		free(link->link2->link1);
		link->link2->link1 = link;
	}
	else
	{
		free(link->info.name);
		free(link->info.type);
		free(link);
		link = NULL;
	}
	return link;
}

//Функция для удаления элементов из списка

CSHIP *C_del(CSHIP *ship)
{
	int len;       //Количество элементов в списке
	int m;         //Номер последнего удаляемого элемента
	int n;         //Номер первого удаляемого элемента
	int i;         //Вспомогательная переменная
	CSHIP *link;   //Вспомогательный указатель
	if (ship != NULL)
	{
		len = C_get_len(ship);
		puts("Удаление элементов с n до m");
		do
		{
			printf("Вводите n(от 1 до %i)\n", len);
			n = input_number();
			if (n<1 || n>len)
				puts("Неправильный ввод");
		} while (n<1 || n>len);

		do
		{
			printf("Вводите m(от %i до %i)\n", n, len);
			m = input_number();
			if (m<n || m>len)
				puts("Неправильный ввод");
		} while (m<n || m>len);

		for (i = 1, link = ship; i < n; i++, link = link->link1);


		i = m - n + 1;

		do
		{
			link=C_del_one(link);
			i--;
		} while (i > 0);
		if (n == 1)
			ship = link;
		puts("Элемент(-ы) успешно удалены");
	}
	else
		puts("Список пуст, удалять нечего");

	return ship;
}

//Функция для вывода информационных полей одной структуры

void C_print_struct(CSHIP *ship)
{
	printf("Имя кораблика: \"%s\"\n", ship->info.name);
	printf("Тип кораблика: \"%s\"\n", ship->info.type);
	printf("Положение кораблика по x в сантиметрах:%i\n", ship->info.x);
	printf("Положение кораблика по y в сантиметрах:%i\n", ship->info.y);
	printf("Положение кораблика по z в сантиметрах:%i\n", ship->info.z);
}

//Функция для вывода списка

void C_print(CSHIP *ship)
{
	int i, r,a; //Вспомогательные переменные
	int type;   //Тип вывода(1-в одну сторону 2-в другую)
	CSHIP *link;//Вспомогательный указатель
	int len;    //Количество элементов в списке
	int n;      //Номер элемента, с которого начинается вывод
	int linkadd;//Количество битов, которое надо прибавить к указателю на элемент списка, чтобы получить адрес следующего выводимого элемента

	if (ship != NULL)
	{
		do
		{
			puts("1-Вывод по часовой с n-ого элемента");
			puts("2-Вывод против часовой с n-ого элемента");
			type = input_number();
			if (type < 1 || type > 2)
				puts("Нет такого пункта");
		} while (type < 1 || type > 2);

		len = C_get_len(ship);

		do
		{
			printf("Вводите n (от 1 до %i)\n",len);
			n = input_number();
			if (n < 1 || n > len)
				puts("Нет такого пункта");
		} while (n < 1 || n > len);

		for (i = 1; i != n; i++, ship = ship->link1);

		printf("\nКораблик номер: %i\n", i);
		C_print_struct(ship);

		if (type == 1)
		{
			r = (n + 2) % 3;
			linkadd = (char*)&(ship->link1) - (char*)ship;
			a = 1;
		}
		else
		{
			i = i + len - 2;
			r = (i + 1) % 3;
			linkadd = (char*)&(ship->link2) - (char*)ship;
			a = -1;
		}
		if (ship->link1 != ship)
		{
			link = *((CSHIP**)((char*)ship + linkadd));
			do
			{
				if (i % 3 == r)
					system("pause");
				printf("\nКораблик номер: %i\n", i % len + 1);
				i += a;
				C_print_struct(link);
				link = *((CSHIP**)((char*)link + linkadd));
			} while (link != ship);
		}

		printf("\nВывод окончен\n");
	}
	else
		puts("Список пуст");
}

//Функция для освобождения памяти из под списка

CSHIP* C_free_all(CSHIP* ship)
{
	CSHIP *link;//Вспомогательный указатель
	if (ship != NULL)
	{
		ship->link2->link1 = NULL;
		do
		{
			link = ship;
			ship = ship->link1;
			free(link->info.name);
			free(link->info.type);
			free(link);
		} while (ship != NULL);
	}
	return NULL;
}


//Функция для создания копии элемента списка

CSHIP *C_create_copy(CSHIP* ship)
{
	CSHIP *new_ship;//Указатель на копированный элемент
	int i;          //Указатель на копированный элемент
	int len;        //Количество элементов в списке
	if (ship != NULL)
	{
		new_ship = (CSHIP*)malloc(sizeof(CSHIP));
		new_ship->info.x = ship->info.x;
		new_ship->info.y = ship->info.y;
		new_ship->info.z = ship->info.z;

		len = str_len(ship->info.name);
		new_ship->info.name = (char*)malloc(len*sizeof(char));
		for (i = 0; i < len; i++)
			new_ship->info.name[i] = ship->info.name[i];

		len = str_len(ship->info.type);
		new_ship->info.type = (char*)malloc(len*sizeof(char));
		for (i = 0; i < len; i++)
			new_ship->info.type[i] = ship->info.type[i];
		new_ship->link1 = new_ship;
		new_ship->link2 = new_ship;
	}
	else
		new_ship = NULL;
	return new_ship;
}

//Функция для вставления элемента в отсортированный список

CSHIP *C_insert(CSHIP *ship, CSHIP *el, SORTDATA*sort)
{
	if (ship != NULL)
		if (!a_sma_b(ship->info, el->info, sort))
		{
			CSHIP *l;//Вспомогатеьный указатель
			l = ship;
			while (ship->link1 != l)
			{
				if (a_sma_b(ship->link1->info, el->info, sort))
				{
					C_add_after(ship, el);
					return l;
				}
				ship = ship->link1;
			}
			C_add_after(ship, el);
			return l;
		}
		else
		{
			C_add_after(ship->link2,el);
			return el;
		}
	else
	{
		el->link1 = el;
		el->link2 = el;
		return el;
	}
}

//Функция для сортировки

CSHIP *C_sort(CSHIP *ship)
{
	if (ship == NULL)
	{
		puts("Нечего сортировать");
		return NULL;
	}
	CSHIP *ship_sorted;//возвращает указатель на первый элемент списка
	CSHIP *link;       //Вспомогательный указатель
	SORTDATA sort;     //Данные о сортировке

	select_sort(&sort);
	ship_sorted = NULL;

	ship->link2->link1 = NULL;

	do
	{
		link = ship;
		ship = ship->link1;
		link->link1 = link;
		link->link2 = link;
		ship_sorted = C_insert(ship_sorted, link, &sort);
	} while (ship != NULL);
	puts("Сортировка окончена");
	return ship_sorted;
}

//Функция для поиска

CSHIP* C_find(CSHIP* ship)
{
	FINDDATA find;  //Данные о поиске
	CSHIP *ship_res;//Указатель на первый элемент обработанного списка
	CSHIP *link;    //Вспомогательный указатель
	CSHIP *new_el;  //Вспомогательный указатель

	select_find(&find);
	ship_res = NULL;
	link = ship;
	ship->link2->link1 = NULL;
	do
	{
		if (is_need(ship->info, &find))
		{
			new_el = C_create_copy(ship);
			ship_res = C_add_frist(ship_res, new_el)->link2;
		}
		ship = ship->link1;
	} while (ship != NULL);
	if (ship_res != NULL)
		puts("Поиск завершён");
	else
		puts("Не найдено элементов, удовлетворяющих заданным критериям");

	link->link2->link1 = link;

	return ship_res;
}

//Функция для создания меню

void C_menu()
{
	int menu;       //Выбранный пункт меню
	CSHIP *ship;    //Указатель на первый элемент списка исходных данных
	CSHIP *ship_res;//Указатель на первый элемент списка результатов поиска
	bool is_find;   //Флажок поиска
	int i;          //Вспомогательная переменная

	is_find = false;
	ship = NULL;
	ship_res = NULL;

	do
	{
		system("cls");
		puts("Кольцевые двухсвязные списки");
		puts("1-добавление элементов в список");
		puts("2-удаление элементов из списка");
		puts("3-вывод элементов списка");
		puts("4-обработка списка");
		puts("5-вывод результатов последнего поиска");
		puts("6-справка");
		puts("0-возврат в надменю");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			ship = C_add(ship);
			system("pause");
			break;
		case 2:
			ship = C_del(ship);
			system("pause");
			break;
		case 3:
			C_print(ship);
			system("pause");
			break;
		case 4:
			if (ship != NULL)
			{
				do
				{
					system("cls");
					puts("Выберите тип обработки");
					puts("1-поиск (при этом формируется новый список)");
					puts("2-сортировка элементов исходного списка");
					puts("0-возврат в надменю");
					i = input_number();
					if (i<0 || i>2)
					{
						puts("Вы неправильно ввели пункт меню");
						system("pause");
					}
				} while (i<0 || i>2);
				switch (i)
				{
				case 1:
					is_find = TRUE;
					ship_res = C_free_all(ship_res);
					ship_res = C_find(ship);
					system("pause");
					break;
				case 2:
					ship = C_sort(ship);
					system("pause");
					break;
				}
			}
			else
			{
				puts("Список пуст, обрабатывать нечего");
				system("pause");
			}
			break;
		case 5:
			if (is_find)
				if (ship_res != NULL)
					C_print(ship_res);
				else
					puts("При последнем поиске не найдено элементов,\nсоответствующих заданным критериям");
			else
				puts("Вы ещё ни разу не проводили поиск");
			system("pause");
			break;
		case 6:
			puts("Справка");
			puts("При вводе строки:");
			puts("1. таб конвертируется в пробел");
			puts("2. пробелы сначала вводить запрещено");
			puts("3. пробелы в конце убираются");
			puts("Кораблик имеет пять полей:");
			puts("1. Имя-строка от 1 до 30 символов");
			puts("2. Тип-строка от 1 до 20 символов");
			puts("3. Положение по x-целое число от -999999 до 9999999");
			puts("4. Положение по y-целое число от -999999 до 9999999");
			puts("5. Положение по z-целое число от -999999 до 9999999");
			puts("Результат обработки - новый массив данных состоящий из элементов исходного");
			puts("массива, которые соответствуют заданным критериям.");
			puts("Русские буквы вводить запрещено.");
			puts("Для двухсвязных кольцевых списков возможен вывод с n-ого элемента");
			puts("по часовой и пртив часовой стрелки");

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
	ship = C_free_all(ship);
	ship_res = C_free_all(ship_res);
}

//@@

int main()
{
	setlocale(0, "RU");
	puts(" ООО  БББББ  ССС  ААА");
	puts("О   О Б     С    А   А");
	puts("О   О ББББ  С    ААААА");
	puts("О   О Б   Б С    А   А");
	puts(" ООО  ББББ   ССС А   А");
	system("pause");
	char menu;//Выбранный пункт меню
	do
	{
		system("cls");
		puts("Списки");
		puts("1-односвязные списки");
		puts("2-двухсвязные списки");
		puts("3-кольцевые двухсвязные списки");
		puts("4-справка");
		puts("0-выход");
		menu = input_number();
		system("cls");
		switch (menu)
		{
		case 1:
			O_menu();
			break;
		case 2:
			D_menu();
			break;
		case 3:
			C_menu();
			break;
		case 4:
			puts("Эта программа обрабатывает заданный список данных о корабликах.");
			puts("Эта программа работает со списками трёх типов: ");
			puts("односвязный, двухсвязный, кольцевой двухсвязный.");
			puts("Координаты в данной программе целочисленные.");
			puts("При сравнении названий\\типов сравнивается сначала первый символ,");
			puts("потом второй и.т.д.");
			puts("При сравнении символов используется их коды.");
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
}


