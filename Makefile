CC=g++ -g
LIB=-lsfml-graphics -lsfml-window -lsfml-system

all: main

main: *.o
	$(CC) $^ -o $@ $(LIB)

%.o: %.cpp
	$(CC) -c $<

clean:
	rm *.o .*.sw*

main.o: Descripteur.h Function.h Console.h Element.h Material.h
Descripteur.o: Descripteur.h Function.h
Function.o: Function.h Attribute.h
Console.o: Console.h
Element.o: Element.h
Attribute.o: Attribute.h
Material.o: Material.h Attribute.h
Gaz.o: Gaz.h Element.h
