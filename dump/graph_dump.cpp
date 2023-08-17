#include "../dump/graph_dump.h"
#include "../settings/const.h"
#include <stdlib.h>
#include <string.h>

const unsigned max_lenght_cmd = 100;

void graph_dump(tree* tr)
{
	if (!tr) return;

	char dot_file[LENGTH_NAME_FILE+5] = ""; // +5 на расширение
	strncpy(dot_file, name_dump_file, LENGTH_NAME_FILE);
	strncat(dot_file, ".dot", 5);

	FILE* dump_file = fopen(dot_file, "w");
	if (!dump_file)
	{
		printf("Cann't open file %s\n", dot_file);
		return;
	}

	fprintf (dump_file,
		"digraph G{\n"
			"rankdir=TB;\n"
			"node[shape=\"rectangle\",fontsize=14];\n"
			"edge[arrowhead=\"open\"];\n");

	unsigned num_node = 0;
	graph_dump_node(tr->root, &num_node, dump_file);

	fprintf (dump_file,
		"}\n");

	fclose(dump_file);
	ctor_tree_png_file();
}

void ctor_tree_png_file()
{
	char cmd[LENGHT_CMD] = "";
	sprintf(cmd, "dot -Tpng %s.dot - o %s.png", name_dump_file, name_dump_file);
	system (cmd);
}

#define draw_edge(color)												\
	fprintf (dump_file,													\
		"	V%d->V%d[color=\"%s\"];\n", num_now_node, *num_node, color);

#define dump(type) graph_dump_ ## type(now_node, *num_node, dump_file)

void graph_dump_node(node* now_node, unsigned* num_node, FILE* dump_file)
{
	if (!now_node)
		return;
	unsigned num_now_node = *num_node;

	switch(type(now_node))
	{
		case OPERATION:
			dump(op);
			break;
		case NUMBER:
			dump(num);
			break;
		case VARIABLE:
			dump(var);
			break;
		default:
			dump(wrong_type);
			break;
	}

	(*num_node)++;

	if (left_node(now_node))
	{
		draw_edge("red")
		graph_dump_node(left_node(now_node), num_node, dump_file);
	}

	if (right_node(now_node))
	{
		draw_edge("blue")
		graph_dump_node(right_node(now_node), num_node, dump_file);
	}
}
#undef dump
#undef draw_edge

void graph_dump_var_num(node* now_node, unsigned num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | { type: NUM | data: %lf } | { left node: %p | right node: %p } }\"];\n",
		num_node, now_node, value_num(now_node), left_node(now_node), right_node(now_node));
}

#define DEF_OP(name, priority, operator)					\
	case OP_ ## name:										\
		fprintf(dump_file, "{ type: OP | %s }", operator);	\
		break;

#define DEF_FUNC(name, priority, operator)					\
	case OP_ ## name:										\
		fprintf(dump_file, "{ type: FUNC | %s }", operator);\
		break; 

void graph_dump_op(node* now_node, int num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | ",
	    num_node, now_node);	

	switch(num_op(now_node))
	{
		#include "../settings/operations.h"
		default:
			fprintf(dump_file, "{ type: none | none }");
	}

	fprintf (dump_file,
		" | { left node: %p | right node: %p } }\"];\n",
	    left_node(now_node), right_node(now_node));
}

#undef DEF_OP
#undef DEF_FUNC

void graph_dump_var(node* now_node, unsigned num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | { type: VAR | name: %s } | {	 left node: %p | right node: %p } }\"];\n",
		num_node, now_node, value_name(now_node), left_node(now_node), right_node(now_node));
}

void graph_dump_wrong_type(node* now_node, unsigned num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | { type: WRONG | data: %lf } | { left node: %p | right node: %p } }\"];\n",
	    num_node, now_node, value_num(now_node), left_node(now_node), right_node(now_node));
}