#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif
#include <stdio.h>
/*Read an instruction*/
Reg get_instruction()
{
	Reg IF;
	IF = *(Reg*)PC;
	return IF;
}
int decode_and_run(Reg IF)
{
	int op = opcode(IF);
	switch(op)
	{
		case(Load):break;
		case(Store): break;
		case(ALUR): break;
		case(ALUI): break;
		case(LUI): break;
		case(AUIPC): break;
		case(Branch ): break;
		case(JAL): break;
 		case(JALR): break;
		case(Syscall): break;
		case(ALUI_64): break;
		case(ALUR_64): break;
		default:
		{
			printf("error! No such opcode:%d\n",op);
			return 1;
		}
	}
	return 0;
}
int Fetch_Instruction()
{
	Reg IF = get_instruction();
	/*update PC*/
	PC += 4;
	if(decode_and_run(IF) == 0)
		return 0;//no problem
	else
	{
		printf("exec wrong in %lld\n",*(long*)PC);	// show the erroneous instruction
		return 1;
	}
}

void* memptr(Addr addr)
{
	return (void*)(Mem + addr);
}

void init(Addr entry)
{
	memset(RegFile, 0, sizeof(RegFile));
	RegFile[2] = Stack_base;		// set sp
	// gp, tp ?
	
	PC = memptr(entry);
}

