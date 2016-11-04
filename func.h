#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif
#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif
#include <stdio.h>
inline void* memptr(Addr addr)
{
	return (void*)(Mem + addr);
}
/*Read an instruction*/
Reg get_instruction()
{
	int IF;					// Instructions are 4-bytes long
	IF = *(int*)memptr(PC);
	/*update PC*/
	PC += 4;
	return IF;
}
int AUIPC_func(Reg IF)
{

}
int LUI_func(Reg IF)
{

}
int Fetch_Instruction()
{
	int IF = get_instruction();
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
int ALUI_64_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:break;//ADDIW
		case 1:break;//SSLIW
		case 5:
		{
			switch(funct7(IF))
			{
				case 0x20:break;//SRAIW
				case 0x00:break;//SRLIW
			} 
			break;//ADDIW
		}
		default:
			{
				printf("ALUI_64 error!No such instruction\n");
				return 1;
			}
	}
	return 0;
}
int ALUR_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:
		{
			switch(funct7(IF))
			{
				case 0:break;//ADD
				case 0x20:break;//SUB
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 4:break;//XOR
		case 6:break;//OR
		case 7:break;//AND
		case 1:break;//SLL
		case 2:break;//SLT
		case 3:break;//SLTU
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:break;//SRL
				case 0x20:break;//SRA
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		default:
		{
			printf("ALUR error!No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int ALUI_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:break;//ADDI
		case 2:break;//SLTI
		case 3:break;//SLTIU
		case 4:break;//XORI
		case 6:break;//ORI
		case 7:break;//ANDI
		case 1:break;//SLLI
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:break;//SRLI
				case 0x20:break;//SRAI
				default:
				{
					printf("ALUI error!No such instruction\n");
					return 1;
				}
			}
		}
		default:
		{
			printf("ALUI error!No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int ALUR_64_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:
		{
			switch(funct7(IF))
			{
				case 0x0:break;//ADDW
				case 0x20:break;//SUBW
				default:
				{
					printf("ALUR_64 error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 1:break;//SLLW
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:break;//SRLW
				case 0x20:break;//SRAW
				default:
				{
					printf("ALUR_64 error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		default:
		{
			printf("ALUR_64 error!No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int Branch_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:break;//BEQ
		case 1:break;//BNE
		case 4:break;//BLT
		case 5:break;//BGE
		case 6:break;//BLTU
		case 7:break;//BGEU
		default:
		{
			printf("Branch_func error!No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int JAL_func(Reg IF)
{
}
int JALR_func(Reg IF)
{
}
int Load_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:break;//LB
		case 1:break;//LH
		case 2:break;//LW
		case 4:break;//LBU
		case 5:break;//LHU
		case 6:break;//LWU
		case 3:break;//LD
		default:
		{
			printf("Load_func error!No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int Store_func(Reg IF)
{
	switch(funct3(IF))
	{
		case 0:break;//SB
		case 1:break;//SH
		case 2:break;//SW
		case 3:break;//SD
		default:
		{
			printf("Store_func error!No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int Syscall_func()
{
	return 0;
}
void init(Addr entry)
{
	memset(RegFile, 0, sizeof(RegFile));
	RegFile[2] = Stack_base;				// set sp
	// gp, tp ?
	
	PC = entry;
}