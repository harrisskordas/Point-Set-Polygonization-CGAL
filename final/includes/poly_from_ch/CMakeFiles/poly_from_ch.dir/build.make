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
CMAKE_SOURCE_DIR = /home/users/sdi1800217/Code/Project/ass1/current

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/users/sdi1800217/Code/Project/ass1/current

# Include any dependencies generated for this target.
include includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/depend.make

# Include the progress variables for this target.
include includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/progress.make

# Include the compile flags for this target's objects.
include includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/flags.make

includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.o: includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/flags.make
includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.o: includes/poly_from_ch/poly_from_ch.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/users/sdi1800217/Code/Project/ass1/current/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.o"
	cd /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch && /usr/bin/X11/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.o -c /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch/poly_from_ch.cpp

includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.i"
	cd /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch && /usr/bin/X11/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch/poly_from_ch.cpp > CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.i

includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.s"
	cd /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch && /usr/bin/X11/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch/poly_from_ch.cpp -o CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.s

# Object files for target poly_from_ch
poly_from_ch_OBJECTS = \
"CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.o"

# External object files for target poly_from_ch
poly_from_ch_EXTERNAL_OBJECTS =

includes/poly_from_ch/libpoly_from_ch.a: includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/poly_from_ch.cpp.o
includes/poly_from_ch/libpoly_from_ch.a: includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/build.make
includes/poly_from_ch/libpoly_from_ch.a: includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/users/sdi1800217/Code/Project/ass1/current/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libpoly_from_ch.a"
	cd /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch && $(CMAKE_COMMAND) -P CMakeFiles/poly_from_ch.dir/cmake_clean_target.cmake
	cd /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/poly_from_ch.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/build: includes/poly_from_ch/libpoly_from_ch.a

.PHONY : includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/build

includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/clean:
	cd /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch && $(CMAKE_COMMAND) -P CMakeFiles/poly_from_ch.dir/cmake_clean.cmake
.PHONY : includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/clean

includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/depend:
	cd /home/users/sdi1800217/Code/Project/ass1/current && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/users/sdi1800217/Code/Project/ass1/current /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch /home/users/sdi1800217/Code/Project/ass1/current /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch /home/users/sdi1800217/Code/Project/ass1/current/includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : includes/poly_from_ch/CMakeFiles/poly_from_ch.dir/depend

