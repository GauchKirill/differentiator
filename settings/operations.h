// name_func/op_priority   !!! when adding a new function, be sure to describe the rules for taking the derivative in ...!!!
DEF_OP(  ADD,       0)      // modes/differentiator.cpp   modes/differentiator.h 
DEF_OP(  SUB,       0)
DEF_OP(  MUL,       1)
DEF_OP(  DIV,       1)
DEF_OP(  POW,       3)
DEF_FUNC(LOG,       2)
DEF_FUNC(SIN,       2)
DEF_FUNC(COS,       2)
DEF_FUNC(TAN,       2)