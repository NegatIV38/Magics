CC=g++ -g
LIB=-lsfml-graphics -lsfml-window -lsfml-system

all: packs main

packs:
	+$(MAKE) -C Attributes
	+$(MAKE) -C Magics
	+$(MAKE) -C Physics
	+$(MAKE) -C System

main: System/GraphManager.o Physics/MaterialPop.o Physics/MaterialArch.o Physics/MatArchNode.o  Physics/MaterialArchView.o Physics/MatArchNodeView.o Physics/Element.o  System/Console.o Magics/Function.o Magics/Descripteur.o main.o

	$(CC) $^ -o $@ $(LIB)

%.o: %.cpp
	$(CC) -c $<

clean:
	find . -name \*.sw* -type f -delete
	find . -name \*.o -type f -delete
	

main.o:
