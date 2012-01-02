/*
* ВЫВОД
* Первое число - примерное количество тактов 
* на итерацию сложения/вычитания (4, 8 или 16 байт)
* Типичная погрешность 0,005. Типичная погрешность 
* растет с уменьшением LOOP_BASE.
* 
* param - кол-во тактов, которое тратилось на вход и выход 
* из методов сложения/вычитания. Для современных процессоров ожидается
* результат 15 - 100 (если gcc оптимизируется хотя бы -O, 
* без него меняются фреймы и может быть хоть 500). 
* Если вывод другой, например отрицательный, 
*"такты" тоже вряд ли близки к истине. В этом случае 
* можно попробовать уменьшить LOOP_BASE. При этом LOOP_BASE
* должен делиться на 4.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <functional>
#include "BigInt.h"

/*
 * Другие дефайны менять не стоит 
 */
#define LOOP_BASE 100

#define WS BigInt::WS
#define TK 1000
#define TOTAL_ITERATIONS (4 * LOOP_BASE * TK)
#define IPL (TOTAL_ITERATIONS * 4.0 / WS)
#define NI 50

// Measure empty loop cycles
long long lpt() {
	unsigned int t1h, t1l, t2h, t2l;
	asm volatile ( "rdtsc\n" : "=a" (t1l), "=d" (t1h) );
	for (int j = 0; j < TK; j++) asm volatile ( "" );
	asm volatile ( "rdtsc\n" : "=a" (t2l), "=d" (t2h) );
	
	return ((unsigned int)-1)*(t2h - t1h) + t2l - t1l;
}


int main() {
    unsigned int t1h, t1l, t2h, t2l;
    long long loop_time = lpt();
     
    long long rr[2];
	long long r[NI]; 
    for (int k = 0; k <= 1; k++) {
		// s0 - number of 32bit limbs in bigints, 
		// ss - number of realisation-specific words
		int s0 = LOOP_BASE * (1 << k), s = s0 * 4 / WS;

		int* words = (int*)calloc(2*s0, 4);
		for (int* t = words; t < words + 2*s0 - 1; t++) *t = rand();
		BigInt *a = new BigInt(s, (void*)words);
		BigInt *b = new BigInt(s, (void*)(words+s0));

		for (int i = 0; i < NI; i++) {
			
			asm volatile ( "rdtsc\n" : "=a" (t1l), "=d" (t1h) );
			
			for (int j = 0; j < TOTAL_ITERATIONS / s0 / 2; j++) {
				*a += *b;
				*a -= *b;
			}
			
			asm volatile ( "rdtsc\n" : "=a" (t2l), "=d" (t2h) );
			
			long long diff = ((unsigned int)-1)*(t2h - t1h) + t2l - t1l;
			r[i] = diff ;
		}
		std::sort(r, r + NI, std::greater<long long>());
		std::reverse(r, r + NI);
		rr[k] = r[0];
	}

	printf(	"%1.3f, param = %lld\n", 
			(2*rr[1] - rr[0] - loop_time)/IPL, 
			(rr[0] - r[1])/TK/2);
    return 0;
}













