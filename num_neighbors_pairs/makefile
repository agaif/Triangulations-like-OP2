CC=g++
CFLAGS= -c -std=c++11
LDFLAGS=
SOURCES=num_neighbors_pairs.cpp ../common/permutation.cpp ../common/myiofunctions.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=num_neighbors_pairs

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS)  $<  -o $@ 

clean:
	rm -rf $(EXECUTABLE) *.o

clean_all:
	rm -rf $(EXECUTABLE) *.o ../common/*.o