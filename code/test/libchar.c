int isalnum(int c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

int isblank(int c) {
	return c == ' ' || c == '\t' || c == '\n';
}

int isprint(int c) {
	return !isblank(c);
}
