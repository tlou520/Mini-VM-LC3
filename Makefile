CC = cl
CFLAGS = /std:c++20 /Zi /Fd"lc3vm.pdb" /nologo /W4 /EHsc /I include /FI"common.hpp"
LDFLAGS = /DEBUG

SRC = src\main.cpp src\cpu.cpp src\io.cpp src\loader.cpp src\memory.cpp src\util.cpp src\instructions.cpp
OBJ_DIR = build
OBJ = $(SRC:src\=build\)
OBJ = $(OBJ:.cpp=.obj)

.PHONY: all clean

all: clean lc3vm.exe   # <-- clean runs first

lc3vm.exe: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) /Fe$@ /link /DEBUG /PDB:lc3vm.pdb /INCREMENTAL:NO

# regla de transformación: de .cpp en src a .obj en build
{src}.cpp{build}.obj:
	$(CC) $(CFLAGS) /c $< /Fo$@

clean:
	if exist *.pdb del /Q *.pdb
	if exist *.exe del /Q *.exe
	if exist $(OBJ_DIR)\*.obj del /Q $(OBJ_DIR)\*.obj

