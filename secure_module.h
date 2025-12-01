#ifndef SECURE_MODULE_H  // 헤더 파일의 중복 포함을 방지하기 위한 가드(Include Guard) 시작
#define SECURE_MODULE_H

// [정보 은닉 핵심]
// 구조체의 구체적인 멤버(변수)들은 이 헤더 파일에 공개하지 않고 이름만 선언합니다.
// 이를 통해 외부(main.c)에서는 구조체 내부가 어떻게 생겼는지 알 수 없게 됩니다. (Opaque Pointer 기법)
struct SecurePointer;

/* * 함수: secure_create
 * 설명: 보안 포인터 모듈을 생성하고 데이터를 안전하게 저장합니다.
 * 매개변수:
 * - real_data: 보호하고 싶은 실제 데이터의 원본 포인터
 * - size: 데이터의 크기 (int형 사용, Deep Copy를 위해 필요)
 * - decoy_data: 해킹 시도 시 해커에게 보여줄 가짜 데이터의 포인터
 * 반환값: 생성된 보안 구조체의 포인터
 */
struct SecurePointer* secure_create(void* real_data, int size, void* decoy_data);

/* * 함수: secure_read
 * 설명: 저장된 데이터에 접근을 시도합니다.
 * 기능: 접근 횟수를 카운트하여 1회만 원본을 보여주고, 이후에는 가짜 데이터를 반환합니다.
 */
void* secure_read(struct SecurePointer* sp);

/* * 함수: secure_destroy
 * 설명: 보안 모듈 사용이 끝나면 메모리를 안전하게 해제합니다.
 * 기능: 내부적으로 복사해둔 데이터(Deep Copy본)와 구조체를 모두 해제합니다.
 */
void secure_destroy(struct SecurePointer* sp);

#endif // 헤더 가드 종료