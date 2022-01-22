CPPCOMPILER=g++
CPPFLAGS=-std=c++17 -O2 -Wall -Wno-c++17-compat
FILES=*.cpp

all:
	$(CPPCOMPILER) $(CPPFLAGS) $(FILES) -o main.out && ./main.out

clean:
	rm *.out
