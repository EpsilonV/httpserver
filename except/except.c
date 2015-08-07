#include <stdlib.h>
#include <stdio.h>
#include "except.h"
#include "assert.h"

#define T Except_T
typedef struct T T;

void Except_raise(T *except, const char *file, int line)
{
	assert(except);
	Except_frame *p = Except_stack;
	if(p)
	{
		p->file = file;
		p->line = line;
		p->exception = except;
		Except_stack = Except_stack->prev;
		longjmp(p->env, Except_raised);

	}
	else
	{
		fprintf(stderr, "Uncaught Exception , The reason is%s\n", except->reason);
		exit(1);
	}
}