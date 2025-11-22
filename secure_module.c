#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memcpy 사용을 위해 추가
#include "secure_module.h"

// 구조체 정의 (내부 구현 숨김)
struct SecurePointer {
    void* real_data_store;  // 실제 데이터를 복사해서 보관할 장소
    void* decoy_data_ptr;   // 가짜 데이터 (포인터만 저장)
    int access_count;
    int is_compromised;
};

// 로그 기록 함수 (내부 전용)
static void log_event(const char* message) {
    FILE* log_file = fopen("audit_log.txt", "a");
    if (log_file != NULL) {
        fprintf(log_file, "[LOG] %s\n", message);
        fclose(log_file);
    }
}

struct SecurePointer* secure_create(void* real_data, int size, void* decoy_data)
{
    struct SecurePointer* sp = (struct SecurePointer*)malloc(sizeof(struct SecurePointer));
    if (sp == NULL) return NULL;

    // [Deep Copy] 원본 데이터 크기만큼 메모리를 할당하고 내용을 복사함
    sp->real_data_store = malloc(size);
    if (sp->real_data_store == NULL) {
        free(sp);
        return NULL;
    }

    memcpy(sp->real_data_store, real_data, size); // 데이터 복제

    sp->decoy_data_ptr = decoy_data;
    sp->access_count = 0;
    sp->is_compromised = 0;

    return sp;
}

void* secure_read(struct SecurePointer* sp)
{
    if (sp == NULL) {
        return NULL;
    }
    sp->access_count++;

    // 이미 침해되었거나, 접근 횟수가 1회를 초과한 경우
    if (sp->is_compromised == 1 || sp->access_count > 1) {
        if (sp->is_compromised == 0) {
            printf("[MODULE] 비정상 접근 감지! (1회 초과)\n");
            log_event("비정상 접근 시도 (1회 초과)");
            sp->is_compromised = 1; // 침해 상태로 변경

        }
        return sp->decoy_data_ptr; // 가짜 데이터 반환
    }

    printf("[MODULE] 정상 접근입니다. (1회)\n");
    return sp->real_data_store; // 복제해둔 실제 데이터 반환
}

void secure_destroy(struct SecurePointer* sp)
{
    if (sp != NULL) {
        //내부 데이터 메모리 먼저 해제
        if (sp->real_data_store != NULL) {
            free(sp->real_data_store);
        }
        //구조체 메모리 해제
        free(sp);
    }
}