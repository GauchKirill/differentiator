#include "modes.h"
#include "stdio.h"
#include "./settings/const.h"

#define DEF_MOD(flag, name, discription, function)	\
	case MOD_ ## name:								\
		function;									\
		break;

void modes(int argc, char** argv)
{
	unsigned num_mode = 0;
	char equation_file[LENGTH_NAME_FILE] = "-";

	if (argc == 1)
		num_mode = get_num_modes();
	
	else
	if (argc == 2 && num_mode != MOD_EXIT && num_mode != MOD_HELP)
		get_equation_file(equation_file);

	while (num_mode != MOD_EXIT)
	{
		switch (num_mode)
		{
			#include "./settings/all_modes.h"
			default:
				printf("\n"
						"Нет режима с номером %u. Попробуйте ещё раз.\n", num_mode);
				help();
				break;
		}
		num_mode = get_num_modes();
		if (num_mode != MOD_EXIT && num_mode != MOD_HELP)
			get_equation_file(equation_file);
	}	
}

#undef DEF_MOD

unsigned get_num_modes()
{
	unsigned num_mode = 0;

	printf("\n"
			"Введите номер режима. Введите целое чисто от 0 до %u.\n\n", CNT_MODES - 1);
	help();

	while (scanf("%u", &num_mode) != 1 || num_mode >= CNT_MODES)
			scanf("%*s");

	return num_mode;
}

void get_equation_file(char* equation_file)
{
	printf("\n"
			"Введите название файла с выражением или \"-\", чтобы ввести выражение в консоль.\n");
		fgets(equation_file, LENGTH_NAME_FILE, stdin);
}

#define DEF_MOD(flag, name, discription, func)	\
	printf("%15s%-10u%-7s    ----- \n", name, MOD_ ## name, flag, discription);

void help()
{
	printf("\n"
			"     name      number function    flag   discription");
	#include "./settings/all_modes.h"

	printf("\n");
	return;
}

#undef DEF_MOD