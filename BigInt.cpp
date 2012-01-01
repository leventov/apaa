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

BigInt & BigInt::operator+=(const BigInt &rhs)
{
	this->grow(rhs.wc);
	uInt *th = (uInt*)words, *oz = (uInt*)rhs.words;
	uLong carry = 0;
	for (int i = 0; i < wc; i++)
	{
		uLong t = carry + th[i];
		t += i < rhs.wc ? oz[i] : 0;
		if (t > MUL)
		{
			carry = 1;
			t &= MUL;
		}
		else carry = 0;
		th[i] = (uInt)t;
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
	uInt *th = (uInt*)words, *oz = (uInt*)rhs.words;
	uLong borrow = 0;
	for (int i = 0; i < wc; i++)
	{
			uLong t = th[i], r = i < rhs.wc ? oz[i] : 0;
			if (borrow)
			{
				if (t <= r) t += MUL; /* leave borrow set to 1 */
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
			
			th[i] = (uInt)(t - r);
	}
	return *this;
}

