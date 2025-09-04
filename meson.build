project('riscv_simulator', 'cpp', 
    version: '0.1',
    default_options: ['cpp_std=c++20'])


json_dep = dependency('nlohmann_json')

incdir = include_directories('include', 'subprojects/elfio-3.12')
srcs = [
'src/main.cpp',
'src/memory/memory_map.cpp', 
'src/memory/elf_reader.cpp',
'src/riscv_core.cpp', 
'src/instruction_formatter.cpp', 
'src/instruction_set.cpp', 
'src/extensions/rv32i.cpp'
]

executable('riscvsim', srcs, include_directories: incdir, dependencies: [json_dep])

