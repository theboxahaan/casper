BUILD_DIR = build
CL_FLAGS = -Wall

all: elf_parser_32

build_dir:
	if ! [ -d "$(BUILD_DIR)" ]; then mkdir -p $(BUILD_DIR); fi

elf_parser_32: build_dir elf_parser_32.c
	gcc $(CL_FLAGS) elf_parser_32.c -o $(BUILD_DIR)/elf_parser_32

clean:
	rm -rf $(BUILD_DIR)/*
