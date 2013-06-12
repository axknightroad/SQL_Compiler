# sql分析器的make file
CC =gcc -g
LEX=flex
YACC=bison
PROGRAMS =SQLComplier_v0.1

all: ${PROGRAMS}

SQLComplier_v0.1:sql.lex.l sql.parsing.y sql.struct.h
  	bison -d sql.parsing.y
		flex -o sql.lex.c sql.lex.l
		gcc -g -o $@ sql.parsing.tab.c sql.lex.c sql.function.c
