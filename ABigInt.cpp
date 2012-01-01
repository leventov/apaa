#include <stdlib.h>
#include "BigInt.h"

#define uInt unsigned long long int
#define INIT_SIZE 1
#define FZ (uInt)0


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
	int owc; // prevent cross initialization error
	
	/*				rev 1.0
	asm goto (
			"clc\n"
			"l1:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"adc\t%[ax], (%[th])\n\t"
			"lahf\n\t"
			"add\t%[ws], %[th]\n\t"
			"add\t%[ws], %[oz]\n\t"
			"sahf\n\t"
			"loop\tl1\n\t"
			"jnc\t%l[nocarry]\n"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ws] "I" (sizeof(uInt)), [ax] "a" (FZ), "c" (wc)
			:
			: nocarry
	);
	//*/
	
	/*				rev 1.1
	asm goto (
			"clc\n"
			"l1:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"adc\t%[ax], (%[th])\n\t"
			"lahf\n\t"
			"add\t%[ws], %[th]\n\t"
			"add\t%[ws], %[oz]\n\t"
			"sahf\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl1\n\t"
			"jnc\t%l[nocarry]\n"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ws] "I" (sizeof(uInt)), [ax] "a" (FZ), [cx] "c" (wc)
			:
			: nocarry
	);
	//*/

	/*				rev 2.0
	asm goto (
			"clc\n"
			"l1:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"adc\t%[ax], (%[th])\n\t"
			"lea\t4(%[th]), %[th]\n\t"
			"lea\t4(%[oz]), %[oz]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl1\n\t"
			"jnc\t%l[nocarry]\n"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (FZ), [cx] "c" (wc)
			:
			: nocarry
	);
	//*/

	//*				rev 2.1
	asm goto (
			"clc\n"
			"l1:\t"
			"mov\t(%[oz], %[off]), %[ax]\n\t"
			"adc\t%[ax], (%[th], %[off])\n\t"
			"lea\t8(%[off]), %[off]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl1\n\t"
			"jnc\t%l[nocarry]\n"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (FZ), [cx] "c" (wc),  [off] "b" ((long long int)0)
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

	/*				rev 1.0
	asm (
			"clc\n"
			"l2:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], (%[th])\n\t"
			"lahf\n\t"
			"add\t%[ws], %[th]\n\t"
			"add\t%[ws], %[oz]\n\t"
			"sahf\n\t"
			"loop\tl2\n\t"
			:
			: [th] "r" (th), [oz] "r" (oz),
			[ws] "I" (sizeof(uInt)), [ax] "a" (FZ), "c" (wc)
	);
	//*/

	/*				rev 1.1
	asm (
			"clc\n"
			"l2:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], (%[th])\n\t"
			"lahf\n\t"
			"add\t%[ws], %[th]\n\t"
			"add\t%[ws], %[oz]\n\t"
			"sahf\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl2\n\t"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ws] "I" (sizeof(uInt)), [ax] "a" (FZ), [cx] "c" (wc)
	);
	//*/
	
	/*				rev 2.0
	asm (
			"clc\n"
			"l2:\t"
			"mov\t(%[oz]), %[ax]\n\t"
			"sbb\t%[ax], (%[th])\n\t"
			"lea\t4(%[th]), %[th]\n\t"
			"lea\t4(%[oz]), %[oz]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl2\n\t"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (FZ), [cx] "c" (wc)
	);
	//*/

	//*				rev 2.1
	asm (
			"clc\n"
			"l2:\t"
			"mov\t(%[oz], %[off]), %[ax]\n\t"
			"sbb\t%[ax], (%[th], %[off])\n\t"
			"lea\t8(%[off]), %[off]\n\t"
			"dec\t%[cx]\n\t"
			"jnz\tl2\n\t"
			:
			: [th] "r" (th), [oz] "r" (oz),
			  [ax] "a" (FZ), [cx] "c" (wc), [off] "b" ((long long int)0)
			: 
	);
	//*/
	
	return *this;
}

