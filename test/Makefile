# Assemble and link the test program
test: data.s
	@echo "Assembling and linking test program..."
	@riscv64-unknown-elf-as -o data.o data.s -march rv32i
	@riscv64-unknown-elf-ld -T link.ld -m elf32lriscv -o test.elf data.o

c_test: c_test.c
	@echo "Compiling C test program..."
	# assemble the bios
	@riscv64-unknown-elf-as -o bios.o bios.s -march rv32i
	@riscv64-unknown-elf-gcc -o c_test.elf -march=rv32i -nostdlib -T link.ld -mabi=ilp32 c_test.c bios.o