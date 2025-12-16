cmake_minimum_required(VERSION 3.23)
project(nwvmt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(dpmi)
add_subdirectory(pci)
add_subdirectory(utils)
add_subdirectory(vbe)

add_executable(nwvmt nwvmt.cpp)
target_link_libraries(nwvmt dpmi vbe utils)

add_executable(nwlspci nwlspci.cpp)
target_link_libraries(nwlspci pci)

# Generate version header file
string(TIMESTAMP PROJECT_BUILD_DATE "%Y-%m-%d")
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
target_include_directories(nwvmt PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
