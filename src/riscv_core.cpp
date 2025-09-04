#include <cassert>
#include <riscv_core.h>
#include <extensions/rv32i.h>

RISCVcore::RISCVcore(MemoryMap* memory_map)
	: memory_map(memory_map)
{
	for(int i = 0; i < 32; ++i)
	{
		x_regs[i] = 0;
	}
	
	pc = 0;
}

InstructionVariant RISCVcore::next_instruction()
{
	uint32_t instr = memory_map->read_word(pc);
	return parse_instruction(instr);
}

void RISCVcore::execute_instruction(InstructionVariant instruction)
{
	x_regs[0] = 0;
			  

	if((InstructionSets::loaded_instructions & IS_RV32I) != 0)
	{
		RV32I::Execute(instruction, x_regs, memory_map, &pc);
	}
	pc += 4;

	x_regs[0] = 0;

	return;
}

void RISCVcore::view_registers() const
{
	// display the contents of the registers
	std::cout << "Registers:" << std::endl;
	for(int i = 0; i < 32; ++i)
	{
		std::cout << "x" << i << ": 0x" << std::hex << x_regs[i] << std::dec << std::endl;
	}
	std::cout << "PC: 0x" << std::hex << pc << std::dec << std::endl;
}

std::string RISCVcore::step(const std::vector<uint32_t> &monitor_registers, bool monitor_pc)
{
	InstructionVariant instr = next_instruction();
	// std::cout << InstructionSets::GetInstructionName(instr) << std::endl;

	execute_instruction(instr);
	// display the contents of the registers
	if(monitor_pc)
	{
		std::cout << "PC: 0x" << std::hex << pc << std::dec << std::endl;
	}
	for(uint32_t reg : monitor_registers)
	{
		assert(reg < 32);
		std::cout << "x" << reg << ": 0x" << std::hex << x_regs[reg] << std::dec << std::endl;
	}

	return InstructionSets::GetInstructionName(instr);
}

uint8_t RISCVcore::peek(uint32_t address)
{
	return memory_map->read(address);
}
