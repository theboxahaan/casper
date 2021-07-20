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

struct elf_32_file_header
{
	/* file header */
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

struct elf_32_program_header_entry
{
	/* program header */
	uint32_t p_type;		// identifies the type of segment
	uint32_t p_offset;		// offset of segment in the file image
	uint32_t p_vaddr;		// virtual address of segment in memory
	uint32_t p_paddr;		// reserved for physical address of segment
	uint32_t p_filesz;		// size in bytes of the segment in file image
	uint32_t p_memsz;		// size in bytes of the segment in memory
	uint32_t p_flags;		// segment dependent flags (32-bit arch.)
	uint32_t p_align;		// look up docus

};


struct elf_32
{
	struct elf_32_file_header *file_header;
	struct elf_32_program_header_entry *program_header;
};



struct elf_32 *parse_elf_32(FILE *file_ptr)
{
	struct elf_32 *parsed_elf = NULL;
	
	if(!( parsed_elf = malloc(sizeof(*parsed_elf)) ))
		fprintf(stderr, "could not allocate memory for parsed  struct...malloc failed\n");
	else
	{
		/* allocate memory for file header */
		if( (parsed_elf->file_header = malloc(sizeof(*parsed_elf->file_header))) && 
			fread(parsed_elf->file_header, sizeof(*parsed_elf->file_header), 1, file_ptr))
		{
			/* parse phnum entries of size ph_entsize from offset phoff */
			size_t phnum = parsed_elf->file_header->e_phnum;
			size_t phentsize = parsed_elf->file_header->e_phentsize;
			size_t phoff = parsed_elf->file_header->e_phoff;
			/* set stream pos indicator to offset=phoff from SEEK_SET */
			fseek(file_ptr, phoff, SEEK_SET);

			/* allocate memory for program header */
			if( (parsed_elf->program_header = malloc(phentsize * phnum)) &&
				fread(parsed_elf->program_header, phentsize, phnum, file_ptr) );
			else
				fprintf(stderr, "parsing elf program_header failed...malloc error or fread error\n");
		}
		else
			fprintf(stderr, "parsing elf file_header failed...malloc error or fread error\n");
	}

	return parsed_elf;
}

void display_file_header_elf_32(struct elf_32 *parsed_elf_32)
{
	struct elf_32_file_header *buff = parsed_elf_32->file_header;
	fprintf(stdout,
			"============[ FILE HEADER ]============\n"
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


void display_program_header_elf_32(struct elf_32 *parsed_elf_32)
{
	size_t phnum = parsed_elf_32->file_header->e_phnum;
	struct elf_32_program_header_entry *table = parsed_elf_32->program_header;
	fprintf(stdout, "============[ PROGRAM HEADER ]============\n");
	for(size_t i=0; i<phnum; i++)
	{
		fprintf(stdout,
				"[%lu]\n"
				"\tp_type  : 0x%X\n"
				"\tp_offset: 0x%X\n"
				"\tp_vaddr : 0x%X\n"
				"\tp_paddr : 0x%X\n"
				"\tp_filesz: 0x%X\n"
				"\tp_memsz : 0x%X\n"
				"\tp_flags : 0x%X\n"
				"\tp_align : 0x%X\n\n",
				i,
				table[i].p_type,
				table[i].p_offset,
				table[i].p_vaddr,
				table[i].p_paddr,
				table[i].p_filesz,
				table[i].p_memsz,
				table[i].p_flags,
				table[i].p_align
				);
	}
}


int main(int argc, char **argv)
{
	if(argc < 2) return 1;
	
	FILE *ptr = fopen( argv[1], "r");
	struct elf_32 *buffer = parse_elf_32(ptr);
	display_file_header_elf_32(buffer);
	display_program_header_elf_32(buffer);
	free(buffer->file_header);
	free(buffer->program_header);
	free(buffer);
	return 0;
}
