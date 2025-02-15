cmake_minimum_required(VERSION 3.22)
project(statDataProcessor)

# Set compiler flags for all warnings and maximum optimization
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # For GCC and Clang
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra -Wpedantic -Werror")
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    # For MSVC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2 /W4 /WX")  # Max optimization and all warnings as errors
endif()

# Add executables
add_executable(prepareDumpForProcessor prepareMain.c hashTable.c statData.c)
add_executable(statDataProcessor main.c hashTable.c statData.c)
add_executable(fastTest fastTest.c hashTable.c statData.c)
add_executable(randomTestUtil unitTests.c hashTable.c statData.c)

