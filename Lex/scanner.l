%{
#include <stdio.h>
#include <stdlib.h>
#include "tn.h"
int process_sym_table(char*);
void print_error(int errno);
extern int line_number; 
%}

%%


"/*"([^*]|\*+[^*/])*\**"*/" {
    for (int i = 0; yytext[i] != '\0'; i++) {
        if (yytext[i] == '\n') {
            line_number++;
        }
    }
}
"//".*							;

"const"						return(TCONST);
"else"						return(TELSE);
"if"						return(TIF);
"int"						return(TINT);
"return"					return(TRETURN);
"void"						return(TVOID);
"while"						return(TWHILE);
"float"					return(TFLOAT);
"char"					return(TCHAR);

"=="						return(TEQUAL);
"!="						return(TNOTEQU);
"<="						return(TLESSE);
">="						return(TGREATE);
"&&"						return(TAND);
"||"						return(TOR);
"++"						return(TINC);
"--"						return(TDEC);
"+="						return(TADDASSIGN);
"-="						return(TSUBASSIGN);
"*="						return(TMULASSIGN);
"/="						return(TDIVASSIGN);
"%="						return(TMODASSIGN);
"*"							return(TMULT);
"+"							return(TPLUS);
"-"							return(TMINUS);
"="							return(TASSIGN);
"!"							return(TNOT);
"<"							return(TLESS);
">"							return(TGREATER);
"("							return(TLPAREN);
")"							return(TRPAREN);
","							return(TCOMMA);
"{"							return(TLBRACE);
"}"							return(TRBRACE);
"["							return(TLBRACKET);
"]"							return(TRBRACKET);
";"							return(TSEMICOLON);

[0-9]+[A-Za-z_][A-Za-z0-9_]*		{ 
	print_error(1);
	return(TERROR);
}

[A-Za-z_][A-Za-z0-9_]{16,}		{
	print_error(2);
	return(TERROR);
}

[A-Za-z_][A-Za-z0-9_]*		{
		return(TIDENT);
}

[+-]?0[xX][0-9A-Fa-f]+		return(THEX);
[+-]?0[0-7]*				return(TOCTAL);
[+-]?[0-9]*\.[0-9]+([eE][+-]?[0-9]+)?	return(TFLOAT);
[+-]?[0-9]+				return(TNUMBER);
"'"([^'\\]|\\.)"'"			return(TCHAR);

\"([^\"\\]|\\.)*\"			return(TSTRING);

[ \t]+    ;
\n        { line_number++; }

.							{
	print_error(3);
	return(TERROR);
}

%%

int yywrap() {
	return 1;
}
