#include "simplification.h"

#include <math.h>

void simply_tree(tree* tr)
{
	while(calculate_node(tr->root) || simply_node(tr->root));
}

bool simply_node(node* now_node)
{
	if(!now_node)
		return false;
	bool did_subtrees_simply = false;
	if (now_node->type == OPERATION)
	{
		did_subtrees_simply = simply_node(left_node(now_node)) | simply_node(right_node(now_node));
		did_subtrees_simply |= is_simply(now_node); 
	}
	return did_subtrees_simply;
}

#define simply_op(name)						\
	case OP_ ## name:						\
		return is_simply_ ## name(now_node);\
		break;


bool is_simply(node* now_node)
{
	switch(num_op(now_node))
	{
		simply_op(ADD)
		simply_op(DIV)
		simply_op(MUL)
		simply_op(SUB)
		default:
			return false;
	}
}

#undef simply_op

bool LNull(node* now_node)
{
	return (left_node(now_node)->type  == NUMBER && var_num(left_node(now_node))  == 0);
}

bool RNull(node* now_node)
{
	return (right_node(now_node)->type == NUMBER && var_num(right_node(now_node)) == 0);
}

bool LUnit(node* now_node)
{
	return (left_node(now_node)->type  == NUMBER && var_num(left_node(now_node))  == 1);
} 

bool RUnit(node* now_node)
{
	return (right_node(now_node)->type == NUMBER && var_num(right_node(now_node)) == 1);
}

bool is_simply_ADD(node* now_node)
{
	if (LNull(now_node))
	{
		node_dtor(left_node(now_node));
		node_copy_dtor(now_node, right_node(now_node));
		return true;
	}
	else if (RNull(now_node))
	{
		node_dtor(right_node(now_node));
		node_copy_dtor(now_node, left_node(now_node));
		return true;
	}
	return false;
}

bool is_simply_SUB(node* now_node)
{
	if (RNull(now_node))
	{
		node_dtor(right_node(now_node));
		node_copy_dtor(now_node, left_node(now_node));
		return true;
	}
	return false;

}

bool is_simply_MUL(node* now_node)
{
	if (LUnit(now_node))
	{
		node_dtor(left_node(now_node));
		node_copy_dtor(now_node, right_node(now_node));
		return true;
	}
	else if (RUnit(now_node))
	{
		node_dtor(right_node(now_node));
		node_copy_dtor(now_node, left_node(now_node));
		return true;
	}
	else if (LNull(now_node) || RNull(now_node))
	{
		node_dtor(right_node(now_node));
		node_dtor(left_node(now_node));
		now_node->type = NUMBER;
		var_num(now_node) = 0;
		left_node(now_node) = nullptr;
		right_node(now_node) = nullptr;
		return true;
	}
	return false;
}

bool is_simply_DIV(node* now_node)
{
	if (LNull(now_node))
	{
		node_copy_dtor(now_node, left_node(now_node));
		return true;
	}
	return false;
}

void node_copy_dtor(node* get_node, node* put_node)
{
	get_node->type = put_node->type;
	left_node(get_node) = left_node(put_node);
	right_node(get_node) = right_node(put_node);
	switch(put_node->type)
	{
	case OPERATION:
		num_op(get_node) 		= num_op(put_node);
		priority_op(get_node) 	= priority_op(put_node);
		break;
	case NUMBER:
		var_num(get_node) = num_op(put_node);
		break;
	case VARIABLE:
		var_name(get_node) = var_name(put_node);
		break;
	}
	free(put_node);
}