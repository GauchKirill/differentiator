#include "./calculate.h"

bool calculate_node(node* now_node)
{
	if (!now_node)
		return false;

	bool did_subtrees_calculate = false;
	if (type(now_node) == OPERATION)
	{
		did_subtrees_calculate = (calculate_node(left_node(now_node)) | calculate_node(right_node(now_node)));
		did_subtrees_calculate |= is_calculate(now_node);
	}
	return did_subtrees_calculate;
}

#define DEF_OP(name, priority, operator)																		\
	case OP_ ## name:																							\
		if (type(right_node(now_node)) == NUMBER &&																\
			type(left_node(now_node))  == NUMBER)																\
		{																										\
			type(now_node) = NUMBER;																			\
			if (num_op(now_node) == OP_POW)																		\
				value_num(now_node) = pow(value_num(left_node(now_node)), value_num(right_node(now_node)));		\
			else																								\
				value_num(now_node) = value_num(left_node(now_node)) operator value_num(right_node(now_node));	\
		}																										\
		break;

#define DEF_FUNC(name, priority, operator)										\
	case OP_ ## name:															\
		if (type(operand_ptr(now_node)) == NUMBER)								\
		{																		\
			type(now_node) = NUMBER;											\
			value_num(now_node) = operator(value_num(operand_ptr(now_node)));	\
		}																		\
		break;

bool is_calculate(node* now_node)
{
	if (!now_node || type(now_node) != OPERATION) return false;

	switch(num_op(now_node))
	{
		#include "../settings/operations.h"
	}

	if (type(now_node) == NUMBER)
	{
		node_dtor(left_node(now_node));
		node_dtor(right_node(now_node));
		left_node(now_node) = nullptr;
		right_node(now_node) = nullptr;
		return true;
	}
	return false;
}

#undef DEF_OP
#undef DEF_FUNC