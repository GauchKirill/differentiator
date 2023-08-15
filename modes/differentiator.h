#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "../struct/tree.h"

node* node_copy(node* now_node);
tree* differ(tree* tr, const char* var);
node* differ_node(node* now_node, const char* name_of_var);
node* differ_var_num(node* now_node, const char* name_of_var);
node* differ_var(node* now_node, const char* name_of_var);

//--------------------------------------------------------------------------------------------------------
node* differ_ADD(node* now_node, const char* name_of_var);
node* differ_SUB(node* now_node, const char* name_of_var);
node* differ_MUL(node* now_node, const char* name_of_var);
node* differ_DIV(node* now_node, const char* name_of_var);
node* differ_POW(node* now_node, const char* name_of_var);
node* differ_SIN(node* now_node, const char* name_of_var);
node* differ_COS(node* now_node, const char* name_of_var);
node* differ_TAN(node* now_node, const char* name_of_var);
node* differ_LOG(node* now_node, const char* name_of_var);
//----------------------------------------------------------------------------------------------------------

#endif