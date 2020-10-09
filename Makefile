CC=g++ -g
LIB=-lsfml-graphics -lsfml-window -lsfml-system

all: main

main: GraphManager.o MaterialArch.o MatArchNode.o  MaterialArchView.o MatArchNodeView.o Element.o  Console.o Function.o Material.o Descripteur.o main.o

	$(CC) $^ -o $@ $(LIB)

%.o: %.cpp
	$(CC) -c $<

clean:
	rm *.o .*.sw*

main.o: Descripteur.h Function.h Console.h  Element.h Material.h
Descripteur.o: Descripteur.h Function.h
Function.o: Function.h #Attribute.h
Console.o: Console.h
Element.o: Element.h
#Attribute.o: Attribute.h
Material.o: Material.h # Attribute.h
MaterialArch.o: MaterialArch.h MatArchNode.h
MatArchNode.o: MatArchNode.h
MaterialArchView.o: MaterialArchView.h MatArchNodeView.h
MatArchNodeView.o: MaterialArchView.h
GraphManager.o: GraphManager.h
##Gaz.o: Gaz.h Element.h
