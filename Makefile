CFLAGS=-std=c++17 -pedantic -Wall -Wextra -Wno-unknown-pragmas
LIBS=-lGL -lGLU -lglut

.PHONY: all
all: graphicsTest

graphicsTest: src/main.cpp
	mkdir -p bin
	clang++ $(CFLAGS) -o bin/$@ $< $(LIBS)
	./bin/$@

clean:
	rm -rf bin/*
