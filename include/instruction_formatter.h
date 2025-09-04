#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>
#include <variant>

template <class... T>
struct overloaded : T...
{
	using T::operator()...;
};
template <class... T>
overloaded(T...) -> overloaded<T...>;

// INSTRUCTION TYPE HANDLER

enum InstructionFormat
{
	R_TYPE,
	I_TYPE,
	S_TYPE,
	B_TYPE,
	J_TYPE,
	U_TYPE,
	UNKNOWN_TYPE
};

struct UnknownInstruction
{
	uint32_t raw;
};

struct RTypeInstruction
{
	uint32_t raw;

	uint32_t opcode() const
	{
		return raw & 0x7F;
	}
	uint32_t rd() const
	{
		return (raw >> 7) & 0x1F;
	}
	uint32_t funct3() const
	{
		return (raw >> 12) & 0x7;
	}
	uint32_t rs1() const
	{
		return (raw >> 15) & 0x1F;
	}
	uint32_t rs2() const
	{
		return (raw >> 20) & 0x1F;
	}
	uint32_t funct7() const
	{
		return (raw >> 25) & 0x7F;
	}
};

struct ITypeInstruction
{
	uint32_t raw;

	uint32_t opcode() const
	{
		return raw & 0x7F;
	}
	uint32_t rd() const
	{
		return (raw >> 7) & 0x1F;
	}
	uint32_t funct3() const
	{
		return (raw >> 12) & 0x7;
	}
	uint32_t rs1() const
	{
		return (raw >> 15) & 0x1F;
	}
	uint32_t imm() const
	{
		return (raw >> 20);
	}
};

struct STypeInstruction
{
	uint32_t raw;

	uint32_t opcode() const
	{
		return raw & 0x7F;
	}
	uint32_t funct3() const
	{
		return (raw >> 12) & 0x7;
	}
	uint32_t rs1() const
	{
		return (raw >> 15) & 0x1F;
	}
	uint32_t rs2() const
	{
		return (raw >> 20) & 0x1F;
	}
	uint32_t imm() const
	{
		return ((raw >> 25) << 5) | ((raw >> 7) & 0x1F);
	}
};

struct BTypeInstruction
{
	uint32_t raw;

	uint32_t opcode() const
	{
		return raw & 0x7F;
	}
	uint32_t funct3() const
	{
		return (raw >> 12) & 0x7;
	}
	uint32_t rs1() const
	{
		return (raw >> 15) & 0x1F;
	}
	uint32_t rs2() const
	{
		return (raw >> 20) & 0x1F;
	}
	uint32_t imm() const
	{
		return (((raw >> 31) & 0x1) << 12) |
			   (((raw >> 7) & 0x1) << 11) |
			   (((raw >> 25) & 0x3F) << 5) |
			   (((raw >> 8) & 0xF) << 1);
	}
};

struct JTypeInstruction
{
	uint32_t raw;

	uint32_t opcode() const
	{
		return raw & 0x7F;
	}
	uint32_t rd() const
	{
		return (raw >> 7) & 0x1F;
	}
	uint32_t imm() const
	{
		int32_t imm = 0;
		imm |= ((raw >> 31) & 0x1) << 20;  // bit 20
		imm |= ((raw >> 21) & 0x3FF) << 1; // bits 10:1
		imm |= ((raw >> 20) & 0x1) << 11;  // bit 11
		imm |= ((raw >> 12) & 0xFF) << 12; // bits 19:12

		return imm;
	}
};

struct UTypeInstruction
{
	uint32_t raw;

	uint32_t opcode() const
	{
		return raw & 0x7F;
	}
	uint32_t rd() const
	{
		return (raw >> 7) & 0x1F;
	}
	uint32_t imm() const
	{
		return (raw >> 12);
	}
};

typedef std::variant<RTypeInstruction,
					 ITypeInstruction,
					 STypeInstruction,
					 BTypeInstruction,
					 JTypeInstruction,
					 UTypeInstruction,
					 UnknownInstruction>
	InstructionVariant;

InstructionFormat get_instruction_format(uint32_t instruction);

InstructionVariant parse_instruction(uint32_t raw);

void print_instruction(const InstructionVariant &inst);
