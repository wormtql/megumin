# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\CLionProjects\megumin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\CLionProjects\megumin\cmake-build-release

# Include any dependencies generated for this target.
include src\playground\CMakeFiles\main2.dir\depend.make
# Include the progress variables for this target.
include src\playground\CMakeFiles\main2.dir\progress.make

# Include the compile flags for this target's objects.
include src\playground\CMakeFiles\main2.dir\flags.make

src\playground\CMakeFiles\main2.dir\main2.cpp.obj: src\playground\CMakeFiles\main2.dir\flags.make
src\playground\CMakeFiles\main2.dir\main2.cpp.obj: ..\src\playground\main2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\CLionProjects\megumin\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/playground/CMakeFiles/main2.dir/main2.cpp.obj"
	cd E:\CLionProjects\megumin\cmake-build-release\src\playground
	"D:\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\bin\Hostx86\x64\cl.exe" @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\main2.dir\main2.cpp.obj /FdCMakeFiles\main2.dir\ /FS -c E:\CLionProjects\megumin\src\playground\main2.cpp
<<
	cd E:\CLionProjects\megumin\cmake-build-release

src\playground\CMakeFiles\main2.dir\main2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main2.dir/main2.cpp.i"
	cd E:\CLionProjects\megumin\cmake-build-release\src\playground
	"D:\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\bin\Hostx86\x64\cl.exe" > CMakeFiles\main2.dir\main2.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\CLionProjects\megumin\src\playground\main2.cpp
<<
	cd E:\CLionProjects\megumin\cmake-build-release

src\playground\CMakeFiles\main2.dir\main2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main2.dir/main2.cpp.s"
	cd E:\CLionProjects\megumin\cmake-build-release\src\playground
	"D:\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\bin\Hostx86\x64\cl.exe" @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\main2.dir\main2.cpp.s /c E:\CLionProjects\megumin\src\playground\main2.cpp
<<
	cd E:\CLionProjects\megumin\cmake-build-release

# Object files for target main2
main2_OBJECTS = \
"CMakeFiles\main2.dir\main2.cpp.obj"

# External object files for target main2
main2_EXTERNAL_OBJECTS =

src\playground\main2.exe: src\playground\CMakeFiles\main2.dir\main2.cpp.obj
src\playground\main2.exe: src\playground\CMakeFiles\main2.dir\build.make
src\playground\main2.exe: src\arm\arm_asm.lib
src\playground\main2.exe: src\megumin\megumin.lib
src\playground\main2.exe: src\arm\arm_asm.lib
src\playground\main2.exe: src\playground\CMakeFiles\main2.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\CLionProjects\megumin\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable main2.exe"
	cd E:\CLionProjects\megumin\cmake-build-release\src\playground
	"D:\JetBrains\CLion 2021.2.1\bin\cmake\win\bin\cmake.exe" -E vs_link_exe --intdir=CMakeFiles\main2.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\mt.exe --manifests -- "D:\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.32.31326\bin\Hostx86\x64\link.exe" /nologo @CMakeFiles\main2.dir\objects1.rsp @<<
 /out:main2.exe /implib:main2.lib /pdb:E:\CLionProjects\megumin\cmake-build-release\src\playground\main2.pdb /version:0.0 /machine:x64 /INCREMENTAL:NO /subsystem:console  ..\arm\arm_asm.lib ..\megumin\megumin.lib ..\arm\arm_asm.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<
	cd E:\CLionProjects\megumin\cmake-build-release

# Rule to build all files generated by this target.
src\playground\CMakeFiles\main2.dir\build: src\playground\main2.exe
.PHONY : src\playground\CMakeFiles\main2.dir\build

src\playground\CMakeFiles\main2.dir\clean:
	cd E:\CLionProjects\megumin\cmake-build-release\src\playground
	$(CMAKE_COMMAND) -P CMakeFiles\main2.dir\cmake_clean.cmake
	cd E:\CLionProjects\megumin\cmake-build-release
.PHONY : src\playground\CMakeFiles\main2.dir\clean

src\playground\CMakeFiles\main2.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" E:\CLionProjects\megumin E:\CLionProjects\megumin\src\playground E:\CLionProjects\megumin\cmake-build-release E:\CLionProjects\megumin\cmake-build-release\src\playground E:\CLionProjects\megumin\cmake-build-release\src\playground\CMakeFiles\main2.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : src\playground\CMakeFiles\main2.dir\depend

