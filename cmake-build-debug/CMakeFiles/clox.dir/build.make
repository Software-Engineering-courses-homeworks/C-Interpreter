# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\test\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\test\AppData\Local\Programs\CLion\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\test\CLionProjects\C-Interpreter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/clox.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/clox.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/clox.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clox.dir/flags.make

CMakeFiles/clox.dir/chunk.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/chunk.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/chunk.c.obj: C:/Users/test/CLionProjects/C-Interpreter/chunk.c
CMakeFiles/clox.dir/chunk.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/clox.dir/chunk.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/chunk.c.obj -MF CMakeFiles\clox.dir\chunk.c.obj.d -o CMakeFiles\clox.dir\chunk.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\chunk.c

CMakeFiles/clox.dir/chunk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/chunk.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\chunk.c > CMakeFiles\clox.dir\chunk.c.i

CMakeFiles/clox.dir/chunk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/chunk.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\chunk.c -o CMakeFiles\clox.dir\chunk.c.s

CMakeFiles/clox.dir/debug.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/debug.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/debug.c.obj: C:/Users/test/CLionProjects/C-Interpreter/debug.c
CMakeFiles/clox.dir/debug.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/clox.dir/debug.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/debug.c.obj -MF CMakeFiles\clox.dir\debug.c.obj.d -o CMakeFiles\clox.dir\debug.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\debug.c

CMakeFiles/clox.dir/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/debug.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\debug.c > CMakeFiles\clox.dir\debug.c.i

CMakeFiles/clox.dir/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/debug.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\debug.c -o CMakeFiles\clox.dir\debug.c.s

CMakeFiles/clox.dir/main.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/main.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/main.c.obj: C:/Users/test/CLionProjects/C-Interpreter/main.c
CMakeFiles/clox.dir/main.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/clox.dir/main.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/main.c.obj -MF CMakeFiles\clox.dir\main.c.obj.d -o CMakeFiles\clox.dir\main.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\main.c

CMakeFiles/clox.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/main.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\main.c > CMakeFiles\clox.dir\main.c.i

CMakeFiles/clox.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/main.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\main.c -o CMakeFiles\clox.dir\main.c.s

CMakeFiles/clox.dir/memory.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/memory.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/memory.c.obj: C:/Users/test/CLionProjects/C-Interpreter/memory.c
CMakeFiles/clox.dir/memory.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/clox.dir/memory.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/memory.c.obj -MF CMakeFiles\clox.dir\memory.c.obj.d -o CMakeFiles\clox.dir\memory.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\memory.c

CMakeFiles/clox.dir/memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/memory.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\memory.c > CMakeFiles\clox.dir\memory.c.i

CMakeFiles/clox.dir/memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/memory.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\memory.c -o CMakeFiles\clox.dir\memory.c.s

CMakeFiles/clox.dir/value.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/value.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/value.c.obj: C:/Users/test/CLionProjects/C-Interpreter/value.c
CMakeFiles/clox.dir/value.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/clox.dir/value.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/value.c.obj -MF CMakeFiles\clox.dir\value.c.obj.d -o CMakeFiles\clox.dir\value.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\value.c

CMakeFiles/clox.dir/value.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/value.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\value.c > CMakeFiles\clox.dir\value.c.i

CMakeFiles/clox.dir/value.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/value.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\value.c -o CMakeFiles\clox.dir\value.c.s

CMakeFiles/clox.dir/vm.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/vm.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/vm.c.obj: C:/Users/test/CLionProjects/C-Interpreter/vm.c
CMakeFiles/clox.dir/vm.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/clox.dir/vm.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/vm.c.obj -MF CMakeFiles\clox.dir\vm.c.obj.d -o CMakeFiles\clox.dir\vm.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\vm.c

CMakeFiles/clox.dir/vm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/vm.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\vm.c > CMakeFiles\clox.dir\vm.c.i

CMakeFiles/clox.dir/vm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/vm.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\vm.c -o CMakeFiles\clox.dir\vm.c.s

CMakeFiles/clox.dir/compiler.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/compiler.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/compiler.c.obj: C:/Users/test/CLionProjects/C-Interpreter/compiler.c
CMakeFiles/clox.dir/compiler.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/clox.dir/compiler.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/compiler.c.obj -MF CMakeFiles\clox.dir\compiler.c.obj.d -o CMakeFiles\clox.dir\compiler.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\compiler.c

CMakeFiles/clox.dir/compiler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/compiler.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\compiler.c > CMakeFiles\clox.dir\compiler.c.i

CMakeFiles/clox.dir/compiler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/compiler.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\compiler.c -o CMakeFiles\clox.dir\compiler.c.s

CMakeFiles/clox.dir/scanner.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/scanner.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/scanner.c.obj: C:/Users/test/CLionProjects/C-Interpreter/scanner.c
CMakeFiles/clox.dir/scanner.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/clox.dir/scanner.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/scanner.c.obj -MF CMakeFiles\clox.dir\scanner.c.obj.d -o CMakeFiles\clox.dir\scanner.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\scanner.c

CMakeFiles/clox.dir/scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/scanner.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\scanner.c > CMakeFiles\clox.dir\scanner.c.i

CMakeFiles/clox.dir/scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/scanner.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\scanner.c -o CMakeFiles\clox.dir\scanner.c.s

CMakeFiles/clox.dir/object.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/object.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/object.c.obj: C:/Users/test/CLionProjects/C-Interpreter/object.c
CMakeFiles/clox.dir/object.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/clox.dir/object.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/object.c.obj -MF CMakeFiles\clox.dir\object.c.obj.d -o CMakeFiles\clox.dir\object.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\object.c

CMakeFiles/clox.dir/object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/object.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\object.c > CMakeFiles\clox.dir\object.c.i

CMakeFiles/clox.dir/object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/object.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\object.c -o CMakeFiles\clox.dir\object.c.s

CMakeFiles/clox.dir/table.c.obj: CMakeFiles/clox.dir/flags.make
CMakeFiles/clox.dir/table.c.obj: CMakeFiles/clox.dir/includes_C.rsp
CMakeFiles/clox.dir/table.c.obj: C:/Users/test/CLionProjects/C-Interpreter/table.c
CMakeFiles/clox.dir/table.c.obj: CMakeFiles/clox.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/clox.dir/table.c.obj"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/clox.dir/table.c.obj -MF CMakeFiles\clox.dir\table.c.obj.d -o CMakeFiles\clox.dir\table.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\table.c

CMakeFiles/clox.dir/table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/clox.dir/table.c.i"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\table.c > CMakeFiles\clox.dir\table.c.i

CMakeFiles/clox.dir/table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/clox.dir/table.c.s"
	C:\Users\test\AppData\Local\Programs\CLion\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\table.c -o CMakeFiles\clox.dir\table.c.s

# Object files for target clox
clox_OBJECTS = \
"CMakeFiles/clox.dir/chunk.c.obj" \
"CMakeFiles/clox.dir/debug.c.obj" \
"CMakeFiles/clox.dir/main.c.obj" \
"CMakeFiles/clox.dir/memory.c.obj" \
"CMakeFiles/clox.dir/value.c.obj" \
"CMakeFiles/clox.dir/vm.c.obj" \
"CMakeFiles/clox.dir/compiler.c.obj" \
"CMakeFiles/clox.dir/scanner.c.obj" \
"CMakeFiles/clox.dir/object.c.obj" \
"CMakeFiles/clox.dir/table.c.obj"

# External object files for target clox
clox_EXTERNAL_OBJECTS =

clox.exe: CMakeFiles/clox.dir/chunk.c.obj
clox.exe: CMakeFiles/clox.dir/debug.c.obj
clox.exe: CMakeFiles/clox.dir/main.c.obj
clox.exe: CMakeFiles/clox.dir/memory.c.obj
clox.exe: CMakeFiles/clox.dir/value.c.obj
clox.exe: CMakeFiles/clox.dir/vm.c.obj
clox.exe: CMakeFiles/clox.dir/compiler.c.obj
clox.exe: CMakeFiles/clox.dir/scanner.c.obj
clox.exe: CMakeFiles/clox.dir/object.c.obj
clox.exe: CMakeFiles/clox.dir/table.c.obj
clox.exe: CMakeFiles/clox.dir/build.make
clox.exe: CMakeFiles/clox.dir/linkLibs.rsp
clox.exe: CMakeFiles/clox.dir/objects1.rsp
clox.exe: CMakeFiles/clox.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C executable clox.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\clox.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clox.dir/build: clox.exe
.PHONY : CMakeFiles/clox.dir/build

CMakeFiles/clox.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\clox.dir\cmake_clean.cmake
.PHONY : CMakeFiles/clox.dir/clean

CMakeFiles/clox.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\test\CLionProjects\C-Interpreter C:\Users\test\CLionProjects\C-Interpreter C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug C:\Users\test\CLionProjects\C-Interpreter\cmake-build-debug\CMakeFiles\clox.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/clox.dir/depend

