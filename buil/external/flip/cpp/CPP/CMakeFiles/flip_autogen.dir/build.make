# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/niels/Documents/repos/nitro

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/niels/Documents/repos/nitro/buil

# Utility rule file for flip_autogen.

# Include any custom commands dependencies for this target.
include external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/progress.make

external/flip/cpp/CPP/CMakeFiles/flip_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/niels/Documents/repos/nitro/buil/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target flip"
	cd /home/niels/Documents/repos/nitro/buil/external/flip/cpp/CPP && /usr/bin/cmake -E cmake_autogen /home/niels/Documents/repos/nitro/buil/external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/AutogenInfo.json ""

flip_autogen: external/flip/cpp/CPP/CMakeFiles/flip_autogen
flip_autogen: external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/build.make
.PHONY : flip_autogen

# Rule to build all files generated by this target.
external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/build: flip_autogen
.PHONY : external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/build

external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/clean:
	cd /home/niels/Documents/repos/nitro/buil/external/flip/cpp/CPP && $(CMAKE_COMMAND) -P CMakeFiles/flip_autogen.dir/cmake_clean.cmake
.PHONY : external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/clean

external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/depend:
	cd /home/niels/Documents/repos/nitro/buil && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/niels/Documents/repos/nitro /home/niels/Documents/repos/nitro/external/flip/cpp/CPP /home/niels/Documents/repos/nitro/buil /home/niels/Documents/repos/nitro/buil/external/flip/cpp/CPP /home/niels/Documents/repos/nitro/buil/external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/flip/cpp/CPP/CMakeFiles/flip_autogen.dir/depend

