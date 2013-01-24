int stringeq(const char *a, const char *b) {
	int i;
	for (i = 0; a[i] == b[i] && a[i] != '\0'; i++) {
	}
	return a[i] == b[i];
}

int stringlen(const char* str) {
	int i;
	for (i = 0; str[i] != '\0'; i++) {
	}
	return i;
}

void stringcopy(const char* str, char* dst, int n) {
	int i;
	for (i = 0; i < n; i++) {
		dst[i] = str[i];
	}
	dst[n] = '\0';
}
