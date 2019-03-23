typechecker: typechecker.tab.cpp lex.yy.o
	g++ -g typechecker.tab.cpp lex.yy.o include/*.cpp -o typechecker
typechecker.tab.cpp: typechecker.ypp
	bison -d -g --verbose --debug typechecker.ypp
lex.yy.o: typechecker.l typechecker.tab.cpp
	lex typechecker.l
	gcc -c -g lex.yy.c -o lex.yy.o

clean:
	rm -f lex.yy.c lex.yy.o typechecker.dot typechecker.output typechecker.tab.cpp typechecker.tab.hpp typechecker 