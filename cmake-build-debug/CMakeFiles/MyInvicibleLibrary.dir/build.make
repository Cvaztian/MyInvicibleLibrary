# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /snap/clion/73/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/73/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/malavassi/Escritorio/MyInvicibleLibrary

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MyInvicibleLibrary.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/MyInvicibleLibrary.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyInvicibleLibrary.dir/flags.make

CMakeFiles/MyInvicibleLibrary.dir/main.cpp.o: CMakeFiles/MyInvicibleLibrary.dir/flags.make
CMakeFiles/MyInvicibleLibrary.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyInvicibleLibrary.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyInvicibleLibrary.dir/main.cpp.o -c /home/malavassi/Escritorio/MyInvicibleLibrary/main.cpp

CMakeFiles/MyInvicibleLibrary.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MyInvicibleLibrary.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/malavassi/Escritorio/MyInvicibleLibrary/main.cpp > CMakeFiles/MyInvicibleLibrary.dir/main.cpp.i

CMakeFiles/MyInvicibleLibrary.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MyInvicibleLibrary.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/malavassi/Escritorio/MyInvicibleLibrary/main.cpp -o CMakeFiles/MyInvicibleLibrary.dir/main.cpp.s

# Object files for target MyInvicibleLibrary
MyInvicibleLibrary_OBJECTS = \
"CMakeFiles/MyInvicibleLibrary.dir/main.cpp.o"

# External object files for target MyInvicibleLibrary
MyInvicibleLibrary_EXTERNAL_OBJECTS =

MyInvicibleLibrary: CMakeFiles/MyInvicibleLibrary.dir/main.cpp.o
MyInvicibleLibrary: CMakeFiles/MyInvicibleLibrary.dir/build.make
MyInvicibleLibrary: CMakeFiles/MyInvicibleLibrary.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MyInvicibleLibrary"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyInvicibleLibrary.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyInvicibleLibrary.dir/build: MyInvicibleLibrary

.PHONY : CMakeFiles/MyInvicibleLibrary.dir/build

CMakeFiles/MyInvicibleLibrary.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyInvicibleLibrary.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyInvicibleLibrary.dir/clean

CMakeFiles/MyInvicibleLibrary.dir/depend:
	cd /home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/malavassi/Escritorio/MyInvicibleLibrary /home/malavassi/Escritorio/MyInvicibleLibrary /home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug /home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug /home/malavassi/Escritorio/MyInvicibleLibrary/cmake-build-debug/CMakeFiles/MyInvicibleLibrary.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MyInvicibleLibrary.dir/depend
