PRG=../prebuild.exe
GCC=x86_64-w64-mingw32-g++
GCCFLAGS=-std=c++20 -static-libgcc -static-libstdc++ -O2 -pedantic-errors -Wall -Wextra -Werror 

DRIVER0=prebuild.cpp

VALGRIND_OPTIONS=-q --leak-check=full

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(GCCFLAGS)
clean:
	rm -f *.exe *.tds *.o *.obj *manifest* diff*
