#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define SYM_TABLE_SIZE  100

char separators[] = " ,;\t\n\r\n";
char str_pool[SYM_TABLE_SIZE];

#define TRUE    1
#define FALSE   0

int main() {
    FILE* fp;
    int result;
    int c; // 읽은 문자를 저장할 변수
    /*string pool에 저장되는 문자는 구분자가 들어올 때까지*/
    int index_start = 0;
    int index_next = 0;
    int index = 0;

    result = fopen_s(&fp, "example.txt", "r"); // "example.txt" 파일을 읽기 모드로 열기
    if (result != 0) {
        printf("파일 열기 실패(%d)\n", result);
        return -1;
    }

    /*만약 idenfifier형식에 맞지 않을 경우 - 에러처리*/
    int err_flag = FALSE;

    do { // 파일 끝까지 문자 읽기
        c = fgetc(fp); //리턴형: int (char로 자료형하면 char에 맞게 나옴)

        /*구분자가 들어오거나 || 파일 끝일 때 */
        if ((strchr(separators, c) != NULL) || c == EOF) {//strchr: 문자열 내에 일치하는 문자가 있는지 검사하는 함수
            str_pool[index_next++] = '\0'; //널 문자 추가 (string pool 안에서는 '\0'으로 한 자 띄워서 다른 변수 구분하므로.
            if (err_flag != TRUE) {//정상이면
                printf("%s\n", str_pool + index_start); //?: 왜 +index_start하지? 이게 무슨의미? : %s는, '\0'까지 출력하고 끝남. 따라서 str_pool 만 출력하면 This만 계속 출력될 것. 따라서 +start_index를 통해서 시작 위치를 옮겨서, 다음 단어가 출력될 수 있도록 조정하는 것임.
                index_start = index_next; //스타트 위치를 넥스트로 바꿈
            }
            else { //오류문자 쌓인 게 여기 출력 
                printf("Error - Invalid identifier (%s)\n", str_pool + index_start);
                index_next = index_start;//start로 되돌림: 오류문자 str_pool에 담겨있던 것을 덮어쓰기 하기 위해서
                err_flag = FALSE;//에러처리 초기화
            }
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        {
            str_pool[index_next++] = (char)c;
        }
        else { //!, & , . .다른 기호들
            str_pool[index_next++] = (char)c;
            err_flag = TRUE;//오류!
        }
    } while (c != EOF); //끝이 아닐 때까지

    fclose(fp); // 파일 닫기
    return 0;
}
