struct SecurePointer;

struct SecurePointer* secure_create(void* real_data, void* decoy_data);

void secure_destroy(struct SecurePointer* sp);

void* secure_read(struct Secure_Pointer* sp);