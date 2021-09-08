#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#include"sqstack.h"

/**
 * main.c - this code use stack to calculate a expression
 * eg: calculate expression "10+(11+3)*2-6/3"
 * the most important thing is the priority of operator
*/

/**
 * compute - push 2numbers, calculate and push the result 
 * @num: number stack, Take 2 numbers to be calculated
 * @op:  the operator of expression( + - * /)
*/
static void compute(sqstack *num, datatype op)
{
	datatype n1,n2,n;
	st_pop(num, &n2);
	st_pop(num, &n1);
	switch(op){
		case '+':
			n = n1 + n2;
			break;
		case '-':
			n = n1 - n2;
			break;
		case '*':
			n = n1 * n2;
			break;
		case '/':
			n = n1 / n2;
			break;
		default:
			exit(1);
	}
	st_push(num, &n);
}

/**
 * deal_bracket - if is not '(', compute them
 * @num: number stack, Take 2 numbers to be calculated
 * @op:  operator stack( + - * /)
*/
static void deal_bracket(sqstack *num, sqstack *op)
{
	datatype old_op;
	st_top(op, &old_op);

	while('(' != old_op){
		st_pop(op, &old_op);
		compute(num, old_op);
		st_top(op, &old_op);
	}
	st_pop(op, &old_op);
}

/**
 * get_pri - get priority of operator
 * @num:     number stack, Take 2 numbers to be calculated
 * @op:      operator stack( + - * /)
 * @op_char: the operator to deal with
*/
int get_pri(datatype op)
{
	switch(op){
		case '(':
			return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	default:
		exit(1);
	}
}


/**
 * deal_op - deal with operator,
 * @num:     number stack, Take 2 numbers to be calculated
 * @op:      operator stack( + - * /)
 * @op_char: the operator to deal with
*/
static void deal_op(sqstack *num, sqstack *op, datatype op_char)
{
	datatype old_op;

	if(st_isempty(op) || '(' == op_char){
		st_push(op, &op_char);
		return;
	}

	st_top(op, &old_op);
	if(get_pri(op_char) > get_pri(old_op)){
		st_push(op, &op_char);
		return;
	}
	while(get_pri(op_char) <= get_pri(old_op)){
		st_pop(op, &old_op);
		compute(num, old_op);
		if(st_isempty(op))
			break;

		st_top(op, &old_op);
	}
	st_push(op, &op_char);
}


int main()
{
	int i = 0;
	char s[] = "10+(11+3)*2-6/3";
	int value = 0;
	int flag;
	sqstack *num, *op;
	datatype old_op;
	
	num = st_create();
	if(NULL == num)
		exit(1);

	op = st_create();
	if(NULL == op)
		exit(1);

	while(s[i]){
		if(isdigit(s[i])){
			value = value*10 + (s[i] - '0');
			flag = 1;
		}
		else{  // op
			if(flag){
				st_push(num, &value);
				flag = 0;
				value = 0;
			}
			if(s[i] == ')'){
				deal_bracket(num, op);
			}
			else{  // ( + - * /
				deal_op(num, op, s[i]);
			}
		}
		++i;
	}
	if(flag){
		st_push(num, &value);
	}
	while(!st_isempty(op)){
		st_pop(op, &old_op);
		compute(num, old_op);
	}

	st_pop(num, &old_op);
	printf("%s = %d\n", s, old_op);
	st_destroy(num);
	st_destroy(op);
	exit(0);
}
