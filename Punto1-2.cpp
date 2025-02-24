#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <unistd.h>

int timeA = 0;
int timeB = 0;
int timeC = 0;
int timeD = 0;
int timeE = 0;


int func_A() {
	timeA = rand() % 3 + 1;  // Espera entre 1 y 3 segundos
	sleep(timeA);
        std::cout << "Ejecución de A (Espero " << timeA << "seg)\n";
	return timeA;
}

int func_B() {
	timeB = rand() % 3 + 1;
	sleep(timeB);
	std::cout << "Ejecución de B (Espero " << timeB << "seg)\n";
	return timeB;
}

int func_C() {
	timeC = rand() % 3 + 1;
        sleep(timeC);
        std::cout << "Ejecución de C (Espero " << timeC << "seg)\n";
        return timeC;
}

int func_D() {
	timeD = timeB + timeC;
        sleep(timeD);
        std::cout << "Ejecución de D (Espero " << timeD << "seg)\n";
        return timeD;
	
}

int func_E() {
	timeE = timeA + timeD;
        sleep(timeE);
        std::cout << "Ejecución de E (Espero " << timeE << "seg)\n";
        return timeE;
	
}

int main() {
	srand(time(NULL));

	#pragma omp parallel
	{
		#pragma omp single
		{
			#pragma omp task depend(out: timeA)
			{
			    timeA = func_A();
			}

			#pragma omp task depend(in: timeA) depend(out: timeB)
			{
			    timeB = func_B();
			}

			#pragma omp task depend(in: timeA) depend(out: timeC)
			{
			    timeC = func_C();
			}	

			#pragma omp task depend(in: timeB, timeC) depend(out: timeD)
			{
			    timeD = func_D();
			}

			#pragma omp task depend(in: timeA, timeD)
			{
			    timeE = func_E();
			}
		}
	}
	return 0;
}
