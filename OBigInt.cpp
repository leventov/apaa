#include <stdlib.h>
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
// 	gcc opt	cycles / iteration, amd K10
// -01		7 
// -02		6
// -O3		6
BigInt & BigInt::operator+=(const BigInt &rhs)
{
	this->grow(rhs.wc);
	uInt *th = (uInt*)words;
	uLong carry = 0;
	for (	uInt *oz = (uInt*)rhs.words, *limit = oz + rhs.wc, *thl = th + wc;
			oz < limit || carry && th < thl;
			oz++, th++)
	{
			uLong t = carry + *th;
			t += oz < limit ? *oz : 0;
			if (t > MUL)
			{
				carry = 1;
				t &= MUL;
			}
			else carry = 0;
			*th = (uInt)t;
	}
	if (carry) {
		int owc = wc;
		this->alloc(owc + 1);
		*((uInt*)words + owc) = (uInt)1;
	}
	return *this;
}

BigInt & BigInt::operator-=(const BigInt &rhs)
{
	this->grow(rhs.wc);
	uInt *th = (uInt*)words;
	uLong borrow = 0;
	for (	uInt *oz = (uInt*)rhs.words, *limit = oz + rhs.wc, *thl = th + wc;
			oz < limit || borrow && th < thl;
			oz++, th++)
	{
			uLong t = *th, r = oz < limit ? *oz : 0;
			if (borrow)
			{
				if (t <= r) t += MUL; // leave borrow set to 1
				else
				{
					borrow = 0;
					t--;
				}
			} else if (t < r)
			{
				borrow = 1;
				t += SET;
			}

			*th = (uInt)(t - r);
	}
	return *this;
}
