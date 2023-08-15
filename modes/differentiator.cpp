#include "./differentiator.h"
#include "./read_tree.h"
#include <stdlib.h>

void mod_diff(const char* equation_file)
{
	tree* tree_expression = get_tree(equation_file);
}

tree* differ(tree* tr, const char* var)
{
	tree* diff_tr = tree_ctor();

	diff_tr->root = diff_node(tr->root, var);

	return diff_tr;
}

#define differ_node(type)	diff_ ## type(now_node, name_of_var)
#define differ_op(name)									\
	case OP_ ## name:									\
		return diff_ ## name(now_node, name_of_var);	\
		break;

node* diff_node(node* now_node, const char* name_of_var)
{
	switch(now_node->type)
	{
		case OPERATION:
			switch(num_op(now_node))
			{
				differ_op(ADD)
				differ_op(SUB)
				differ_op(MUL)
				differ_op(DIV)
				differ_op(POW)
				differ_op(SIN)
				differ_op(COS)
				differ_op(TG)
				differ_op(LOG)
			default:
				return nullptr;
				break;
			}
			break;
		case NUMBER:
			return differ_node(num);
			break;
		case VARIABLE:
			return differ_node(var);
			break;
		default:
			printf("Wrong type. Cann't differentiate\n");
			return nullptr;
			break;
	}
}

#undef differ_op
#undef diff_node


node* diff_ADD(node* now_node, const char* name_of_var)
{
	node* diffed_node = create_node_op (OP_ADD,
										diff_node(left_node (now_node), name_of_var),
										diff_node(right_node(now_node), name_of_var));

	return diffed_node;
}

node* diff_SUB(node* now_node, const char* name_of_var)
{
	node* diffed_node = create_node_op (OP_SUB,
										diff_node(left_node (now_node), name_of_var),
										diff_node(right_node(now_node), name_of_var));
	
	return diffed_node;
}

node* diff_MUL(node* now_node, const char* name_of_var)
{	
	node* left_diffed_node  = create_node_op(OP_MUL, diff_node(left_node (now_node), name_of_var),
															 node_copy(right_node(now_node)));
	node* right_diffed_node = create_node_op(OP_MUL, node_copy(left_node (now_node)),
															 diff_node(right_node(now_node), name_of_var));
	node* diffed_node 		= create_node_op(OP_ADD, left_diffed_node, right_diffed_node);

	return diffed_node;
}

node* diff_DIV(node* now_node, const char* name_of_var)
{
	node* mul_l_node 	= create_node_op (OP_MUL,	diff_node(left_node (now_node), name_of_var),
															node_copy(right_node(now_node)));
	node* mul_r_node 	= create_node_op (OP_MUL, 	node_copy(left_node (now_node)),
															diff_node(right_node(now_node), name_of_var));
	node* sub_node 	 	= create_node_op (OP_SUB, mul_l_node, mul_r_node);
	node* const_2_node	= create_node_var_num(2);
	node* pow_node 		= create_node_op (OP_POW, right_node(now_node), const_2_node);
	node* diffed_node 	= create_node_op (OP_DIV, sub_node, pow_node);

	return diffed_node;
}

node* diff_POW(node* now_node, const char* name_of_var)
{
	node* div_node		 = create_node_op	(OP_DIV, right_node(now_node), left_node(now_node));
	node* log_node 		 = create_node_func (OP_LOG, node_copy(left_node(now_node)));
	node* left_add_node  = create_node_op	(OP_MUL, div_node, diff_node(left_node (now_node), name_of_var));
	node* right_add_node = create_node_op	(OP_MUL, log_node, diff_node(right_node(now_node), name_of_var));
	node* right_mul_node = create_node_op	(OP_ADD, left_add_node, right_add_node); 
	node* diffed_node 	 = create_node_op	(OP_MUL, node_copy(now_node), right_mul_node);

	return diffed_node;
}

node* diff_SIN(node* now_node, const char* name_of_var)
{
	node* cos_node 		= create_node_func(OP_COS, node_copy(right_node(now_node)));
	node* diffed_node 	= create_node_op  (OP_MUL, cos_node, diff_node(right_node(now_node), name_of_var));
	return diffed_node;
}

node* diff_COS(node* now_node, const char* name_of_var)
{
	node* sin_node 		= create_node_func	(OP_SIN, node_copy(right_node(now_node)));
	node* mul_node 		= create_node_op  	(OP_MUL, sin_node, diff_node(right_node(now_node), name_of_var));
	node* const_node 	= create_node_num	(-1);
	node* diffed_node 	= create_node_op	(OP_MUL, const_node, mul_node);
	return diffed_node;
}

node* diff_LOG(node* now_node, const char* name_of_var)
{
	node* const_node 	= create_node_num(1); 
	node* left_mul_node	= create_node_op(OP_DIV, const_node, node_copy(right_node(now_node)));
	node* diffed_node 	= create_node_op(OP_MUL, left_mul_node, diff_node(right_node(now_node), name_of_var));

	return diffed_node;
}

node* diff_TAN(node* now_node, const char* name_of_var)
{
	node* const_node 	= create_node_num(1);
	node* const_2_node	= create_node_num(2);
	node* cos_node 		= create_node_func(OP_COS, node_copy(right_node(now_node)));
	node* pow_node 		= create_node_op(OP_POW, cos_node, const_2_node);
	node* mul_l_node 	= create_node_op(OP_DIV, const_node, pow_node);
	node* diffed_node 	= create_node_op(OP_MUL, mul_l_node, diff_node(right_node(now_node), name_of_var));

	return diffed_node;
}

node* diff_var_num(node* now_node, const char* name_of_var)
{
	return create_node_num(0);
}

node* diff_var(node* now_node, const char* name_of_var)
{
	const int max_lenght_name = 10;
	if (strncmp(name_of_var, var_name(now_node), max_lenght_name) == 0)
		return create_node_num(1);
	else
		return create_node_num(0);
}

