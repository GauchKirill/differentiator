#include "read_tree.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../settings/const.h"

tree* get_tree(const char* equation_file)
{
	tree* tr = tree_ctor();
	char* buf = read_text(equation_file);
	tr->root = get_graph(buf);
	free(buf);
	return tr;
}

char* read_text(const char* equation_file)
{
	int cmp = strncmp(equation_file, "-", 3);

	if (cmp != 0)
	{
		struct stat stbuf;

		if (stat(equation_file, &stbuf) == -1)
		{
			printf("\n"
					"Can not open file \"%s\"\n", equation_file);
			return nullptr;
		}
		else
		{
			long long cnt_char = stbuf.st_size;
			char* buf = (char*) calloc(cnt_char, sizeof(char));
			if(!buf)
			{
				printf("\n"
						"Has not memory for buf\n");
				return nullptr;
			}

			FILE* text_stream = nullptr;
			if (cmp == 0)
				text_stream = stdin;
			else
				text_stream = fopen(equation_file, "r");

			if (!text_stream)
			{
				printf("\n"
						"Can not open file \"%s\"\n", equation_file);
				return nullptr;
			}

			fread (buf, stbuf.st_size, sizeof(char), text_stream);

			fclose(text_stream);
			return buf;
		}
	} else
	{
		char* buf = nullptr;
		size_t size = 0;
		getline(&buf, &size, stdin);
		return buf;
	}
}

void skip_space(const char** buf)
{
	while(**buf == ' '  || **buf == '\n' || **buf == '\t' || **buf == '\r' || **buf == '\v')
		(*buf)++;
}

node* get_graph(const char* buf)
{
	node* new_node = get_add_sub(&buf);
	skip_space(&buf);
	assert(*buf == '\0');
	return new_node;
}

node* get_add_sub(const char** buf)
{
	node* new_operator = nullptr;
	node* left_operand = nullptr;
	new_operator = left_operand = get_mul_div(buf);
	while(**buf == '+' || **buf == '-')
	{
		char op = **buf;
		(*buf)++;
		node* right_operand = get_mul_div(buf);
		if (op == '+')
			new_operator = create_node_op(OP_ADD, new_operator, right_operand);
		else
			new_operator = create_node_op(OP_SUB, new_operator, right_operand);
		skip_space(buf);
	}
	return new_operator;
}

node* get_mul_div(const char** buf)
{
	node* new_operator = nullptr;
	node* left_operand = nullptr;
	new_operator = left_operand = get_pow(buf);
	while(**buf == '*' || **buf == '/')
	{
		char op = **buf;
		(*buf)++;
		node* right_operand = get_pow(buf);
		if (op == '*')
			new_operator = create_node_op(OP_MUL, new_operator, right_operand);
		else
			new_operator = create_node_op(OP_DIV, new_operator, right_operand);
		skip_space(buf);
	}
	return new_operator;
}

node* get_pow(const char** buf)
{
	node* new_operator = nullptr;
	node* left_operand = nullptr;
	new_operator = left_operand = get_prim_expr(buf);
	while(**buf == '^')
	{
		(*buf)++;
		node* right_operand = get_prim_expr(buf);
		new_operator = create_node_op(OP_POW, new_operator, right_operand);
		skip_space(buf);
	}
	return new_operator;
}

node* get_prim_expr(const char** buf)
{

	node* new_operator = nullptr;
	int sign = 1;
	skip_space(buf);
	if (**buf == '+' || **buf == '-')
	{
		sign = (**buf == '+') ? 1 : -1;
		(*buf)++;
		skip_space(buf);
	}
	if (**buf == '(')
	{
		(*buf)++;
		new_operator = get_add_sub(buf);
		assert(**buf == ')');
		(*buf)++;
	}
	else if ('a' <= **buf && **buf <= 'z')
		new_operator = get_func(buf);
	else
		new_operator = get_num(buf);
	skip_space(buf);
	if (sign == -1)
	{
		node* sign_node = create_node_num(-1);
		new_operator = create_node_op(OP_MUL, sign_node, new_operator);
	}
	return new_operator;
}

#define DEF_OP(name, priority, operator)
#define DEF_FUNC(name, priority, operator)																				\
	if (strncmp(name_func, #operator, (LENGHT_NAME_VAR > LENGHT_NAME_FUNC) ? LENGHT_NAME_VAR : LENGHT_NAME_FUNC) == 0)	\
		new_func = create_node_func(OP_ ## name, new_func);																\
	else

node* get_func(const char** buf)
{
	node* new_func = nullptr;
	char name_func[(LENGHT_NAME_VAR > LENGHT_NAME_FUNC) ? LENGHT_NAME_VAR : LENGHT_NAME_FUNC] = "";
	get_name_func_var(name_func, buf);
	skip_space(buf);
	if (**buf == '(')
	{
		new_func = get_prim_expr(buf);
		#include "../settings/operations.h"
			assert(false);
	}
	else
		new_func = create_node_var(name_func); // value
	
	skip_space(buf);
	return new_func;
}

#undef DEF_OP
#undef DEF_FUNC

void get_name_func_var(char* name_func, const char** buf)
{
	
	for(unsigned i = 0; 'a' <= **buf  && **buf <= 'z' && i < (LENGHT_NAME_FUNC - 1); i++, (*buf)++)
		name_func[i] = **buf;
}

node* get_num(const char** buf)
{
	skip_space(buf);
	double val = 0;
	const char* start = *buf;
	while('0' <= **buf && **buf <= '9')
	{
		val = 10*val + **buf - '0';
		(*buf)++;
	}
	assert(start != *buf);

	if (**buf == '.' || **buf == ',')
	{
		(*buf)++;
		unsigned k = 1;
		while('0' <= **buf && **buf <= '9')
		{
			val = 10*val + **buf - '0';
			k *= 10;
			(*buf)++;
		}
		assert(k != 1);
		val /= k;
	}
	skip_space(buf);
	return create_node_num(val);
}
