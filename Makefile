parser: y.tab.c lex.yy.c
	gcc y.tab.c lex.yy.c -o parser
y.tab.c: parser.y
	yacc -d -g --verbose --debug parser.y
lex.yy.c: parser.l y.tab.c
	lex parser.l

clean:
	rm -f lex.yy.c y.tab.c