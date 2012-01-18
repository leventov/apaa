#include <stdlib.h>
#include "BigInt.h"
#include "Offsets.h"

#define uInt unsigned long long int
#define INIT_SIZE 1
#define BIG_INT (uInt)2000000000

#define STR_EXPAND(tok) #tok		// http://www.guyrutenberg.com/2008/12/20/
#define STR(tok) STR_EXPAND(tok)	// expanding-macros-into-string-constants-in-c/
#define _WS STR(__WS)
#define __WS 16


int BigInt::WS = __WS;

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
	int owc; // prevent cross initialization error
	
	// cycles per iteration, amd k10
	// 3
	asm goto (
			"clc\n"
			
			"o1:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"mov\t8(%[oz]), %[bx]\n\t"
			
			"adc\t%[ax], (%[th])\n\t"
			"adc\t%[bx], 8(%[th])\n\t"
			
			"lea\t" _WS "(%[oz]), %[oz]\n\t"
			"dec\t%[cx]\n\t"
			"lea\t" _WS "(%[th]), %[th]\n\t"
			
			"jnz\to1\n\t"
			
			
			"mov\t%[rem], %[cx]\n\t"
			"jecxz\tb1\n"
			
			"t1:\t"
			"jnc\t%l[nocarry]\n\t"
			
			"adc\t$0, (%[th])\n\t"
			"adc\t$0, 8(%[th])\n\t"
			
			"lea\t" _WS "(%[th]), %[th]\n\t"
			
			"dec\t%[cx]\n\t"
			
			"jnz\tt1\n"
			
			"b1:\t"
			"jnc\t%l[nocarry]\n\t"
			:
			: [th] "r" (words), [oz] "r" (rhs.words),
			  [ax] "a" ((uInt)0), [bx] "b" ((uInt)0),
			  [cx] "c" (rhs.wc), [rem] "r" (wc - rhs.wc)
			:
			: nocarry
	);

	owc = wc;
	this->alloc(owc + 1);
	*((uInt*)words + (__WS / sizeof(uInt)) * owc) = (uInt)1;

	nocarry:	return *this;
}

BigInt & BigInt::operator-=(const BigInt &rhs)
{
	this->grow(rhs.wc);
	
	asm goto (
			"clc\n"
			
			"o2:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"mov\t8(%[oz]), %[bx]\n\t"
			
			"sbb\t%[ax], (%[th])\n\t"
			"sbb\t%[bx], 8(%[th])\n\t"
			
			"lea\t" _WS "(%[oz]), %[oz]\n\t"
			"dec\t%[cx]\n\t"
			"lea\t" _WS "(%[th]), %[th]\n\t"
			
			"jnz\to2\n\t"
			
			
			"mov\t%[rem], %[cx]\n\t"
			"jecxz\t%l[noborrow]\n"
			
			"t2:\t"
			"jnc\t%l[noborrow]\n\t"
			
			"sbb\t$0, (%[th])\n\t"
			"sbb\t$0, 8(%[th])\n\t"
			
			"lea\t" _WS "(%[th]), %[th]\n\t"
			
			"dec\t%[cx]\n\t"
			
			"jnz\tt2\n\t"
			:
			: [th] "r" (words), [oz] "r" (rhs.words),
			  [ax] "a" ((uInt)0), [bx] "b" ((uInt)0),
			  [cx] "c" (rhs.wc), [rem] "r" (wc - rhs.wc)
			:
			: noborrow
	);
	
	noborrow: return *this;
}



















