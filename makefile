CC = g++

all: sfml-app 

sfml-app: main.o 
	${CC} -std=gnu++0x main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system 
	
main.o: main.cpp
	${CC} -c main.cpp

clean: 
	rm -rf *.o sfml-app
