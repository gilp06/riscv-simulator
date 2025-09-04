
#include <memory/memory_map.h>
#include <memory/elf_reader.h>

#include <riscv_core.h>
#include <chrono>

int main(void)
{
	InstructionSets::LoadInstructionSet("rv32i.json");

	ELFReader reader("test/c_test.elf");
	std::unique_ptr<MemoryMap> memory_map = reader.load();

	// for (auto &block : memory_map.get()->blocks)
	// {
	// 	std::cout << "0x" << std::hex << block->start_address << "-0x" << block->end_address << std::dec << std::endl;
	// }

	RISCVcore core(memory_map.get());
	core.pc = reader.get_entry_point();

	// std::vector<uint32_t> monitor_registers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
	// std::vector<uint32_t> monitor_registers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	std::vector<uint32_t> monitor_registers = { };
	bool monitor_pc = false;

	while (true)
	{
		core.step(monitor_registers, monitor_pc);

		uint8_t uart_out = core.peek(0x100F0000);

		if (uart_out != 0)
		{
			std::cout << uart_out;
			core.poke(0x100F0000, 0); // clear the output
		}
	}
	return 0;
}
