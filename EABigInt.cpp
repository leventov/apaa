#include <stdlib.h>
#include "BigInt.h"

#define uInt unsigned long long int
#define INIT_SIZE 1
#define ANY_UINT (uInt)1


int BigInt::WS = 2*sizeof(uInt);

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
	int owc; // prevent cross initialization error
	
	/*
	asm goto (
			"clc\n"
			"l1:\t"
			"mov\t(%[oz], %[off]), %[ax]\n\t"
			"adc\t%[ax], (%[th], %[off])\n\t"
			"mov\t8(%[oz], %[off]), %[ax]\n\t"
			"adc\t%[ax], 8(%[th], %[off])\n\t"
		/*	
			"mov\t16(%[oz], %[off]), %[ax]\n\t"
			"adc\t%[ax], 16(%[th], %[off])\n\t"
			"mov\t24(%[oz], %[off]), %[ax]\n\t"
			"adc\t%[ax], 24(%[th], %[off])\n\t"		//*
			
			"lea\t16(%[off]), %[off]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl1\n\t"
			"jnc\t%l[nocarry]\n"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (ANY_UINT), [cx] "c" (wc), [off] "b" ((long long int)0)
			: 
			: nocarry
	);
	//*/
	
	
	
	//*
	asm goto (
			"clc\n"
			"l1:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"adc\t%[ax], (%[th])\n\t"
			"mov\t8(%[oz]), %[ax]\n\t"
			"adc\t%[ax], 8(%[th])\n\t"
		/*	
			"mov\t16(%[oz]), %[ax]\n\t"
			"adc\t%[ax], 16(%[th])\n\t"
			"mov\t24(%[oz]), %[ax]\n\t"
			"adc\t%[ax], 24(%[th])\n\t"		//*/
			
			"lea\t16(%[oz]), %[oz]\n\t"
			"lea\t16(%[th]), %[th]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl1\n\t"
			"jnc\t%l[nocarry]\n"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (ANY_UINT), [cx] "c" (wc)
			: 
			: nocarry
	);
	//*/

	owc = wc;
	this->alloc(owc + 1);
	*((uInt*)words + owc) = (uInt)1;

	nocarry:	return *this;
}

BigInt & BigInt::operator-=(const BigInt &rhs)
{
	uInt *th = (uInt*)words, *oz = (uInt*)rhs.words;

	/*
	asm (
			"clc\n"
			"l2:\t"
			"mov\t(%[oz], %[off]), %[ax]\n\t"
			"sbb\t%[ax], (%[th], %[off])\n\t"
			"mov\t8(%[oz], %[off]), %[ax]\n\t"
			"sbb\t%[ax], 8(%[th], %[off])\n\t"
			
		/*	"mov\t16(%[oz], %[off]), %[ax]\n\t"
			"sbb\t%[ax], 16(%[th], %[off])\n\t"
			"mov\t24(%[oz], %[off]), %[ax]\n\t"
			"sbb\t%[ax], 24(%[th], %[off])\n\t"		//*
			
			"lea\t16(%[off]), %[off]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl2\n\t"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (ANY_UINT), [cx] "c" (wc), [off] "b" ((long long int)0)
			: 
	);
	//*/
	
	//*
	asm (
			"clc\n"
			"l2:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], (%[th])\n\t"
			"mov\t8(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], 8(%[th])\n\t"
			
		/*	
			"mov\t16(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], 16(%[th])\n\t"
			"mov\t24(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], 24(%[th])\n\t"		//*/
			
			"lea\t16(%[oz]), %[oz]\n\t"
			"lea\t16(%[th]), %[th]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl2\n\t"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (ANY_UINT), [cx] "c" (wc)
			: 
	);
	//*/
	
	return *this;
}

