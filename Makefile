LIBS=-DdDOUBLE -lGL -lGLU -lglut -lGLEW -lode -lpthread 
CC=g++

SOURCES=main_file.cpp tga.cpp shaderprogram.cpp
HEADERS=tga.h surface.h shaderprogram.h
OBJECTS=$(SOURCES:.cpp=.o)

all: main_file

main_file: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o main_file
