/*
 * ВЫВОД
 * NI2 замеров количества тактов 
 * на итерацию сложения/вычитания (4, 8, 16 или 32 байт)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <functional>
#include "BigInt.h"

/*
 * Длина массивов длинных целых "в интах". 
 * Т. е. длина в байтах = LOOP_BASE * 4
 * Чем больше - тем выше точность, но может не влезать
 * в некоторое число строк L1 или какой-нибудь еще 
 *"мгновенный" кеш, надо подбирать. 200 это достаточно много.
 * LOOP_BASE должен делиться на (WS / 4) = 8 (GBigInt).
 */ 
#define LOOP_BASE 200
/*
 * Выводится столько измерений. 
 */
#define NI2 5
/*
 * В качестве результата берется наименьший из стольки.
 * Фильтрует всякие кеш-миссы и т. д.
 */ 
#define NI 50

/*
 * Эти дефайны менять не стоит 
 */
#define WS BigInt::WS
#define TK 1000
#define TOTAL_ITERATIONS (4 * LOOP_BASE * TK)
#define IPL (TOTAL_ITERATIONS * 4.0 / WS)



// Measure timing
long long lpt() {
	unsigned int t1h, t1l, t2h, t2l;
	asm volatile ( "rdtsc\n" : "=a" (t1l), "=d" (t1h) );
	for (int i = 0; i < 2 * TK; i++) asm volatile ( "" );
	asm volatile ( "rdtsc\n" : "=a" (t2l), "=d" (t2h) );
	
	return ((unsigned int)-1)*(t2h - t1h) + t2l - t1l;
}


int main() {
    unsigned int t1h, t1l, t2h, t2l;
    long long m_time = lpt();
    
    
    long long rr[2];
	long long r[NI];
	double rrr[NI2];
	for (int i2 = 0; i2 < NI2; i2++) { 
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
			rr[k] = r[NI - 1];
		}
		rrr[i2] = ( 2*rr[1] - rr[0] - m_time % 1000 ) / IPL;
		/*
		 * кол-во тактов, которое тратилось на вход и выход 
		 * из методов сложения/вычитания. 
		 * Для современных процессоров ожидается
		 * результат 15 - 100 (если gcc оптимизируется хотя бы -O, 
		 * без него меняются фреймы и может быть хоть 500). 
		 * Если вывод другой, например отрицательный, 
		 *"такты" тоже вряд ли близки к истине.
		 */
		//(rr[0] - r[1] - m_time + m_time % 1000) / TK / 2
	}
	
	std::sort(rrr, rrr + NI2, std::greater<double>());
	
	for (int i2 = 0; i2 < NI2; i2++) 
		printf(	"%1.3f\t", rrr[i2]);
	printf("\n");
	
    return 0;
}













