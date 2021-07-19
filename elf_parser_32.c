#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

enum e_ident_offsets
{
	EI_MAG0,		// 00x7F
	EI_MAG1,		// `E`
	EI_MAG2,		// `L`
	EI_MAG3,		// `F`
	EI_CLASS,		// signify 32/64 bit format
	EI_DATA,		// signify endian-ness
	EI_VERSION,		// set to 1 for original and current version of ELF
	EI_OSABI,		// identify target OS ABI
	EI_ABIVERSION,	// usage varies sometimes important other times not 
	EI_PAD			// padded with 0s - 7 pad bytes
};

struct elf_32
{
	uint8_t e_ident[16];	// identification bytes
	uint16_t e_type;		// identify object file type
	uint16_t e_machine;		// specify the target ISA
	uint32_t e_version;		// set to 1 for original ELF
	uint32_t e_entry;		// mem-address of entrypoint for process
	uint32_t e_phoff;		// points to the start of the program header table
	uint32_t e_shoff;		// points to the start of the section header table
	uint32_t e_flags;		// interpretation depends on architecture
	uint16_t e_ehsize;		// size of the header
	uint16_t e_phentsize;	// size of program header table entry
	uint16_t e_phnum;		// number of entries in program header table
	uint16_t e_shentsize;	// size of a section header table entry
	uint16_t e_shnum;		// number of entries in the section header table
	uint16_t e_shstrndx;	// index of the section header table entry that contains section names

};

struct elf_32 *parser_elf_32(FILE *file_ptr)
{
	struct elf_32 *buff;
	
	if(!( buff = malloc(sizeof(*buff)) ))
	{
		printf("Error with malloc\n");
		return NULL;
	}
	if(fread( buff, sizeof(*buff), 1, file_ptr)) printf("0X%X %c %c %c \n", buff->e_ident[EI_MAG0], buff->e_ident[EI_MAG1], buff->e_ident[EI_MAG2], buff->e_ident[EI_MAG3]);
	else printf("fread failed\n");

	return buff;
}

/* primitive testrig  code */

int main(int argc, char **argv)
{
	if(argc < 2) return 1;
	
	FILE *ptr = fopen( argv[1], "r");
	free(parser_elf_32(ptr));
	return 0;
}
