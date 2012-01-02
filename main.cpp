#include "BigInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <functional>
#include <gmp.h>

int main1() {
	BigInt *a = new BigInt(1);
	BigInt *b = new BigInt(1);
	BigInt * t;
	for (int i = 0; i < 40; i++)
	{
		*a += *b;
		t = a;
		a = b;
		b = t;
		printf("%s %s\n", a->hex(), b->hex());
	}
	for (int i = 0; i < 40; i++)
	{
		*b -= *a;
		t = a;
		a = b;
		b = t;
		printf("%s %s\n", a->hex(), b->hex());
	}
	return 0;
}

int main2() {
	BigInt a = BigInt(2, 8);
	BigInt b = BigInt(5);
	printf("%s %s\n", a.hex(), b.hex());
	BigInt c = a - b;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	a -= BigInt::ZERO;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	b -= BigInt::ONE;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	c -= BigInt::ONE;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	a -= BigInt::ZERO;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	b -= BigInt::ZERO;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	c -= BigInt::ZERO;
	printf("%s %s %s\n", a.hex(), b.hex(), c.hex());
	return 0;
}
int TOTAL_ITERATIONS = 9830400;

int main() {
    struct timespec t1, t2;
    int is = sizeof(int);
    
    double rr[3];
    // 32 instead of BigInt::WS when measure GMP
	double TID = TOTAL_ITERATIONS * is / BigInt::WS /* 32 */ *1.0L; 
 	//printf("%d\n", s);

	int NI = 10;
	double r[NI]; 
    
    for (int k = 0; k <= 2; k++) {
		// s0 - number of int words int bigint sources, s - int realisation-specific words
		int s0 = 96 * (1 << k), s = s0 * is / BigInt::WS;
		//printf("%d %d\n", s, TOTAL_ITERATIONS / s0 / 2);
		int* words = (int*)calloc(2*s0, is);
		for (int* t = words; t < words + 2*s0; t++) *t = rand();
		BigInt *a = new BigInt(s, (void*)words);
		BigInt *b = new BigInt(s, (void*)(words+s0));
		//mpz_t ga, gb, gt;
		//mpz_init2(ga, (s0+1)*32);
		//mpz_init2(gb, (s0+1)*32);
		//mpz_init2(gt, (s0+1)*32);
		//mpz_set_str(ga, a->hex(), 16);
		//mpz_set_str(gb, b->hex(), 16);
		for (int i = 0; i < NI; i++) {
			clock_gettime(CLOCK_MONOTONIC, &t1);

			for (int j = 0; j < TOTAL_ITERATIONS / s0 / 2; j++) {
				//mpz_add(gt, ga, gb);
				//mpz_sub(gt, ga, gb);
				*a += *b;
				*a -= *b;
			}
			
			clock_gettime(CLOCK_MONOTONIC, &t2);
			
			long long diff = 1000000000LL*(t2.tv_sec - t1.tv_sec) + t2.tv_nsec - t1.tv_nsec;
			// print cpu clock ticks per addition of one 32 bit block of bigint words in plus/minus operation
			r[i] = diff*3.2/TID;
			// my cpu is 3.2 GHz
		}
		std::sort(r, r + NI, std::greater<double>());
		std::reverse(r, r + NI);
		//printf("\n");
		rr[k] = r[0];
	}
	printf("%1.3f\n", rr[2] - (rr[0]-rr[1])*0.5);
    return 0;
}













