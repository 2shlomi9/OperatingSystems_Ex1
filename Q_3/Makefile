CXX=gcc
CXXFLAGS= -Wall -g

SOURCES = polyencoder.c
HEADERS = polyencoder.h

main: polyencoder.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o main

%.o: %.c $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@

clean:
	rm -f *.o main
