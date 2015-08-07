#ifndef EXCEPT_H_
#define EXCEPT_H_

#include <setjmp.h>

#define T Except_T
/***********************Types*************************************/
typedef struct T{
	const char *reason;
}T;

typedef struct Except_frame Except_frame;
struct Except_frame{
	Except_frame *prev;
	jmp_buf env;
	T *exception;
	const char *file;
	int line;
};

Except_frame *Except_stack;
enum {
	Except_entered = 0,
	Except_raised,
	Except_handled,
	Except_finsh
};

/*************************MACRO************************************/
#define RAISE(e) Except_raise((&e), __FILE__, __LINE__)

#define TRY do{                     	  \
		Except_frame frame;         	  \
		int Except_flag; 				  \
		frame.prev = Except_stack;        \
		Except_stack = &frame;      \
		Except_flag = setjmp(frame.env);  \
		if(Except_flag == Except_entered){

#define EXCEPT(e)                                 \
		if(Except_flag == Except_entered)         \
			Except_stack = Except_stack->prev;    \
		}else if(frame.exception == &(e)){        \
			Except_flag = Except_handled;

#define ELSE                                         \
		if(Except_flag == Except_entered)            \
			Except_stack = Except_stack->prev;       \
		}else {                                      \
			Except_flag = Except_handled;

#define FINALY                                       \
			if(Except_flag == Except_entered)        \
				Except_stack = Except_stack->prev;   \
		}{                                           \
			if(Except_flag == Except_entered)        \
				Except_flag = Except_finsh;  


#define END_TRY                                        \
			if(Except_flag == Except_entered)          \
				Except_stack = Except_stack->prev;     \
		}                                            \
			if(Except_flag == Except_raised)           \
			  Except_raise(frame.exception, frame.file, frame.line); \
		}while(0);



/*************************FUNC*************************************/
extern void Except_raise(T *except, const char *file, int line);

#undef T
#endif