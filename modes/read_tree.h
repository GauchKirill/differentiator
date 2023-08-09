#ifndef READ_TREE_H
#define READ_TREE_H

#include "../struct/tree.h"

tree* read_tree();
char* read_text();
void skip_space(const char** buf);

node* get_graph		(const char* buf);
node* get_add_sub		(const char** str);
node* get_mul_div  	(const char** str);
node* get_pow		(const char** str);
node* get_prim_expr   (const char** str);
node* get_num  		(const char** str);
node* get_func		(const char** str);
void get_name_func	(char* name_func, const char** str);

#endif