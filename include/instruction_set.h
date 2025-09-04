#pragma once
#include <fstream>
#include <nlohmann/json.hpp>
#include <map>
using json = nlohmann::json;

enum InstructionSet
{
	INVALID = (1u << 0),
	IS_RV32I = (1u << 1),
};

NLOHMANN_JSON_SERIALIZE_ENUM(InstructionSet, {{IS_RV32I, "RV32I"}, {INVALID, nullptr}});

typedef uint32_t LoadedInstructionSetMask;
class InstructionSets
{
public:
	static LoadedInstructionSetMask loaded_instructions;
	static void LoadInstructionSet(const std::string& path);
	static uint32_t MakeKey(uint8_t opcode, uint8_t func3 = 0, uint8_t func7 = 0);
	static std::string GetInstructionName(InstructionVariant instruction);
    // static std::map<InstructionSet, json> is_tables;
};

