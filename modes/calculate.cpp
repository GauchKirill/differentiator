#include "calculate.h"

bool calculate_node(node* now_node)
{
	if (!now_node)
		return false;
	bool did_subtrees_calculate = false;
	if(now_node->type == OPERATION)
	{
		did_subtrees_calculate = (calculate_node(left_node(now_node)) | calculate_node(right_node(now_node)));
		did_subtrees_calculate |= is_calculate(now_node);
	}
	return did_subtrees_calculate;
}

#define calculate_simple_op(name, op)												\
	case OP_ ## name:																\
		if (right_node(now_node)->type == NUMBER &&									\
			left_node(now_node)->type == NUMBER)									\
		{																			\
			now_node->type = NUMBER;												\
			value_num(now_node) = value_num(left_node(now_node)) op value_num(right_node(now_node));	\
			node_dtor(left_node(now_node));											\
			node_dtor(right_node(now_node));											\
			left_node(now_node)  = nullptr;											\
			right_node(now_node) = nullptr;											\
			return true;															\
		}																			\
		break;

#define calculate_func(name, op)							\
	case OP_ ## name:										\
		if (right_node(now_node)->type == NUMBER)			\
		{													\
			now_node->type = NUMBER;						\
			value_num(now_node) = op(value_num(right_node(now_node)));	\
			node_dtor(right_node(now_node));					\
			right_node(now_node) = nullptr;					\
			return true;									\
		}													\
		break;

bool is_calculate(node* now_node)
{
	switch(num_op(now_node))
	{
		calculate_func(SIN, sin)
		calculate_func(COS, cos)
		calculate_func(LOG, log)
		calculate_func(TG,  tan)
		calculate_simple_op(ADD, +)
		calculate_simple_op(SUB, -)
		calculate_simple_op(MUL, *)
		calculate_simple_op(DIV, /)
		case OP_POW:
			if (right_node(now_node)->type == NUMBER &&
				left_node(now_node)->type == NUMBER)
			{
				now_node->type = NUMBER;
				value_num(now_node) = pow(value_num(left_node(now_node)), value_num(right_node(now_node)));	
				node_dtor(left_node(now_node));												
				node_dtor(right_node(now_node));												
				left_node(now_node)  = nullptr;												
				right_node(now_node) = nullptr;	
				return true;											
			}
			else
				return false;
			break;
		default:
			printf("Cann't calculate operator of number: %d\n", num_op(now_node));
			break;
	}
	return false;
}

#undef calculate_func
#undef calculate_simple_op