#ifndef LIBIO_H
#define LIBIO_H

#define printf(format, ...) \
	{\
		void* __args[] = { __VA_ARGS__ };\
		user_printf(format, __args);\
	}
#define scanf(format, ...) \
	{\
		void* __args[] = { __VA_ARGS__ };\
		user_scanf(format, __args);\
	}

void user_printf(const char* str, void** args);
int user_scanf(const char* str, void** args);

#endif

