build: test_xml_parser.run

test_xml_parser.run: main.o parser.o
	g++ main.o parser.o -o $@

main.o: main.cpp
	g++ -c main.cpp

parser.o: parser.cpp parser.hpp
	g++ -c parser.cpp



run: test_xml_parser.run
	./test_xml_parser.run

.PHONY: clean

clean:
	rm -f *.o