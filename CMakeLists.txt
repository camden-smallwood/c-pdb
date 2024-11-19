cmake_minimum_required(VERSION 3.5.0)
project(pdb)

file(GLOB_RECURSE PDB_C_SOURCE_FILES "source/*.c")
set(PDB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/source/")
add_executable(pdb ${PDB_C_SOURCE_FILES})
target_compile_options(pdb PRIVATE -Wall -Wextra)
target_include_directories(pdb PRIVATE ${PDB_INCLUDE_DIRS})
set_target_properties(pdb PROPERTIES C_STANDARD 23 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
