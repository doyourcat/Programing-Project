#include <stdio.h>      // 표준 입출력 함수 사용 (printf, fprintf, fopen 등)
#include <stdlib.h>     // 메모리 할당 및 해제 함수 사용 (malloc, free)
#include <string.h>     // 메모리 복사 함수 사용 (memcpy)
#include "secure_module.h" // 헤더 파일 포함

// [구조체 정의]
// 헤더 파일에는 숨겨져 있던 구조체의 실제 모습입니다.
// 이 내용은 오직 이 .c 파일 내부에서만 볼 수 있습니다.
struct SecurePointer {
    void* real_data_store;  // Deep Copy(깊은 복사)를 통해 원본 데이터를 따로 보관할 장소
    void* decoy_data_ptr;   // 비정상 접근 시 반환할 가짜 데이터의 주소
    int access_count;       // 데이터에 몇 번 접근했는지 세는 카운터 변수
    int is_compromised;     // 보안이 뚫렸는지(해킹 시도 여부) 확인하는 플래그 (0: 정상, 1: 침해됨)
};

// [내부 함수] 로그 기록
// 외부에서 호출할 수 없도록 static으로 선언했습니다.
static void log_event(const char* message) {
    // "audit_log.txt" 파일을 쓰기 모드("a": append, 덧붙이기)로 엽니다.
    FILE* log_file = fopen("audit_log.txt", "a");

    // 파일 열기에 성공했다면 로그를 작성합니다.
    if (log_file != NULL) {
        fprintf(log_file, "[LOG] %s\n", message); // 파일에 메시지 기록
        fclose(log_file); // 사용이 끝난 파일은 반드시 닫아줍니다.
    }
}

// [함수 구현] 보안 모듈 생성
struct SecurePointer* secure_create(void* real_data, int size, void* decoy_data)
{
    // 1. 보안 구조체 자체를 위한 메모리를 힙(Heap) 영역에 할당합니다.
    struct SecurePointer* sp = (struct SecurePointer*)malloc(sizeof(struct SecurePointer));

    // 메모리 할당에 실패했다면 NULL을 반환하고 종료합니다.
    if (sp == NULL) return NULL;

    // 2. [Deep Copy 핵심] 실제 데이터를 보관할 '독립적인' 메모리 공간을 따로 할당합니다.
    // 이렇게 하면 main 함수에서 원본 데이터가 사라져도, 여기에는 사본이 안전하게 남습니다.
    sp->real_data_store = malloc(size);

    // 만약 데이터 저장 공간 할당에 실패했다면, 앞서 만든 구조체도 해제하고 실패 처리합니다.
    if (sp->real_data_store == NULL) {
        free(sp);
        return NULL;
    }

    // 3. memcpy를 사용하여 원본 데이터의 내용을 새로 만든 공간으로 복제합니다.
    memcpy(sp->real_data_store, real_data, size);

    // 4. 나머지 보안 설정값들을 초기화합니다.
    sp->decoy_data_ptr = decoy_data; // 가짜 데이터 주소 저장
    sp->access_count = 0;            // 접근 횟수 0으로 초기화
    sp->is_compromised = 0;          // 침해 상태 '정상(0)'으로 설정

    return sp; // 생성된 모듈의 포인터 반환
}

// [함수 구현] 데이터 읽기 (접근 제어 로직 포함)
void* secure_read(struct SecurePointer* sp)
{
    // 모듈이 없으면 아무것도 반환하지 않습니다.
    if (sp == NULL) return NULL;

    // 접근 횟수를 1 증가시킵니다. (상태 변화)
    sp->access_count++;

    // [보안 로직]
    // 1. 이미 해킹된 상태(is_compromised == 1)이거나
    // 2. 접근 횟수가 1회를 초과했다면 (access_count > 1)
    if (sp->is_compromised == 1 || sp->access_count > 1) {

        // 이번이 첫 번째 해킹 시도라면 로그를 남깁니다.
        if (sp->is_compromised == 0) {
            printf("[MODULE] 비정상 접근 감지! (1회 초과)\n");
            log_event("비정상 접근 시도 (1회 초과)"); // 파일에 증거 기록
            sp->is_compromised = 1; // 상태를 '침해됨'으로 변경
        }

        // [핵심 기능] 진짜 데이터 대신 '가짜 데이터(Decoy)'를 반환합니다.
        // 포인터가 가리키는 방향을 바꿔치기하여 공격자를 속입니다.
        return sp->decoy_data_ptr;
    }

    // 위 조건에 걸리지 않았다면 정상 접근(1회차)입니다.
    printf("[MODULE] 정상 접근입니다. (1회)\n");

    // Deep Copy 해두었던 '진짜 데이터'를 반환합니다.
    return sp->real_data_store;
}

// [함수 구현] 메모리 해제
void secure_destroy(struct SecurePointer* sp)
{
    if (sp != NULL) {
        // 1. 내부에 따로 보관해둔 데이터 메모리를 먼저 해제합니다. (순서 중요)
        if (sp->real_data_store != NULL) {
            free(sp->real_data_store);
        }
        // 2. 구조체 껍데기를 해제합니다.
        free(sp);
    }
}