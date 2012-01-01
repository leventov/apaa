#include <stdlib.h>
#include <stdio.h>
#include "BigInt.h"

typedef signed int sInt;
typedef signed long long sLong;
typedef unsigned int uInt;
typedef unsigned long long uLong;

#define INIT_SIZE 2
#define MAX_uInt ((uInt)(-1))
#define MUL ((uLong)MAX_uInt) // MUL: Max_uInt in Unsigned Long
#define SET MUL+1

int BigInt::WS = sizeof(uInt);

BigInt::BigInt(int init_size, int v)	
{
	wc = init_size;
	words = calloc(wc, WS);
	*(uInt*)words = (uInt)v;
}

BigInt::BigInt(int v)
{
	wc = INIT_SIZE;
	words = calloc(wc, WS);
	*(uInt*)words = (uInt)v;
}

BigInt & BigInt::operator+=(const BigInt &rhs)
{	
	this->grow(rhs.wc);
	uLong carry = 0ULL;
	uInt *th = (uInt *)words;
	uInt * const thl = th + wc;
	uInt const * oz = (uInt *)rhs.words;
	uInt const * const ozl = oz + rhs.wc;
	while (oz < ozl)
	{
		carry += *oz;
		carry += *th;
		*th = carry;
		carry = (sLong)carry < 0 ? 1 : 0;
		++th, ++oz;
	}
	while (th < thl)
	{
		carry += *th;
		*th = carry;
		carry = (sLong)carry < 0 ? 1 : 0;
		++th;
	}
	if (carry)
	{
		int owc = wc;
		this->alloc(owc + 1);
		*((uInt*)words + owc) = (uInt)1;
	}
	return *this;	
}

BigInt & BigInt::operator-=(const BigInt &rhs)
{
	this->grow(rhs.wc);
	
	uLong borrow = 0;
	uInt *th = (uInt *)words;
	uInt * const thl = th + wc;
	uInt const * oz = (uInt *)rhs.words;
	uInt const * const ozl = oz + rhs.wc;
	while (oz < ozl)
	{
		borrow += *th;
		borrow -= *oz;
		*th = borrow;
		borrow = (sLong)borrow < 0 ? -1LL : 0;  
		th++; oz++;	// WTF? все так просто?
	}
	while (th < thl)
	{
		borrow += *th;
		*th = borrow;
		borrow = (sLong)borrow < 0 ? -1LL : 0;  
		th++; 
	}
	return *this;
}

