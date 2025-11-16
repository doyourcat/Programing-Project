#include <stdio.h>
#include "secure_module.h"

int main() {
	char* my_secret = "My_API_Key_12345";
	char* my_decoy = "Fake_Key_Access_Denied";

	printf("메인 함수: 금고 생성을 요청합니다..\n");

	struct Secure_Pointer* sp=secure_create(my_secret, my_decoy);

	if (sp == NULL) {
		printf("메인 함수: 금고 생성 실패!\n");
		return 1;
	}
	printf("\n--- 1차 접근 시도 (정상) ---\n");
	char* data1 = (char*)secure_read(sp);
	printf(" >> 메인 함수가 받은 데이터: %s\n", data1);

	printf("\n--- 2차 접근 시도 (비정상) ---\n");
	char* data2 = (char*)secure_read(sp);
	printf(" >> 메인 함수가 받은 데이터: %s\n", data2);

	printf("\n--- 3차 접근 시도(이미 침해됨) ---\n");
	char* data3 = (char*)secure_read(sp);
	printf(" >> 메인 함수가 받은 데이터: %s\n", data3);

	printf("\n메인 함수: 금고를 다 사용했으므로 해제를 요청합니다...\n");
	secure_destroy(sp);

	return 0;

}