mjavac: mjavac.tab.cpp lex.yy.o
	g++ -g mjavac.tab.cpp lex.yy.o include/asm/*.cpp include/ast/*.cpp -o mjavac
mjavac.tab.cpp: mjavac.ypp
	bison -d -g --verbose --debug mjavac.ypp
lex.yy.o: mjavac.l mjavac.tab.cpp
	lex mjavac.l
	gcc -c -g lex.yy.c -o lex.yy.o

clean:
	rm -f lex.yy.c lex.yy.o mjavac.dot mjavac.output mjavac.tab.cpp mjavac.tab.hpp mjavac
