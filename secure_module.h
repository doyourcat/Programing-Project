#ifndef SECURE_MODULE_H
#define SECURE_MODULE_H


// 불투명 구조체 선언
struct SecurePointer;

/* * 보안 포인터 생성 함수
 * real_data: 숨길 실제 데이터
 * size: 실제 데이터의 크기 (바이트 단위)
 * decoy_data: 비정상 접근 시 보여줄 가짜 데이터
 */
struct SecurePointer* secure_create(void* real_data, int size, void* decoy_data);

/* 보안 포인터 읽기 함수 (접근 제어 포함) */
void* secure_read(struct SecurePointer* sp);

/* 보안 포인터 파괴 함수 (메모리 소각 포함) */
void secure_destroy(struct SecurePointer* sp);

#endif