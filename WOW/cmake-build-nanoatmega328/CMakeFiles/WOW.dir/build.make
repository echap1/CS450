# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/ethan/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/192.6817.18/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ethan/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/192.6817.18/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/OneDrive/OneDrive/Coding/CS450/WOW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328

# Include any dependencies generated for this target.
include CMakeFiles/WOW.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/WOW.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/WOW.dir/flags.make

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.o: CMakeFiles/WOW.dir/flags.make
CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.o: ../lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.o"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.o -c /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.i"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp > CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.i

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.s"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp -o CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.s

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.o: CMakeFiles/WOW.dir/flags.make
CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.o: ../lib/Adafruit_NeoPixel/esp8266.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.o"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.o   -c /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/esp8266.c

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.i"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/esp8266.c > CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.i

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.s"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/esp8266.c -o CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.s

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.o: CMakeFiles/WOW.dir/flags.make
CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.o: ../lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.o"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.o -c /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.i"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp > CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.i

CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.s"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/OneDrive/OneDrive/Coding/CS450/WOW/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp -o CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.s

CMakeFiles/WOW.dir/src/main.cpp.o: CMakeFiles/WOW.dir/flags.make
CMakeFiles/WOW.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/WOW.dir/src/main.cpp.o"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/WOW.dir/src/main.cpp.o -c /media/OneDrive/OneDrive/Coding/CS450/WOW/src/main.cpp

CMakeFiles/WOW.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/WOW.dir/src/main.cpp.i"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/OneDrive/OneDrive/Coding/CS450/WOW/src/main.cpp > CMakeFiles/WOW.dir/src/main.cpp.i

CMakeFiles/WOW.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/WOW.dir/src/main.cpp.s"
	/home/ethan/.platformio/packages/toolchain-atmelavr/bin/avr-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/OneDrive/OneDrive/Coding/CS450/WOW/src/main.cpp -o CMakeFiles/WOW.dir/src/main.cpp.s

# Object files for target WOW
WOW_OBJECTS = \
"CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.o" \
"CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.o" \
"CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.o" \
"CMakeFiles/WOW.dir/src/main.cpp.o"

# External object files for target WOW
WOW_EXTERNAL_OBJECTS =

WOW: CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/Adafruit_NeoPixel.cpp.o
WOW: CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/esp8266.c.o
WOW: CMakeFiles/WOW.dir/lib/Adafruit_NeoPixel/examples/StrandtestBLE/BLESerial.cpp.o
WOW: CMakeFiles/WOW.dir/src/main.cpp.o
WOW: CMakeFiles/WOW.dir/build.make
WOW: CMakeFiles/WOW.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable WOW"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/WOW.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/WOW.dir/build: WOW

.PHONY : CMakeFiles/WOW.dir/build

CMakeFiles/WOW.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/WOW.dir/cmake_clean.cmake
.PHONY : CMakeFiles/WOW.dir/clean

CMakeFiles/WOW.dir/depend:
	cd /media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/OneDrive/OneDrive/Coding/CS450/WOW /media/OneDrive/OneDrive/Coding/CS450/WOW /media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328 /media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328 /media/OneDrive/OneDrive/Coding/CS450/WOW/cmake-build-nanoatmega328/CMakeFiles/WOW.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/WOW.dir/depend
