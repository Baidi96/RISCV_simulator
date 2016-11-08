#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif

Reg RegFile[Reg_number];
float FRegFile[Reg_Number];
int fcsr = 0;								// Floating control and state register
char Mem[Mem_size];
Addr Stack_base = Mem_size - (1 << 12);		// save 4KB (may need to modify)
Addr PC;

int opcode(int IF)
{
	return IF&((1<<7)-1);
}
int rd(int IF)
{
	return (IF>>7)&((1<<5)-1);
}
int funct3(int IF)
{
	return (IF>>12)&((1<<3)-1);
}
int rs1(int IF)
{
	return (IF>>15)&((1<<5)-1);
}
int rs2(int IF)
{
	return (IF>>20)&((1<<5)-1);
}
int rs3(int IF)
{
	return (IF>>27)&((1<<5)-1);
}
int funct7(int IF)
{
	return (IF>>25)&((1<<7)-1);
}

// F extension
int funct5(int IF)
{
	return (IF>>27)&((1<<5)-1);
}
int rm(int IF)
{
	return (IF>>12)&((1<<3)-1);
}
int fmt(int IF)
{
	return (IF>>25)&0x3;
}

