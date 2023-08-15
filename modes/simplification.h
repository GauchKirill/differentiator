#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "../struct/tree.h"

void simply_tree(tree* tr);

bool calculate_node(node* now_node);
bool simply_node(node* now_node);

bool is_approx_equal(node* now_node, double value);
bool LNull(node* now_node);
bool RNull(node* now_node);
bool LUnit(node* now_node);
bool RUnit(node* now_node);

bool is_simply(node* now_node);
bool is_simply_ADD(node* now_node);
bool is_simply_SUB(node* now_node);
bool is_simply_MUL(node* now_node);
bool is_simply_DIV(node* now_node);

void node_rewrite(node* dest, node* src);

#endif