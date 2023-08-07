#include "modes.h"
#include "stdio.h"
#include "./settings/const.h"

void modes(int argc, char** argv)
{
	unsigned num_modes = 0;

	if (argc == 1)
	{
		printf("\n"
				"Введите номер режима. Введите целое чисто от 0 до %u.\n\n", CNT_MODES - 1);
		help();

		while (scanf("%u", &num_modes) != 1 && num_modes >= CNT_MODES)
			scanf("%*s");

	} else
	if (argc == 2)
	{
		printf("Введите название файла с выражением или \"-\", чтобы ввести выражение в консоль.\n");
		scanf("%s", equation_file);


	} else
	{

	}

	do
	{
		switch (num_modes)
		{
		
		}
	} while (num_modes);
	
	
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