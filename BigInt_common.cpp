#include <stdlib.h>
#include <string.h>
#include "BigInt.h"

#define uChar unsigned char

BigInt::BigInt(int init_size, void* _words)	
{
	wc = init_size;
	words = _words;
}

BigInt::BigInt(const BigInt &s)
{
	wc = s.wc;
	int size = wc*WS;
	words = malloc(size);
	memcpy(words, s.words, size);
}

BigInt & BigInt::operator=(const BigInt &s)
{
	if (this == &s)
		return *this;
	free(words);
	wc = s.wc;
	int size = wc*WS;
	words = malloc(size);
	memcpy(words, s.words, size);
}

BigInt BigInt::ONE = BigInt(1, 1);
BigInt BigInt::ZERO = BigInt(1, 0);


const char *digits = "0123456789ABCDEF";
char* BigInt::hex()
{
	int len = wc*WS;
	char *r = (char*)calloc(len*2 + 1, 1), *r0 = r; // len in bytes => x2 hex digits; 0-terminator

	uChar *w = (uChar*)words;
	for (uChar *i = w + len - 1; i >= w; i--)
	{
		char off = *i >> 4;
		*r++ = *(digits + off);
		
		off = *i & 15;
		*r++ = *(digits + off);
	}
	return r0;
}


BigInt::~BigInt(void)
{
	free(words);
}

void BigInt::alloc(int nwc)
{
	void *nwords = calloc(nwc, WS);
	memcpy(nwords, words, wc*WS);
	free(words);
	wc = nwc;
	words = nwords;
}

void BigInt::grow(int cwc)
{
	if (cwc > wc) this->alloc(cwc);
}


BigInt & BigInt::operator+(const BigInt &other)
{
	BigInt *result = new BigInt(*this);
	*result += other;
	return *result;
}

BigInt & BigInt::operator-(const BigInt &other)
{
	BigInt *result = new BigInt(*this);
	*result -= other;
	return *result;
}

