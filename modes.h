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

void modes  (int argc, char** argv);
void help   (void);

#endif