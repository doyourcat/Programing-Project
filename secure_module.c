#include <stdio.h>
#include <stdlib.h>
#include "secure_module.h"

struct Secure_Pointer {
	void* real_data_ptr;
	void* decoy_data_ptr;
	int access_count;
	int is_compromised;

};


struct SecurePointer* secure_create(void* real_data, void* decoy_data)
{
	struct Secure_Pointer* sp = (struct Secure_Pointer*)malloc(sizeof(struct Secure_Pointer));
	if (sp != NULL) {
		sp->real_data_ptr = real_data;
		sp->decoy_data_ptr = decoy_data;
		sp->access_count = 0;
		sp->is_compromised = 0;
	}
	return sp;

}

static void log_event(const char* message) {
	FILE* log_file = fopen("audit_log.txt", "a");
	if (log_file != NULL) {
		fprintf(log_file, "[LOG] %s\n", message);
		fclose(log_file);
	}
}

void* secure_read(struct Secure_Pointer* sp)
{
	if (sp == NULL) {
		return NULL;
	}
	sp->access_count++;

	if (sp->is_compromised == 1 || sp->access_count > 1) {
		if (sp->is_compromised == 0) {
			printf("[MODULE] 비정상 접근 감지! (1회 초과)\n");
			log_event("비정상 접근 시도 (1회 초과)");
			sp->is_compromised = 1;
		}
		return sp->decoy_data_ptr;
	}
	printf("[MODULE] 정상 접근입니다. (1회)\n");
	return sp->real_data_ptr;
}

void secure_destroy(struct SecrePointer* sp)
{
	if (sp != NULL) {
		free(sp);
	}

}