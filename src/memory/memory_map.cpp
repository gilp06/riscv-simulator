#include <cstdint>
#include <iostream>
#include <memory/memory_map.h>

void RAMBlock::reset()
{
	for (size_t i = 0; i < end_address - start_address; i++)
	{
		data[i] = 0;
	}
}

bool RAMBlock::write(uint32_t address, uint8_t value)
{
	if (!contains(address))
		return false;
	data[address - start_address] = value;
	return true;
}

uint8_t RAMBlock::read(uint32_t address) const
{
	if (!contains(address))
	{
		std::cout << "Tried reading at 0x" << std::hex << address << std::dec << std::endl;
		throw std::out_of_range("Address out of range");
	}
		
	return data[address - start_address];
}

void ROMBlock::reset()
{
	for (size_t i = 0; i < end_address - start_address; i++)
	{
		data[i] = 0;
	}
}

bool ROMBlock::write(uint32_t address, uint8_t value)
{
	std::cout << "Attempted to write to read-only memory at address 0x" << std::hex << address
			  << std::dec << std::endl;
	return false;
}

uint8_t ROMBlock::read(uint32_t address) const
{
	if (!contains(address))
	{
		std::cout << "Tried reading at 0x" << std::hex << address << std::dec << std::endl;
		throw std::out_of_range("Address out of range");
	}
	return data[address - start_address];
}

void ROMBlock::rom_write(uint32_t address, uint8_t value)
{
	if (!contains(address))
	{
		std::cout << "Tried writing rom at 0x" << std::hex << address << std::dec << std::endl;
		throw std::out_of_range("Address out of range");
	}
	data[address - start_address] = value;
}

RAMBlock::RAMBlock(uint32_t start_address, uint32_t end_address)
	: MemoryBlock(start_address, end_address)
{
	data.resize(end_address - start_address, 0);
}

bool RAMBlock::write_word(uint32_t base_address, uint32_t value)
{
	if (!contains(base_address) || !contains(base_address + 3))
		return false;
	// Little endian: least significant byte at lowest address
	data[base_address - start_address] = value & 0xFF;
	data[base_address + 1 - start_address] = (value >> 8) & 0xFF;
	data[base_address + 2 - start_address] = (value >> 16) & 0xFF;
	data[base_address + 3 - start_address] = (value >> 24) & 0xFF;
	return true;
}

uint32_t RAMBlock::read_word(uint32_t base_address) const
{
	if (!contains(base_address) || !contains(base_address + 3))
	{
		std::cout << "Tried reading word in RAM at 0x" << std::hex << base_address << std::dec << std::endl;
		throw std::out_of_range("Address out of range");
	}
		
	// Little endian: least significant byte at lowest address
	return (data[base_address - start_address]) |
		   (data[base_address + 1 - start_address] << 8) |
		   (data[base_address + 2 - start_address] << 16) |
		   (data[base_address + 3 - start_address] << 24);
}

ROMBlock::ROMBlock(uint32_t start_address, uint32_t end_address)
	: MemoryBlock(start_address, end_address)
{
	data.resize(end_address - start_address, 0);
}

bool ROMBlock::write_word(uint32_t base_address, uint32_t value)
{
	return false; // ROM is read-only
}

uint32_t ROMBlock::read_word(uint32_t base_address) const
{
	if (!contains(base_address) || !contains(base_address + 3))
	{
		std::cout << "Tried reading word in ROM at 0x" << std::hex << base_address << std::dec << std::endl;
		throw std::out_of_range("Address out of range");
	}
		
	// Little endian: least significant byte at lowest address
	return (data[base_address - start_address]) |
		   (data[base_address + 1 - start_address] << 8) |
		   (data[base_address + 2 - start_address] << 16) |
		   (data[base_address + 3 - start_address] << 24);
}

void MemoryMap::add_block(std::unique_ptr<MemoryBlock> new_block)
{
	for (const auto &block : blocks)
	{
		// Two blocks [a,b) and [c,d) overlap if a < d && c < b
		if (new_block->start_address < block->end_address &&
			block->start_address < new_block->end_address)
		{
			throw std::runtime_error("Memory block overlaps with existing block");
		}
	}

	blocks.push_back(std::move(new_block));
}

MemoryBlock *MemoryMap::find_block(uint32_t address) const
{
	for (auto &&i : blocks)
	{
		if (i->contains(address))
		{
			return i.get();
		}
	}
	return nullptr;
}

uint8_t MemoryMap::read(uint32_t address) const
{
	MemoryBlock *block = find_block(address);
	if (block == nullptr)
	{
		std::cout << "Attempted read at unknown memory location 0x" << std::hex << address
				  << std::dec << std::endl;
		return 0xFF;
	}

	return block->read(address);
}

uint32_t MemoryMap::read_word(uint32_t address) const
{
	MemoryBlock *block = find_block(address);
	if (!block)
	{
		std::cout << "Attempted read at unknown memory location 0x" << std::hex << address
				  << std::dec << std::endl;
		return false;
	}

	return block->read_word(address);
}

bool MemoryMap::write(uint32_t address, uint8_t value)
{
	MemoryBlock *block = find_block(address);
	if (block == nullptr)
	{
		std::cout << "Attempted write at unknown memory location 0x" << std::hex << address
				  << std::dec << std::endl;
		return false;
	}
	return block->write(address, value);
}

MemoryMap::MemoryMap() {}

bool MemoryMap::write_word(uint32_t base_address, uint32_t value)
{
	MemoryBlock *block = find_block(base_address);
	if (!block)
	{
		std::cout << "Attempted write at unknown memory location 0x" << std::hex << base_address
				  << std::dec << std::endl;
		return false;
	}

	return block->write_word(base_address, value);
}
