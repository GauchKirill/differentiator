#ifndef READ_TREE_H
#define READ_TREE_H

#include "../struct/tree.h"

tree* get_tree(const char* equation_file);
char* read_text(const char* equation_file);
void  skip_space(const char** buf);

node* get_graph         (const char* buf);
node* get_add_sub	    (const char** buf);
node* get_mul_div  	    (const char** buf);
node* get_pow		    (const char** buf);
node* get_prim_expr     (const char** buf);
node* get_func		    (const char** buf);
void  get_name_func_var	(char* name_func, const char** buf);
node* get_num           (const char** buf);

#endif