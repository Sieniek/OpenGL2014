LIBS=-lGL -lglut -DdDOUBLE -lGL -lGLU -lglut -lode -lpthread

main_file: main_file.cpp ball.cpp
	g++ -o main_file ball.cpp main_file.cpp $(LIBS)
