#include <memory/elf_reader.h>
#include <iostream>
#include <stdexcept>

ELFReader::ELFReader(const std::string &file_path) : file_path(file_path)
{
	std::ifstream file(file_path, std::ios::binary);
	if (!file)
	{
		throw std::runtime_error("Failed to open ELF file: " + file_path);
	}
	if (!reader.load(file_path))
	{
		throw std::runtime_error("Failed to load ELF file: " + file_path);
	}
	std::cout << "ELF file loaded successfully: " << file_path << std::endl;
}

std::unique_ptr<MemoryMap> ELFReader::load()
{
	std::unique_ptr<MemoryMap> memory_map = std::make_unique<MemoryMap>();

	// check if 32 bit
	if (reader.get_class() != ELFIO::ELFCLASS32)
	{
		throw std::runtime_error("Failed to load ELF file: " + file_path + "; expected 32-bit class.");
	}

	std::cout << "ELF entry point: 0x" << std::hex << reader.get_entry() << std::dec << '\n';

	for (const auto &segment : reader.segments)
	{
		if (segment->get_type() != ELFIO::PT_LOAD)
			continue;

		uint32_t paddr = segment->get_virtual_address();
		uint32_t mem_size = segment->get_memory_size();
		uint32_t file_size = segment->get_file_size();
		const char *data = segment->get_data();

		const bool writable = (segment->get_flags() & ELFIO::PF_W) != 0;

		std::unique_ptr<MemoryBlock> block;
		if (writable)
		{
			auto ram_block = std::make_unique<RAMBlock>(paddr, paddr + mem_size);

			if (data != nullptr && file_size > 0)
			{
				for (size_t i = 0; i < file_size; i++)
				{
					ram_block->write(paddr + i, data[i]);
				}
				for (size_t i = file_size; i < mem_size; i++)
				{
					ram_block->write(paddr + i, 0);
				}
			}
			else
			{
				// NOLOAD or zero-file-size segment - zero entire block
				for (size_t i = 0; i < mem_size; i++)
				{
					ram_block->write(paddr + i, 0);
				}
			}

			block = std::move(ram_block);
		}
		else
		{
			auto rom_block = std::make_unique<ROMBlock>(paddr, paddr + mem_size);

			if (data != nullptr && file_size > 0)
			{
				for (size_t i = 0; i < file_size; i++)
				{
					rom_block->rom_write(paddr + i, data[i]);
				}
				for (size_t i = file_size; i < mem_size; i++)
				{
					rom_block->rom_write(paddr + i, 0);
				}
			}
			else
			{
				// NOLOAD ROM segment, zero fill anyway
				for (size_t i = 0; i < mem_size; i++)
				{
					rom_block->rom_write(paddr + i, 0);
				}
			}

			block = std::move(rom_block);
		}

		memory_map->add_block(std::move(block));
	}

	return memory_map;
}
