#include <stdio.h>
#include <stdlib.h>

extern int yyparse(void);
extern char* yytext;
extern void init_sym_table(void);
extern void print_sym_table(void);
extern void print_hash_table(void);

int line_num = 1;

int main(void) {
    init_sym_table();
    yyparse();
    print_sym_table();
    print_hash_table();
    return 0;
}
