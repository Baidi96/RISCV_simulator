#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif


#include "func.h"
#include <stdio.h>

int decode_and_run(Reg IF)
{
	int op = opcode(IF);
	switch(op)
	{
		case(Load):Load_func(IF);break;
		case(Store):Store_func(IF);break;
		case(ALUR): break;
		case(ALUI): break;
		case(LUI):LUI_func(IF);break;
		case(AUIPC):AUIPC_func(IF);break;
		case(Branch):Branch_func(IF);break;
		case(JAL):JAL_func(IF);break;
 		case(JALR):JALR_func(IF);break;
		case(Syscall): Syscall_func();break;
		case(ALUI_64): ALUI_64_func(IF); break;
		case(ALUR_64): ALUR_64_func(IF);break;
		default:
		{
			printf("error! No such opcode:%d\n",op);
			return 2;
		}
	}
	return 0;
}


