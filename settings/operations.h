// name_func/op_priority/   operator  !!! when adding a new function, be sure to describe
DEF_OP(  ADD,       0,          +)  //  the rules for taking the derivative in ...!!! 
DEF_OP(  SUB,       0,          -)  //   modes/differentiator.cpp   modes/differentiator.h
DEF_OP(  MUL,       1,          *)
DEF_OP(  DIV,       1,          /)
DEF_OP(  POW,       3,          ^)
DEF_FUNC(LOG,       2,         log)
DEF_FUNC(SIN,       2,         sin)
DEF_FUNC(COS,       2,         cos)
DEF_FUNC(TAN,       2,          tg)
DEF_FUNC(SQRT,      3,         sqrt)