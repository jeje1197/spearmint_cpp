output: main.o Exception.o Token.o Lexer.o Parser.o
	g++ main.o Exception.o Token.o Lexer.o Parser.o -o output

main.o: main.cpp
	g++ -c main.cpp

Exception.o: Exception.cpp Exception.h
	g++ -c Exception.cpp

Token.o: Token.cpp Token.h
	g++ -c Token.cpp

Lexer.o: Lexer.cpp Lexer.h
	g++ -c Lexer.cpp

Parser.o: Parser.cpp Parser.h
	g++ -c Parser.cpp