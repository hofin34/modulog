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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/martin/School/bachelor-hofbauer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/martin/School/bachelor-hofbauer/build

# Include any dependencies generated for this target.
include _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/flags.make

_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.o: _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/flags.make
_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.o: _deps/reproc++-src/reproc++/src/reproc.cpp
_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.o: _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.o"
	cd /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.o -MF CMakeFiles/reproc++.dir/src/reproc.cpp.o.d -o CMakeFiles/reproc++.dir/src/reproc.cpp.o -c /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-src/reproc++/src/reproc.cpp

_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reproc++.dir/src/reproc.cpp.i"
	cd /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-src/reproc++/src/reproc.cpp > CMakeFiles/reproc++.dir/src/reproc.cpp.i

_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reproc++.dir/src/reproc.cpp.s"
	cd /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-src/reproc++/src/reproc.cpp -o CMakeFiles/reproc++.dir/src/reproc.cpp.s

# Object files for target reproc++
reproc_______OBJECTS = \
"CMakeFiles/reproc++.dir/src/reproc.cpp.o"

# External object files for target reproc++
reproc_______EXTERNAL_OBJECTS =

_deps/reproc++-build/reproc++/lib/libreproc++.a: _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/src/reproc.cpp.o
_deps/reproc++-build/reproc++/lib/libreproc++.a: _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/build.make
_deps/reproc++-build/reproc++/lib/libreproc++.a: _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library lib/libreproc++.a"
	cd /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ && $(CMAKE_COMMAND) -P CMakeFiles/reproc++.dir/cmake_clean_target.cmake
	cd /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reproc++.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/build: _deps/reproc++-build/reproc++/lib/libreproc++.a
.PHONY : _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/build

_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/clean:
	cd /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ && $(CMAKE_COMMAND) -P CMakeFiles/reproc++.dir/cmake_clean.cmake
.PHONY : _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/clean

_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/depend:
	cd /home/martin/School/bachelor-hofbauer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/martin/School/bachelor-hofbauer /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-src/reproc++ /home/martin/School/bachelor-hofbauer/build /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++ /home/martin/School/bachelor-hofbauer/build/_deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/reproc++-build/reproc++/CMakeFiles/reproc++.dir/depend

