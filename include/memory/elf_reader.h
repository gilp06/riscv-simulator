#pragma once

#include <elfio/elfio.hpp>
#include <memory>
#include <memory/memory_map.h>
#include <string>
#include <fstream>

// ELFReader to convert an elf file into a usable Memory Map

class ELFReader {
    public:
        ELFReader(const std::string& file_path);
        ~ELFReader() = default;

        // Load the ELF file and return a MemoryMap
        std::unique_ptr<MemoryMap> load();
        uint32_t get_entry_point() const {
            return reader.get_entry();
        }
    private:
        std::string file_path;
        ELFIO::elfio reader;
};