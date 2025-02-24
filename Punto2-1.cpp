#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

pthread_t tidA, tidB, tidC, tidF;
int timeA = 0;
int timeB = 0;
int timeC = 0;
int timeD = 0;
int timeF = 0;

void* func_A(void* arg) {
        timeA = rand() % 3 + 1; 
        sleep(timeA);
        cout << "Ejecución de A en " << timeA << " seg"<< endl;
        pthread_exit(NULL);
}

void* func_B(void* arg) {
        timeB = rand() % 3 + 1;
        sleep(timeB);
	cout << "Ejecución de B en " << timeB << " seg"<< endl;
        pthread_exit(NULL);
}

void* func_C(void* arg) {
        timeC = rand() % 3 + 1;
        sleep(timeC);
        cout << "Ejecución de C en " << timeC << " seg"<< endl;
        pthread_exit(NULL);
}

void func_E(void* arg) { // E se ejecuta despues de D y F
	pthread_join(tidF, NULL);
        sleep(timeF + timeD);
        cout << "Ejecución de E en " << timeF + timeD << " seg" << endl;
}

void* func_F(void* arg) { // F se ejecuta despues de A y B
        timeF = timeA + timeB;
        sleep(timeF);
	cout << "Ejecución de F en " << timeF << " seg" << endl;
	pthread_exit(NULL);
}

void func_D(void* arg) {
        timeD = timeB + timeC;
        sleep(timeD);
        cout << "Ejecución de D en " << timeD << " seg" << endl;
        func_E(NULL);
}

int main() {
	srand(time(NULL));

        // A, B y C se ejecutan en paralelo
         pthread_create(&tidA, NULL, func_A, NULL);
         pthread_create(&tidB, NULL, func_B, NULL);
	 pthread_create(&tidC, NULL, func_C, NULL);
	
	 pthread_join(tidA, NULL);
	 pthread_join(tidB, NULL);
	 pthread_join(tidC, NULL);

	 // Creación del hilo para func_F
	 pthread_create(&tidF, NULL, func_F, NULL);

	 // D debe esperar a B y C
         func_D(NULL);
         
	 return 0;
}
