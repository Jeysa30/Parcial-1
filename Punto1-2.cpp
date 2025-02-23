#include <iostream>
#include <omp.h>

void func_A() {
	std::cout << "Ejecucion de A\n";
}

void func_B() {
	std::cout << "Ejecucion de B\n";
}

void func_C() {
	std::cout << "Ejecucion de C\n";
}

void func_D() {
	std::cout << "Ejecucion de D\n";
}

void func_E() {
	std::cout << "Ejecucion de E\n";
}

int main() {
	// Variables para que las dependencias funcionen correctamente
	int A, B, C, D;

	#pragma omp parallel
	{
		#pragma omp single
		{
			#pragma omp task depend(out: A)
			{
			    func_A();
			}

			#pragma omp task depend(in: A) depend(out: B)
			{
			    func_B();
			}

			#pragma omp task depend(in: A) depend(out: C)
			{
			    func_C();
			}	

			#pragma omp task depend(in: B, C) depend(out: D)
			{
			    func_D();
			}

			#pragma omp task depend(in: A, D)
			{
			    func_E();
			}
		}
	}
	return 0;
}
