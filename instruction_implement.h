#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif
#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif
#ifndef FUNC_H
#define FUNC_H
#include "func.h"
#endif

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
	char tmp;
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
	short tmp;
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
	int tmp;
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