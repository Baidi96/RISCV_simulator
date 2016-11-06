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

// Read an instruction
int get_instruction()
{
	int IF;					// Instructions are 4-bytes long
	read_memory(&IF, sizeof(int), PC);
	/*update PC*/
	PC += 4;
	return IF;
}
extern int decode_and_run(int IF);
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

// Instruction implementations

#define ADD(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]+RegFile[rs2(IF)]
#define ADDW(IF) RegFile[rd(IF)]=(int)RegFile[rs1(IF)]+(int)RegFile[rs2(IF)]
#define SUB(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]-RegFile[rs2(IF)]
#define SUBW(IF) RegFile[rd(IF)]=(int)RegFile[rs1(IF)]-(int)RegFile[rs2(IF)]
#define XOR(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]^RegFile[rs2(IF)]
#define OR(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]|RegFile[rs2(IF)]
#define AND(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]&RegFile[rs2(IF)]
#define SLL(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]<<(RegFile[rs2(IF)]&0x03F)
#define SLLW(IF) RegFile[rd(IF)]=((int)RegFile[rs1(IF)])<<(RegFile[rs2(IF)]&0x01F)
#define SRL(IF) RegFile[rd(IF)]=((unsigned long long)RegFile[rs1(IF)])>>(RegFile[rs2(IF)]&0x03F)
#define SRLW(IF) RegFile[rd(IF)]=(((unsigned int)RegFile[rs1(IF)]))>>(RegFile[rs2(IF)]&0x01F)
#define SRA(IF) RegFile[rd(IF)]=((long long)RegFile[rs1(IF)])>>(RegFile[rs2(IF)]&0x03F)
#define SRAW(IF) RegFile[rd(IF)]=(((int)RegFile[rs1(IF)])>>(RegFile[rs2(IF)]&0x01F))
#define SLT(IF) RegFile[rd(IF)]=((long long)RegFile[rs1(IF)]<(long long)RegFile[rs2(IF)])
#define SLTU(IF) RegFile[rd(IF)]=((unsigned long long)RegFile[rs1(IF)]<(unsigned long long)RegFile[rs2(IF)])
void LB(int IF)
{
	char tmp;
	read_memory(&tmp,1,RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=(long long)tmp;
}
void LBU(int IF)
{
	unsigned char tmp;
	read_memory(&tmp,1,RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=(unsigned long long)tmp;
}
void LH(int IF)
{
	short tmp;
	read_memory(&tmp,2,RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=(long long)tmp;
}
void LHU(int IF)
{
	unsigned short tmp;
	read_memory(&tmp,2,RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=(unsigned long long)tmp;
}
void LW(int IF)
{
	int tmp;
	read_memory(&tmp,sizeof(int),RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=(long long)tmp;
}
void LWU(int IF)
{
	unsigned int tmp;
	read_memory(&tmp,sizeof(int),RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=(unsigned long long)tmp;
}
void LD(int IF)
{
	long long tmp;
	read_memory(&tmp,sizeof(long long),RegFile[rs1(IF)]+((IF>>20)&((1<<12)-1)));
	RegFile[rd(IF)]=tmp;
}
void SB(int IF)
{
	char tmp=RegFile[rd(IF)]&0XFF;
	write_memory(&tmp,1,RegFile[rs1(IF)]+((IF>>7)&((1<<5)-1))+(((IF>>25)&((1<<7)-1))<<5));
}
void SH(int IF)
{
	short tmp=RegFile[rd(IF)]&0XFFFF;
	write_memory(&tmp,2,RegFile[rs1(IF)]+((IF>>7)&((1<<5)-1))+(((IF>>25)&((1<<7)-1))<<5));
}
void SW(int IF)
{
	int tmp=RegFile[rd(IF)]&0XFFFFFFFF;
	write_memory(&tmp,4,RegFile[rs1(IF)]+((IF>>7)&((1<<5)-1))+(((IF>>25)&((1<<7)-1))<<5));
}
void SD(int IF)
{
	write_memory(&RegFile[rs2(IF)],8,RegFile[rs1(IF)]+((IF>>7)&((1<<5)-1))+(((IF>>25)&((1<<7)-1))<<5));
}

// Instruction category parsing

int AUIPC_func(int IF)      //RV32 function
{
    long long imm = ((long long)IF >> 12) << 12;
    RegFile[rd(IF)] = PC + imm;
    return 0;
}
int LUI_func(int IF)        //RV32 function
{
    int U = IF >> 12;
    RegFile[rd(IF)] = ((long long)U) << 12;
    return 0;
}
int ALUI_64_func(int IF)
{
	switch(funct3(IF))
	{
        case 0:{           //ADDIW
            signed int imm = IF >> 20;
            RegFile[rd(IF)] = RegFile[rs1(IF)] + imm;
            break;
        }
        case 1:{           //SLLIW
            if((IF>>25)&1 !=0){
                printf("Illegal instruction exception\n");
                return 1;
            }
            int imm = RegFile[rs1(IF)]<<rs2(IF);
            long long tmp = ~(1<<32-1);
            RegFile[rd(IF)] = (RegFile[rd(IF)]&tmp) + imm;
            break;
        }
		case 5:
		{
			switch(funct7(IF))
			{
                case 0x20:{//SRAIW
                    if((IF>>25)&1 !=0){
                        printf("Illegal instruction exception\n");
                        return 1;
                    }
                    int imm = RegFile[rs1(IF)]>>rs2(IF);
                    long long tmp = ~(1<<32-1);
                    RegFile[rd(IF)] = (RegFile[rd(IF)]&tmp) + imm;
                    break;
                }
                case 0x00:{//SRLIW
                    if((IF>>25)&1 !=0){
                        printf("Illegal instruction exception\n");
                        return 1;
                    }
                    unsigned int imm = RegFile[rs1(IF)]>>rs2(IF);
                    long long tmp = ~(1<<32-1);
                    RegFile[rd(IF)] = (RegFile[rd(IF)]&tmp) + imm;
                    break;
                }
			} 
			break;
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
				case 0x0:ADDW(IF);break;//ADDW
				case 0x20:SUBW(IF);break;//SUBW
				default:
				{
					printf("ALUR_64 error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 1:SLLW(IF);break;//SLLW
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:SRLW(IF);break;//SRLW
				case 0x20:SRAW(IF);break;//SRAW
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
	int offset = ((IF>>7)&1)<<11;
        offset |= ((IF>>8)&0xF)<<1;
        offset |= ((IF>>25)&0x3F)<<5;
        offset |= (IF>>19);				// Signed extend
        offset -= 4;					// If branch, restore current PC
    
	switch(funct3(IF))
	{
        case 0:{        //BEQ
            if(RegFile[rs1(IF)]==RegFile[rs2(IF)])
                PC += offset;
            break;
        }
        case 1:{        //BNE
            if(RegFile[rs1(IF)]!=RegFile[rs2(IF)])
                PC += offset;
            break;
        }
        case 4:{        //BLT
            if(RegFile[rs1(IF)]<RegFile[rs2(IF)])
                PC += offset;
            break;
        }
        case 5:{        //BGE
            if(RegFile[rs1(IF)]>=RegFile[rs2(IF)])
                PC += offset;
            break;
        }
        case 6:{        //BLTU
            if((unsigned)RegFile[rs1(IF)]<(unsigned)RegFile[rs2(IF)])
                PC += offset;
            break;
        }
        case 7:{        //BGEU
            if((unsigned)RegFile[rs1(IF)]>=(unsigned)RegFile[rs2(IF)])
                PC += offset;
            break;
        }
	default:{
		printf("Branch_func error!No such instruction\n");
		return 1;
	}
	}
	return 0;
}
int JAL_func(int IF)
{
    int offset = ((IF>>12)&(1<<8-1))<<12;
    offset += ((IF>>20)&1)<<11;
    offset += ((IF>>21)&(1<<10-1))<<1;
    offset += ((IF>>31)&1)<<20;
    RegFile[rd(IF)] = PC + 4;
    PC +=offset;
    return 0;
}
int JALR_func(int IF)
{
    int offset = ((IF>>20)&(1<<11-1));
    long long toAddress = (RegFile[rs1(IF)] + offset);
    if(toAddress&1)
        toAddress -=1;
    RegFile[rd(IF)] = PC + 4;
    PC = toAddress;
    return 0;
}
int Load_func(int IF)
{
	switch(funct3(IF))
	{
		case 0:LB(IF);break;//LB
		case 1:LH(IF);break;//LH
		case 2:LW(IF);break;//LW
		case 4:LBU(IF);break;//LBU
		case 5:LHU(IF);break;//LHU
		case 6:LWU(IF);break;//LWU
		case 3:LD(IF);break;//LD
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
		case 0:SB(IF);break;//SB
		case 1:SH(IF);break;//SH
		case 2:SW(IF);break;//SW
		case 3:SD(IF);break;//SD
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
