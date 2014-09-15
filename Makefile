
LIBS=-DdDOUBLE -lGL -lGLU -lglut -lode -lpthread

main_file: main_file.cpp
	g++ -o main_file main_file.cpp $(LIBS)