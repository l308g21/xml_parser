CC=g++
C_FLAGS=-g -Wall

build: test_xml_parser.run

test_xml_parser.run: main.o parser.o
	$(CC) $(C_FLAGS) main.o parser.o -o $@

main.o: main.cpp
	$(CC) $(C_FLAGS) -c main.cpp

parser.o: parser.cpp parser.hpp
	$(CC) $(C_FLAGS) -c parser.cpp



run: test_xml_parser.run
	./test_xml_parser.run

.PHONY: clean

clean:
	rm -f *.o *.run