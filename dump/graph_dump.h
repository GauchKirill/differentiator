#ifndef GRAPH_DUMP_H
#define GRAPH_DUMP_H

#include "tree.h"

void graph_dump (tree* tr);
void graph_dump_node (node* top, int* num_node, FILE* dump_file);
void graph_dump_op 			(node* now_node, int num_node, FILE* dump_file);
void graph_dump_num			(node* now_node, int num_node, FILE* dump_file);
void graph_dump_var			(node* now_node, int num_node, FILE* dump_file);
void graph_dump_wrong_type	(node* now_node, int num_node, FILE* dump_file);
void CtorDumpFile_dot(void);

#endif