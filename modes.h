#ifndef MODES_H
#define MODES_H

#define DEF_MOD(flag, name, discription, func)    \
    MOD_ ## name,

enum modes
{
    #include "./settings/all_modes.h"
    CNT_MODES
};

#undef DEF_MOD

void modes(int argc, char** argv);
void help(void);
void mod_diff(const char* equation_file);
void mod_tailor(const char* equation_file);
void calculator(const char* equation_file);

unsigned    get_num_modes(void);
void        get_equation_file(char* equation_file);

#endif