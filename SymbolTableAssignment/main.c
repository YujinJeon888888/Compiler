/*****************************************************************
 * \file   main.c
 * \brief 심볼테이블 구현 과제
 * 
 * 
 * \author 전유진
 * \date   March 2025
 *********************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> // isalnum 함수를 사용하기 위해 포함

#define STRING_SIZE 16 //15 + 1('\0')
#define STRING_POOL_SIZE 20 
#define TABLE_SIZE 20
#define TRUE 1
#define FALSE 0
#define MAX_SYMBOL_LENGTH 15
#define FILE_BUFFER_SIZE 256
//고유 아이디 부여하기 위해 전역변수 정의
static int HashID = 100;

/**
 * @brief string 타입 만들기
 * @detail 변수도 []를 변수에 붙이니까 별칭도 []를 별칭에 붙임 (자바랑 헷갈리기x)
 */
typedef char string[STRING_SIZE];  

/**
 * @brief 해시테이블 엔티티.
 */
typedef struct HTentry* HTpointer; //HTpointer==HTentry. 같은의미
typedef struct HTentry {
	string Name;
	int ID;
	int Index;
	int Length;
	HTpointer next;//충돌 시 후방법 쓰기 위함
}HTentry;

/**
 * @brief 심볼 노드.
 */
typedef struct STEntry{
	int ID;
	int StringPoolIndex;
	int Length;
}STpointer;

//해시테이블
HTpointer HT[TABLE_SIZE] = { 0 }; //HT는 매크로로 정의한 사이즈만큼. HT은 HTpointer타입의 1차원 배열로 존재한다.
//심볼테이블
STpointer ST[TABLE_SIZE] = { 0 };

/**
 * @brief string length를 반환하는 함수
 * @param string
 * @return string length
 */
int StringLength(string S) {
	int Count = 0;
	while (TRUE) {
		if (S[Count] == '\0') {
			break;
		}
		Count++;
	}
	return Count;
}
/**
 * @brief 해시코드 리턴하는 함수.
 * @param symbol에 해당하는 string
 * @return 구해진 해시코드 값
 */
int GetHashCode(string String) {
	int fx = 0;
	
	for (int i = 0; i < StringLength(String); i++){
		fx += String[i];
	}

	return (fx%TABLE_SIZE) + 1;
}

/**
 * @brief 해시테이블에 해당 심볼이 이미 있는지 확인.
 * @param symbol의 string pool내에서의 인덱스
 * @param 심볼
 * @return  찾고자 하는 symbol이 이미 존재할 경우 symbol이 저장된 HTpointer 반환
 *  없을경우 NULL반환
 */
HTpointer LookUpHashTable(int Index, string Symbol) {
	//hscode구하기
	int HScode = GetHashCode(Symbol);
	//HT에 hscode위치에 값이 존재하는가?
	HTpointer entry = HT[HScode];

	//체이닝된 리스트를 탐색
	/*비교작업: 있으면 찾은 항목 반환, 없으면 NULL반환 */
	while (entry != NULL) {
		//Symbol과 엔티티 이름이 같은가?
		if (strcmp(entry->Name, Symbol)==0) {
			return entry;
		}
		//옆으로 이동: NULL일때까지
		entry = entry->next;
	}
	return NULL;
}

/**
 * @brief 새로운 노드를 해시테이블에 추가함.
 * @param 해시코드
 * @param 심볼
 * @param 심볼의 스트링풀 내 인덱스위치
 */
void AddHashTable(int HScode,string String ,int StringPoolIndex) {
	//객체 생성
	HTpointer NewEntry = (HTpointer)malloc(sizeof(HTentry));
	//메모리할당 실패 예외처리
	if (NewEntry == NULL) {
		printf("메모리 할당 실패\n");
		//강종
		exit(1);
	}
	
	//속성채우기
	NewEntry->Index= StringPoolIndex;
	NewEntry->ID = HashID++;
    strcpy(NewEntry->Name, String);
	NewEntry->Length = StringLength(String);
	NewEntry->next = NULL;

	/*해시테이블에 노드 넣어주기*/
	//HT의 그 자리가 비어있으면, 첫 항목으로 추가
	if (HT[HScode] == NULL) {
		HT[HScode] = NewEntry;
	}
	//HT의 그 자리를 누가 이미 차지하고 있으면, 후방법으로 충돌처리
	else {
		/**
		 * @todo 후방법처리.
		 */
		HTpointer current = HT[HScode];
		while (current->next != NULL) {
			//오른쪽이 NULL일때까지 이동
			current = current->next;
		}
		current->next = NewEntry;
	}
}

/**
 * @brief 심볼테이블에 저장하는 함수.
 * @param 해시테이블
 */
void SetSymbolTable() {
	
	/**
	 * @todo 해시테이블 순회하여, 심볼테이블에 저장.
	 */
	int STindex = 0;
	for (int i = 0; i < TABLE_SIZE; i++) {
		HTpointer entry = HT[i];
		while (entry != NULL) {
			ST[STindex].ID = entry->ID;
			ST[STindex].StringPoolIndex = entry->Index;
			ST[STindex].Length = entry->Length;
			entry = entry->next;
			STindex++;
		}
	}
}

/**
 * @brief 심볼테이블 출력.
 */
void PrintSymbolTable() {
	printf("%s", "심볼테이블-----------------\n");
	printf("ID\t\tStringPoolIndex\t\tLength\n");
	for (int i = 0; i < TABLE_SIZE; i++) {
		printf("%d\t\t%d\t\t\t%d\n", ST[i].ID, ST[i].StringPoolIndex, ST[i].Length);
	}

}
/**
 * @brief 해시테이블 출력.
 * 
 */
void PrintHashTable() {
	printf("%s", "해시테이블-----------------\n");
	for (int i = 0; i < TABLE_SIZE; i++) {
		HTpointer entry = HT[i];
		while(entry != NULL) {
			printf("해시인덱스(해시코드) - %d (Symbol ID: %d, String Pool Index: %d, String Length: %d)\n", i, entry->ID, entry->Index, entry->Length);			
			entry = entry->next;
			if (entry != NULL) {
				printf("\t->");
			}
		}
	}
	printf("\n");
}

/**
 * @brief 변수의 유효성을 검사하는 함수.
 * @brief 심볼
 * @return 유효/비유효
 */
int IsValidIdentifier(string identifier) {
	if ((identifier[0] >= '0' && identifier[0] <= '9')) {
		return FALSE;//첫 숫자: 비유효
	}
	for (int i = 0; i < StringLength(identifier); i++) {
		//한글같은 특수문자 = 아스키코드 초과-> 음수로	변환되어버림-> 따라서 unsigned char로 양수 강제화
		unsigned char c = (unsigned char)identifier[i];
		if (!isalnum(c) && c != '_') {
			return FALSE;//알파벳, 숫자, _ 외: 비유효
		}
	}
	if (StringLength(identifier)>MAX_SYMBOL_LENGTH) {
		return FALSE;//길이 15초과: 비유효

	}

	return TRUE;//다 통과: 유효
}

int main() {
	string Input = {0};
	char StringPool[STRING_POOL_SIZE] = { 0 };
	int StringPoolIndex = 0;
	char buffer[FILE_BUFFER_SIZE];
	FILE* file;

	//파일열기
	fopen_s(&file, "input2.txt", "r");
	if (file == NULL) {
		printf("파일 열기 실패\n");
		return -1;
	}

	while (fgets(buffer, sizeof(buffer), file)) {
		//식별자 기준대로 토큰 split
		char* token = strtok(buffer, " \t\n,;");
		//식별자 단위로 읽어들이기
		while (token != NULL) {
			if (!IsValidIdentifier(token)) {
				printf("유효하지 않은 식별자: %s\n", token);
			}
			else {
				/*string pool 크기 초과하는지 확인*/
				if (StringPoolIndex + StringLength(token) + 1 > STRING_POOL_SIZE) {
					printf("String Pool 크기 초과\n");
					fclose(file);
					//심볼테이블저장
					SetSymbolTable();
					//결과프린트
					PrintSymbolTable();
					PrintHashTable();
					return -1;
				}

				//Stringpool에 심볼대입 *두번째인자 : 남은버퍼크기
				strcpy_s(&StringPool[StringPoolIndex], sizeof(StringPool) - StringPoolIndex, token);

				//HT저장
				int HashValue = GetHashCode(token);
				HTpointer Htp = LookUpHashTable(StringPoolIndex, token);
				if (Htp == NULL) {
					AddHashTable(HashValue, token, StringPoolIndex);
				}
				else {
					printf("hash code(index) : %d\tSymbol name: %s (already exists)\n", HashValue, token);
				}

				//string pool index 이름
				StringPoolIndex += StringLength(token) + 1;
			}
			token = strtok(NULL, " \t\n,;");
		}
	}

	fclose(file);

	/*결과프린트*/
	//심볼테이블에 저장
	SetSymbolTable();
	//결과프린트
	PrintSymbolTable();
	PrintHashTable();

	return 0;
}