#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define SYM_TABLE_SIZE	100
#define HASH_TABLE_SIZE 100
#define STR_POOL_SIZE   1000
char separators[] = " ,;\t\n\r\n";
char str_pool[STR_POOL_SIZE];

// 기존 3열에 추가적으로 5개의 열을 추가합니다.
#define ADDITIONAL_COLUMNS 5
#define TOTAL_COLUMNS (2 + ADDITIONAL_COLUMNS)

int sym_table[SYM_TABLE_SIZE][TOTAL_COLUMNS];
int index_start = 0;
int sym_table_index = 0;

const char* getTypeName(int typeCode);
const char* getFuncName(int flagCode);
const char* getParamTypeName(int typeCode);
const char* getReturnTypeName(int typeCode);


#define isLetter(x) ( ((x) >= 'a' && (x) <='z') || ((x) >= 'A' && (x) <= 'Z') || ((x) == '_')) 
#define isDigit(x) ( (x) >= '0' && (x) <= '9' )

typedef struct HTentry* HTpointer;
typedef struct HTentry {
    int index;
    HTpointer next;
}HTentry;

HTpointer HT[HASH_TABLE_SIZE];

int divisionMethod(char* key, int tableSize) {
    unsigned int hash_key = 0;
    int c;

    while (c = *key++) {
        hash_key += c;
    }
    return hash_key % tableSize;
}

int midsquareMethod(char* key, int tableSize) {
    int squared;
    int midPart;
    unsigned int hash_key = 0;
    int c;

    while (c = *key++) {
        hash_key += c;
    }

    squared = hash_key * hash_key;
    midPart = squared / 100 % 1000; // 제곱값의 중간 부분 추출
    return midPart % tableSize;
}

int foldingMethod(char* key, int tableSize) {
    int fold = 0;
    unsigned int hash_key = 0;
    int c;

    while (c = *key++) {
        hash_key += c;
    }

    while (hash_key > 0) {
        fold += hash_key % 10000; // 4자리씩 나누어 더함
        hash_key /= 10000;
    }
    return fold % tableSize;
}
void init_sym_table() {
    int i, j;
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        for (j = 0; j < TOTAL_COLUMNS; j++) {
            sym_table[i][j] = -1;
        }
    }
}

void update_sym_table(int id_index, int attr_num, int attr_value) {
    sym_table[id_index - 1][attr_num + 2] = attr_value;
}

void print_sym_table() {
    int i;
    printf("\nSymbol Table\n");
    // 변경 전 헤더: "ID\tIndex\tLength\tSymbol\tAttributes\tAttrType\tFuncName\tParamType\tReturnType\tLineNum\n"
    printf("ID\tIndex\tLength\tSymbol\tAttrType\tFuncName\tParamType\tReturnType\tLineNum\n");
    for (i = 0; i < SYM_TABLE_SIZE; i++) {
        if (sym_table[i][0] != -1) {
            printf("%d\t", i + 1);
            printf("%d\t", sym_table[i][0]);
            printf("%d\t", sym_table[i][1]);
            printf("%s\t", str_pool + sym_table[i][0]);
            // 변경 전 출력: sym_table[i][2] : Attributes, sym_table[i][3] ~ [7] : 나머지 열
            // 변경 후: [2]: AttrType, [3]: FuncName, [4]: ParamType, [5]: ReturnType, [6]: LineNum
            printf("%s\t", getTypeName(sym_table[i][2]));
            printf("%s\t", getFuncName(sym_table[i][3]));
            printf("%s\t", getParamTypeName(sym_table[i][4]));
            if (strcmp(getFuncName(sym_table[i][3]), "function") == 0)
                printf("%s\t", getReturnTypeName(sym_table[i][5]));
            else
                printf("-\t");
            printf("%d\n", sym_table[i][6]);
        }
    }
}

HTpointer lookup_hash_table(int id_index, int hscode) {
    HTpointer entry = HT[hscode];

    // 체이닝된 리스트를 탐색
    while (entry != NULL) {
        if (strcmp(str_pool + sym_table[entry->index - 1][0], str_pool + id_index) == 0) {
            return entry; // 찾은 항목 반환
        }
        entry = entry->next;
    }
    return NULL; // 항목을 찾지 못한 경우
}

void add_hash_table(int id_index, int hscode) {
    // 새 항목 생성 및 초기화
    HTpointer newEntry = (HTpointer)malloc(sizeof(HTentry));
    if (newEntry == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    newEntry->index = id_index;
    newEntry->next = NULL;

    if (HT[hscode] == NULL) {
        // 첫 번째 항목으로 추가
        HT[hscode] = newEntry;
    }
    else {
        // 이미 항목이 있으면, 리스트의 맨 앞에 추가
        newEntry->next = HT[hscode];
        HT[hscode] = newEntry;
    }
}

void print_hash_table() {
    printf("\nHash Table:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        HTpointer entry = HT[i];
        if (entry != NULL) {
            printf("[%d]: ", i);
            while (entry != NULL) {
                printf("%d -> ", entry->index);
                entry = entry->next;
            }
            printf("NULL\n");
        }
    }
}

int process_sym_table(char* identifier) {
    int hash_value = 0;
    bool flag_undefined = false;
    int result;

    strcpy(str_pool + index_start, identifier);

    hash_value = divisionMethod(str_pool + index_start, HASH_TABLE_SIZE);

    HTpointer htp = lookup_hash_table(index_start, hash_value);
    if (htp == NULL) {
        sym_table[sym_table_index][0] = index_start;
        sym_table[sym_table_index++][1] = (int)strlen(str_pool + index_start);

        add_hash_table(sym_table_index, hash_value);
        //printf("%d\t%s\n", hash_value, str_pool + index_start); // 버퍼의 내용을 화면에 출력
        result = sym_table_index;
    }
    else {
        result = htp->index;
        //printf("%d\t%s (already exists)\n", hash_value, str_pool + index_start); // 버퍼의 내용을 화면에 출력
        return result;
    }

    index_start += strlen(identifier);
    str_pool[index_start++] = '\0';

    return result;
}

// AttrType 업데이트 (새 열번호: 2)
void update_attr_type(int id_index, int attrType_value) {
    if (sym_table[id_index - 1][2] == -1) {
        sym_table[id_index - 1][2] = attrType_value;
    }
}

// 함수이름 업데이트 (새 열번호: 3)
void update_func_name(int id_index, int funcName_value) {
    if (sym_table[id_index - 1][3] == -1) {
        sym_table[id_index - 1][3] = funcName_value;
    }
}

// 함수 파라미터 타입 업데이트 (새 열번호: 4)
void update_param_type(int id_index, int paramType_value) {
    if (sym_table[id_index - 1][4] == -1) {
        sym_table[id_index - 1][4] = paramType_value;
    }
}

// 함수반환타입 업데이트 (새 열번호: 5)
void update_return_type(int id_index, int returnType_value) {
    if (sym_table[id_index - 1][5] == -1) {
        sym_table[id_index - 1][5] = returnType_value;
    }
}

// 라인넘버 업데이트 (새 열번호: 6)
// 만약 해당 심볼의 라인넘버가 -1이면만 업데이트(초반에만 업데이트)
void update_line_number(int id_index, int lineNumber_value) {
    if (sym_table[id_index - 1][6] == -1) {
        sym_table[id_index - 1][6] = lineNumber_value;
    }
}

const char* getFuncName(int flagCode) {
    switch (flagCode) {
    case 1: return "function";
    default: return "unknown";
    }
}


const char* getTypeName(int typeCode) {
    switch (typeCode) {
    case 0: return "int";
    case 1: return "float";
    case 2: return "char";
    case 3: return "void";
    default: return "unknown";
    }
}

const char* getParamTypeName(int typeCode) {
    switch (typeCode) {
    case 0: return "int";
    case 1: return "float";
    case 2: return "char";
    case 3: return "void";
    default: return "unknown";
    }
}

const char* getReturnTypeName(int typeCode) {
    switch (typeCode) {
    case 0: return "int";
    case 1: return "float";
    case 2: return "char";
    case 3: return "void";
    default: return "unknown";
    }
}