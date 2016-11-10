#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif
#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif

#include "stdio.h"
#include "math.h"

//syscall
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

struct stat_rv {
 mode_t     smode;      //文件对应的模式，文件，目录等
 ino_t      ino;        //inode节点号
 dev_t      dev;        //设备号码
 dev_t      rdev;       //特殊设备号码
 nlink_t    nlink;      //文件的连接数
 uid_t      uid;        //文件所有者
 gid_t      gid;        //文件所有者对应的组
 off_t      size;       //普通文件，对应的文件字节数
 time_t     atime;      //文件最后被访问的时间
 time_t     mtime;      //文件内容最后被修改的时间
 time_t     ctime_;     //文件状态改变时间
 };

void* memptr(Addr addr)
{
	//if(Mem_size)
	return (void*)(Mem + addr);
}

void read_memory(void* buff, int size, Addr mem_address)
{
	if(mem_address >= Mem_size)
	{
		printf("Read out of size: 0x%llx\n",mem_address);
		//*(long long*)buff = 0;
		return;
	}
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
	if(mem_address >= Mem_size)
	{
		printf("Write out of size: 0x%llx\n",mem_address);
		//*(long long*)buff = 0;
		return;
	}
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
		printf("exec wrong in %d\n",*(int*)memptr(PC-4));	// show the erroneous instruction
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
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(char),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=(long long)tmp;
}
void LBU(int IF)
{
	unsigned char tmp;
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(char),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=(unsigned long long)tmp;
}
void LH(int IF)
{
	short tmp;
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(short),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=(long long)tmp;
}
void LHU(int IF)
{
	unsigned short tmp;
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(short),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=(unsigned long long)tmp;
}
void LW(int IF)
{
	int tmp;
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(int),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=(long long)tmp;
}
void LWU(int IF)
{
	unsigned int tmp;
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(int),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=(unsigned long long)tmp;
}
void LD(int IF)
{
	long long tmp;
	int imm = (IF >> 20);
	read_memory(&tmp,sizeof(long long),RegFile[rs1(IF)] + imm);
	RegFile[rd(IF)]=tmp;
}
void SB(int IF)
{
	char tmp=RegFile[rs2(IF)]&0XFF;
	int imm = ((IF>>7) & 0x1F) | ((IF>>25) << 5);
	write_memory(&tmp,sizeof(char),RegFile[rs1(IF)] + imm);
}
void SH(int IF)
{
	short tmp=RegFile[rs2(IF)]&0XFFFF;
	int imm = ((IF>>7) & 0x1F) | ((IF>>25) << 5);
	write_memory(&tmp,sizeof(short),RegFile[rs1(IF)] + imm);
}
void SW(int IF)
{
	int tmp=RegFile[rs2(IF)]&0XFFFFFFFF;
	int imm = ((IF>>7) & 0x1F) | ((IF>>25) << 5);
	write_memory(&tmp,sizeof(int),RegFile[rs1(IF)] + imm);
}
void SD(int IF)
{
	long long tmp = RegFile[rs2(IF)];
	int imm = ((IF>>7) & 0x1F) | ((IF>>25) << 5);
	write_memory(&tmp,sizeof(long long),RegFile[rs1(IF)] + imm);
}

void MUL(int IF)
{
    RegFile[rd(IF)] = RegFile[rs1(IF)] * RegFile[rs2(IF)];
}
void MULH(int IF)
{
    unsigned long long l1 = RegFile[rs1(IF)];
    unsigned long long l2 = RegFile[rs2(IF)];
    unsigned long long h1 = (long long)l1 >> 63;				// sign extend
    unsigned long long h2 = (long long)l2 >> 63;				// sign extend
    unsigned long long ll1 = l1 & 0xFFFFFFFF;
    unsigned long long lh1 = l1 >> 32;
    unsigned long long ll2 = l2 & 0xFFFFFFFF;
    unsigned long long lh2 = l2 >> 32;
    unsigned long long m64 = l1 * h2 + l2 * h1 + lh1 * lh2;		// (64b) * 2^64
    unsigned long long m32_1 = (ll1 * ll2) >> 32;				// (64b) * 2^32
    unsigned long long m32_2 = lh1 * ll2;
    unsigned long long m32_3 = ll1 * lh2;
    unsigned long long m32_l = (m32_1 & 0xFFFFFFFF) + (m32_2 & 0xFFFFFFFF) + (m32_3 & 0xFFFFFFFF);	// (32b) * 2^0
    unsigned long long m32_h = (m32_1 >> 32) + (m32_2 >> 32) + (m32_3 >> 32);						// (32b) * 2^32
    unsigned long long m32_co = m32_h + (m32_l >> 32);			// (b) * 2^64 (= m32 >> 32)
    m64 += m32_co;
    RegFile[rd(IF)] = m64;
}
void MULHSU(int IF)
{
    unsigned long long l1 = RegFile[rs1(IF)];
    unsigned long long l2 = RegFile[rs2(IF)];
    unsigned long long h1 = (long long)l1 >> 63;				// sign extend
    unsigned long long h2 = 0;									// zero extend
    unsigned long long ll1 = l1 & 0xFFFFFFFF;
    unsigned long long lh1 = l1 >> 32;
    unsigned long long ll2 = l2 & 0xFFFFFFFF;
    unsigned long long lh2 = l2 >> 32;
    unsigned long long m64 = l1 * h2 + l2 * h1 + lh1 * lh2;		// (64b) * 2^64
    unsigned long long m32_1 = (ll1 * ll2) >> 32;				// (64b) * 2^32
    unsigned long long m32_2 = lh1 * ll2;
    unsigned long long m32_3 = ll1 * lh2;
    unsigned long long m32_l = (m32_1 & 0xFFFFFFFF) + (m32_2 & 0xFFFFFFFF) + (m32_3 & 0xFFFFFFFF);	// (32b) * 2^0
    unsigned long long m32_h = (m32_1 >> 32) + (m32_2 >> 32) + (m32_3 >> 32);						// (32b) * 2^32
    unsigned long long m32_co = m32_h + (m32_l >> 32);			// (b) * 2^64 (= m32 >> 32)
    m64 += m32_co;
    RegFile[rd(IF)] = m64;
}
void MULHU(int IF)
{
    unsigned long long l1 = RegFile[rs1(IF)];
    unsigned long long l2 = RegFile[rs2(IF)];
    unsigned long long h1 = 0;									// zero extend
    unsigned long long h2 = 0;									// zero extend
    unsigned long long ll1 = l1 & 0xFFFFFFFF;
    unsigned long long lh1 = l1 >> 32;
    unsigned long long ll2 = l2 & 0xFFFFFFFF;
    unsigned long long lh2 = l2 >> 32;
    unsigned long long m64 = l1 * h2 + l2 * h1 + lh1 * lh2;		// (64b) * 2^64
    unsigned long long m32_1 = (ll1 * ll2) >> 32;				// (64b) * 2^32
    unsigned long long m32_2 = lh1 * ll2;
    unsigned long long m32_3 = ll1 * lh2;
    unsigned long long m32_l = (m32_1 & 0xFFFFFFFF) + (m32_2 & 0xFFFFFFFF) + (m32_3 & 0xFFFFFFFF);	// (32b) * 2^0
    unsigned long long m32_h = (m32_1 >> 32) + (m32_2 >> 32) + (m32_3 >> 32);						// (32b) * 2^32
    unsigned long long m32_co = m32_h + (m32_l >> 32);			// (b) * 2^64 (= m32 >> 32)
    m64 += m32_co;
    RegFile[rd(IF)] = m64;
}
void DIV(int IF)
{
	if(RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = -1;		// Divided by 0 semantic
	else
    	RegFile[rd(IF)] = RegFile[rs1(IF)] / RegFile[rs2(IF)];
}
void DIVU(int IF)
{
	if(RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = -1;
	else
    	RegFile[rd(IF)] = (unsigned long long)RegFile[rs1(IF)] / (unsigned long long)RegFile[rs2(IF)];
}
void REM(int IF)
{
	if(RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = RegFile[rs1(IF)];
	else
    	RegFile[rd(IF)] = RegFile[rs1(IF)] % RegFile[rs2(IF)];
}
void REMU(int IF)
{
	if(RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = RegFile[rs1(IF)];
	else
    	RegFile[rd(IF)] = (unsigned long long)RegFile[rs1(IF)] % (unsigned long long)RegFile[rs2(IF)];
}

void MULW(int IF)
{
    RegFile[rd(IF)] = (int)RegFile[rs1(IF)] * (int)RegFile[rs2(IF)];
}
void DIVW(int IF)
{
	if((int)RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = -1;
	else
    	RegFile[rd(IF)] = (int)RegFile[rs1(IF)] / (int)RegFile[rs2(IF)];
}
void DIVUW(int IF)
{
	if((int)RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = -1;
	else
    	RegFile[rd(IF)] = (unsigned long long)((unsigned int)RegFile[rs1(IF)] / (unsigned int)RegFile[rs2(IF)]);
}
void REMW(int IF)
{
	if((int)RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = (int)RegFile[rs1(IF)];
	else
    	RegFile[rd(IF)] = (int)RegFile[rs1(IF)] % (int)RegFile[rs2(IF)];
}
void REMUW(int IF)
{
	if((int)RegFile[rs2(IF)] == 0)
		RegFile[rd(IF)] = (unsigned long long)((unsigned int)RegFile[rs1(IF)]);
	else
    	RegFile[rd(IF)] = (unsigned long long)((unsigned int)RegFile[rs1(IF)] % (unsigned int)RegFile[rs2(IF)]);
}
// Instruction category parsing

int AUIPC_func(int IF)      //RV32 function
{
    long long imm = ((long long)IF >> 12) << 12;
    RegFile[rd(IF)] = PC - 4 + imm;
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
            RegFile[rd(IF)] = (long long)((int)RegFile[rs1(IF)] + imm);
            break;
        }
        case 1:{           //SLLIW
            if((IF>>25)&1 !=0){
                printf("Illegal instruction exception\n");
                return 1;
            }
            int shamt = (IF>>20)&0x1F;
            RegFile[rd(IF)] = (long long)((int)RegFile[rs1(IF)]<<shamt);
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
                    int shamt = (IF>>20)&0x1F;
                    RegFile[rd(IF)] = (long long)((int)RegFile[rs1(IF)]>>shamt);
                    break;
                }
                case 0x00:{//SRLIW
                    if((IF>>25)&1 !=0){
                        printf("Illegal instruction exception\n");
                        return 1;
                    }
                    int shamt = (IF>>20)&0x1F;
                    RegFile[rd(IF)] = (unsigned long long)((unsigned int)RegFile[rs1(IF)]>>shamt);
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
				case 1:MUL(IF);break;//MUL
				case 0x20:SUB(IF);break;//SUB
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 4:
		{
			switch(funct7(IF))
			{
				case 0:XOR(IF);break;//XOR
				case 1:DIV(IF);break;//DIV
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}	
			}
			break;
		}
		case 6:
		{
			switch(funct7(IF))
			{
				case 0:OR(IF);break;//OR
				case 1:REM(IF);break;//REM
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 7:
		{
			switch(funct7(IF))
			{
				case 0:AND(IF);break;//AND
				case 1:REMU(IF);break;//REMU
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 1:
		{
		      switch(funct7(IF))
		      {
			      case 0:SLL(IF);break;//SLL
			      case 1:MULH(IF);break;//MULH
			      default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
		      }
		      break;
		}
		case 2:
		{
			switch(funct7(IF))
			{
				case 0:SLT(IF);break;//SLT
				case 1:MULHSU(IF);break;//MULSHU
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}	
			}
			break;
		}
		case 3:
		{
			switch(funct7(IF))
			{
				case 0:SLTU(IF);break;//SLTU
				case 1:MULHU(IF);break;//MULHU
				default:
				{
					printf("ALUR error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:SRL(IF);break;//SRL
				case 1:DIVU(IF);break;//DIVU
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
            long long imm = IF >> 20;
            if(RegFile[rs1(IF)] < imm)
                RegFile[rd(IF)] = 1;
            else
                RegFile[rd(IF)] = 0;
            break;
        }
        case 3:{        //SLTIU
            long long imm = IF >> 20;
            if((unsigned long long)RegFile[rs1(IF)] < (unsigned long long)imm)
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
            RegFile[rd(IF)] = RegFile[rs1(IF)]<<((IF>>20)&0x3F);
            break;
        }
		case 5:
		{
			switch(funct7(IF)>>1)
			{
                case 0:{   //SRLI
                    RegFile[rd(IF)] = (unsigned long long)RegFile[rs1(IF)]>>((IF>>20)&0x3F);
                    break;
                }
                case 0x10:{//SRAI
                    RegFile[rd(IF)] = RegFile[rs1(IF)]>>((IF>>20)&0x3F);
                    break;
                }
				default:
				{
					printf("ALUI error!No such instruction\n");
					return 1;
				}
			}
			break;
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
				case 0x1:MULW(IF);break;//MULW
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
		case 4:DIVW(IF);break;//DIVW
		case 5:
		{
			switch(funct7(IF))
			{
				case 0:SRLW(IF);break;//SRLW
				case 1:DIVUW(IF);break;//DIVUW
				case 0x20:SRAW(IF);break;//SRAW
				default:
				{
					printf("ALUR_64 error!No such instruction\n");
					return 1;
				}
			}
			break;
		}
		case 6:REMW(IF);break;//REMW
		case 7:REMUW(IF);break;//REMUW
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
        offset |= (IF>>19)&0xFFFFF000;		// Signed extend
        offset -= 4;						// If branch, restore current PC
    
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
    int offset = ((IF>>12) & 0xFF) << 12;
    offset |= ((IF>>20) & 0x1) << 11;
    offset |= ((IF>>21) & 0x3FF) << 1;
    offset |= (IF>>11) & 0xFFF00000;
    if(rd(IF) != 0)
        RegFile[rd(IF)] = PC;		// Here PC = cur_PC + 4
    PC = PC - 4 + offset;
    return 0;
}
int JALR_func(int IF)
{
    int offset = (IF >> 20);
    Addr toAddress = (RegFile[rs1(IF)] + offset) & (~1);
    if(rd(IF) != 0)	
        RegFile[rd(IF)] = PC;
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
			printf("Load_func error! No such instruction\n");
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
			printf("Store_func error! No such instruction\n");
			return 1;
		}
	}
	return 0;
}
int FLoad_func(int IF)
{
	if(rm(IF) == 2)			// FLW
	{
		int imm = (IF >> 20);
		float tmp;
		read_memory(&tmp, sizeof(float), RegFile[rs1(IF)] + imm);
		FRegFile[rd(IF)] = tmp;
		return 0;
	}
	else if(rm(IF) == 3)	// FLD
	{
		int imm = (IF >> 20);
		double tmp;
		read_memory(&tmp, sizeof(double), RegFile[rs1(IF)] + imm);
		DRegFile[rd(IF)] = tmp;
		return 0;
	}
	return 1;
}
int FStore_func(int IF)
{
	if(rm(IF) == 2)			// FSW
	{
		int imm = ((IF >> 7) & 0x1F) | ((IF >> 25) << 5);
		float tmp = FRegFile[rs2(IF)];
		write_memory(&tmp, sizeof(float), RegFile[rs1(IF)] + imm);
		return 0;
	}
	else if(rm(IF) == 3)	// FSD
	{
		int imm = ((IF >> 7) & 0x1F) | ((IF >> 25) << 5);
		double tmp = DRegFile[rs2(IF)];
		write_memory(&tmp, sizeof(double), RegFile[rs1(IF)] + imm);
		return 0;
	}
	return 1;
}
int OP_FP_func(int IF)		// Did not check the rounding mode!
{
	if(fmt(IF) == 0)		// (Mainly) F instructions
	{
		if(funct5(IF) == 0)			// FADD.S
		{
			FRegFile[rd(IF)] = FRegFile[rs1(IF)] + FRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x1)	// FSUB.S
		{
			FRegFile[rd(IF)] = FRegFile[rs1(IF)] - FRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x2)	// FMUL.S
		{
			FRegFile[rd(IF)] = FRegFile[rs1(IF)] * FRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x3)	// FDIV.S
		{
			FRegFile[rd(IF)] = FRegFile[rs1(IF)] / FRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0xB)	// FSQRT.S
		{
			FRegFile[rd(IF)] = sqrt(FRegFile[rs1(IF)]);
			return 0;
		}
		else if(funct5(IF) == 0x5)	// FMIN/MAX.S (NaN not checked!)
		{
			if(rm(IF) == 0)			// FMIN.S
			{
				FRegFile[rd(IF)] = (FRegFile[rs1(IF)] < FRegFile[rs2(IF)]) ? FRegFile[rs1(IF)] : FRegFile[rs2(IF)];
				return 0;
			}
			else if(rm(IF) == 1)	// FMAX.S
			{
				FRegFile[rd(IF)] = (FRegFile[rs1(IF)] > FRegFile[rs2(IF)]) ? FRegFile[rs1(IF)] : FRegFile[rs2(IF)];
				return 0;
			}
			else
			{
				// printf("FMIN/MAX.S error! No such instruction\n");
				return 1;
			}
		}
		else if(funct5(IF) == 0x18)	// FCVT.W[U].S
		{
			if(rs2(IF) == 0)		// FCVT.W.S
			{
				RegFile[rd(IF)] = (int)FRegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 1)	// FCVT.WU.S
			{
				RegFile[rd(IF)] = (unsigned int)FRegFile[rs1(IF)];
				return 0;
			}
			else
			{
				// printf("FCVT.W[U].S error! No such instruction\n");
				return 1;
			}
		}
		else if(funct5(IF) == 0x1A)	// FCVT.S.W[U]
		{
			if(rs2(IF) == 0)		// FCVT.S.W
			{
				FRegFile[rd(IF)] = (float)(int)RegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 1)	// FCVT.S.WU
			{
				FRegFile[rd(IF)] = (float)(unsigned int)RegFile[rs1(IF)];
				return 0;
			}
			else
			{
				// printf("FCVT.S.W[U] error! No such instruction\n");
				return 1;
			}
		}
		else if(funct5(IF) == 0x8)	// FCVT.S.D
		{
			FRegFile[rd(IF)] = DRegFile[rs1(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x4)	// FSGNJ.S
		{
			int src1 = *(int*)&FRegFile[rs1(IF)];
			int src2 = *(int*)&FRegFile[rs2(IF)];
			int sgn1 = ((src1 >> 31) << 31);
			int sgn2 = ((src2 >> 31) << 31);
			int sgn;
			int val = src1 & 0x7FFFFFFF;
			if(rm(IF) == 0)			// FSGNJ.S
				sgn = sgn2;
			else if(rm(IF) == 1)	// FSGNJN.S
				sgn = sgn2 ^ (1 << 31);
			else if(rm(IF) == 2)	// FSGNJX.S
				sgn = sgn1 ^ sgn2;
			else
			{
				// printf("FSGNJ.S error! No such instruction\n");
				return 1;
			}
		
			int res = sgn | val;
			FRegFile[rd(IF)] = *(float*)&res;
			return 0;
		}
		else if(funct5(IF) == 0x1C)	// FMV.X.S or FCLASS.S
		{
			if(rm(IF) == 0)			// FMV.X.S
			{
				RegFile[rd(IF)] = *(int*)&FRegFile[rs1(IF)];
				return 0;
			}
			else if(rm(IF) == 1)	// FCLASS.S
			{
				int type;
				int tmp = *(int*)&FRegFile[rs1(IF)];
				int S = (tmp >> 31) & 0x1;
				int E = (tmp >> 23) & 0xFF;
				int F = tmp & 0x7FFFFF;
				if(E == 0xFF && F != 0)	// nan
				{
					if(F >> 22)			// quiet nan
						type = 9;
					else				// signaling nan
						type = 8;
				}
				else if(S)				// negative
				{
					if(E == 0xFF)		// -inf
						type = 0;
					else if(E > 0)		// -norm
						type = 1;
					else if(F > 0)		// -subnorm
						type = 2;
					else				// -0
						type = 3;
				}
				else					// positive
				{
					if(E == 0xFF)		// +inf
						type = 7;
					else if(E > 0)		// +norm
						type = 6;
					else if(F > 0)		// +subnorm
						type = 5;
					else				// +0
						type = 4;
				}
			
				RegFile[rd(IF)] = (1 << type);
				return 0;
			}
		}
		else if(funct5(IF) == 0x1E)	// FMV.S.X
		{
			if(rm(IF) != 0)
			{
				// printf("FMV.S.X error! No such instruction\n");
				return 1;
			}
			int tmp = RegFile[rs1(IF)];
			FRegFile[rd(IF)] = *(float*)&tmp;
			return 0;
		}
		else if(funct5(IF) == 0x14)	// FCMP.S (NaN not checked!)
		{
			if(rm(IF) == 2)			// FEQ.S
			{
				RegFile[rd(IF)] = (FRegFile[rs1(IF)] == FRegFile[rs2(IF)]);
				return 0;
			}
			else if(rm(IF) == 1)	// FLT.S
			{
				RegFile[rd(IF)] = (FRegFile[rs1(IF)] < FRegFile[rs2(IF)]);
				return 0;
			}
			else if(rm(IF) == 0)	// FLE.S
			{
				RegFile[rd(IF)] = (FRegFile[rs1(IF)] <= FRegFile[rs2(IF)]);
				return 0;
			}
			else
			{
				// printf("FCMP.S error! No such instruction\n");
				return 1;
			}
		}
	}
	else if(fmt(IF) == 1)	// (Mainly) D instructions
	{
		if(funct5(IF) == 0)			// FADD.D
		{
			DRegFile[rd(IF)] = DRegFile[rs1(IF)] + DRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x1)	// FSUB.D
		{
			DRegFile[rd(IF)] = DRegFile[rs1(IF)] - DRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x2)	// FMUL.D
		{
			DRegFile[rd(IF)] = DRegFile[rs1(IF)] * DRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x3)	// FDIV.D
		{
			DRegFile[rd(IF)] = DRegFile[rs1(IF)] / DRegFile[rs2(IF)];
			return 0;
		}
		else if(funct5(IF) == 0xB)	// FSQRT.D
		{
			DRegFile[rd(IF)] = sqrt(DRegFile[rs1(IF)]);
			return 0;
		}
		else if(funct5(IF) == 0x5)	// FMIN/MAX.D (NaN not checked!)
		{
			if(rm(IF) == 0)			// FMIN.D
			{
				DRegFile[rd(IF)] = (DRegFile[rs1(IF)] < DRegFile[rs2(IF)]) ? DRegFile[rs1(IF)] : DRegFile[rs2(IF)];
				return 0;
			}
			else if(rm(IF) == 1)	// FMAX.D
			{
				DRegFile[rd(IF)] = (DRegFile[rs1(IF)] > DRegFile[rs2(IF)]) ? DRegFile[rs1(IF)] : DRegFile[rs2(IF)];
				return 0;
			}
			else
			{
				// printf("FMIN/MAX.D error! No such instruction\n");
				return 1;
			}
		}
		else if(funct5(IF) == 0x18)	// FCVT.W[U].D
		{
			if(rs2(IF) == 0)		// FCVT.W.D
			{
				RegFile[rd(IF)] = (int)DRegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 1)	// FCVT.WU.D
			{
				RegFile[rd(IF)] = (unsigned int)DRegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 2)	// FCVT.L.D
			{
				RegFile[rd(IF)] = (long long)DRegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 3)	// FCVT.LU.D
			{
				RegFile[rd(IF)] = (unsigned long long)DRegFile[rs1(IF)];
				return 0;
			}
			// printf("FCVT.x.D error! No such instruction\n");
			return 1;
		}
		else if(funct5(IF) == 0x1A)	// FCVT.D.W[U]
		{
			if(rs2(IF) == 0)		// FCVT.D.W
			{
				DRegFile[rd(IF)] = (double)(int)RegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 1)	// FCVT.D.WU
			{
				DRegFile[rd(IF)] = (double)(unsigned int)RegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 2)	// FCVT.D.L
			{
				DRegFile[rd(IF)] = (double)RegFile[rs1(IF)];
				return 0;
			}
			else if(rs2(IF) == 3)	// FCVT.D.LU
			{
				DRegFile[rd(IF)] = (double)(unsigned long long)RegFile[rs1(IF)];
				return 0;
			}
			// printf("FCVT.D.x error! No such instruction\n");
			return 1;
		}
		else if(funct5(IF) == 0x8)	// FCVT.D.S
		{
			DRegFile[rd(IF)] = FRegFile[rs1(IF)];
			return 0;
		}
		else if(funct5(IF) == 0x4)	// FSGNJ.D
		{
			long long src1 = *(long long*)&DRegFile[rs1(IF)];
			long long src2 = *(long long*)&DRegFile[rs2(IF)];
			long long sgn1 = ((src1 >> 63) << 63);
			long long sgn2 = ((src2 >> 63) << 63);
			long long sgn;
			long long val = src1 & (~(1LL << 63));
			if(rm(IF) == 0)			// FSGNJ.D
				sgn = sgn2;
			else if(rm(IF) == 1)	// FSGNJN.D
				sgn = sgn2 ^ (1LL << 63);
			else if(rm(IF) == 2)	// FSGNJX.D
				sgn = sgn1 ^ sgn2;
			else
			{
				// printf("FSGNJ.D error! No such instruction\n");
				return 1;
			}
		
			long long res = sgn | val;
			DRegFile[rd(IF)] = *(double*)&res;
			return 0;
		}
		else if(funct5(IF) == 0x1C)	// FMV.X.D or FCLASS.D
		{
			if(rm(IF) == 0)			// FMV.X.D
			{
				RegFile[rd(IF)] = *(long long*)&DRegFile[rs1(IF)];
				return 0;
			}
			else if(rm(IF) == 1)	// FCLASS.D
			{
				int type;
				long long tmp = *(long long*)&DRegFile[rs1(IF)];
				long long S = (tmp >> 63) & 0x1;		// 1 bit
				long long E = (tmp >> 52) & 0x7FF;		// 11 bits
				long long F = tmp & ((1LL << 52)-1);	// 52 bits
				if(E == 0x7FF && F != 0)// nan
				{
					if(F >> 51)			// quiet nan
						type = 9;
					else				// signaling nan
						type = 8;
				}
				else if(S)				// negative
				{
					if(E == 0x7FF)		// -inf
						type = 0;
					else if(E > 0)		// -norm
						type = 1;
					else if(F > 0)		// -subnorm
						type = 2;
					else				// -0
						type = 3;
				}
				else					// positive
				{
					if(E == 0x7FF)		// +inf
						type = 7;
					else if(E > 0)		// +norm
						type = 6;
					else if(F > 0)		// +subnorm
						type = 5;
					else				// +0
						type = 4;
				}
			
				RegFile[rd(IF)] = (1 << type);
				return 0;
			}
		}
		else if(funct5(IF) == 0x1E)	// FMV.D.X
		{
			if(rm(IF) != 0)
			{
				// printf("FMV.D.X error! No such instruction\n");
				return 1;
			}
			long long tmp = RegFile[rs1(IF)];
			DRegFile[rd(IF)] = *(double*)&tmp;
			return 0;
		}
		else if(funct5(IF) == 0x14)	// FCMP.D (NaN not checked!)
		{
			if(rm(IF) == 2)			// FEQ.D
			{
				RegFile[rd(IF)] = (DRegFile[rs1(IF)] == DRegFile[rs2(IF)]);
				return 0;
			}
			else if(rm(IF) == 1)	// FLT.D
			{
				RegFile[rd(IF)] = (DRegFile[rs1(IF)] < DRegFile[rs2(IF)]);
				return 0;
			}
			else if(rm(IF) == 0)	// FLE.D
			{
				RegFile[rd(IF)] = (DRegFile[rs1(IF)] <= DRegFile[rs2(IF)]);
				return 0;
			}
			else
			{
				// printf("FCMP.D error! No such instruction\n");
				return 1;
			}
		}
	}
		
	
	// printf("OP_FP_func error! No such instruction\n");
	return 1;
}
int FMADD_func(int IF)
{
	if(fmt(IF) == 0)		// FMADD.S
	{
		FRegFile[rd(IF)] = FRegFile[rs1(IF)] * FRegFile[rs2(IF)] + FRegFile[rs3(IF)];
		return 0;
	}
	else if(fmt(IF) == 1)	// FMADD.D
	{
		DRegFile[rd(IF)] = DRegFile[rs1(IF)] * DRegFile[rs2(IF)] + DRegFile[rs3(IF)];
		return 0;
	}
	return 1;
}
int FMSUB_func(int IF)
{
	if(fmt(IF) == 0)		// FMSUB.S
	{
		FRegFile[rd(IF)] = FRegFile[rs1(IF)] * FRegFile[rs2(IF)] - FRegFile[rs3(IF)];
		return 0;
	}
	else if(fmt(IF) == 1)	// FMSUB.D
	{
		DRegFile[rd(IF)] = DRegFile[rs1(IF)] * DRegFile[rs2(IF)] - DRegFile[rs3(IF)];
		return 0;
	}
	return 1;
}
int FNMADD_func(int IF)
{
	if(fmt(IF) == 0)		// FNMADD.S
	{
		FRegFile[rd(IF)] = -( FRegFile[rs1(IF)] * FRegFile[rs2(IF)] + FRegFile[rs3(IF)] );
		return 0;
	}
	else if(fmt(IF) == 1)	// FNMADD.D
	{
		DRegFile[rd(IF)] = -( DRegFile[rs1(IF)] * DRegFile[rs2(IF)] + DRegFile[rs3(IF)] );
		return 0;
	}
	return 1;
}
int FNMSUB_func(int IF)
{
	if(fmt(IF) == 0)		// FNMSUB.S
	{
		FRegFile[rd(IF)] = -( FRegFile[rs1(IF)] * FRegFile[rs2(IF)] - FRegFile[rs3(IF)] );
		return 0;
	}
	else if(fmt(IF) == 1)	// FNMSUB.D
	{
		DRegFile[rd(IF)] = -( DRegFile[rs1(IF)] * DRegFile[rs2(IF)] - DRegFile[rs3(IF)] );
		return 0;
	}
	return 1;
}

int SYSTEM_func(int IF)
{
	if((IF >> 20) == 0)			// ECALL
	{
		// printf("SYSTEM CALL: %d\n", (int)RegFile[17]);
		switch(RegFile[17])
		{
		    case 57://close
		    {
		        // RegFile[10]=close(RegFile[10]);
		        break;
		    }
		    case 62://lseek
		    {
		        RegFile[10]=lseek(RegFile[10],RegFile[11],RegFile[12]);
		        break;
		    }
		    case 63://read
		    {
		        RegFile[10]=read(RegFile[10],memptr(RegFile[11]),RegFile[12]);
		        break;
		    }
		    case 64://write
		    {
		        RegFile[10]=write(RegFile[10],(const void*)(memptr(RegFile[11])),RegFile[12]);
		        break;
		    }
		    case 80://fstat
		    {
		        struct stat t;
		        RegFile[10] = fstat(RegFile[10],&t);
		        struct stat_rv *ptr = (struct stat_rv *)(memptr(RegFile[11]));
		        ptr->dev = t.st_dev;
		        ptr->ino = t.st_ino;
		        ptr->smode = t.st_mode;
		        ptr->nlink = t.st_nlink;
		        ptr->uid = t.st_uid;
		        ptr->gid = t.st_gid;
		        ptr->rdev = t.st_rdev;
		        ptr->size = t.st_size;
		        ptr->atime = t.st_atime;
		        ptr->mtime = t.st_mtime;
		        ptr->ctime_ = t.st_ctime;
		        break;
		    }
		    case 93://exit
		    {
		        return 1;
		    }
		    case 169://gettimeofday not 100% sure
		    {
		        RegFile[10] = gettimeofday((struct timeval*)memptr(RegFile[10]), NULL);
		        break;
		    }
		    case 214://sbrk
		    {
		    	// RegFile[10] = 0;	// return 'success' ? 
		        break;
		    }
		    default:
		    {
		        printf("SYSTEM_func error!No such instruction\n");
		        return 1;
		    }
		}
	}
	else if((IF >> 20) == 3)	// FxCSR
	{
		if(rm(IF) == 1)			// FSCSR
		{
			RegFile[rd(IF)] = fcsr;
			fcsr = RegFile[rs1(IF)];
			return 0;
		}
		else if(rm(IF) == 2)	// FRCSR
		{
			RegFile[rd(IF)] = fcsr;
			return 0;
		}
		else
		{
			printf("FxCSR_func error! No such instruction\n");
			return 1;
		}
	}
	else if((IF >> 20) == 2)	// FxRM[I]
	{
		if(rm(IF) == 1)			// FSRM
		{
			RegFile[rd(IF)] = (fcsr >> 5) & 0x7;
			fcsr = fcsr & (~0xE0) | ((RegFile[rs1(IF)] & 0x7) << 5);
			return 0;
		}
		else if(rm(IF) == 2)	// FRRM
		{
			RegFile[rd(IF)] = (fcsr >> 5) & 0x7;
			return 0;
		}
		else if(rm(IF) == 5)	// FSRMI
		{
			RegFile[rd(IF)] = (fcsr >> 5) & 0x7;
			fcsr = (fcsr & (~0xE0)) | ((rs1(IF) & 0x7) << 5);
			return 0;
		}
		else
		{
			printf("FxRM[I]_func error! No such instruction\n");
			return 1;
		}
	}
	else if((IF >> 20) == 1)	// FxFLAGS[I]
	{
		if(rm(IF) == 1)			// FSFLAGS
		{
			RegFile[rd(IF)] = fcsr & 0x1F;
			fcsr = (fcsr & (~0x1F)) | (RegFile[rs1(IF)] & 0x1F);
			return 0;
		}
		else if(rm(IF) == 2)	// FRFLAGS
		{
			RegFile[rd(IF)] = fcsr & 0x1F;
			return 0;
		}
		else if(rm(IF) == 5)	// FSFLAGSI
		{
			RegFile[rd(IF)] = fcsr & 0x1F;
			fcsr = (fcsr & (~0x1F)) | rs1(IF);
			return 0;
		}
		else
		{
			printf("FxFLAGS[I]_func error! No such instruction\n");
			return 1;
		}
	}
		
    return 0;
}

void init(Addr entry)
{
	memset(RegFile, 0, sizeof(RegFile));
	RegFile[2] = Stack_base;				// set sp
	// gp, tp ?
	memset(FRegFile, 0, sizeof(FRegFile));
	memset(DRegFile, 0, sizeof(DRegFile));
	
	PC = entry;
}
