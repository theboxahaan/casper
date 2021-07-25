#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

#include "elf.h"

uint8_t is_elf_64(FILE *file_ptr)
{
	uint8_t eiclass;
	fseek(file_ptr, 0x04, SEEK_SET); // 0x04 contains the byte e_ident[EI_CLASS]
	fread(&eiclass, sizeof(eiclass), 1, file_ptr);
	rewind(file_ptr);
	return eiclass;
}

uint8_t is_little_endian(FILE *file_ptr)
{
	uint8_t eidata;
	fseek(file_ptr, 0x05, SEEK_SET); // 0x05 contains the byte e_ident[EI_DATA]
	fread(&eidata, sizeof(eidata), 1, file_ptr);
	rewind(file_ptr);
	return eidata;
}


struct elf_32 *parse_elf_32(FILE *file_ptr)
{
	struct elf_32 *parsed_elf = NULL;
	
	if(!( parsed_elf = malloc(sizeof(*parsed_elf))))
		fprintf(stderr, "could not allocate memory for parsed  struct...malloc failed\n");
	else {
		/* allocate memory for file header */
		if((parsed_elf->file_header = malloc(sizeof(*parsed_elf->file_header))) && 
			fread(parsed_elf->file_header, sizeof(*parsed_elf->file_header), 1, file_ptr)) {
			
			/* parse phnum entries of size ph_entsize from offset phoff */
			size_t phnum = parsed_elf->file_header->e_phnum;
			size_t phentsize = parsed_elf->file_header->e_phentsize;
			size_t phoff = parsed_elf->file_header->e_phoff;

			fseek(file_ptr, phoff, SEEK_SET);

			/* allocate memory for program header */
			if((parsed_elf->program_header = malloc(phentsize * phnum)) &&
				fread(parsed_elf->program_header, phentsize, phnum, file_ptr));
			else {
				elf_32_cleanup(parsed_elf);
				fprintf(stderr, "parsing elf program_header failed...malloc error or fread error\n");
				return NULL;
			} 

			size_t shnum = parsed_elf->file_header->e_shnum;
			size_t shentsize = parsed_elf->file_header->e_shentsize;
			size_t shoff = parsed_elf->file_header->e_shoff;

			fseek(file_ptr, shoff, SEEK_SET);
			
			/* allocate memory for section header */
			if((parsed_elf->section_header = malloc(shentsize * shnum)) &&
				fread(parsed_elf->section_header, shentsize, shnum, file_ptr));
			else {
				elf_32_cleanup(parsed_elf);
				fprintf(stderr, "parsing elf section_header failed...malloc error or fread error\n");
				return NULL;
			}
		} else {
			fprintf(stderr, "parsing elf file_header failed...malloc error or fread error\n");
		}
	}

	return parsed_elf;
}

struct elf_64 *parse_elf_64(FILE *file_ptr)
{
	struct elf_64 *parsed_elf = NULL;
	
	if(!( parsed_elf = malloc(sizeof(*parsed_elf))))
		fprintf(stderr, "could not allocate memory for parsed  struct...malloc failed\n");
	else {
		/* allocate memory for file header */
		if((parsed_elf->file_header = malloc(sizeof(*parsed_elf->file_header))) && 
			fread(parsed_elf->file_header, sizeof(*parsed_elf->file_header), 1, file_ptr)) {
			
			/* parse phnum entries of size ph_entsize from offset phoff */
			size_t phnum = parsed_elf->file_header->e_phnum;
			size_t phentsize = parsed_elf->file_header->e_phentsize;
			size_t phoff = parsed_elf->file_header->e_phoff;

			fseek(file_ptr, phoff, SEEK_SET);

			/* allocate memory for program header */
			if((parsed_elf->program_header = malloc(phentsize * phnum)) &&
				fread(parsed_elf->program_header, phentsize, phnum, file_ptr));
			else {
				elf_64_cleanup(parsed_elf);
				fprintf(stderr, "parsing elf program_header failed...malloc error or fread error\n");
				return NULL;
			} 

			size_t shnum = parsed_elf->file_header->e_shnum;
			size_t shentsize = parsed_elf->file_header->e_shentsize;
			size_t shoff = parsed_elf->file_header->e_shoff;

			fseek(file_ptr, shoff, SEEK_SET);
			
			/* allocate memory for section header */
			if((parsed_elf->section_header = malloc(shentsize * shnum)) &&
				fread(parsed_elf->section_header, shentsize, shnum, file_ptr));
			else {
				elf_64_cleanup(parsed_elf);
				fprintf(stderr, "parsing elf section_header failed...malloc error or fread error\n");
				return NULL;
			}
		} else {
			fprintf(stderr, "parsing elf file_header failed...malloc error or fread error\n");
		}
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

void display_file_header_elf_64(struct elf_64 *parsed_elf_64)
{
	struct elf_64_file_header *buff = parsed_elf_64->file_header;
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
			"e_entry               : 0x%llX\n"
			"e_phoff               : 0x%llX\n"
			"e_shoff               : 0x%llX\n"
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

void display_program_header_elf_64(struct elf_64 *parsed_elf_64)
{
	size_t phnum = parsed_elf_64->file_header->e_phnum;
	struct elf_64_program_header_entry *table = parsed_elf_64->program_header;
	fprintf(stdout, "============[ PROGRAM HEADER ]============\n");
	for(size_t i=0; i<phnum; i++)
	{
		fprintf(stdout,
				"[%lu]\n"
				"\tp_type  : 0x%X\n"
				"\tp_flags : 0x%X\n"
				"\tp_offset: 0x%llX\n"
				"\tp_vaddr : 0x%llX\n"
				"\tp_paddr : 0x%llX\n"
				"\tp_filesz: 0x%llX\n"
				"\tp_memsz : 0x%llX\n"
				"\tp_align : 0x%llX\n\n",
				i,
				table[i].p_type,
				table[i].p_flags,
				table[i].p_offset,
				table[i].p_vaddr,
				table[i].p_paddr,
				table[i].p_filesz,
				table[i].p_memsz,
				table[i].p_align
				);
	}
}


void display_section_header_elf_32(struct elf_32 *parsed_elf_32)
{
	size_t shnum = parsed_elf_32->file_header->e_shnum;
	struct elf_32_section_header_entry *table = parsed_elf_32->section_header;
	fprintf(stdout, "============[ SECTION HEADER ]============\n");
	for(size_t i=0; i<shnum; i++)
	{
		fprintf(stdout,
				"[%lu]\n"
				"\tsh_name     : 0x%X\n"
				"\tsh_type     : 0x%X\n"
				"\tsh_flags    : 0x%X\n"
				"\tsh_addr     : 0x%X\n"
				"\tsh_offset   : 0x%X\n"
				"\tsh_size     : 0x%X\n"
				"\tsh_link     : 0x%X\n"
				"\tsh_info     : 0x%X\n"
				"\tsh_addralign: 0x%X\n"
				"\tsh_entsize  : 0x%X\n\n",
				i,
				table[i].sh_name,
				table[i].sh_type,
				table[i].sh_flags,
				table[i].sh_addr,
				table[i].sh_offset,
				table[i].sh_size,
				table[i].sh_link,
				table[i].sh_info,
				table[i].sh_addralign,
				table[i].sh_entsize
				);
	}
}


void display_section_header_elf_64(struct elf_64 *parsed_elf_64)
{
	size_t shnum = parsed_elf_64->file_header->e_shnum;
	struct elf_64_section_header_entry *table = parsed_elf_64->section_header;
	fprintf(stdout, "============[ SECTION HEADER ]============\n");
	for(size_t i=0; i<shnum; i++)
	{
		fprintf(stdout,
				"[%lu]\n"
				"\tsh_name     : 0x%X\n"
				"\tsh_type     : 0x%X\n"
				"\tsh_flags    : 0x%llX\n"
				"\tsh_addr     : 0x%llX\n"
				"\tsh_offset   : 0x%llX\n"
				"\tsh_size     : 0x%llX\n"
				"\tsh_link     : 0x%X\n"
				"\tsh_info     : 0x%X\n"
				"\tsh_addralign: 0x%llX\n"
				"\tsh_entsize  : 0x%llX\n\n",
				i,
				table[i].sh_name,
				table[i].sh_type,
				table[i].sh_flags,
				table[i].sh_addr,
				table[i].sh_offset,
				table[i].sh_size,
				table[i].sh_link,
				table[i].sh_info,
				table[i].sh_addralign,
				table[i].sh_entsize
				);
	}
}


void elf_32_cleanup(struct elf_32 *elf)
{
	fprintf(stdout, "freeing memory for elf_32 struct at 0x%p\n", elf);
	free(elf->file_header);
	free(elf->program_header);
	free(elf->section_header);
	free(elf);
	
}

void elf_64_cleanup(struct elf_64 *elf)
{
	fprintf(stdout, "freeing memory for elf_32 struct at 0x%p\n", elf);
	free(elf->file_header);
	free(elf->program_header);
	free(elf->section_header);
	free(elf);
	
}


int main(int argc, char **argv)
{
	if(argc < 2) return 1;
	
	FILE *ptr = NULL;
	if((ptr = fopen( argv[1], "r"))){
		uint8_t iself64 = is_elf_64(ptr);
		if(iself64 == 1){
			fprintf(stdout, "parsing elf_32...\n");
			struct elf_32 *buffer = NULL;
			if( (buffer = parse_elf_32(ptr)) ){
				display_file_header_elf_32(buffer);
				display_program_header_elf_32(buffer);
				display_section_header_elf_32(buffer);
				elf_32_cleanup(buffer);
				return 0;
			} else {
				fprintf(stderr, "parse_elf_32 failed\n");
				return 0;
			}
		} else if(iself64 == 2){
			fprintf(stdout, "parsing elf_64...\n");
			struct elf_64 *buffer = NULL;
			if( (buffer = parse_elf_64(ptr)) ){
				display_file_header_elf_64(buffer);
				display_program_header_elf_64(buffer);
				display_section_header_elf_64(buffer);
				elf_64_cleanup(buffer);
				return 0;
			} else { 
				fprintf(stderr, "parse_elf_64 failed\n");
				return 0;
			}
		} else {
			fprintf(stderr, "unknown EICLASS neither 32-bit/64-bit\n");
		}

	} else{
		fprintf(stderr, "error fopening file...\n");
		return 1;
	}
}
