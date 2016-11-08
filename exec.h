#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif

#include "func.h"
#include <stdio.h>

int decode_and_run(int IF)
{
	int op = opcode(IF);
	switch(op)
	{
		case(Load):
			return Load_func(IF);
		case(Store):
			return Store_func(IF);
		case(ALUR):
			return ALUR_func(IF);
		case(ALUI):
			return ALUI_func(IF);
		case(LUI):
			return LUI_func(IF);
		case(AUIPC):
			return AUIPC_func(IF);
		case(Branch):
			return Branch_func(IF);
		case(JAL):
			return JAL_func(IF);
 		case(JALR):
 			return JALR_func(IF);
		case(SYSTEM):
			return SYSTEM_func(IF);
		case(ALUI_64):
			return ALUI_64_func(IF);
		case(ALUR_64):
			return ALUR_64_func(IF);
		case(FLoad):
			return FLoad_func(IF);
		case(FStore):
			return FStore_func(IF);
		case(OP_FP):
			return OP_FP_func(IF);
		case(FMADD):
			return FMADD_func(IF);
		case(FMSUB):
			return FMSUB_func(IF);
		case(FNMADD):
			return FNMADD_func(IF);
		case(FNMSUB):
			return FNMSUB_func(IF);
		default:
		{
			printf("error! No such opcode:%d\n",op);
			return 2;
		}
	}
	return 0;
}


