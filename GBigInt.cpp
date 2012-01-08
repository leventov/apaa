#include <stdlib.h>
#include "BigInt.h"
#include "Offsets.h"

#define uInt unsigned long long int
#define INIT_SIZE 1

int BigInt::WS = 32;
 

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
	// 5.84
	asm goto (	//	NOP_RPT
			"clc\n" 
			
			"o1:\t"
			"mov\t(%[th]), %[r8]\n\t"
			"mov\t8(%[th]), %[r9]\n\t"
			"mov\t16(%[th]), %[r10]\n\t"
			"mov\t24(%[th]), %[r11]\n\t"
						
			
			
			"adc\t(%[oz]), %[r8]\n\t"
			"adc\t8(%[oz]), %[r9]\n\t"
			"adc\t16(%[oz]), %[r10]\n\t"
			"adc\t24(%[oz]), %[r11]\n\t"
			
			
			
			"mov\t%[r8], (%[th])\n\t"
			"mov\t%[r9], 8(%[th])\n\t"
			"mov\t%[r10], 16(%[th])\n\t"
			"mov\t%[r11], 24(%[th])\n\t"
			
			"lea\t32(%[oz]), %[oz]\n\t"
			
			"dec\t%[cx]\n\t"
			
			"lea\t32(%[th]), %[th]\n\t"
			
			"jnz\to1\n\t"
			
			
			"mov\t%[rem], %[cx]\n\t"
			"jecxz\tb1\n"
			
			"t1:\t"
			"jnc\t%l[nocarry]\n\t"
			
			"adc\t$0, (%[th])\n\t"
			"adc\t$0, 8(%[th])\n\t"
			"adc\t$0, 16(%[th])\n\t"
			"adc\t$0, 24(%[th])\n\t"
			
			"lea\t32(%[th]), %[th]\n\t"
			
			"dec\t%[cx]\n\t"
			
			"jnz\tt1\n"
			
			"b1:\t"
			"jnc\t%l[nocarry]\n\t"
			:
			: [th] "r" (words), [oz] "r" (rhs.words),
			  [r8] "r" (0LL), [r9] "r" (1LL), [r10] "r" (2LL), [r11] "r" (3LL),
			  [cx] "c" (rhs.wc), [rem] "r" (wc - rhs.wc)
			:
			: nocarry
	);
	
	owc = wc;
	this->alloc(owc + 1);
	*((uInt*)words + (WS / sizeof(uInt)) * owc) = (uInt)1;

	nocarry: return *this;
}

BigInt & BigInt::operator-=(const BigInt &rhs)
{
	this->grow(rhs.wc);
	
	asm goto (	//	NOP_RPT
			"clc\n" 
			
			"o2:\t"
			"mov\t(%[th]), %[r8]\n\t"
			"mov\t8(%[th]), %[r9]\n\t"
			"mov\t16(%[th]), %[r10]\n\t"
			"mov\t24(%[th]), %[r11]\n\t"
						
			
			
			"sbb\t(%[oz]), %[r8]\n\t"
			"sbb\t8(%[oz]), %[r9]\n\t"
			"sbb\t16(%[oz]), %[r10]\n\t"
			"sbb\t24(%[oz]), %[r11]\n\t"
			
			
			
			"mov\t%[r8], (%[th])\n\t"
			"mov\t%[r9], 8(%[th])\n\t"
			"mov\t%[r10], 16(%[th])\n\t"
			"mov\t%[r11], 24(%[th])\n\t"
			
			"lea\t32(%[oz]), %[oz]\n\t"
			
			"dec\t%[cx]\n\t"
			
			"lea\t32(%[th]), %[th]\n\t"
			
			"jnz\to2\n\t"
			
						
			"mov\t%[rem], %[cx]\n\t"
			"jecxz\t%l[noborrow]\n"
			
			"t2:\t"
			"jnc\t%l[noborrow]\n\t"
			
			"sbb\t$0, (%[th])\n\t"
			"sbb\t$0, 8(%[th])\n\t"
			"sbb\t$0, 16(%[th])\n\t"
			"sbb\t$0, 24(%[th])\n\t"
			
			"lea\t32(%[th]), %[th]\n\t"
			
			"dec\t%[cx]\n\t"
			
			"jnz\tt2\n"
			:
			: [th] "r" (words), [oz] "r" (rhs.words),
			  [r8] "r" (0LL), [r9] "r" (1LL), [r10] "r" (2LL), [r11] "r" (3LL),
			  [cx] "c" (rhs.wc), [rem] "r" (wc - rhs.wc) 
			:
			: noborrow
	);
	
	noborrow: return *this;
}









