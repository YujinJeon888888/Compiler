#include <stdio.h>
#include <stdlib.h>
#include "tn.h"

extern yylex();
extern char* yytext;
extern void init_sym_table(void);
extern void print_sym_table(void);
extern void print_hash_table(void);

void main()
{
	enum tnumber tn;  // token number

	init_sym_table();

	printf("  Start of Lex\n");
	while ((tn = yylex()) != TEOF) {
		switch (tn) {
		case TCONST: printf("const\n");  break;
		case TELSE: printf("else\n");  break;
		case TIF: printf("if\n");  break;
		case TINT: printf("int\n");  break;
		case TRETURN: printf("return\n");  break;
		case TVOID: printf("void\n");  break;
		case TWHILE: printf("while\n");  break;
		case TEQUAL: printf("==\n");  break;
		case TNOTEQU: printf("!=\n");  break;
		case TLESSE: printf("<=\n");  break;
		case TGREATE: printf(">=\n");  break;
		case TAND: printf("&&\n");  break;
		case TOR: printf("||\n");  break;
		case TINC: printf("++\n");  break;
		case TDEC: printf("--\n");  break;
		case TADDASSIGN: printf("+=\n");  break;
		case TSUBASSIGN: printf("-=\n");  break;
		case TMULASSIGN: printf("*=\n");  break;
		case TDIVASSIGN: printf("/=\n");  break;
		case TMODASSIGN: printf("%=\n");  break;
		case TIDENT: printf("identifier (%s)\n", yytext);  break;
		case TNUMBER: printf("number (%s)\n", yytext);  break;
		default: printf("%s", yytext); break;
		}
	}

	print_sym_table();
	print_hash_table();
}

void PrintToken() {

}
