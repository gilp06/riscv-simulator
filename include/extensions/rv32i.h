#pragma once

#include <string>
#include <unordered_map>
#include <instruction_formatter.h>
#include <memory/memory_map.h>
namespace RV32I
{
    class RV32I_IS
    {
    public:
        static void Load();
        static void AddInstruction(std::string name, uint32_t key);
        static uint32_t MakeKey(uint32_t opcode, uint32_t func3 = 0, uint32_t func7 = 0);
        // static void LookupInstruction(I
        static std::unordered_map<uint32_t, std::string> instruction_lookup;
    };

    void Execute(InstructionVariant instruction, uint32_t *x_regs, MemoryMap *memory_map, uint32_t *pc);
    void PerformRTypeInstruction(const RTypeInstruction &instr, uint32_t *x_regs);
    void PerformITypeInstruction(const ITypeInstruction &instr, uint32_t *x_regs, MemoryMap *memory_map, uint32_t *pc = nullptr);
    void PerformUTypeInstruction(const UTypeInstruction &instr, uint32_t *x_regs, uint32_t *pc = nullptr);
    void PerformJTypeInstruction(const JTypeInstruction &instr, uint32_t *x_regs, uint32_t *pc = nullptr);
    void PerformSTypeInstruction(const STypeInstruction &instr, uint32_t *x_regs, MemoryMap *memory_map);
    void PerformBTypeInstruction(const BTypeInstruction &instr, uint32_t *x_regs, uint32_t *pc = nullptr);
    
}