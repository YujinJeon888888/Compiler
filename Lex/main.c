#include "tn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* yylex, yytext, . . : 외부에 정의되어있다는 걸 알려줘야함  - extern키워드 */
extern char* yytext;
extern enum EToken yylex();
/* 외부선언 함수임을 선언 */
extern void init_sym_table();
extern void print_sym_table();

/* 전역 변수 */
int line_number = 1; // 현재 라인 번호

/* 토큰 타입을 문자열로 변환 */
const char* get_token_type(enum EToken token) {
    switch (token) {
        case TCONST: return "TCONST";
        case TELSE: return "TELSE";
        case TIF: return "TIF";
        case TINT: return "TINT";
        case TRETURN: return "TRETURN";
        case TVOID: return "TVOID";
        case TWHILE: return "TWHILE";
        case TEQUAL: return "TEQUAL";
        case TNOTEQU: return "TNOTEQU";
        case TLESSE: return "TLESSE";
        case TGREATE: return "TGREATE";
        case TAND: return "TAND";
        case TOR: return "TOR";
        case TINC: return "TINC";
        case TDEC: return "TDEC";
        case TADDASSIGN: return "TADDASSIGN";
        case TSUBASSIGN: return "TSUBASSIGN";
        case TMULASSIGN: return "TMULASSIGN";
        case TDIVASSIGN: return "TDIVASSIGN";
        case TMODASSIGN: return "TMODASSIGN";
        case TMULT: return "TMULT";
        case TPLUS: return "TPLUS";
        case TMINUS: return "TMINUS";
        case TASSIGN: return "TASSIGN";
        case TNOT: return "TNOT";
        case TLESS: return "TLESS";
        case TGREATER: return "TGREATER";
        case TLPAREN: return "TLPAREN";
        case TRPAREN: return "TRPAREN";
        case TCOMMA: return "TCOMMA";
        case TLBRACE: return "TLBRACE";
        case TRBRACE: return "TRBRACE";
        case TLBRACKET: return "TLBRACKET";
        case TRBRACKET: return "TRBRACKET";
        case TSEMICOLON: return "TSEMICOLON";
        case TIDENT: return "TIDENT";
        case TNUMBER: return "TNUMBER";
        case TOCTAL: return "TOCTAL";
        case THEX: return "THEX";
        case TFLOAT: return "TFLOATNUM";
        case TCHAR: return "TCHAR";
        case TERROR: return "**Error**";
        default: return "unknown";
    }
}

void print_error_message(const char* message) {
    printf("Error - Invalid character (%s)\n", message);
}

void main() {
    enum EToken ET;
    int st_index = -1;

    init_sym_table();
    printf("  Start of Lex\n");
    printf("%-8s%-12s%-8s%s\n", "Line", "Token type", "ST-index", "Token");

    while ((ET = yylex()) != TEOF) {
        switch (ET) {
        case TIDENT:
            st_index = process_sym_table(yytext);
            if (st_index == -1) {
                printf("%-8d%-12s%-8s%s (already exists)\n", line_number, get_token_type(ET), "", yytext);
            }
            else {
                printf("%-8d%-12s%-8d%s\n", line_number, get_token_type(ET), st_index, yytext);
            }
            break;
        case TSTRING:
        case TNUMBER:
        case TOCTAL:
        case THEX:
        case TFLOAT:
        case TCHAR:
        case TSEMICOLON:
        case TPLUS:
        case TMINUS:
        case TASSIGN:
        case TNOT:
        case TLESS:
        case TGREATER:
        case TLPAREN:
        case TRPAREN:
        case TCOMMA:
        case TLBRACE:
        case TRBRACE:
        case TLBRACKET:
        case TRBRACKET:
        case TMULT:
        case TERROR:
            printf("%-8d%-12s%-8s%s\n", line_number, get_token_type(ET), "", yytext);
            break;

        default:
            printf("%-8d%-12s%-8s%s\n", line_number, get_token_type(ET), "", yytext);
            break;
        }

        // 라인 번호 증가 로직 개선
        for (int i = 0; yytext[i] != '\0'; i++) {
            if (yytext[i] == '\n') {
                line_number++;
            }
        }
    }

    print_sym_table();
}
