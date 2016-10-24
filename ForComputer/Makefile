CXX = g++
CXXFLAGS = -O3 -ansi -Wall
DEBUGFLAGS = -g -ansi -Wall
INCLUDE = -I./TheReaderUniverse

#Link command:
reader: main.o TheReaderUniverse.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) main.o TheReaderUniverse.o -o reader

#Compilation commands:
main.o: main.C
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c main.C -o main.o

TheReaderUniverse.o: TheReaderUniverse/TheReaderUniverse.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c  TheReaderUniverse/TheReaderUniverse.cpp -o TheReaderUniverse.o


#make clean
clean:
	rm -f *.o
