#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif
#include <stdio.h>

void* memptr(Addr addr)
{
	return (void*)(Mem + addr);
}

/*Read an instruction*/
Reg get_instruction()
{
	int IF;					// Instructions are 4-bytes long
	IF = *(int*)memptr(PC);
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
			return 2;
		}
	}
	return 0;
}
int Fetch_Instruction()
{
	int IF = get_instruction();
	/*update PC*/
	PC += 4;
	int stat = decode_and_run(IF);
	if(stat == 0)
		return 0;									// no problem
	else if(stat == 1)
		return 1;									// normal exit
	else
	{
		printf("exec wrong in %lld\n",*(long*)PC);	// show the erroneous instruction
		return 2;									// abnormal exit
	}
}

void init(Addr entry)
{
	memset(RegFile, 0, sizeof(RegFile));
	RegFile[2] = Stack_base;				// set sp
	// gp, tp ?
	
	PC = entry;
}

