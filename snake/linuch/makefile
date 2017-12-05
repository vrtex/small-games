wunsz: main.o game.o gamestate.o wunsz.o
	g++ -std=c++17 main.o game.o gamestate.o wunsz.o -o wunsz -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp
	g++ -c -std=c++17 main.cpp
game.o: game.cpp
	g++ -c -std=c++17 game.cpp
gamestate.o: gamestate.cpp
	g++ -c -std=c++17 gamestate.cpp
wunsz.o: wunsz.cpp
	g++ -std=c++17 -c wunsz.cpp
