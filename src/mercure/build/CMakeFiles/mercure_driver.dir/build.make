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
CMAKE_SOURCE_DIR = /home/cwl/Desktop/irobot-23/RM_vison/src/mercure

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build

# Include any dependencies generated for this target.
include CMakeFiles/mercure_driver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mercure_driver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mercure_driver.dir/flags.make

CMakeFiles/mercure_driver.dir/mercure_driver.cpp.o: CMakeFiles/mercure_driver.dir/flags.make
CMakeFiles/mercure_driver.dir/mercure_driver.cpp.o: ../mercure_driver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mercure_driver.dir/mercure_driver.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/mercure_driver.dir/mercure_driver.cpp.o -c /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/mercure_driver.cpp

CMakeFiles/mercure_driver.dir/mercure_driver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mercure_driver.dir/mercure_driver.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/mercure_driver.cpp > CMakeFiles/mercure_driver.dir/mercure_driver.cpp.i

CMakeFiles/mercure_driver.dir/mercure_driver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mercure_driver.dir/mercure_driver.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/mercure_driver.cpp -o CMakeFiles/mercure_driver.dir/mercure_driver.cpp.s

# Object files for target mercure_driver
mercure_driver_OBJECTS = \
"CMakeFiles/mercure_driver.dir/mercure_driver.cpp.o"

# External object files for target mercure_driver
mercure_driver_EXTERNAL_OBJECTS =

libmercure_driver.a: CMakeFiles/mercure_driver.dir/mercure_driver.cpp.o
libmercure_driver.a: CMakeFiles/mercure_driver.dir/build.make
libmercure_driver.a: CMakeFiles/mercure_driver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmercure_driver.a"
	$(CMAKE_COMMAND) -P CMakeFiles/mercure_driver.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mercure_driver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mercure_driver.dir/build: libmercure_driver.a

.PHONY : CMakeFiles/mercure_driver.dir/build

CMakeFiles/mercure_driver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mercure_driver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mercure_driver.dir/clean

CMakeFiles/mercure_driver.dir/depend:
	cd /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cwl/Desktop/irobot-23/RM_vison/src/mercure /home/cwl/Desktop/irobot-23/RM_vison/src/mercure /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build /home/cwl/Desktop/irobot-23/RM_vison/src/mercure/build/CMakeFiles/mercure_driver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mercure_driver.dir/depend
