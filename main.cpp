/*
 * ВЫВОД
 * Количество тактов 
 * на итерацию сложения/вычитания (4, 8, 16 или 32 байта)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <functional>
#include "BigInt.h"

/*
 * Длина массивов длинных целых в байтах. 
 * LOOP_BASE должен делиться на WS = 32 (GBigInt).
 * Желательно кратно размеру кеш-строки (64 на AMD K10, Intel Core2, SB)
 * 1152 - рекомендуемое значение для AMD (64*18)
 * 1024 - для Intel (?)
 */ 
#define LOOP_BASE (64*18)

/*
 * В качестве результата берется наименьший из стольки прогонов.
 * Фильтрует всякие кеш-миссы и т. д.
 */ 
#define NI 30
/*
 * Эти дефайны менять не стоит 
 */
#define WS BigInt::WS
#define IPL (LOOP_BASE * 1.0 / WS)

typedef unsigned long long ull;

inline ull rdtsc() {
	unsigned int lo, hi;
	asm volatile ( "rdtsc\n" : "=a" (lo), "=d" (hi) );
	return ((ull)hi << 32) | lo;
}

int main() {
	ull rr[2];
	int s = LOOP_BASE*2;
	void * s1 = malloc(s);
	void * s2 = malloc(s);
	//for (int j = 0; j < NI2; j++)
		
		for (int k = 1; k <= 2; k++) {
			s = LOOP_BASE*k;
			BigInt *a = new BigInt(s/WS, s1);
			BigInt *b = new BigInt(s/WS, s2);
			ull t1, t2;
			ull r[NI];
			for (int i = 0; i < NI; i++) {
				t1 = rdtsc();
				*a += *b;
				t2 = rdtsc();
				r[i] = t2 - t1;
				
			}
			std::sort(r, r + NI, std::greater<ull>());
			rr[k-1] = r[NI-1];
		}
	printf("%1.3f\n", (rr[1]-rr[0])/IPL);
}














