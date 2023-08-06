#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "tree.h"

node* node_copy(node* now_node);
tree* differ(tree* tr, const char* var);
node* diff_node(node* now_node, const char* name_of_var);
node* diff_ADD(node* now_node, const char* name_of_var);
node* diff_SUB(node* now_node, const char* name_of_var);
node* diff_MUL(node* now_node, const char* name_of_var);
node* diff_DIV(node* now_node, const char* name_of_var);
node* diff_SIN(node* now_node, const char* name_of_var);
node* diff_COS(node* now_node, const char* name_of_var);
node* diff_TG (node* now_node, const char* name_of_var);
node* diff_LOG(node* now_node, const char* name_of_var);
node* diff_POW(node* now_node, const char* name_of_var);
node* diff_var_num(node* now_node, const char* name_of_var);
node* diff_var(node* now_node, const char* name_of_var);

#endif