#include "tree.h"
#include "../settings/tree_settings.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../settings/const.h"

tree* tree_ctor(void)
{
	tree* tr = (tree*) calloc(1, sizeof(tree));
	if (!tr)
	{
		printf("\n"
				"Has not memory for tree.\n");
		return nullptr;
	}

	node* root = nullptr;
	return tr;
}

node* create_node_op (unsigned num_operation, node* left_ptr, node* right_ptr)
{
	node* new_node = (node*) calloc(1, sizeof(node));
	if (!new_node)
	{
		printf("\n"
				"Has not memory for new node of operation.\n");
		return nullptr;
	}

	type(new_node)			= OPERATION;
	num_op(new_node)		= num_operation;
	priority_op(new_node)	= get_op_func_priority(num_operation);
	left_node(new_node)  	= left_ptr;
	right_node(new_node) 	= right_ptr;

	return new_node;
}

node* create_node_num (double num)
{
	node* new_node = (node*) calloc(1, sizeof(node));
	if (!new_node)
	{
		printf("\n"
				"Has not memory for new node of number.\n");
		return nullptr;
	}

	new_node->type			= NUMBER;
	value_num(new_node)		= num;
	left_node(new_node) 	= nullptr;
	right_node(new_node) 	= nullptr;

	return new_node;
}

node* create_node_var (const char* variable_name)
{
	node* new_node = (node*) calloc(1, sizeof(node));
	if (!new_node)
	{
		printf("\n"
				"Has not memory for new node of variable.\n");
		return nullptr;
	}

	value_name(new_node) = (char*) calloc(LENGHT_VAR, sizeof(char));
	if (!value_name(new_node))
	{
		printf("\n"
				"Has not memory for variable name.\n");
		free(new_node);
		return nullptr;
	}

	new_node->type 	 	 	= VARIABLE;
	strncpy(value_name(new_node), variable_name, LENGHT_VAR);
	left_node(new_node)		= nullptr;
	right_node(new_node) 	= nullptr;

	return new_node;
}

node* create_node_func (unsigned num_func, node* operand_node)
{
	node* func_node = (node*) calloc(1, sizeof(node));
	if(!func_node)
	{
		printf("\n"
				"Has not memory for new node of function.\n");
		return nullptr;	
	}

	func_node->type			= OPERATION;
	num_op(func_node) 		= num_func;
	priority_op(func_node) 	= get_op_func_priority(num_func);
	operand_ptr(func_node)	= operand_node;

	return func_node;
}

node* node_copy(node* now_node)
{
	if (!now_node)
		return nullptr;
 
	switch(now_node->type)
	{
		case OPERATION:
			return create_node_op(num_op(now_node), node_copy(left_node(now_node)), node_copy(right_node(now_node)));
			break;
		case NUMBER:
			return create_node_num(value_num(now_node));
			break;
		case VARIABLE:
			return create_node_var(value_name(now_node));
			break;
		default:
			printf("\n"
					"node_copy: Don't find type with number %u\n", type(now_node));
			return nullptr;
			break;
	}
}

void node_rewrite(node* dest, node* src)
{
	type(dest) = type(src);

	switch (type(src))
	{
	case OPERATION:
		num_op(dest) = num_op(src);
		priority_op(dest) = priority_op(src);
		break;
	case NUMBER:
		value_num(dest) = value_num(src);
		break;
	case VARIABLE:
		value_name(dest) = (char*) calloc(LENGHT_NAME_VAR, sizeof(char));
		if (!value_name(dest))
		{
			printf("\n"
					"node_rewrite: Has not memory for rewrite node \"%s\"\n", value_name(src));
			return;
		}
		strncpy(value_name(dest), value_name(src), LENGHT_NAME_VAR);
		free(value_name(src));
		break;
	}
	left_node(dest) = left_node(src);
	right_node(dest) = right_node(src);
	free(src);
}

void tree_dtor(tree* tr)
{
	node_dtor(tr->root);
	free(tr);
	return;
}

void node_dtor(node* now_node)
{
	if (now_node)
	{
		node_dtor(left_node (now_node));
		node_dtor(right_node(now_node));
		if (type(now_node) == VARIABLE && value_name(now_node))
			free(value_name(now_node));
	}
	return;
}

#define DEF_OP(name, priority)	\
	case OP_ ## name:			\
		return PR_ ## name;		\
		break;

#define DEF_FUNC(name, priority)	\
	case OP_ ## name:				\
		return PR_ ## name;			\
		break;


unsigned get_op_func_priority(unsigned num_operation)
{
	switch(num_operation)
	{
		#include "../settings/operations.h"
		default:
			printf("\n"
					"Cann't find operatin.\n");
			return POISON_PRIORITY;
			break;
	}
}

#undef preority_of
