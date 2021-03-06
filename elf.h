#ifndef ELF_H
#define ELF_H

enum e_ident_offsets{
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

struct elf_32_file_header{
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

struct elf_64_file_header{
	/* file header */
	uint8_t e_ident[16];	// identification bytes
	uint16_t e_type;		// identify object file type
	uint16_t e_machine;		// specify the target ISA
	uint32_t e_version;		// set to 1 for original ELF
	uint64_t e_entry;		// mem-address of entrypoint for process
	uint64_t e_phoff;		// points to the start of the program header table
	uint64_t e_shoff;		// points to the start of the section header table
	uint32_t e_flags;		// interpretation depends on architecture
	uint16_t e_ehsize;		// size of the header
	uint16_t e_phentsize;	// size of program header table entry
	uint16_t e_phnum;		// number of entries in program header table
	uint16_t e_shentsize;	// size of a section header table entry
	uint16_t e_shnum;		// number of entries in the section header table
	uint16_t e_shstrndx;	// index of the section header table entry that contains section names
};


struct elf_32_program_header_entry{
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

struct elf_64_program_header_entry{
	/* program header */
	uint32_t p_type;		// identifies the type of segment
	uint32_t p_flags;		// segment dependent flags (64-bit arch.)
	uint64_t p_offset;		// offset of segment in the file image
	uint64_t p_vaddr;		// virtual address of segment in memory
	uint64_t p_paddr;		// reserved for physical address of segment
	uint64_t p_filesz;		// size in bytes of the segment in file image
	uint64_t p_memsz;		// size in bytes of the segment in memory
	uint64_t p_align;		// look up docus

};


struct elf_32_section_header_entry{
	/* section header */
	uint32_t sh_name;		// offset to name string in .shstrtab section
	uint32_t sh_type;		// identifies type of the header
	uint32_t sh_flags;		// identifies attributes of the section
	uint32_t sh_addr;		// virtual addr of section in mem (loaded)
	uint32_t sh_offset;		// offset of the section in file image
	uint32_t sh_size;		// size of section in file image
	uint32_t sh_link;		// section index of assoc section
	uint32_t sh_info;		// extra info about the section 
	uint32_t sh_addralign;	// required alignment for the section
	uint32_t sh_entsize;	// size of each entry for fixed-length entries; else 0
};

struct elf_64_section_header_entry{
	/* section header */
	uint32_t sh_name;		// offset to name string in .shstrtab section
	uint32_t sh_type;		// identifies type of the header
	uint64_t sh_flags;		// identifies attributes of the section
	uint64_t sh_addr;		// virtual addr of section in mem (loaded)
	uint64_t sh_offset;		// offset of the section in file image
	uint64_t sh_size;		// size of section in file image
	uint32_t sh_link;		// section index of assoc section
	uint32_t sh_info;		// extra info about the section 
	uint64_t sh_addralign;	// required alignment for the section
	uint64_t sh_entsize;	// size of each entry for fixed-length entries; else 0
};


struct elf_32{
	struct elf_32_file_header *file_header;				// pointer to file_header struct
	struct elf_32_program_header_entry *program_header;	// array of program_header_entry structs
	struct elf_32_section_header_entry *section_header;	// array of section_header_entry structs
};

struct elf_64{
	struct elf_64_file_header *file_header;				// pointer to file_header struct
	struct elf_64_program_header_entry *program_header;	// array of program_header_entry structs
	struct elf_64_section_header_entry *section_header;	// array of section_header_entry structs
};

uint8_t is_elf_64(FILE *);
uint8_t is_little_endian(FILE *);

struct elf_32 *parse_elf_32(FILE *);
void display_file_header_elf_32(struct elf_32 *);
void display_program_header_elf_32(struct elf_32 *);
void display_section_header_elf_32(struct elf_32 *);
void elf_32_cleanup(struct elf_32 *);

struct elf_64 *parse_elf_64(FILE *);
void display_file_header_elf_64(struct elf_64 *);
void display_program_header_elf_64(struct elf_64 *);
void display_section_header_elf_64(struct elf_64 *);
void elf_64_cleanup(struct elf_64 *);

#endif
