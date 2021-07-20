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

struct elf_32 *parse_elf_32(FILE *file_ptr)
{
	struct elf_32 *buff = NULL;
	
	if(!( buff = malloc(sizeof(*buff)) ))
		fprintf(stderr, "could not allocate memory for buffer...malloc failed\n");
	else
		if(!fread( buff, sizeof(*buff), 1, file_ptr)) 
			fprintf(stderr, "error while reading from file...fread failed\n");

	return buff;
}

void display_header_elf_32(struct elf_32 *buff)
{
	fprintf(stdout,
			"============[ HEADER ]============\n"
			"e_ident[EI_MAG0-3]    : 0x%X %c%c%c\n"
			"e_ident[EI_CLASS]     : 0x%X\n"
			"e_ident[EI_DATA]      : 0x%X\n"
			"e_ident[EI_VERSION]   : 0x%X\n"
			"e_ident[EI_OSABI]     : 0x%X\n"
			"e_ident[EI_ABIVERSION]: 0x%X\n"
			"e_ident[EI_PAD]       : 0x%X%X%X%X%X%X%X\n"
			"e_type                : 0x%X\n"
			"e_machine             : 0x%X\n"
			"e_version             : 0x%X\n"
			"e_entry               : 0x%X\n"
			"e_phoff               : 0x%X\n"
			"e_shoff               : 0x%X\n"
			"e_flags               : 0x%X\n"
			"e_ehsize              : 0x%X\n"
			"e_phentsize           : 0x%X\n"
			"e_phnum               : 0x%X\n"
			"e_shentsize           : 0x%X\n"
			"e_shnum               : 0x%X\n"
			"e_shstrndx            : 0x%X\n"
			,
			buff->e_ident[EI_MAG0], buff->e_ident[EI_MAG1], buff->e_ident[EI_MAG2], buff->e_ident[EI_MAG3],
			buff->e_ident[EI_CLASS],
			buff->e_ident[EI_DATA],
			buff->e_ident[EI_VERSION],
			buff->e_ident[EI_OSABI],
			buff->e_ident[EI_ABIVERSION],
			buff->e_ident[EI_PAD], buff->e_ident[EI_PAD+1],buff->e_ident[EI_PAD+2],buff->e_ident[EI_PAD+3],
			buff->e_ident[EI_PAD+4],buff->e_ident[EI_PAD+5],buff->e_ident[EI_PAD+6],
			buff->e_type,
			buff->e_machine,
			buff->e_version,
			buff->e_entry,
			buff->e_phoff,
			buff->e_shoff,
			buff->e_flags,
			buff->e_ehsize,
			buff->e_phentsize,
			buff->e_phnum,
			buff->e_shentsize,
			buff->e_shnum,
			buff->e_shstrndx
			);

}

int main(int argc, char **argv)
{
	if(argc < 2) return 1;
	
	FILE *ptr = fopen( argv[1], "r");
	struct elf_32 *buffer= parse_elf_32(ptr);
	display_header_elf_32(buffer);
	free(buffer);
	return 0;
}
