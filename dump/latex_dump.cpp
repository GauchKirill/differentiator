#include "latex_dump.h"

const char* name_latex_file = "latex.txt";


void latex_dump(tree* tr)
{
	if (!tr) return;

	FILE* latex_file = fopen(name_latex_file, "w");
	if (!latex_file)
	{
		printf("Cann't open file %s\n", name_latex_file);
		return;
	}

	fprintf (latex_file,
		"\\documentclass[a4paper,12pt]{article}\n"
		"\\usepackage[T2A]{fontenc}\n"
		"\\usepackage[utf8]{inputenc}\n"
		"\\usepackage[english,russian]{babel}\n"
		"\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n"
		"\\begin{document}\n"
		"\\[\n");

	latex_dump_node(tr->root, latex_file, PR_NO_OP);

	fprintf (latex_file,
		"\n"
		"\\]\n"
		"\\end{document}\n");

	fclose(latex_file);
}

#define dump(op) latex_dump_ ## op(now_node, latex_file, prior_before_node)

void latex_dump_node(node* now_node, FILE* latex_file, int prior_before_node)
{
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
	}
}

#undef dump

#define print_simple_op(name, op)													\
	case OP_ ## name:																\
		latex_dump_node(left_node (now_node), latex_file, priority_op(now_node));	\
		fprintf(latex_file, "%s", op);												\
		latex_dump_node(right_node(now_node), latex_file, priority_op(now_node));	\
		break;

#define print_func(name, op)														\
	case OP_ ## name:																\
		fprintf(latex_file, "\\%s\\left(", op);		 								\
		latex_dump_node(right_node(now_node), latex_file, priority_op(now_node));	\
		fprintf(latex_file, "\\right)");											\
		break;

void latex_dump_op(node* now_node, FILE* latex_file, int prior_before_op)
{
	if (prior_before_op > priority_op(now_node))
		fprintf(latex_file,	"\\left(");

	switch(num_op(now_node))
	{
		print_simple_op(ADD, "+")
		print_simple_op(SUB, "-")
		print_simple_op(MUL, "\\cdot ")
		case OP_DIV:
			fprintf(latex_file,
				"\\frac{");
			latex_dump_node(left_node (now_node), latex_file, priority_op(now_node));
			fprintf(latex_file,
				"}{");
			latex_dump_node(right_node(now_node), latex_file, priority_op(now_node));
			fprintf(latex_file,
				"}");
			break;
		case OP_POW:
			latex_dump_node(left_node (now_node), latex_file, priority_op(now_node));
			fprintf(latex_file, "^{");
			latex_dump_node(right_node(now_node), latex_file, priority_op(now_node));
			fprintf(latex_file, "}");
			break;
		print_func(SIN, "sin")
		print_func(COS, "cos")
		print_func(LOG, "ln")
		print_func(TAN,  "tan")
		default:
			printf("Didn't find operation %d\n", num_op(now_node));

	}
	if (prior_before_op > priority_op(now_node))
		fprintf(latex_file, "\\right)");
}

#undef print_func
#undef print_simple_op

void latex_dump_var_num(node* now_node, FILE* latex_file, int prior_before_op)
{
	fprintf(latex_file, "%.2lf", var_num(now_node));
}

void latex_dump_var(node* now_node, FILE* latex_file, int prior_before_op)
{
	fprintf(latex_file,"%s", var_name(now_node));
}
