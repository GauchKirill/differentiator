#include "./simplification.h"
#include "../settings/const.h"

void simply_tree(tree* tr)
{
	if (!tr || !tr->root)
		return;

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
	if (!now_node || (num_op(now_node) != OPERATION))
		return false;

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

bool is_approx_equal(node* now_node, double value)
{
	if (!now_node) return false;
	
	double num = value_num(now_node);
	if (num > value && num - value < accuracy ||
		num < value && value - num < accuracy) return true;
	else return false;
}

bool LNull(node* now_node)
{
	if (!now_node || !left_node(now_node) || !right_node(now_node))
		return false;
	return (left_node(now_node)->type == NUMBER && is_approx_equal(left_node(now_node), 0));
}

bool RNull(node* now_node)
{
	return (right_node(now_node)->type == NUMBER && is_approx_equal(right_node(now_node), 0));
}

bool LUnit(node* now_node)
{
	return (left_node(now_node)->type  == NUMBER && is_approx_equal(left_node(now_node), 1));
} 

bool RUnit(node* now_node)
{
	return (right_node(now_node)->type == NUMBER && is_approx_equal(right_node(now_node), 1));
}

bool is_simply_ADD(node* now_node)
{
	if (!now_node || num_op(now_node) != OP_ADD) return false;

	if (LNull(now_node))
	{
		node_dtor(left_node(now_node));
		node_rewrite(now_node, right_node(now_node));
		return true;
	}
	else if (RNull(now_node))
	{
		node_dtor(right_node(now_node));
		node_rewrite(now_node, left_node(now_node));
		return true;
	}
	return false;
}

bool is_simply_SUB(node* now_node)
{
	if (!now_node || num_op(now_node) != OP_SUB) return false;

	if (RNull(now_node))
	{
		node_dtor(right_node(now_node));
		node_rewrite(now_node, left_node(now_node));
		return true;
	}
	return false;

}

bool is_simply_MUL(node* now_node)
{
	if (!now_node || num_op(now_node) != OP_MUL) return false;

	if (LUnit(now_node))
	{
		node_dtor(left_node(now_node));
		node_rewrite(now_node, right_node(now_node));
		return true;
	}
	else if (RUnit(now_node))
	{
		node_dtor(right_node(now_node));
		node_rewrite(now_node, left_node(now_node));
		return true;
	}
	else if (LNull(now_node) || RNull(now_node))
	{
		node_dtor(right_node(now_node));
		node_dtor(left_node(now_node));
		now_node->type = NUMBER;
		value_num(now_node) = 0;
		left_node(now_node) = nullptr;
		right_node(now_node) = nullptr;
		return true;
	}
	return false;
}

bool is_simply_DIV(node* now_node)
{
	if (!now_node || num_op(now_node) != OP_DIV) return false;

	if (LNull(now_node))
	{
		node_rewrite(now_node, left_node(now_node));
		return true;
	}
	return false;
}

void node_rewrite(node* dest, node* src)
{
	
}