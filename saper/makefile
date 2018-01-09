saper : main.o cell.o cellfield.o
	g++ main.o cell.o cellfield.o -o saper -lsfml-graphics -lsfml-window -lsfml-system
main.o : main.cpp
	g++ -c main.cpp -std=c++17
cell.o : cell.cpp
	g++ -c cell.cpp -std=c++17
cellfield.o : cellfield.cpp
	g++ -c cellfield.cpp -std=c++17
clean :
	-rm cell.o
	-rm main.o
	-rm cellfield.o
