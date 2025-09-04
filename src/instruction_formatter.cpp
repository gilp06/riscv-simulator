#include <instruction_formatter.h>
#include <fstream>

InstructionVariant parse_instruction(uint32_t raw)
{
	InstructionFormat format = get_instruction_format(raw);
	switch(format)
	{
	case R_TYPE:
		return RTypeInstruction{raw};
	case I_TYPE:
		return ITypeInstruction{raw};
	case S_TYPE:
		return STypeInstruction{raw};
	case B_TYPE:
		return BTypeInstruction{raw};
	case J_TYPE:
		return JTypeInstruction{raw};
	case U_TYPE:
		return UTypeInstruction{raw};
	default:
		return UnknownInstruction{raw};
	}
};

InstructionFormat get_instruction_format(uint32_t instruction)
{
	// first parse opcode then determine the instruction type
	uint32_t opcode = instruction & 0x7F;
	InstructionFormat format;
	if(opcode == 0b0110011)
	{
		return R_TYPE;
	}
	else if(opcode == 0b0000011 || opcode == 0b0010011 || opcode == 0b1100111)
	{
		return I_TYPE;
	}
	else if(opcode == 0b0100011)
	{
		return S_TYPE;
	}
	else if(opcode == 0b1100011)
	{
		return B_TYPE;
	}
	else if(opcode == 0b1101111)
	{
		return J_TYPE;
	}
	else if(opcode == 0b0110111 || opcode == 0b0010111)
	{
		return U_TYPE;
	}
	else
	{
		return UNKNOWN_TYPE;
	}
};

void print_instruction(const InstructionVariant& inst)
{
	std::visit(overloaded{[](const RTypeInstruction& i) {
							  std::cout << "R-Type:\n"
										<< "  opcode: " << std::bitset<7>(i.opcode()) << "\n"
										<< "  rd:     " << std::bitset<5>(i.rd()) << "\n"
										<< "  funct3: " << std::bitset<3>(i.funct3()) << "\n"
										<< "  rs1:    " << std::bitset<5>(i.rs1()) << "\n"
										<< "  rs2:    " << std::bitset<5>(i.rs2()) << "\n"
										<< "  funct7: " << std::bitset<7>(i.funct7()) << "\n";
						  },
						  [](const ITypeInstruction& i) {
							  std::cout << "I-Type:\n"
										<< "  opcode: " << std::bitset<7>(i.opcode()) << "\n"
										<< "  rd:     " << std::bitset<5>(i.rd()) << "\n"
										<< "  funct3: " << std::bitset<3>(i.funct3()) << "\n"
										<< "  rs1:    " << std::bitset<5>(i.rs1()) << "\n"
										<< "  imm:    " << std::bitset<12>(i.imm()) << "\n";
						  },
						  [](const STypeInstruction& i) {
							  std::cout << "S-Type:\n"
										<< "  opcode: " << std::bitset<7>(i.opcode()) << "\n"
										<< "  funct3: " << std::bitset<3>(i.funct3()) << "\n"
										<< "  rs1:    " << std::bitset<5>(i.rs1()) << "\n"
										<< "  rs2:    " << std::bitset<5>(i.rs2()) << "\n"
										<< "  imm:    " << std::bitset<12>(i.imm()) << "\n";
						  },
						  [](const BTypeInstruction& i) {
							  std::cout << "B-Type:\n"
										<< "  opcode: " << std::bitset<7>(i.opcode()) << "\n"
										<< "  funct3: " << std::bitset<3>(i.funct3()) << "\n"
										<< "  rs1:    " << std::bitset<5>(i.rs1()) << "\n"
										<< "  rs2:    " << std::bitset<5>(i.rs2()) << "\n"
										<< "  imm:    " << std::bitset<13>(i.imm())
										<< "\n"; // B-type has a 13-bit immediate when assembled
						  },
						  [](const JTypeInstruction& i) {
							  std::cout << "J-Type:\n"
										<< "  opcode: " << std::bitset<7>(i.opcode()) << "\n"
										<< "  rd:     " << std::bitset<5>(i.rd()) << "\n"
										<< "  imm:    " << std::bitset<21>(i.imm()) << "\n";
						  },
						  [](const UTypeInstruction& i) {
							  std::cout << "U-Type:\n"
										<< "  opcode: " << std::bitset<7>(i.opcode()) << "\n"
										<< "  rd:     " << std::bitset<5>(i.rd()) << "\n"
										<< "  imm:    " << std::bitset<20>(i.imm()) << "\n";
						  },
						  [](const UnknownInstruction& i) {
							  std::cout << "Unknown Type:\n"
										<< " bits: " << std::bitset<32>(i.raw) << "\n";
						  }},
			   inst);
}