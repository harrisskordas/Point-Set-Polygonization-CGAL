# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/users/sdi1800217/Code/Project/ass1/take7

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/users/sdi1800217/Code/Project/ass1/take7

# Include any dependencies generated for this target.
include includes/module1/CMakeFiles/utils.dir/depend.make

# Include the progress variables for this target.
include includes/module1/CMakeFiles/utils.dir/progress.make

# Include the compile flags for this target's objects.
include includes/module1/CMakeFiles/utils.dir/flags.make

includes/module1/CMakeFiles/utils.dir/utils.cpp.o: includes/module1/CMakeFiles/utils.dir/flags.make
includes/module1/CMakeFiles/utils.dir/utils.cpp.o: includes/module1/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/users/sdi1800217/Code/Project/ass1/take7/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object includes/module1/CMakeFiles/utils.dir/utils.cpp.o"
	cd /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 && /usr/bin/X11/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utils.dir/utils.cpp.o -c /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1/utils.cpp

includes/module1/CMakeFiles/utils.dir/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utils.dir/utils.cpp.i"
	cd /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 && /usr/bin/X11/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1/utils.cpp > CMakeFiles/utils.dir/utils.cpp.i

includes/module1/CMakeFiles/utils.dir/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utils.dir/utils.cpp.s"
	cd /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 && /usr/bin/X11/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1/utils.cpp -o CMakeFiles/utils.dir/utils.cpp.s

# Object files for target utils
utils_OBJECTS = \
"CMakeFiles/utils.dir/utils.cpp.o"

# External object files for target utils
utils_EXTERNAL_OBJECTS =

includes/module1/libutils.a: includes/module1/CMakeFiles/utils.dir/utils.cpp.o
includes/module1/libutils.a: includes/module1/CMakeFiles/utils.dir/build.make
includes/module1/libutils.a: includes/module1/CMakeFiles/utils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/users/sdi1800217/Code/Project/ass1/take7/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libutils.a"
	cd /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean_target.cmake
	cd /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
includes/module1/CMakeFiles/utils.dir/build: includes/module1/libutils.a

.PHONY : includes/module1/CMakeFiles/utils.dir/build

includes/module1/CMakeFiles/utils.dir/clean:
	cd /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 && $(CMAKE_COMMAND) -P CMakeFiles/utils.dir/cmake_clean.cmake
.PHONY : includes/module1/CMakeFiles/utils.dir/clean

includes/module1/CMakeFiles/utils.dir/depend:
	cd /home/users/sdi1800217/Code/Project/ass1/take7 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/users/sdi1800217/Code/Project/ass1/take7 /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 /home/users/sdi1800217/Code/Project/ass1/take7 /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1 /home/users/sdi1800217/Code/Project/ass1/take7/includes/module1/CMakeFiles/utils.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : includes/module1/CMakeFiles/utils.dir/depend
