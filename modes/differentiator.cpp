#include "./differentiator.h"
#include <string.h>
#include <stdio.h>
#include "./read_tree.h"
#include "../settings/const.h"
#include "./simplification.h"

void mode_differ(const char* equation_file)
{
	tree* tree_expression = get_tree(equation_file);

	unsigned derivative_num = 0;
	do
	{
		printf("\n"
			"Введите номер производной, которую следует взять:\n");
	} while ((scanf("%u", derivative_num) != 1) && scanf("%*s"));

	printf("\n"
			"Введите название переменной, по которой взять производную:\n");

	char name_of_var[LENGHT_NAME_VAR] = "";
	fgets(name_of_var, LENGHT_NAME_VAR, stdin);

	tree* new_tree_expression = tree_expression;
	while (derivative_num--)
	{
		tree* delete_tree = new_tree_expression;
		new_tree_expression = differ(new_tree_expression, name_of_var);
		simply_tree(new_tree_expression);
		tree_dtor(delete_tree);
	}
}

tree* differ(tree* tr, const char* var)
{
	tree* differ_tr = tree_ctor();

	differ_tr->root = differ_node(tr->root, var);

	return differ_tr;
}

#define DEF_OP(name, priority, operator)				\
	case OP_ ## name:									\
		return differ_ ## name(now_node, name_of_var);	\
		break;

#define DEF_FUNC(name, priority, operator)				\
	case OP_ name:										\
		return deffer_ ## name(now_node, name_of_var)	\
		break;

node* differ_node(node* now_node, const char* name_of_var)
{
	switch(type(now_node))
	{
		case OPERATION:
			switch(num_op(now_node))
			{
				#include "../settings/operations.h"
			default:
				printf("\n"
						"deffer_node: Don't find operations with number %u\n", num_op(now_node));
				return nullptr;
				break;
			}
			break;
		case NUMBER:
			return differ_num(now_node);
			break;
		case VARIABLE:
			return differ_var(now_node, name_of_var);
			break;
		default:
			printf("\n"
					"differ_node: Wrong type %u. Cann't differentiate\n", now_node->type);
			return nullptr;
			break;
	}
}

#undef DEF_OP
#undef DEF_FUNC

//-------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------     Add new func     -----------------------------------------------------

node* differ_ADD(node* now_node, const char* name_of_var)
{
	return create_node_op (OP_ADD,
							differ_node(left_node (now_node), name_of_var),
							differ_node(right_node(now_node), name_of_var));
}

node* differ_SUB(node* now_node, const char* name_of_var)
{
	return create_node_op (OP_SUB,
							differ_node(left_node (now_node), name_of_var),
							differ_node(right_node(now_node), name_of_var));
}

node* differ_MUL(node* now_node, const char* name_of_var)
{	
	node* left_differentiated_node  = create_node_op(OP_MUL, differ_node(left_node (now_node), name_of_var),
															 node_copy(right_node(now_node)));

	node* right_differentiated_node = create_node_op(OP_MUL, node_copy(left_node (now_node)),
															 differ_node(right_node(now_node), name_of_var));

	return create_node_op(OP_ADD, left_differentiated_node, right_differentiated_node);
}

node* differ_DIV(node* now_node, const char* name_of_var)
{
	node* mul_left_nodes 	= create_node_op (OP_MUL,	differ_node(left_node (now_node), name_of_var),
														node_copy(right_node(now_node)));

	node* mul_right_nodes 	= create_node_op (OP_MUL, 	node_copy(left_node (now_node)),
														differ_node(right_node(now_node), name_of_var));

	node* subb_node 	= create_node_op (OP_SUB, mul_left_nodes, mul_right_nodes);
	node* const_node	= create_node_num(2);
	node* pow_node		= create_node_op (OP_POW, right_node(now_node), const_node);

	return create_node_op (OP_DIV, subb_node, pow_node);
}

node* differ_POW(node* now_node, const char* name_of_var)
{
	node* div_node				= create_node_op	(OP_DIV, right_node(now_node), left_node(now_node));
	node* log_node				= create_node_func	(OP_LOG, node_copy(left_node(now_node)));
	node* left_add_node			= create_node_op	(OP_MUL, div_node, differ_node(left_node (now_node), name_of_var));
	node* right_add_node		= create_node_op	(OP_MUL, log_node, differ_node(right_node(now_node), name_of_var));
	node* right_mul_node 		= create_node_op	(OP_ADD, left_add_node, right_add_node); 
	return						create_node_op		(OP_MUL, node_copy(now_node), right_mul_node);
}

node* differ_LOG(node* now_node, const char* name_of_var)
{
	node* const_node 	= create_node_num	(1); 
	node* left_mul_node	= create_node_op	(OP_DIV, const_node, node_copy(right_node(now_node)));
	return				create_node_op		(OP_MUL, left_mul_node, differ_node(operand_ptr(now_node), name_of_var));
}

node* differ_SIN(node* now_node, const char* name_of_var)
{
	node* cos_node	= create_node_func	(OP_COS, node_copy(operand_ptr(now_node)));
	return 			create_node_op  	(OP_MUL, cos_node, differ_node(operand_ptr(now_node), name_of_var));
}

node* differ_COS(node* now_node, const char* name_of_var)
{
	node* sin_node 		= create_node_func	(OP_SIN, node_copy(operand_ptr(now_node)));
	node* mul_node 		= create_node_op  	(OP_MUL, sin_node, differ_node(operand_ptr(now_node), name_of_var));
	node* const_node	= create_node_num	(-1);
	return				create_node_op		(OP_MUL, const_node, mul_node);
}

node* differ_TAN(node* now_node, const char* name_of_var)
{
	node* const_node 	= create_node_num	(1);
	node* const_2_node	= create_node_num	(2);
	node* cos_node 		= create_node_func	(OP_COS, node_copy(operand_ptr(now_node)));
	node* pow_node 		= create_node_op	(OP_POW, cos_node, const_2_node);
	node* mul_l_node 	= create_node_op	(OP_DIV, const_node, pow_node);
	return				create_node_op		(OP_MUL, mul_l_node, differ_node(operand_ptr(now_node), name_of_var));
}

node* deffer_SQRT(node* now_node, const char* name_of_var)
{
	node* const_node	= create_node_num	(0.5);
	node* div_node		= create_node_op	(OP_DIV, const_node, node_copy(now_node));
	return 				create_node_op		(OP_MUL, div_node, differ_node(operand_ptr(now_node), name_of_var));
}

//-------------------------------------------------------------------------------------------------------------------------

node* differ_num(node* now_node)
{
	return create_node_num(0);
}

node* differ_var(node* now_node, const char* name_of_var)
{
	if (strncmp(name_of_var, value_name(now_node), LENGHT_NAME_VAR) == 0)
		return create_node_num(1);
	else
		return create_node_num(0);
}

