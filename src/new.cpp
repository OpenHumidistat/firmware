#if __cplusplus >= 201402L && ARDUINO_AVR_UNO
#include <stdlib.h>

void operator delete(void* ptr, size_t size) noexcept {
	operator delete(ptr);
}
void operator delete[](void * ptr, size_t size) noexcept {
	operator delete[](ptr);
}
#endif
