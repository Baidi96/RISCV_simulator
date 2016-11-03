#include <string.h>
typedef long long Reg;
typedef unsigned long long Addr;
const int Reg_number = 32;
const unsigned long long Mem_size = 1LL<<29;	// 512MB
const Addr Stack_base = Mem_size - (1 << 12);	// save 4KB (may need to modify)
char* PC;
#define Load 0x03//0000011b
#define Store 0x23//0100011b
#define ALUR 0x33//0110011b
#define ALUI 0x13//0010011b//ADDI
#define LUI 0x37//0110111b
#define AUIPC 0x17//0010111b
#define Branch 0x63//1100011b
#define JAL 0x6F//1101111b
#define JALR 0x67//1100111b
#define Syscall 0x73//1110011b
#define ALUI_64 0x1B//0011011b
#define ALUR_64 0x3B//0111011b
