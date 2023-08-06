#ifndef LATEX_DUMP_H
#define LATEX_DUMP_H

#include "tree.h"

void latex_dump(tree* tr);
void latex_dump_node(node* now_node, FILE* latex_file, int prior_before_node);
void latex_dump_op	(node* now_node, FILE* latex_file, int prior_before_node);
void latex_dump_num	(node* now_node, FILE* latex_file, int prior_before_node);
void latex_dump_var	(node* now_node, FILE* latex_file, int prior_before_node);

#endif