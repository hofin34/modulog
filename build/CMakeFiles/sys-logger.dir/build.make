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
include CMakeFiles/sys-logger.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/sys-logger.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/sys-logger.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sys-logger.dir/flags.make

CMakeFiles/sys-logger.dir/main.cpp.o: CMakeFiles/sys-logger.dir/flags.make
CMakeFiles/sys-logger.dir/main.cpp.o: ../main.cpp
CMakeFiles/sys-logger.dir/main.cpp.o: CMakeFiles/sys-logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sys-logger.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sys-logger.dir/main.cpp.o -MF CMakeFiles/sys-logger.dir/main.cpp.o.d -o CMakeFiles/sys-logger.dir/main.cpp.o -c /home/martin/School/bachelor-hofbauer/main.cpp

CMakeFiles/sys-logger.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sys-logger.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/main.cpp > CMakeFiles/sys-logger.dir/main.cpp.i

CMakeFiles/sys-logger.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sys-logger.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/main.cpp -o CMakeFiles/sys-logger.dir/main.cpp.s

CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o: CMakeFiles/sys-logger.dir/flags.make
CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o: ../core/src/Agent.cpp
CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o: CMakeFiles/sys-logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o -MF CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o.d -o CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o -c /home/martin/School/bachelor-hofbauer/core/src/Agent.cpp

CMakeFiles/sys-logger.dir/core/src/Agent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sys-logger.dir/core/src/Agent.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/core/src/Agent.cpp > CMakeFiles/sys-logger.dir/core/src/Agent.cpp.i

CMakeFiles/sys-logger.dir/core/src/Agent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sys-logger.dir/core/src/Agent.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/core/src/Agent.cpp -o CMakeFiles/sys-logger.dir/core/src/Agent.cpp.s

CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o: CMakeFiles/sys-logger.dir/flags.make
CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o: ../core/src/AgentHandler.cpp
CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o: CMakeFiles/sys-logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o -MF CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o.d -o CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o -c /home/martin/School/bachelor-hofbauer/core/src/AgentHandler.cpp

CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/core/src/AgentHandler.cpp > CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.i

CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/core/src/AgentHandler.cpp -o CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.s

CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o: CMakeFiles/sys-logger.dir/flags.make
CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o: ../core/src/AgentInfo.cpp
CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o: CMakeFiles/sys-logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o -MF CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o.d -o CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o -c /home/martin/School/bachelor-hofbauer/core/src/AgentInfo.cpp

CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/core/src/AgentInfo.cpp > CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.i

CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/core/src/AgentInfo.cpp -o CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.s

CMakeFiles/sys-logger.dir/core/src/Core.cpp.o: CMakeFiles/sys-logger.dir/flags.make
CMakeFiles/sys-logger.dir/core/src/Core.cpp.o: ../core/src/Core.cpp
CMakeFiles/sys-logger.dir/core/src/Core.cpp.o: CMakeFiles/sys-logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/sys-logger.dir/core/src/Core.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sys-logger.dir/core/src/Core.cpp.o -MF CMakeFiles/sys-logger.dir/core/src/Core.cpp.o.d -o CMakeFiles/sys-logger.dir/core/src/Core.cpp.o -c /home/martin/School/bachelor-hofbauer/core/src/Core.cpp

CMakeFiles/sys-logger.dir/core/src/Core.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sys-logger.dir/core/src/Core.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/core/src/Core.cpp > CMakeFiles/sys-logger.dir/core/src/Core.cpp.i

CMakeFiles/sys-logger.dir/core/src/Core.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sys-logger.dir/core/src/Core.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/core/src/Core.cpp -o CMakeFiles/sys-logger.dir/core/src/Core.cpp.s

CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o: CMakeFiles/sys-logger.dir/flags.make
CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o: ../core/src/LogSaver.cpp
CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o: CMakeFiles/sys-logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o -MF CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o.d -o CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o -c /home/martin/School/bachelor-hofbauer/core/src/LogSaver.cpp

CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/School/bachelor-hofbauer/core/src/LogSaver.cpp > CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.i

CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/School/bachelor-hofbauer/core/src/LogSaver.cpp -o CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.s

# Object files for target sys-logger
sys__logger_OBJECTS = \
"CMakeFiles/sys-logger.dir/main.cpp.o" \
"CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o" \
"CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o" \
"CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o" \
"CMakeFiles/sys-logger.dir/core/src/Core.cpp.o" \
"CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o"

# External object files for target sys-logger
sys__logger_EXTERNAL_OBJECTS =

sys-logger: CMakeFiles/sys-logger.dir/main.cpp.o
sys-logger: CMakeFiles/sys-logger.dir/core/src/Agent.cpp.o
sys-logger: CMakeFiles/sys-logger.dir/core/src/AgentHandler.cpp.o
sys-logger: CMakeFiles/sys-logger.dir/core/src/AgentInfo.cpp.o
sys-logger: CMakeFiles/sys-logger.dir/core/src/Core.cpp.o
sys-logger: CMakeFiles/sys-logger.dir/core/src/LogSaver.cpp.o
sys-logger: CMakeFiles/sys-logger.dir/build.make
sys-logger: communication/libCommunicationLib.a
sys-logger: _deps/reproc++-build/reproc++/lib/libreproc++.a
sys-logger: _deps/reproc++-build/reproc/lib/libreproc.a
sys-logger: CMakeFiles/sys-logger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/martin/School/bachelor-hofbauer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable sys-logger"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sys-logger.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sys-logger.dir/build: sys-logger
.PHONY : CMakeFiles/sys-logger.dir/build

CMakeFiles/sys-logger.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sys-logger.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sys-logger.dir/clean

CMakeFiles/sys-logger.dir/depend:
	cd /home/martin/School/bachelor-hofbauer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/martin/School/bachelor-hofbauer /home/martin/School/bachelor-hofbauer /home/martin/School/bachelor-hofbauer/build /home/martin/School/bachelor-hofbauer/build /home/martin/School/bachelor-hofbauer/build/CMakeFiles/sys-logger.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sys-logger.dir/depend

