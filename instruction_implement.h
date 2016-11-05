#ifndef DEF_H
#define DEF_H
#include "def.h"
#endif
#ifndef ARCH_H
#define ARCH_H
#include "arch.h"
#endif

#define ADD(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]+RegFile[rs2(IF)]
#define SUB(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]-RegFile[rs2(IF)]
#define XOR(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]^RegFile[rs2(IF)]
#define OR(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]|RegFile[rs2(IF)]
#define AND(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]&RegFile[rs2(IF)]
#define SLL(IF) RegFile[rd(IF)]=RegFile[rs1(IF)]<<(RegFile[rs2(IF)]&0x03F)
#define SRL(IF) RegFile[rd(IF)]=((unsgined long long)RegFile[rs1(IF)])>>(RegFile[rs2(IF)]&0x03F)
#define SRA(IF) RegFile[rd(IF)]=((long long)RegFile[rs1(IF)])>>(RegFile[rs2(IF)]&0x03F)
#define SLT(IF) RegFile[rd(IF)]=((long long)RegFile[rs1(IF)]<(long long)RegFile[rs2(IF)])
#define SLTU(IF) RegFile[rd(IF)]=((unsigned long long)RegFile[rs1(IF)]<(unsigned long long)RegFile[rs2(IF)])