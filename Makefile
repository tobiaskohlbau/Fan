PROJECTNAME=fan
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp Fan.cpp Control.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(PROJECTNAME)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
