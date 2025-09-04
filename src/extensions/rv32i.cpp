#include <extensions/rv32i.h>
#include <instruction_set.h>
#include <variant>
#include <iostream>

using namespace RV32I;

std::unordered_map<uint32_t, std::string> RV32I_IS::instruction_lookup{};

void RV32I_IS::Load()
{
    instruction_lookup[MakeKey(0b0010011, 0b000)] = "addi";
    instruction_lookup[MakeKey(0b0010011, 0b010)] = "slti";
    instruction_lookup[MakeKey(0b0010011, 0b011)] = "sltiu";
    instruction_lookup[MakeKey(0b0010011, 0b111)] = "andi";
    instruction_lookup[MakeKey(0b0010011, 0b110)] = "ori";
    instruction_lookup[MakeKey(0b0010011, 0b100)] = "xori";
    instruction_lookup[MakeKey(0b0010011, 0b001)] = "slli";

    instruction_lookup[MakeKey(0b0010011, 0b101, 0b0000000)] = "srli";
    instruction_lookup[MakeKey(0b0010011, 0b101, 0b0100000)] = "srai";

    instruction_lookup[MakeKey(0b0110111)] = "lui";
    instruction_lookup[MakeKey(0b0010111)] = "auipc";

    instruction_lookup[MakeKey(0b0110011, 0b000, 0b0000000)] = "add";
    instruction_lookup[MakeKey(0b0110011, 0b000, 0b0100000)] = "sub";
    instruction_lookup[MakeKey(0b0110011, 0b010, 0b0000000)] = "slt";
    instruction_lookup[MakeKey(0b0110011, 0b011, 0b0000000)] = "sltu";
    instruction_lookup[MakeKey(0b0110011, 0b111, 0b0000000)] = "and";
    instruction_lookup[MakeKey(0b0110011, 0b110, 0b0000000)] = "or";
    instruction_lookup[MakeKey(0b0110011, 0b100, 0b0000000)] = "xor";
    instruction_lookup[MakeKey(0b0110011, 0b001, 0b0000000)] = "sll";
    instruction_lookup[MakeKey(0b0110011, 0b101, 0b0000000)] = "srl";
    instruction_lookup[MakeKey(0b0110011, 0b101, 0b0100000)] = "sra";

    instruction_lookup[MakeKey(0b1101111)] = "jal";
    instruction_lookup[MakeKey(0b1100111, 0b000)] = "jalr";
    instruction_lookup[MakeKey(0b1100011, 0b000)] = "beq";
    instruction_lookup[MakeKey(0b1100011, 0b001)] = "bne";
    instruction_lookup[MakeKey(0b1100011, 0b100)] = "blt";
    instruction_lookup[MakeKey(0b1100011, 0b110)] = "bltu";
    instruction_lookup[MakeKey(0b1100011, 0b101)] = "bge";
    instruction_lookup[MakeKey(0b1100011, 0b111)] = "bgeu";

    instruction_lookup[MakeKey(0b0000011, 0b010)] = "lw";
    instruction_lookup[MakeKey(0b0000011, 0b001)] = "lh";
    instruction_lookup[MakeKey(0b0000011, 0b000)] = "lb";
    instruction_lookup[MakeKey(0b0000011, 0b100)] = "lbu";
    instruction_lookup[MakeKey(0b0000011, 0b101)] = "lhu";

    instruction_lookup[MakeKey(0b0100011, 0b010)] = "sw";
    instruction_lookup[MakeKey(0b0100011, 0b001)] = "sh";
    instruction_lookup[MakeKey(0b0100011, 0b000)] = "sb";

    instruction_lookup[MakeKey(0b0001111, 0b000)] = "fence";

    std::cout << "Loaded RV32I Instructions" << std::endl;
}

void RV32I_IS::AddInstruction(std::string name, uint32_t key) {}

uint32_t RV32I_IS::MakeKey(uint32_t opcode, uint32_t func3, uint32_t func7)
{
    return opcode | (func3 << 7) | (func7 << 10);
}
void RV32I::Execute(InstructionVariant instruction, uint32_t *x_regs, MemoryMap *memory_map, uint32_t *pc)
{
    // generate empty for each instruction variant
    std::visit(overloaded{[&](const RTypeInstruction &instr)
                          {
                              PerformRTypeInstruction(instr, x_regs);
                              return;
                          },
                          [&](const ITypeInstruction &instr)
                          {
                              PerformITypeInstruction(instr, x_regs, memory_map, pc);
                              return;
                          },
                          [&](const STypeInstruction &instr)
                          {
                              PerformSTypeInstruction(instr, x_regs, memory_map);
                              return;
                          },
                          [&](const BTypeInstruction &instr)
                          {
                              PerformBTypeInstruction(instr, x_regs, pc);
                              return;
                          },
                          [&](const JTypeInstruction &instr)
                          {
                              PerformJTypeInstruction(instr, x_regs, pc);
                              return;
                          },
                          [&](const UTypeInstruction &instr)
                          {
                              PerformUTypeInstruction(instr, x_regs, pc);
                              return;
                          },
                          [&](const UnknownInstruction &instr)
                          {
                              std::cout << "Unknown instruction: " << instr.raw << std::endl;
                              std::cout << "At PC: 0x" << std::hex << *pc << std::dec << std::endl;
                              return;
                          }},
               instruction);
}

void RV32I::PerformRTypeInstruction(const RTypeInstruction &instr, uint32_t *x_regs)
{

    uint32_t rd = instr.rd();
    uint32_t rs1 = instr.rs1();
    uint32_t rs2 = instr.rs2();
    uint32_t funct3 = instr.funct3();
    switch (funct3)
    {
    case 0b000:
        if (instr.funct7() == 0b0000000) // ADD
        {
            x_regs[rd] = x_regs[rs1] + x_regs[rs2];
        }
        else if (instr.funct7() == 0b0100000) // SUB
        {
            x_regs[rd] = x_regs[rs1] - x_regs[rs2];
        }
        break;
    case 0b001:
        if (instr.funct7() == 0b0000000) // SLL
        {
            x_regs[rd] = x_regs[rs1] << (x_regs[rs2] & 0x1F);
        }
        break;
    case 0b010:
        if (instr.funct7() == 0b0000000) // SLT
        {
            x_regs[rd] = (int32_t)x_regs[rs1] < (int32_t)x_regs[rs2];
        }
        break;
    case 0b011:
        if (instr.funct7() == 0b0000000) // SLTU
        {
            x_regs[rd] = x_regs[rs1] < x_regs[rs2];
        }
        break;
    case 0b100:
        if (instr.funct7() == 0b0000000) // XOR
        {
            x_regs[rd] = x_regs[rs1] ^ x_regs[rs2];
        }
        break;
    case 0b101:
        if (instr.funct7() == 0b0000000) // SRL
        {
            x_regs[rd] = x_regs[rs1] >> (x_regs[rs2] & 0x1F);
        }
        else if (instr.funct7() == 0b0100000) // SRA
        {
            x_regs[rd] = (int32_t)x_regs[rs1] >> (x_regs[rs2] & 0x1F);
        }
        break;
    case 0b110:
        if (instr.funct7() == 0b0000000) // OR
        {
            x_regs[rd] = x_regs[rs1] | x_regs[rs2];
        }
        break;
    case 0b111:
        if (instr.funct7() == 0b0000000) // AND
        {
            x_regs[rd] = x_regs[rs1] & x_regs[rs2];
        }
        break;
    default:
        // instruction may be part of a different instruction set
        return;
    }
}

void RV32I::PerformITypeInstruction(const ITypeInstruction &instr, uint32_t *x_regs, MemoryMap *memory_map, uint32_t *pc)
{
    uint32_t rd = instr.rd();
    uint32_t rs1 = instr.rs1();
    uint32_t funct3 = instr.funct3();
    int32_t imm = instr.imm();

    // sign extend the immediate value
    if (imm & 0x800)
    {
        imm |= 0xFFFFF000;
    }

    // std::cout << imm << std::endl;

    // // std::cout << "Instruction: " << InstructionSets::GetInstructionName(instr) << std::endl;
    // // std::cout << "Opcode: " << std::bitset<7>(instr.opcode()) << std::endl;
    // std::cout << "RD: " << std::bitset<5>(rd) << std::endl;
    // std::cout << "RS1: " << std::bitset<5>(rs1) << std::endl;
    // std::cout << "Immediate value: " << std::bitset<12>(imm) << std::endl;

    switch (instr.opcode())
    {
    case 0b0000011: // Load instructions
        switch (funct3)
        {
        case 0b010: // LW
            x_regs[rd] = memory_map->read_word(x_regs[rs1] + imm);
            break;
        case 0b001: // LH
            x_regs[rd] = memory_map->read(x_regs[rs1] + imm) | (memory_map->read(x_regs[rs1] + imm + 1) << 8);
            if (x_regs[rd] & 0x8000) // sign extend
            {
                x_regs[rd] |= 0xFFFF0000;
            }
            break;
        case 0b000: // LB
            x_regs[rd] = memory_map->read(x_regs[rs1] + imm);
            if (x_regs[rd] & 0x80) // sign extend
            {
                x_regs[rd] |= 0xFFFFFF00;
            }
            break;
        case 0b100: // LBU
            // std::cout << std::hex << x_regs[rs1] + imm << std::dec << std::endl;
            x_regs[rd] = memory_map->read(x_regs[rs1] + imm);
            // std::cout << (char)x_regs[rd] << std::endl; // print the character
            break;
        case 0b101: // LHU
            x_regs[rd] = memory_map->read(x_regs[rs1] + imm) | (memory_map->read(x_regs[rs1] + imm + 1) << 8);
            break;
        default:
            return; // instruction may be part of a different instruction set
        }
        break;
    case 0b0010011:
        switch (funct3)
        {
        case 0b000: // ADDI
            x_regs[rd] = x_regs[rs1] + imm;
            break;
        case 0b010: // SLTI
            x_regs[rd] = (int32_t)x_regs[rs1] < imm;
            break;
        case 0b011: // SLTIU
            x_regs[rd] = x_regs[rs1] < (uint32_t)imm;
            break;
        case 0b100: // XORI
            x_regs[rd] = x_regs[rs1] ^ imm;
            break;
        case 0b110: // ORI
            x_regs[rd] = x_regs[rs1] | imm;
            break;
        case 0b111: // ANDI
            x_regs[rd] = x_regs[rs1] & imm;
            break;
        case 0b001: // SLLI
            x_regs[rd] = x_regs[rs1] << (imm & 0x1F);
            break;
        case 0b101:
            // TODO FIX THESE
            std::cout << "SHIFT" << std::endl;
            if (imm & 0x20) // SRLI
                x_regs[rd] = x_regs[rs1] >> (imm & 0x1F);
            else // SRAI
                x_regs[rd] = (int32_t)x_regs[rs1] >> (imm & 0x1F);
            break;
        default:
            return; // instruction may be part of a different instruction set
        }
        break;
    case 0b1100111:          // JALR
        if (funct3 == 0b000) // JALR
        {
            x_regs[rd] = *pc + 4;           // store return address
            *pc = ((x_regs[rs1] + imm) & ~1) - 4; // align to even address
            // std::cout << "JALR to address: " << std::hex << *pc << std::dec << std::endl;
        }
        else
        {
            return; // instruction may be part of a different instruction set
        }
        break;
    default:
        return; // instruction may be part of a different instruction set
    }
}

void RV32I::PerformUTypeInstruction(const UTypeInstruction &instr, uint32_t *x_regs, uint32_t *pc)
{
    uint32_t rd = instr.rd();
    uint32_t imm = instr.imm() << 12; // shift left by 12 bits to get the full immediate value

    // std::cout << "UTypeInstruction: " << InstructionSets::GetInstructionName(instr) << std::endl;
    // std::cout << "RD: " << std::bitset<5>(rd) << std::endl;
    // std::cout << "Immediate value: " << std::bitset<20>(imm) << std::endl;

    // sign extend the immediate value
    if (imm & 0x80000000)
    {
        imm |= 0xFFF00000;
    }

    switch (instr.opcode())
    {
    case 0b0110111: // LUI
        x_regs[rd] = imm;
        break;
    case 0b0010111: // AUIPC
        x_regs[rd] = *pc + imm;
        break;
    default:
        return; // instruction may be part of a different instruction set
    }
}

void RV32I::PerformJTypeInstruction(const JTypeInstruction &instr, uint32_t *x_regs, uint32_t *pc)
{
    uint32_t rd = instr.rd();
    int32_t imm = instr.imm();

    // std::cout << "JTypeInstruction: " << InstructionSets::GetInstructionName(instr) << std::endl;
    // std::cout << "RD: " << std::bitset<5>(rd) << std::endl;
    // std::cout << "Immediate value: " << std::bitset<20>(imm) << std::endl;

    // sign extend the immediate value
    if (imm & 0x80000)
    {
        imm |= 0xFFF00000;
    }

    // std::cout << "Immediate value: " << std::bitset<20>(imm) << std::endl;

    switch (instr.opcode())
    {
    case 0b1101111:           // JAL
        x_regs[rd] = *pc + 4; // store return address
        *pc += imm - 4;       // revert increment in next_instruction
        // std::cout << std::hex << *pc + 4 << std::dec << std::endl;
        break;
    default:
        return; // instruction may be part of a different instruction set
    }
}

void RV32I::PerformSTypeInstruction(const STypeInstruction &instr, uint32_t *x_regs, MemoryMap *memory_map)
{
    uint32_t rs1 = instr.rs1();
    uint32_t rs2 = instr.rs2();
    uint32_t funct3 = instr.funct3();
    int32_t imm = instr.imm();

    // std::cout << "STypeInstruction: " << InstructionSets::GetInstructionName(instr) << std::endl;
    // std::cout << "RS1: " << std::bitset<5>(rs1) << std::endl;
    // std::cout << "RS2: " << std::bitset<5>(rs2) << std::endl;
    // std::cout << "Immediate value: " << std::bitset<12>(imm) << std::endl;
    // std::cout << "Funct3: " << std::bitset<3>(funct3) << std::endl;

    // sign extend the immediate value
    if (imm & 0x800)
    {
        imm |= 0xFFFFF000;
    }

    switch (funct3)
    {
    case 0b010: // SW
        // std::cout << "Writing word to address: " << std::hex << (x_regs[rs1] + imm) << std::dec << std::endl;
        // std::cout << "From register x" << rs2 << ": " << std::hex << x_regs[rs2] << std::dec << std::endl;
        memory_map->write_word(x_regs[rs1] + imm, x_regs[rs2]);
        break;
    case 0b001: // SH
        memory_map->write(x_regs[rs1] + imm, x_regs[rs2] & 0xFF);
        memory_map->write(x_regs[rs1] + imm + 1, (x_regs[rs2] >> 8) & 0xFF);
        break;
    case 0b000: // SB
        memory_map->write(x_regs[rs1] + imm, x_regs[rs2] & 0xFF);
        break;
    default:
        return; // instruction may be part of a different instruction set
    }
}

void RV32I::PerformBTypeInstruction(const BTypeInstruction &instr, uint32_t *x_regs, uint32_t *pc)
{
    uint32_t rs1 = instr.rs1();
    uint32_t rs2 = instr.rs2();
    uint32_t funct3 = instr.funct3();
    int32_t imm = instr.imm();

    // sign extend the immediate value
    if (imm & 0x800)
    {
        imm |= 0xFFFFF000;
    }

    if (instr.opcode() == 0b1100011)
    {
        switch (funct3)
        {
        case 0b000: // BEQ
            if (x_regs[rs1] == x_regs[rs2])
            {
                // std::cout << "Tried branch!" << std::endl;
                // std::cout << std::hex << imm << std::dec << std::endl;
                *pc += imm - 4; // revert increment in next_instruction
            }
            break;
        case 0b001: // BNE
            if (x_regs[rs1] != x_regs[rs2])
            {
                *pc += imm - 4; // revert increment in next_instruction
            }
            break;
        case 0b100: // BLT
            if ((int32_t)x_regs[rs1] < (int32_t)x_regs[rs2])
            {
                *pc += imm - 4; // revert increment in next_instruction
            }
            break;
        case 0b101: // BGE
            if ((int32_t)x_regs[rs1] >= (int32_t)x_regs[rs2])
            {
                *pc += imm - 4; // revert increment in next_instruction
            }
            break;
        case 0b110: // BLTU
            if (x_regs[rs1] < x_regs[rs2])
            {
                *pc += imm - 4; // revert increment in next_instruction
            }
            break;
        case 0b111: // BGEU
            if (x_regs[rs1] >= x_regs[rs2])
            {
                *pc += imm - 4; // revert increment in next_instruction
            }
            break;
        default:
            break; // instruction may be part of a different instruction set
        }
    }
    else
    {
        return; // instruction may be part of a different instruction set
    }
}
