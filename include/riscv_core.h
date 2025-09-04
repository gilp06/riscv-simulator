#pragma once

#include <cstdint>
#include <memory>
#include <memory/memory_map.h>
#include <instruction_formatter.h>
#include <instruction_set.h>
#include <vector>

// class riscv_core
// {
//     uint32_t x[32];
//     uint32_t pc;
// };

class RISCVcore
{
private:
    uint32_t x_regs[32];

    MemoryMap *memory_map;

public:
    uint32_t pc;
    RISCVcore(MemoryMap *memory_map);
    ~RISCVcore() = default;

    InstructionVariant next_instruction();
    void execute_instruction(InstructionVariant instruction);
    void view_registers() const;
    std::string step(const std::vector<uint32_t> &monitor_registers, bool monitor_pc = true);
    uint8_t peek(uint32_t address);
    uint32_t peek_word(uint32_t address)
    {
        return memory_map->read_word(address);
    };
    void poke(uint32_t address, uint8_t value)
    {
        memory_map->write(address, value);
    };
};
