%{
/*lex source for calculator programC*/
#include <stdio.h>
#include <stdlib.h>
/*cal.y와 cal.l에서 사용하는 토큰을 일치시키기 위함*/
#include "cal_tab.h"

%}
%%
[a-z]*	{return NAME;}
[0-9]*	{yylval = atoi(yytext); return NUMBER;}
[ \t]		;
\n		|
.		return yytext[0];
%%

int yywrap() {
	return 1;
}