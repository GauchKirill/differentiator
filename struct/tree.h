#ifndef TREE_H
#define TREE_H

typedef struct operation
{
	unsigned num_op;
	unsigned priority_op;
} operation;

#define DEF_OP(name, priority)	\
	OP_ ## name, 
#define DEF_FUNC(name, priority)\
	OP_ ## name,

enum number_of_operation
{
	#include "../settings/operations.h"
};

#undef DEF_OP
#undef DEF_FUNC

#define DEF_OP(name, priority)	\
	PR_ ## name = priority,
#define DEF_FUNC(name, priority)\
	PR_ ## name = preority,

enum priority_of_operation
{
	#include "../settings/operations.h"
	PR_NO_OP	=	0,
};

#undef DEF_OP
#undef DEF_FUNC

enum type_of_value
{
	OPERATION 	= 1 << 0,
	NUMBER 		= 1 << 1,
	VARIABLE	= 1 << 2,
};

typedef struct node
{
	unsigned type;
	union {
		operation	op_info;
		double 		num; 
		char* 		var_name;
	} value;
	struct node* left;
	struct node* right;
} node;

typedef struct
{
	node* root;
} tree;


#define type(Node)			Node->type
#define left_node(Node)  	Node->left 
#define right_node(Node)	Node->right 
#define num_op(Node)		Node->value.op_info.num_op
#define priority_op(Node)	Node->value.op_info.priority_op
#define var_num(Node)		Node->value.num
#define var_name(Node)		Node->value.var_name
#define operand_ptr(Node)	Node->right

tree* 		tree_ctor			(void);
node* 		create_node_var 	(const char* variable_name);
node* 		create_node_num 	(double num);
node* 		create_node_op  	(int num_operation, node* left_ptr, node* right_ptr);
node* 		create_node_func	(int num_func, node* operand_node);
void  		tree_dtor			(tree* tr);
void  		node_dtor			(node* now_node);
unsigned	get_op_func_priority(unsigned num_operation);

#endif