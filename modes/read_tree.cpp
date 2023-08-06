#include "read_tree.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <assert.h>


tree* read_tree()
{
	tree* tr = tree_ctor();
	char* buf = read_text();
	tr->root = GetGraph(buf);
	return tr;
}

const unsigned max_lenght_data = 20;

char* read_text()
{
	printf("Введите название файла\n");
	char name_file[max_lenght_data] = {0};
	scanf("%s", name_file);

	struct stat stbuf;
	if (stat(name_file, &stbuf) == -1)
	{
		printf("Can not open file \"%s\"\n", name_file);
		return nullptr;
	}
	else
	{
		long long cnt_char = stbuf.st_size;
		char* buf = (char*) calloc(cnt_char, sizeof(char));
		if(!buf)
		{
			printf("Has not memory\n");
			return nullptr;
		}

		FILE* file = fopen(name_file, "r");
		if (!file)
		{
			printf("Can not open file \"%s\"\n", name_file);
			return nullptr;
		}

		fread (buf, stbuf.st_size, sizeof(char), file);

		fclose(file);
		return buf;
	}
}

void skip_space(const char** buf)
{
	while(**buf == ' '  || **buf == '\n' || **buf == '\t' || **buf == '\r' || **buf == '\v')
		(*buf)++;
}

size_t max_lenght_name = 5;

node* GetGraph(const char* buf)
{
	node* new_node = GetAddSub(&buf);
	skip_space(&buf);
	assert(*buf == '\0');
	return new_node;
}

node* GetAddSub(const char** str)
{
	node* new_operator = nullptr;
	node* left_operand = nullptr;
	new_operator = left_operand = GetMulDiv(str);
	while(**str == '+' || **str == '-')
	{
		char op = **str;
		(*str)++;
		node* right_operand = GetMulDiv(str);
		if (op == '+')
			new_operator = create_node_op(OP_ADD, new_operator, right_operand);
		else
			new_operator = create_node_op(OP_SUB, new_operator, right_operand);
		skip_space(str);
	}
	return new_operator;
}

node* GetMulDiv(const char** str)
{
	node* new_operator = nullptr;
	node* left_operand = nullptr;
	new_operator = left_operand = GetPow(str);
	while(**str == '*' || **str == '/')
	{
		char op = **str;
		(*str)++;
		node* right_operand = GetPow(str);
		if (op == '*')
			new_operator = create_node_op(OP_MUL, new_operator, right_operand);
		else
			new_operator = create_node_op(OP_DIV, new_operator, right_operand);
		skip_space(str);
	}
	return new_operator;
}

node* GetPow(const char** str)
{
	node* new_operator = nullptr;
	node* left_operand = nullptr;
	new_operator = left_operand = GetPrimExpr(str);
	while(**str == '^')
	{
		(*str)++;
		node* right_operand = GetPrimExpr(str);
		new_operator = create_node_op(OP_POW, new_operator, right_operand);
		skip_space(str);
	}
	return new_operator;
}

node* GetPrimExpr(const char** str)
{

	node* new_operator = nullptr;
	int sign = 1;
	skip_space(str);
	if (**str == '+' || **str == '-')
	{
		sign = (**str == '+') ? 1 : -1;
		(*str)++;
		skip_space(str);
	}
	if (**str == '(')
	{
		(*str)++;
		new_operator = GetAddSub(str);
		assert(**str == ')');
		(*str)++;
	}
	else if ('a' <= **str && **str <= 'z')
		new_operator = GetFunc(str);
	else
		new_operator = Getvar_num(str);
	skip_space(str);
	if (sign == -1)
	{
		node* sign_node = create_node_var_num(-1);
		new_operator = create_node_op(OP_MUL, sign_node, new_operator);
	}
	return new_operator;
}

#define calculate_func(name, op)							\
	if (strncmp(name_func, #name, max_lenght_name) == 0)	\
		new_func = create_node_func(OP_ ## op, new_func);	\
	else

node* GetFunc(const char** str)
{
	node* new_func = nullptr;
	char name_func[max_lenght_name] = {0};
	GetNameFunc(name_func, str);
	skip_space(str);
	if (**str == '(')
	{
		new_func = GetPrimExpr(str);

		calculate_func(sin, SIN)
		calculate_func(cos, COS)
		calculate_func(ln,  LOG)
		calculate_func(tg,  TG)
		if (strncmp(name_func, "sqrt", max_lenght_name) == 0)
			new_func = create_node_op(OP_POW, new_func, create_node_var_num(0.5));
		else
		assert(false);
	}
	else
	{
		new_func = create_node_var(name_func);
	}
	skip_space(str);
	return new_func;
}

#undef calculate_func

void GetNameFunc(char* name_func, const char** str)
{
	int i = 0;
	while('a' <= **str  && **str <= 'z' && i < max_lenght_name - 1)
	{
		name_func[i] = **str;
		(*str)++;
		i++;
	}
}

node* Getvar_num(const char** str)
{
	skip_space(str);
	double val = 0;
	const char* old_s = *str;
	while('0' <= **str && **str <= '9')
	{
		val = 10*val + **str - '0';
		(*str)++;
	}
	assert(old_s != *str);

	if (**str == '.' || **str == ',')
	{
		(*str)++;
		int k = 1;
		while('0' <= **str && **str <= '9')
		{
			val = 10*val + **str - '0';
			k *= 10;
			(*str)++;
		}
		assert(k != 1);
		val /= k;
	}
	skip_space(str);
	node* new_node_num = create_node_var_num(val);
	return new_node_num;
}
