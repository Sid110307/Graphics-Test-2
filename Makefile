CC=clang++
EXECUTABLE_DIR=bin

CPPFLAGS=-std=c++20 -pedantic -g
CPPWARNINGS=-Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter
LIBS=-lGL -lGLU -lglut -lGLEW -lassimp -lSOIL

.PHONY: all
all: graphicsTest2

graphicsTest2: src/main.cpp
	mkdir -p $(EXECUTABLE_DIR)
	$(CC) $(CPPFLAGS) $(CPPWARNINGS) -o $(EXECUTABLE_DIR)/$@ $< $(LIBS)
	chmod +x $(EXECUTABLE_DIR)/$@
	./$(EXECUTABLE_DIR)/$@

clean:
	rm -rf $(EXECUTABLE_DIR)
