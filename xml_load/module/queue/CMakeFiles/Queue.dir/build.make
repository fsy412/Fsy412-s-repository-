# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/smb/Fsy412-s-repository-/xml_load

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/smb/Fsy412-s-repository-/xml_load

# Include any dependencies generated for this target.
include module/queue/CMakeFiles/Queue.dir/depend.make

# Include the progress variables for this target.
include module/queue/CMakeFiles/Queue.dir/progress.make

# Include the compile flags for this target's objects.
include module/queue/CMakeFiles/Queue.dir/flags.make

module/queue/CMakeFiles/Queue.dir/queue.cpp.o: module/queue/CMakeFiles/Queue.dir/flags.make
module/queue/CMakeFiles/Queue.dir/queue.cpp.o: module/queue/queue.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/smb/Fsy412-s-repository-/xml_load/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object module/queue/CMakeFiles/Queue.dir/queue.cpp.o"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Queue.dir/queue.cpp.o -c /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/queue.cpp

module/queue/CMakeFiles/Queue.dir/queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Queue.dir/queue.cpp.i"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/queue.cpp > CMakeFiles/Queue.dir/queue.cpp.i

module/queue/CMakeFiles/Queue.dir/queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Queue.dir/queue.cpp.s"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/queue.cpp -o CMakeFiles/Queue.dir/queue.cpp.s

module/queue/CMakeFiles/Queue.dir/queue.cpp.o.requires:
.PHONY : module/queue/CMakeFiles/Queue.dir/queue.cpp.o.requires

module/queue/CMakeFiles/Queue.dir/queue.cpp.o.provides: module/queue/CMakeFiles/Queue.dir/queue.cpp.o.requires
	$(MAKE) -f module/queue/CMakeFiles/Queue.dir/build.make module/queue/CMakeFiles/Queue.dir/queue.cpp.o.provides.build
.PHONY : module/queue/CMakeFiles/Queue.dir/queue.cpp.o.provides

module/queue/CMakeFiles/Queue.dir/queue.cpp.o.provides.build: module/queue/CMakeFiles/Queue.dir/queue.cpp.o

module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o: module/queue/CMakeFiles/Queue.dir/flags.make
module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o: module/queue/t_queue.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/smb/Fsy412-s-repository-/xml_load/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Queue.dir/t_queue.cpp.o -c /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/t_queue.cpp

module/queue/CMakeFiles/Queue.dir/t_queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Queue.dir/t_queue.cpp.i"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/t_queue.cpp > CMakeFiles/Queue.dir/t_queue.cpp.i

module/queue/CMakeFiles/Queue.dir/t_queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Queue.dir/t_queue.cpp.s"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/t_queue.cpp -o CMakeFiles/Queue.dir/t_queue.cpp.s

module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.requires:
.PHONY : module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.requires

module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.provides: module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.requires
	$(MAKE) -f module/queue/CMakeFiles/Queue.dir/build.make module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.provides.build
.PHONY : module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.provides

module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.provides.build: module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o

# Object files for target Queue
Queue_OBJECTS = \
"CMakeFiles/Queue.dir/queue.cpp.o" \
"CMakeFiles/Queue.dir/t_queue.cpp.o"

# External object files for target Queue
Queue_EXTERNAL_OBJECTS =

module/queue/libQueue.a: module/queue/CMakeFiles/Queue.dir/queue.cpp.o
module/queue/libQueue.a: module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o
module/queue/libQueue.a: module/queue/CMakeFiles/Queue.dir/build.make
module/queue/libQueue.a: module/queue/CMakeFiles/Queue.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libQueue.a"
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && $(CMAKE_COMMAND) -P CMakeFiles/Queue.dir/cmake_clean_target.cmake
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Queue.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
module/queue/CMakeFiles/Queue.dir/build: module/queue/libQueue.a
.PHONY : module/queue/CMakeFiles/Queue.dir/build

module/queue/CMakeFiles/Queue.dir/requires: module/queue/CMakeFiles/Queue.dir/queue.cpp.o.requires
module/queue/CMakeFiles/Queue.dir/requires: module/queue/CMakeFiles/Queue.dir/t_queue.cpp.o.requires
.PHONY : module/queue/CMakeFiles/Queue.dir/requires

module/queue/CMakeFiles/Queue.dir/clean:
	cd /mnt/smb/Fsy412-s-repository-/xml_load/module/queue && $(CMAKE_COMMAND) -P CMakeFiles/Queue.dir/cmake_clean.cmake
.PHONY : module/queue/CMakeFiles/Queue.dir/clean

module/queue/CMakeFiles/Queue.dir/depend:
	cd /mnt/smb/Fsy412-s-repository-/xml_load && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/smb/Fsy412-s-repository-/xml_load /mnt/smb/Fsy412-s-repository-/xml_load/module/queue /mnt/smb/Fsy412-s-repository-/xml_load /mnt/smb/Fsy412-s-repository-/xml_load/module/queue /mnt/smb/Fsy412-s-repository-/xml_load/module/queue/CMakeFiles/Queue.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : module/queue/CMakeFiles/Queue.dir/depend

