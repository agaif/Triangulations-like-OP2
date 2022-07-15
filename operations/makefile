CC=g++
CFLAGS= -c -std=c++11
LDFLAGS=
SOURCES=operations.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=operations

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS)  $<  -o $@ 

clean:
	rm -rf $(EXECUTABLE) *.o
