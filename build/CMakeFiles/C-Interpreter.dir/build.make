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
CMAKE_COMMAND = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\test\CLionProjects\C-Interpreter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\test\CLionProjects\C-Interpreter\build

# Include any dependencies generated for this target.
include CMakeFiles/c-interpreter.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/c-interpreter.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/c-interpreter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/c-interpreter.dir/flags.make

CMakeFiles/c-interpreter.dir/chunk.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/chunk.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/chunk.c.obj: C:/Users/test/CLionProjects/C-Interpreter/chunk.c
CMakeFiles/c-interpreter.dir/chunk.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/c-interpreter.dir/chunk.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/chunk.c.obj -MF CMakeFiles\c-interpreter.dir\chunk.c.obj.d -o CMakeFiles\c-interpreter.dir\chunk.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\chunk.c

CMakeFiles/c-interpreter.dir/chunk.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/chunk.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\chunk.c > CMakeFiles\c-interpreter.dir\chunk.c.i

CMakeFiles/c-interpreter.dir/chunk.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/chunk.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\chunk.c -o CMakeFiles\c-interpreter.dir\chunk.c.s

CMakeFiles/c-interpreter.dir/debug.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/debug.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/debug.c.obj: C:/Users/test/CLionProjects/C-Interpreter/debug.c
CMakeFiles/c-interpreter.dir/debug.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/c-interpreter.dir/debug.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/debug.c.obj -MF CMakeFiles\c-interpreter.dir\debug.c.obj.d -o CMakeFiles\c-interpreter.dir\debug.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\debug.c

CMakeFiles/c-interpreter.dir/debug.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/debug.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\debug.c > CMakeFiles\c-interpreter.dir\debug.c.i

CMakeFiles/c-interpreter.dir/debug.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/debug.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\debug.c -o CMakeFiles\c-interpreter.dir\debug.c.s

CMakeFiles/c-interpreter.dir/main.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/main.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/main.c.obj: C:/Users/test/CLionProjects/C-Interpreter/main.c
CMakeFiles/c-interpreter.dir/main.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/c-interpreter.dir/main.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/main.c.obj -MF CMakeFiles\c-interpreter.dir\main.c.obj.d -o CMakeFiles\c-interpreter.dir\main.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\main.c

CMakeFiles/c-interpreter.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/main.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\main.c > CMakeFiles\c-interpreter.dir\main.c.i

CMakeFiles/c-interpreter.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/main.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\main.c -o CMakeFiles\c-interpreter.dir\main.c.s

CMakeFiles/c-interpreter.dir/memory.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/memory.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/memory.c.obj: C:/Users/test/CLionProjects/C-Interpreter/memory.c
CMakeFiles/c-interpreter.dir/memory.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/c-interpreter.dir/memory.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/memory.c.obj -MF CMakeFiles\c-interpreter.dir\memory.c.obj.d -o CMakeFiles\c-interpreter.dir\memory.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\memory.c

CMakeFiles/c-interpreter.dir/memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/memory.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\memory.c > CMakeFiles\c-interpreter.dir\memory.c.i

CMakeFiles/c-interpreter.dir/memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/memory.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\memory.c -o CMakeFiles\c-interpreter.dir\memory.c.s

CMakeFiles/c-interpreter.dir/value.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/value.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/value.c.obj: C:/Users/test/CLionProjects/C-Interpreter/value.c
CMakeFiles/c-interpreter.dir/value.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/c-interpreter.dir/value.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/value.c.obj -MF CMakeFiles\c-interpreter.dir\value.c.obj.d -o CMakeFiles\c-interpreter.dir\value.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\value.c

CMakeFiles/c-interpreter.dir/value.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/value.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\value.c > CMakeFiles\c-interpreter.dir\value.c.i

CMakeFiles/c-interpreter.dir/value.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/value.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\value.c -o CMakeFiles\c-interpreter.dir\value.c.s

CMakeFiles/c-interpreter.dir/vm.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/vm.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/vm.c.obj: C:/Users/test/CLionProjects/C-Interpreter/vm.c
CMakeFiles/c-interpreter.dir/vm.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/c-interpreter.dir/vm.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/vm.c.obj -MF CMakeFiles\c-interpreter.dir\vm.c.obj.d -o CMakeFiles\c-interpreter.dir\vm.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\vm.c

CMakeFiles/c-interpreter.dir/vm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/vm.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\vm.c > CMakeFiles\c-interpreter.dir\vm.c.i

CMakeFiles/c-interpreter.dir/vm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/vm.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\vm.c -o CMakeFiles\c-interpreter.dir\vm.c.s

CMakeFiles/c-interpreter.dir/compiler.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/compiler.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/compiler.c.obj: C:/Users/test/CLionProjects/C-Interpreter/compiler.c
CMakeFiles/c-interpreter.dir/compiler.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/c-interpreter.dir/compiler.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/compiler.c.obj -MF CMakeFiles\c-interpreter.dir\compiler.c.obj.d -o CMakeFiles\c-interpreter.dir\compiler.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\compiler.c

CMakeFiles/c-interpreter.dir/compiler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/compiler.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\compiler.c > CMakeFiles\c-interpreter.dir\compiler.c.i

CMakeFiles/c-interpreter.dir/compiler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/compiler.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\compiler.c -o CMakeFiles\c-interpreter.dir\compiler.c.s

CMakeFiles/c-interpreter.dir/scanner.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/scanner.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/scanner.c.obj: C:/Users/test/CLionProjects/C-Interpreter/scanner.c
CMakeFiles/c-interpreter.dir/scanner.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/c-interpreter.dir/scanner.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/scanner.c.obj -MF CMakeFiles\c-interpreter.dir\scanner.c.obj.d -o CMakeFiles\c-interpreter.dir\scanner.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\scanner.c

CMakeFiles/c-interpreter.dir/scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/scanner.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\scanner.c > CMakeFiles\c-interpreter.dir\scanner.c.i

CMakeFiles/c-interpreter.dir/scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/scanner.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\scanner.c -o CMakeFiles\c-interpreter.dir\scanner.c.s

CMakeFiles/c-interpreter.dir/object.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/object.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/object.c.obj: C:/Users/test/CLionProjects/C-Interpreter/object.c
CMakeFiles/c-interpreter.dir/object.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/c-interpreter.dir/object.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/object.c.obj -MF CMakeFiles\c-interpreter.dir\object.c.obj.d -o CMakeFiles\c-interpreter.dir\object.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\object.c

CMakeFiles/c-interpreter.dir/object.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/object.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\object.c > CMakeFiles\c-interpreter.dir\object.c.i

CMakeFiles/c-interpreter.dir/object.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/object.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\object.c -o CMakeFiles\c-interpreter.dir\object.c.s

CMakeFiles/c-interpreter.dir/table.c.obj: CMakeFiles/c-interpreter.dir/flags.make
CMakeFiles/c-interpreter.dir/table.c.obj: CMakeFiles/c-interpreter.dir/includes_C.rsp
CMakeFiles/c-interpreter.dir/table.c.obj: C:/Users/test/CLionProjects/C-Interpreter/table.c
CMakeFiles/c-interpreter.dir/table.c.obj: CMakeFiles/c-interpreter.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/c-interpreter.dir/table.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/c-interpreter.dir/table.c.obj -MF CMakeFiles\c-interpreter.dir\table.c.obj.d -o CMakeFiles\c-interpreter.dir\table.c.obj -c C:\Users\test\CLionProjects\C-Interpreter\table.c

CMakeFiles/c-interpreter.dir/table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/c-interpreter.dir/table.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\test\CLionProjects\C-Interpreter\table.c > CMakeFiles\c-interpreter.dir\table.c.i

CMakeFiles/c-interpreter.dir/table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/c-interpreter.dir/table.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\test\CLionProjects\C-Interpreter\table.c -o CMakeFiles\c-interpreter.dir\table.c.s

# Object files for target c-interpreter
c__interpreter_OBJECTS = \
"CMakeFiles/c-interpreter.dir/chunk.c.obj" \
"CMakeFiles/c-interpreter.dir/debug.c.obj" \
"CMakeFiles/c-interpreter.dir/main.c.obj" \
"CMakeFiles/c-interpreter.dir/memory.c.obj" \
"CMakeFiles/c-interpreter.dir/value.c.obj" \
"CMakeFiles/c-interpreter.dir/vm.c.obj" \
"CMakeFiles/c-interpreter.dir/compiler.c.obj" \
"CMakeFiles/c-interpreter.dir/scanner.c.obj" \
"CMakeFiles/c-interpreter.dir/object.c.obj" \
"CMakeFiles/c-interpreter.dir/table.c.obj"

# External object files for target c-interpreter
c__interpreter_EXTERNAL_OBJECTS =

c-interpreter.exe: CMakeFiles/c-interpreter.dir/chunk.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/debug.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/main.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/memory.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/value.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/vm.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/compiler.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/scanner.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/object.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/table.c.obj
c-interpreter.exe: CMakeFiles/c-interpreter.dir/build.make
c-interpreter.exe: CMakeFiles/c-interpreter.dir/linkLibs.rsp
c-interpreter.exe: CMakeFiles/c-interpreter.dir/objects1.rsp
c-interpreter.exe: CMakeFiles/c-interpreter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking C executable c-interpreter.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\c-interpreter.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/c-interpreter.dir/build: c-interpreter.exe
.PHONY : CMakeFiles/c-interpreter.dir/build

CMakeFiles/c-interpreter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\c-interpreter.dir\cmake_clean.cmake
.PHONY : CMakeFiles/c-interpreter.dir/clean

CMakeFiles/c-interpreter.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\test\CLionProjects\C-Interpreter C:\Users\test\CLionProjects\C-Interpreter C:\Users\test\CLionProjects\C-Interpreter\build C:\Users\test\CLionProjects\C-Interpreter\build C:\Users\test\CLionProjects\C-Interpreter\build\CMakeFiles\C-Interpreter.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/c-interpreter.dir/depend

