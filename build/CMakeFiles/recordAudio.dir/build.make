# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.8/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.8/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/kavindail/BCIT/Client Server/VoiceOverIp"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/kavindail/BCIT/Client Server/VoiceOverIp/build"

# Include any dependencies generated for this target.
include CMakeFiles/recordAudio.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/recordAudio.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/recordAudio.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/recordAudio.dir/flags.make

CMakeFiles/recordAudio.dir/main.cpp.o: CMakeFiles/recordAudio.dir/flags.make
CMakeFiles/recordAudio.dir/main.cpp.o: /Users/kavindail/BCIT/Client\ Server/VoiceOverIp/main.cpp
CMakeFiles/recordAudio.dir/main.cpp.o: CMakeFiles/recordAudio.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/kavindail/BCIT/Client Server/VoiceOverIp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/recordAudio.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/recordAudio.dir/main.cpp.o -MF CMakeFiles/recordAudio.dir/main.cpp.o.d -o CMakeFiles/recordAudio.dir/main.cpp.o -c "/Users/kavindail/BCIT/Client Server/VoiceOverIp/main.cpp"

CMakeFiles/recordAudio.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/recordAudio.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/kavindail/BCIT/Client Server/VoiceOverIp/main.cpp" > CMakeFiles/recordAudio.dir/main.cpp.i

CMakeFiles/recordAudio.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/recordAudio.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/kavindail/BCIT/Client Server/VoiceOverIp/main.cpp" -o CMakeFiles/recordAudio.dir/main.cpp.s

# Object files for target recordAudio
recordAudio_OBJECTS = \
"CMakeFiles/recordAudio.dir/main.cpp.o"

# External object files for target recordAudio
recordAudio_EXTERNAL_OBJECTS =

recordAudio: CMakeFiles/recordAudio.dir/main.cpp.o
recordAudio: CMakeFiles/recordAudio.dir/build.make
recordAudio: /opt/homebrew/Cellar/portaudio/19.7.0/lib/libportaudio.dylib
recordAudio: CMakeFiles/recordAudio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/kavindail/BCIT/Client Server/VoiceOverIp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable recordAudio"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/recordAudio.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/recordAudio.dir/build: recordAudio
.PHONY : CMakeFiles/recordAudio.dir/build

CMakeFiles/recordAudio.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/recordAudio.dir/cmake_clean.cmake
.PHONY : CMakeFiles/recordAudio.dir/clean

CMakeFiles/recordAudio.dir/depend:
	cd "/Users/kavindail/BCIT/Client Server/VoiceOverIp/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/kavindail/BCIT/Client Server/VoiceOverIp" "/Users/kavindail/BCIT/Client Server/VoiceOverIp" "/Users/kavindail/BCIT/Client Server/VoiceOverIp/build" "/Users/kavindail/BCIT/Client Server/VoiceOverIp/build" "/Users/kavindail/BCIT/Client Server/VoiceOverIp/build/CMakeFiles/recordAudio.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/recordAudio.dir/depend

