
#include <extensions/rv32i.h>
#include <instruction_set.h>
#include <iostream>

LoadedInstructionSetMask InstructionSets::loaded_instructions = 0;
// std::map<InstructionSet, json> InstructionSets::is_tables{};

void InstructionSets::LoadInstructionSet(const std::string& path)
{
	std::ifstream i(path);
	json j = json::parse(i);
	InstructionSet is = j["instruction_set"].template get<InstructionSet>();
	loaded_instructions |= is;
	// is_tables[is] = j;
	// // std::cout << "Loaded Instruction Set: " << j["instruction_set"].template get<std::string>()
	// 		  << std::endl;
	if(is == IS_RV32I)
	{
		RV32I::RV32I_IS::Load();
	}
	else
	{
		std::cerr << "Unsupported instruction set: "
				  << j["instruction_set"].template get<std::string>() << std::endl;
	}
}

std::string InstructionSets::GetInstructionName(InstructionVariant instruction)
{
	// look through all currently loaded instruction sets
	if(loaded_instructions & IS_RV32I)
	{
		return std::visit(
			overloaded{
				[](const RTypeInstruction& i) {
					return std::string(RV32I::RV32I_IS::instruction_lookup[RV32I::RV32I_IS::MakeKey(
						i.opcode(), i.funct3(), i.funct7())]);
				},
				[](const ITypeInstruction& i) {
					return std::string(
						RV32I::RV32I_IS::instruction_lookup[RV32I::RV32I_IS::MakeKey(i.opcode(), i.funct3())]);
				},
				[](const STypeInstruction& i) {
					return std::string(
						RV32I::RV32I_IS::instruction_lookup[RV32I::RV32I_IS::MakeKey(i.opcode(), i.funct3())]);
				},
				[](const BTypeInstruction& i) {
					return std::string(
						RV32I::RV32I_IS::instruction_lookup[RV32I::RV32I_IS::MakeKey(i.opcode(), i.funct3())]);
				},
				[](const JTypeInstruction& i) {
					return std::string(RV32I::RV32I_IS::instruction_lookup[RV32I::RV32I_IS::MakeKey(i.opcode())]);
				},
				[](const UTypeInstruction& i) {
					return std::string(RV32I::RV32I_IS::instruction_lookup[RV32I::RV32I_IS::MakeKey(i.opcode())]);
				},
				[](const UnknownInstruction&) { return std::string("UNKNOWN"); }},
			instruction);
	}
	else
	{
		return "UNKNOWN";
	}
}
