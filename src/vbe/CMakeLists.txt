add_library(vbe vbe.cpp)
target_include_directories(vbe PUBLIC .)
target_link_libraries(vbe dpmi)
