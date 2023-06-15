NCURSES_FLAGS = -lncurses -DNCURSES_STATIC

main: main.cpp SnakeGame.o Snake.o Wall.o
	g++ -o main.exe main.cpp SnakeGame.o Snake.o Wall.o $(NCURSES_FLAGS)

Wall.o: Wall.cpp Wall.h
	g++ -c Wall.cpp $(NCURSES_FLAGS)

Snake.o: Snake.cpp Snake.h
	g++ -c Snake.cpp $(NCURSES_FLAGS)

SnakeGame.o: SnakeGame.cpp SnakeGame.h Snake.h
	g++ -c SnakeGame.cpp $(NCURSES_FLAGS)
