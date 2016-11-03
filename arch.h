#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif
Reg RegFile[Reg_number];
int Mem[Mem_size];
inline int opcode(Reg IF)
{
	return IF&((1<<7)-1);
}
inline int rd(Reg IF)
{
	return (IF>>7)&((1<<5)-1);
}
inline int funct3(Reg IF)
{
	return (IF>>12)&((1<<3)-1);
}
inline int rs1(Reg IF)
{
	return (IF>>15)&((1<<5)-1);
}
inline int rs2(Reg IF)
{
	return (IF>>20)&((1<<5)-1);
}
inline int rs3(Reg IF)
{
	return (IF>>27)&((1<<5)-1);
}
inline int funct7(Reg IF)
{
	return (IF>>25)&((1<<7)-1);
}