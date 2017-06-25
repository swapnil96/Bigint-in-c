bignumcalc: tokenizer.l parser.y bignumcalc.c
	bison -d parser.y
	flex tokenizer.l
	cc -o bignumcalc parser.tab.c lex.yy.c -lfl bignumcalc.c

clean:
	rm lex.yy.c parser.tab.c parser.tab.h bignumcalc
