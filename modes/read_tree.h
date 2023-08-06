#ifndef READ_TREE_H
#define READ_TREE_H

#include "tree.h"

tree* read_tree();
char* read_text();
void skip_space(const char** buf);

node* GetGraph		(const char* buf);
node* GetAddSub		(const char** str);
node* GetMulDiv  	(const char** str);
node* GetPow		(const char** str);
node* GetPrimExpr   (const char** str);
node* GetNum  		(const char** str);
node* GetFunc		(const char** str);
void GetNameFunc	(char* name_func, const char** str);

#endif