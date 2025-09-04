#pragma once
#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>
#include <stdexcept>

class MemoryBlock
{
public:
	MemoryBlock(uint32_t start_address, uint32_t end_address)
		: start_address(start_address), end_address(end_address)
	{
	}
	virtual ~MemoryBlock() = default;
	virtual bool contains(uint32_t address) const
	{
		return address >= start_address && address < end_address;
	}
	virtual uint8_t read(uint32_t address) const = 0;
	virtual uint32_t read_word(uint32_t base_address) const = 0;
	virtual bool write(uint32_t address, uint8_t value) = 0;
	virtual bool write_word(uint32_t base_address, uint32_t value) = 0;
	virtual void reset() = 0;
	uint32_t start_address;
	uint32_t end_address;
};

class RAMBlock : public MemoryBlock
{
public:
	RAMBlock(uint32_t start_address, uint32_t end_address);
	~RAMBlock() = default;
	virtual void reset() override;
	virtual bool write(uint32_t address, uint8_t value) override;
	virtual uint8_t read(uint32_t address) const override;
	virtual bool write_word(uint32_t base_address, uint32_t value) override;
	virtual uint32_t read_word(uint32_t base_address) const override;

private:
	std::vector<uint8_t> data;
};

class ROMBlock : public MemoryBlock
{
public:
	ROMBlock(uint32_t start_address, uint32_t end_address);
	~ROMBlock() = default;
	virtual void reset() override;
	virtual bool write(uint32_t address, uint8_t value) override;
	virtual bool write_word(uint32_t base_address, uint32_t value) override;
	virtual uint32_t read_word(uint32_t base_address) const override;
	virtual uint8_t read(uint32_t address) const override;
	void rom_write(uint32_t address, uint8_t value);

private:
	std::vector<uint8_t> data;
};

class MemoryMap
{
public:
	std::vector<std::unique_ptr<MemoryBlock>> blocks;
	MemoryMap();
	~MemoryMap() = default;
	void add_block(std::unique_ptr<MemoryBlock> new_block);
	uint8_t read(uint32_t address) const;
	uint32_t read_word(uint32_t base_address) const;
	bool write(uint32_t address, uint8_t value);
	bool write_word(uint32_t base_address, uint32_t value);
	MemoryBlock *find_block(uint32_t address) const;
};
