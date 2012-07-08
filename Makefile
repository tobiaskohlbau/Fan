#
#    Created on: 07.07.2012
#    Author: Tobias Kohlbau <tobias.kohlbau@gmail.com>
#


PROJECTNAME=fan
CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp Control.cpp Sensor.cpp Fan.cpp Load.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=$(PROJECTNAME)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
