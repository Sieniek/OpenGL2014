LIBS=-DdDOUBLE -lGL -lGLU -lglut -lode -lpthread 
CC=g++

SOURCES=main_file.cpp tga.cpp
HEADERS=tga.h surface.h
OBJECTS=$(SOURCES:.cpp=.o)

all: main_file

main_file: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o main_file
