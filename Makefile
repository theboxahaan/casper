BUILD_DIR = build
CL_FLAGS = -Wall

all: elf_parser

build_dir:
	if ! [ -d "$(BUILD_DIR)" ]; then mkdir -p $(BUILD_DIR); fi

elf_parser: build_dir elf_parser.c
	gcc $(CL_FLAGS) elf_parser.c -o $(BUILD_DIR)/elf_parser

clean:
	rm -rf $(BUILD_DIR)/*
