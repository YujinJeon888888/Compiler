%{
#include <stdio.h>
#include <ctype.h>
#include <malloc.h>

void yyerror(const char* s);
extern int yyparse();
%}

%token NAME NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%%
stat		: stat '\n' 
		| lambda
		|NAME '=' expr '\n' 
		| expr '\n'
			{
				/*NAME '=' expr에 대해서는 action code가 없음*/
				printf("=%d\n", $1);
			}
		;
expr		: expr '+' expr 	{$$ = $1 + $3;}
		| expr '-' expr 		{$$ = $1 - $3;}
		| expr '*' expr 		{$$ = $1 * $3;}
		| expr '/' expr 		{if ($3 ==0) yyerror("divide by zero");
						else $$ = $1 / $3;}
		| '-' expr %prec UMINUS	{$$ = -$2;}
		| NUMBER 
		;
lambda	:
		;
%%

void yyerror(const char* s)
{	
	printf("%s\n",s);
	return -1;
}

int main(){
	while(1){
		return yyparse();
	}
	
}
