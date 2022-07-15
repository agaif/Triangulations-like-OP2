CC=g++
CFLAGS= -c -std=c++11
LDFLAGS=
SOURCES=allsimp.cpp ../common/permutation.cpp ../common/myiofunctions.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=allsimp

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS)  $<  -o $@ 

clean:
	rm -rf $(EXECUTABLE) *.o

clean_all:
	rm -rf $(EXECUTABLE) *.o ../common/*.o