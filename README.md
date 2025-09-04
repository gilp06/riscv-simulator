# Notes

Very basic RV32I unprivileged simulator. Will execute elf files produced by the riscv-unknown-elf toolchain. No standard library is provided but there is currently a very hacky print function made by monitoring a memory location.

The main goal for this project is to eventually convert it into a power simulator for the runtime of a program, hopefully with pipeline support as well; however right now there is not much functionality. I wanted to make it modular with the extensions being loaded at runtime, however this got pretty finicky where I would need to provide a C implementation of the operations and then also a json file providing the human readable names.

The simulator will take in an elf file and from that create virtual RAM and ROM blocks that will be merged into one interface (see memory_map.h). If you look in main you can see the main pipeline is the elf reader will produce the memory map then used by the processor. The program will then step along as a processor would; fetching, decoding and performing the operation. 
