add_library(pci pci.cpp io.cpp types.cpp devices.cpp)
target_include_directories(pci PUBLIC ${CMAKE_CURRENT_LIST_DIR})


