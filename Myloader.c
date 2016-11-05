#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "exec.h"

// ELF format
#define EI_NIDENT   (16)

struct elf32_head
{
    unsigned char e_ident[EI_NIDENT];   // the magic number  
    unsigned short e_type;              // identify the object file type 
    unsigned short e_machine;           // specify the machine architechture 
    unsigned int e_version;             // identify the object file version
    unsigned long e_entry;              // entry point address of the process 
    unsigned long e_phoff;              // start of program header 
    unsigned long e_shoff;              // start of section header 
    unsigned int e_flags;               // flags 
    unsigned short e_ehsize;            // size of the ELF header 
    unsigned short e_phentsize;         // size of program header 
    unsigned short e_phnum;             // number of program header 
    unsigned short e_shentsize;         // size of section header 
    unsigned short e_shnum;             // number of section header 
    unsigned short e_shstrndx;          // section header string table index 
} e;

struct sec_head
{
	unsigned name;
	unsigned type;
	unsigned long flags;
	unsigned long addr;			// base memory address
	unsigned long offset;		// file offset of the section
	unsigned long size;			// size in byte
	unsigned link;
	unsigned info;
	unsigned long align;
	unsigned long entsize;
} s;

struct prog_head
{
	unsigned type;
	unsigned flags;
	unsigned long offset;			// file offset of the segment
	unsigned long virtaddr;			// virtual address to map segment
	unsigned long physaddr;			// (no use)
	unsigned long filesize;			// segment size in file
	unsigned long memsize;			// segment size in memory
	unsigned long align;
} p;
	
int main(int argc, char **argv)
{
	char *exec_filename = argv[1];
	
	FILE *fp = fopen(exec_filename,"rb");
	FILE *fp2 = fopen(exec_filename, "rb");
	fread((void*)&e, sizeof(struct elf32_head), 1, fp);

	// visit & load program header (segments)
	fseek(fp, e.e_phoff, SEEK_SET);
	int i;
	for(i = 0; i < e.e_phnum; i++)
	{
		fread((void*)&p, sizeof(struct prog_head), 1, fp);						// fp: read program header
		fseek(fp2, p.offset, SEEK_SET);											// fp2: read segment content
		void *seg_start = memptr(p.virtaddr);
		fread(seg_start, 1, p.filesize, fp2);									// load segment content into Mem
		memset((void*)(seg_start + p.filesize), 0, p.memsize - p.filesize);		// clear bss area in Mem
	}
	
	init(e.e_entry);
	int stat;
	while(1)
	{
		int IF = get_instruction();
		// IF = 0x100513;				// addi a0, zero, 1
		printf("IF = %x\n", IF);
		stat = decode_and_run(IF);
		if(stat != 0)
			break;
	}
	
	// Test...
	printf("Registers:\n");
	for(i = 0; i < Reg_number; i++)
	{
		printf("Reg %d: %lld\n", i, RegFile[i]);
	}
	
	// Exit
	fclose(fp2);
	fclose(fp);
	return 0;
}






