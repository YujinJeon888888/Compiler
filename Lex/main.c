#include "tn.h"
#include <stdio.h>
#include <stdlib.h>

/*yylex, yytext, . . : 외부에 정의되어있다는 걸 알려줘야함  - extern키워드*/
extern char* yytext;
extern enum tnumber yylex();
/*외부선언 함수임을 선언*/
extern void init_sym_table();
extern void print_sym_table();

void main()
{
	enum EToken ET;  // token number
	//토큰읽기 전 symbol table을 초기화
	init_sym_table();
	printf("  Start of Lex\n");
	
	while ((ET = yylex()) != TEOF) {
		switch (ET) {
		case TCONST:      printf("Token: TCONST (const)\n"); break;
		case TELSE:       printf("Token: TELSE (else)\n"); break;
		case TIF:         printf("Token: TIF (if)\n"); break;
		case TINT:        printf("Token: TINT (int)\n"); break;
		case TRETURN:     printf("Token: TRETURN (return)\n"); break;
		case TVOID:       printf("Token: TVOID (void)\n"); break;
		case TWHILE:      printf("Token: TWHILE (while)\n"); break;
		case TEQUAL:      printf("Token: TEQUAL (==)\n"); break;
		case TNOTEQU:     printf("Token: TNOTEQU (!=)\n"); break;
		case TLESSE:      printf("Token: TLESSE (<=)\n"); break;
		case TGREATE:     printf("Token: TGREATE (>=)\n"); break;
		case TAND:        printf("Token: TAND (&&)\n"); break;
		case TOR:         printf("Token: TOR (||)\n"); break;
		case TINC:        printf("Token: TINC (++)\n"); break;
		case TDEC:        printf("Token: TDEC (--)\n"); break;
		case TADDASSIGN:  printf("Token: TADDASSIGN (+=)\n"); break;
		case TSUBASSIGN:  printf("Token: TSUBASSIGN (-=)\n"); break;
		case TMULASSIGN:  printf("Token: TMULASSIGN (*=)\n"); break;
		case TDIVASSIGN:  printf("Token: TDIVASSIGN (/=)\n"); break;
		case TMODASSIGN:  printf("Token: TMODASSIGN (%%=)\n"); break;
		case TIDENT:      printf("Token: TIDENT (identifier): %s\n",yytext); break;
		case TNUMBER:     printf("Token: TNUMBER (number)\n"); break;
		default:          printf("Unknown token\n"); break;
		}
	}

	print_sym_table();
}