# Necroware's Video Memory Tester

*Warning:* This is a very early development version which potentially could
break your graphics card, your house, your city, your country, and also come
home drunken and impregnate your cat. So please, use it only at your own risk. 

# Why do we need another video memory test tool?

First of all, this one is fast. Furthermore there are already tools out there
which just return broken addresses, where you have to derive which of the
memory chips on your card are broken. It often ends up in guessing games and
replacing the wrong chips. So this tool doesn't only tell if the memory of your
graphics card is broken, it also tells you which chip you should replace. This
tool runs in DOS and is theoretically able to test all kinds of graphics cards
which support VBE 2.0

# How it works?

The tool detects the graphics card and uses VBE 2.0 to allocate a linear frame
buffer over the total size of the video memory. Then it writes different
patterns into the video memory and validates the content. If any address turns
out to be bad the tool calculates which memory chip exactly is affected on the
card. Unfortunately it is not possible to tell which chips and how many of those
are installed on the tested card, so this information has to be provided by the
user and is used for the result calculations.

# Current limitations

- Needs pure DOS without any memory managers loaded
- Supports only VBE 2.0 capable grpahics cards
- The memory chips on the card have to be at least 8-bit or multiple of 8-bits
- No graphical interface or user feedback during the tests

# Programming details

With this project I finally got to a point which I had on my TODO list for a
very long time: programming in modern C++ for DOS. So the tool is written in 
C++20 with STL, exceptions and almost all the fancy stuf. Only iostreams were
not used because just inclusion of the headers grew the binary footprint of the 
tool by 300%. The project utilizes DJGPP C++ (12.0) compiler and CMake (3.23).

# How to build

cmake -B build -S src \
    -DCMAKE_TOOLCHAIN_FILE=toolchain-djgpp.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build

# License

The project is licensed under GPL v3.0

