#include <stdio.h>
#include <string.h> // 데이터 크기 계산을 위한 strlen 함수 사용
#include "secure_module.h"

int main() {
    // 보호할 실제 비밀 데이터 (원본)
    char* my_secret = "My_API_Key_12345";

    // 해커를 속이기 위한 가짜 데이터
    char* my_decoy = "Fake_Key_Access_Denied";

    printf("메인 함수: 금고 생성을 요청합니다..\n");

    // [보안 모듈 생성]
    // 비밀 데이터와 그 크기, 그리고 가짜 데이터를 전달합니다.
    // strlen(my_secret) + 1 은 문자열의 끝(NULL 문자)까지 포함한 크기입니다.
    struct SecurePointer* sp = secure_create(my_secret, (int)(strlen(my_secret) + 1), my_decoy);

    // 생성 실패 시 프로그램 종료
    if (sp == NULL) {
        printf("메인 함수: 금고 생성 실패!\n");
        return 1;
    }

    // [테스트 1] 첫 번째 접근 - 정상적으로 원본을 받아와야 함
    printf("\n--- 1차 접근 시도 (정상) ---\n");
    char* data1 = (char*)secure_read(sp);
    if (data1) printf(" >> 메인 함수가 받은 데이터: %s\n", data1);

    // [테스트 2] 두 번째 접근 - 규칙 위반! 가짜 데이터를 받아와야 함
    printf("\n--- 2차 접근 시도 (비정상) ---\n");
    char* data2 = (char*)secure_read(sp);
    if (data2) printf(" >> 메인 함수가 받은 데이터: %s\n", data2);

    // [테스트 3] 세 번째 접근 - 이미 침해된 상태. 여전히 가짜 데이터를 줌
    printf("\n--- 3차 접근 시도 (이미 침해됨) ---\n");
    char* data3 = (char*)secure_read(sp);
    if (data3) printf(" >> 메인 함수가 받은 데이터: %s\n", data3);

    printf("\n메인 함수: 금고를 다 사용했으므로 해제를 요청합니다...\n");

    // [종료] 모듈과 내부 데이터의 메모리를 모두 정리합니다.
    secure_destroy(sp);

    return 0;
}