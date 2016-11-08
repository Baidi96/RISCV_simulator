#include <string.h>

typedef long long Reg;
typedef unsigned long long Addr;

#define Reg_number 32
#define Mem_size (1LL<<29)						// 512MB
#define Load 0x03//0000011b
#define Store 0x23//0100011b
#define ALUR 0x33//0110011b
#define ALUI 0x13//0010011b
#define LUI 0x37//0110111b
#define AUIPC 0x17//0010111b
#define Branch 0x63//1100011b
#define JAL 0x6F//1101111b
#define JALR 0x67//1100111b
#define SYSTEM 0x73//1110011b
#define ALUI_64 0x1B//0011011b
#define ALUR_64 0x3B//0111011b

#define FLoad 0x07	//0000111b
#define FStore 0x27	//0100111b
#define OP_FP 0x53	//1010011b
#define FMADD 0x43	//1000011b
#define FMSUB 0x47	//1000111b
#define FNMADD 0x4F	//1001111b
#define FNMSUB 0x4B	//1001011b
