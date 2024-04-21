all:
	g++ -Isrc/Include -Lsrc/lib -Isrc/game -o game main.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf