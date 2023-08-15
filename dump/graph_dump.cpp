#include "graph_dump.h"

const char* name_dump_file = "tree.dot";
const unsigned max_lenght_cmd = 100;

void graph_dump(tree* tr)
{
	if (!tr) return;

	FILE* dump_file = fopen(name_dump_file, "w");
	if (!dump_file)
	{
		printf("Cann't open file %s\n", name_dump_file);
		return;
	}

	fprintf (dump_file,
		"digraph G{\n"
		"	rankdir=TB;\n"
		"	node[shape=\"rectangle\",fontsize=14];\n"
		"	edge[arrowhead=\"open\"];\n");

	int num_node = 0;
	graph_dump_node(tr->root, &num_node, dump_file);

	fprintf (dump_file,
		"}\n");

	fclose(dump_file);
	CtorDumpFile_dot();	
}

void CtorDumpFile_dot()
{
	printf("1\n");
	char cmd[max_lenght_cmd] = {0};
	sprintf(cmd, "dot -Tpng %s -o tree.png", name_dump_file);
	system (cmd);
}

#define edge(color)														\
	fprintf (dump_file,													\
		"	V%d->V%d[color=\"%s\"];\n", num_now_node, *num_node, color)

#define dump(type) graph_dump_ ## type(now_node, *num_node, dump_file)

void graph_dump_node(node* now_node, int* num_node, FILE* dump_file)
{
	if (!now_node)
		return;
	int num_now_node = *num_node;

	switch(now_node->type)
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
		edge("red");
		graph_dump_node(left_node(now_node), num_node, dump_file);
	}

	if (right_node(now_node))
	{
		edge("blue");
		graph_dump_node(right_node(now_node), num_node, dump_file);
	}
}
#undef dump
#undef edge

void graph_dump_var_num(node* now_node, int num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | { type: NUM | data: %lf } | { left node: %p | right node: %p } }\"];\n",
	    num_node, now_node, value_num(now_node), left_node(now_node), right_node(now_node));
}

#define print_op(name, op)							\
	case OP_ ## name:								\
		fprintf(dump_file, "{ type: OP | %s }", op);\
		break; 

void graph_dump_op(node* now_node, int num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | ",
	    num_node, now_node);	

	switch(num_op(now_node))
	{
		print_op(ADD, "+")
		print_op(SUB, "-")
		print_op(MUL, "*")
		print_op(DIV, "/")
		print_op(POW, "^")
		print_op(SIN, "sin")
		print_op(COS, "cos")
		print_op(LOG, "log")
		print_op(TAN,  "tg")
		default:
			printf("Didn't find operation %d\n", num_op(now_node));
	}

	fprintf (dump_file,
		" | { left node: %p | right node: %p } }\"];\n",
	    left_node(now_node), right_node(now_node));
}

#undef print_op

void graph_dump_var(node* now_node, int num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | { type: VAR | name: %s } | {	 left node: %p | right node: %p } }\"];\n",
		num_node, now_node, value_name(now_node), left_node(now_node), right_node(now_node));
}

void graph_dump_wrong_type(node* now_node, int num_node, FILE* dump_file)
{
	fprintf (dump_file,
		"V%d[shape=record, label="
		"\"{ ptr: %p | { type: WRONG | data: %lf } | { left node: %p | right node: %p } }\"];\n",
	    num_node, now_node, value_num(now_node), left_node(now_node), right_node(now_node));
}