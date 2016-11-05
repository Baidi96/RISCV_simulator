#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif
#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif
#include <stdio.h>
void* memptr(Addr addr)
{
	return (void*)(Mem + addr);
}

void read_memory(void* buff, int size, Addr mem_address)
{
	if(size == 1)
		*(char*)buff = *(char*)memptr(mem_address);
	else if(size == 2)
		*(short*)buff = *(short*)memptr(mem_address);
	else if(size == 4)
		*(int*)buff = *(int*)memptr(mem_address);
	else if(size == 8)
		*(long long*)buff = *(long long*)memptr(mem_address);
}
void write_memory(void* buff, int size, Addr mem_address)
{
	if(size == 1)
		*(char*)memptr(mem_address) = *(char*)buff;
	else if(size == 2)
		*(short*)memptr(mem_address) = *(short*)buff;
	else if(size == 4)
		*(int*)memptr(mem_address) = *(int*)buff;
	else if(size == 8)
		*(long long*)memptr(mem_address) = *(long long*)buff;
}

/*Read an instruction*/
int get_instruction()
{
	int IF;					// Instructions are 4-bytes long
	IF = *(int*)memptr(PC);
	/*update PC*/
	PC += 4;
	return IF;
}
int AUIPC_func(int IF)
{

}
int LUI_func(int IF)
{

}
int Fetch_Instruction()
{
	int IF = get_instruction();
	int stat = decode_and_run(IF);
	if(stat == 0)
		return 0;											// no problem
	else if(stat == 1)
		return 1;											// normal exit
	else
	{
		printf("exec wrong in %d\n",*(int*)memptr(PC));		// show the erroneous instruction
		return 2;											// abnormal exit
	}
}
int ALUI_64_func(int IF)
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
int ALUR_func(int IF)
{
	switch(funct3(IF))
	{
		case 0:
		{
			switch(funct7(IF))
			{
				case 0:ADD(IF);break;//ADD
				case 0x20:SUB(IF);break;//SUB
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 4:XOR(IF);break;//XOR
		case 6:OR(IF);break;//OR
		case 7:AND(IF);break;//AND
		case 1:SLL(IF);break;//SLL
		case 2:SLT(IF);break;//SLT
		case 3:SLTU(IF);break;//SLTU
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:SRL(IF);break;//SRL
				case 0x20:SRA(IF);break;//SRA
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
int ALUI_func(int IF)
{
	switch(funct3(IF))
	{
		case 0:			//ADDI
		{
			int imm = IF >> 20;
			RegFile[rd(IF)] = RegFile[rs1(IF)] + imm;
			break;
		}
        case 2:{        //SLTI
            signed int imm = IF >> 20;
            long long imm64 = imm;
            if(RegFile[rs1(IF)] < imm64)
                RegFile[rd(IF)] = 1;
            else
                RegFile[rd(IF)] = 0;
            break;
        }
        case 3:{        //SLTIU
            unsigned int imm = IF >> 20;
            long long imm64 = imm;
            if((unsigned)RegFile[rs1(IF)] < imm64)
                RegFile[rd(IF)] = 1;
            else
                RegFile[rd(IF)] = 0;
            break;
        }
        case 4:{        //XORI
            signed int imm = IF >> 20;
            long long imm64 = imm;
            RegFile[rd(IF)] = imm64 ^ RegFile[rs1(IF)];
            break;
        }
        case 6:{        //ORI
            signed int imm = IF >> 20;
            long long imm64 = imm;
            RegFile[rd(IF)] = imm64 | RegFile[rs1(IF)];
            break;
        }
        case 7:{        //ANDI
            signed int imm = IF >> 20;
            long long imm64 = imm;
            RegFile[rd(IF)] = imm64 & RegFile[rs1(IF)];
            break;
        }
        case 1:{        //SLLI
            RegFile[rd(IF)] = RegFile[rs1(IF)]<<rs2(IF);
            break;
        }
		case 5:
		{
			switch(funct7(IF))
			{
                case 0:{   //SRLI
                    RegFile[rd(IF)] = (unsigned long long)RegFile[rs1(IF)]>>rs2(IF);
                    break;
                }
                case 0x20:{//SRAI
                    RegFile[rd(IF)] = RegFile[rs1(IF)]>>rs2(IF);
                    break;
                }
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
int ALUR_64_func(int IF)
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
int Branch_func(int IF)
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
int JAL_func(int IF)
{
}
int JALR_func(int IF)
{
}
int Load_func(int IF)
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
int Store_func(int IF)
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
