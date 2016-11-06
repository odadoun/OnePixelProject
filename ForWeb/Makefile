CXX = g++
CXXFLAGS = -O3 -ansi -Wall
DEBUGFLAGS = -g -ansi -Wall
INCLUDE = -I./TheReaderUniverse

## For OpenGL Utility Toolkit (GLUT)
# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE += -I/usr/local/include -I/opt/X11/include
#
# # LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib
#
# LINKER_FLAGS specifies the libraries we're linking against
# # Cocoa, IOKit, and CoreVideo are needed for static GLFW3.
LINKER_FLAGS = -framework OpenGL -lglfw3 -lglew -framework GLUT
## 

#Link command:
reader: main.o TheReaderUniverse.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) main.o TheReaderUniverse.o $(LINKER_FLAGS) -o reader

#Compilation commands:
main.o: main.C
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBRARY_PATHS) -c main.C  -o main.o

TheReaderUniverse.o: TheReaderUniverse/TheReaderUniverse.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c  TheReaderUniverse/TheReaderUniverse.cpp -o TheReaderUniverse.o


#make clean
clean:
	rm -f *.o
