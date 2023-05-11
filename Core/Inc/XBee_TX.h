#include <string.h>

#ifndef __BEE_H__
#define __BEE_H__

uint8_t bee(char *str) {

	uint8_t data = 0;

	if (strcmp((char*) str, (char*) "one") == 0) {
		data = 'a';
	}
	else if (strcmp((char*) str, (char*) "two") == 0) {
		data = 'b';
	}
	else if (strcmp((char*) str, (char*) "three") == 0) {
		data = 'c';
	}
	else if (strcmp((char*) str, (char*) "legend") == 0) {
		data = 'l';
	}
	else if (strcmp((char*) str, (char*) "cherry") == 0) {
		data = 'r';
	}
	else if (strcmp((char*) str, (char*) "funny") == 0) {
		data = 'f';
	}
	else if (strcmp((char*) str, (char*) "steal") == 0) {
		data = 's';
	}
	else if (strcmp((char*) str, (char*) "home") == 0) {
		data = 'h';
	}
	else if (strcmp((char*) str, (char*) "end") == 0) {
		data = 'o';
	}
	else {
		data = 'X';
	}

	return data;
}

#endif
