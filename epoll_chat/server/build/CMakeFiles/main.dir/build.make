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
CMAKE_SOURCE_DIR = /home/zyt/ex1/other/项目/epoll_chat/server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zyt/ex1/other/项目/epoll_chat/server/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/src/epoll_s.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/epoll_s.c.o: ../src/epoll_s.c
CMakeFiles/main.dir/src/epoll_s.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zyt/ex1/other/项目/epoll_chat/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.dir/src/epoll_s.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/src/epoll_s.c.o -MF CMakeFiles/main.dir/src/epoll_s.c.o.d -o CMakeFiles/main.dir/src/epoll_s.c.o -c /home/zyt/ex1/other/项目/epoll_chat/server/src/epoll_s.c

CMakeFiles/main.dir/src/epoll_s.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/src/epoll_s.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zyt/ex1/other/项目/epoll_chat/server/src/epoll_s.c > CMakeFiles/main.dir/src/epoll_s.c.i

CMakeFiles/main.dir/src/epoll_s.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/src/epoll_s.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zyt/ex1/other/项目/epoll_chat/server/src/epoll_s.c -o CMakeFiles/main.dir/src/epoll_s.c.s

CMakeFiles/main.dir/src/link_operation.c.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/link_operation.c.o: ../src/link_operation.c
CMakeFiles/main.dir/src/link_operation.c.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zyt/ex1/other/项目/epoll_chat/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/main.dir/src/link_operation.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.dir/src/link_operation.c.o -MF CMakeFiles/main.dir/src/link_operation.c.o.d -o CMakeFiles/main.dir/src/link_operation.c.o -c /home/zyt/ex1/other/项目/epoll_chat/server/src/link_operation.c

CMakeFiles/main.dir/src/link_operation.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/main.dir/src/link_operation.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zyt/ex1/other/项目/epoll_chat/server/src/link_operation.c > CMakeFiles/main.dir/src/link_operation.c.i

CMakeFiles/main.dir/src/link_operation.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/main.dir/src/link_operation.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zyt/ex1/other/项目/epoll_chat/server/src/link_operation.c -o CMakeFiles/main.dir/src/link_operation.c.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/src/epoll_s.c.o" \
"CMakeFiles/main.dir/src/link_operation.c.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

../bin/main: CMakeFiles/main.dir/src/epoll_s.c.o
../bin/main: CMakeFiles/main.dir/src/link_operation.c.o
../bin/main: CMakeFiles/main.dir/build.make
../bin/main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zyt/ex1/other/项目/epoll_chat/server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable ../bin/main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: ../bin/main
.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /home/zyt/ex1/other/项目/epoll_chat/server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zyt/ex1/other/项目/epoll_chat/server /home/zyt/ex1/other/项目/epoll_chat/server /home/zyt/ex1/other/项目/epoll_chat/server/build /home/zyt/ex1/other/项目/epoll_chat/server/build /home/zyt/ex1/other/项目/epoll_chat/server/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

