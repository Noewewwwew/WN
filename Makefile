main: main.cpp SnakeGame.cpp SnakeGame.h
	g++ -o main main.cpp SnakeGame.cpp SnakeGame.h -lncurses -DNCURSES_STATIC
