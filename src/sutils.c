#include <string.h>
#include <stdlib.h>
#include "sutils.h"

char* skipWS(char* s) {
	while ((*s)==' ' || (*s)=='\t') {
		s++;
	}
	return s;
}

void trimEndingChar(char* s, char c) {
	char* p=s+strlen(s)-1;

	while ( p>=s && ((*p)==c) ) {
		p--;
	}

	(*(++p))=0;
}

void trimEndingWS(char* s) {
	char* p=s+strlen(s)-1;

	while ( p>=s && ((*p)==' ' || (*p)=='\t') ) {
		p--;
	}

	(*(++p))=0;
}

int strStartsWith(char* s, char* start) {

	while ((*start)!=0 && (*s)!=0 && (*start)==(*s)) {
		*start++; *s++;
	}

	return ((*start)==0);
}

int endsWith(char* s, char* end) {
	if (strlen(end)>strlen(s)) {
		return 0;
	} else {
		char* sEnd=s+strlen(s)-strlen(end);

		return (strcmp(sEnd, end)==0);
	}
}

void strReplaceChar(char* str, char s, char d) {
	while (*str!=0) {
		if (*str==s) *str=d;

		str++;
	}
}
