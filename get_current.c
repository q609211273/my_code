#include <stdio.h>
#include <stdlib.h>

#ifdef CONFIG_X86_64
#define __percpu_seg		gs
#define __percpu_mov_op		movq
#else
#define __percpu_seg		fs
#define __percpu_mov_op		movl
#endif

#define __stringify_1(x...)	#x
#define __stringify(x...)	__stringify_1(x)

#define __percpu_prefix		"%%"__stringify(__percpu_seg)":"
#define __percpu_arg(x)		__percpu_prefix "%" #x

#define percpu_stable_op(op, var)			\
({							\
	typeof(var) pfo_ret__;				\
	switch (sizeof(var)) {				\
	case 1:						\
		asm(op "b "__percpu_arg(P1)",%0"	\
		    : "=q" (pfo_ret__)			\
		    : "p" (&(var)));			\
		break;					\
	case 2:						\
		asm(op "w "__percpu_arg(P1)",%0"	\
		    : "=r" (pfo_ret__)			\
		    : "p" (&(var)));			\
		break;					\
	case 4:						\
		asm(op "l "__percpu_arg(P1)",%0"	\
		    : "=r" (pfo_ret__)			\
		    : "p" (&(var)));			\
		break;					\
	case 8:						\
		asm(op "q "__percpu_arg(P1)",%0"	\
		    : "=r" (pfo_ret__)			\
		    : "p" (&(var)));			\
		break;					\
	default: __bad_percpu_size();			\
	}						\
	pfo_ret__;					\
})

#define this_cpu_read_stable(var)	percpu_stable_op("mov", var)

void* current_task = 0;

static __always_inline struct task_struct *get_current(void)
{
	return this_cpu_read_stable(current_task);
}

#define current get_current()

 
#define percpu_stable_op(op, var)			\
		asm(op "q "__percpu_arg(P1)",%0"	\
		    : "=r" (pfo_ret__)			\
		    : "p" (&(var)));			\


#define STR(s) #s
#define XSTR(s) STR(s)  

int main(){
	printf("%s\n", "start");
	printf(XSTR(percpu_stable_op("mov", current_task)));
	printf("\n%s\n", "end");
	printf(XSTR(percpu_stable_op("mov", current_task)));
}