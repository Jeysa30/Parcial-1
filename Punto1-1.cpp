#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

// Estructura para manejar dependencias con tiempos
struct ThreadInfo {
	pthread_t tid;
	int wait_time;
};

void* func_A(void* arg) {
	int* wait_time = new int(rand() % 3 + 1);
	cout << "Ejecucion de A (" << *wait_time << " s)" << endl;
	sleep(*wait_time);
	return wait_time;
}

void* func_B(void* arg) {
	int* wait_time = new int(rand() % 3 + 1);
        cout << "Ejecucion de B (" << *wait_time << " s)" << endl;
        sleep(*wait_time);
	return wait_time;
}

void* func_C(void* arg) {
	int* wait_time = new int(rand() % 3 + 1);
        cout << "Ejecucion de C (" << *wait_time << " s)" << endl;
        sleep(*wait_time);
        return wait_time;
}

void* func_D(void* arg) {
	ThreadInfo* tids = (ThreadInfo*) arg;
	int *timeB, *timeC;
        pthread_join(tids[0].tid, (void**)&timeB);
	pthread_join(tids[1].tid, (void**)&timeC);

        int* wait_time = new int(rand() % 3 + 1);
	cout << "Ejecucion de D (tiempo de espera B:" << *timeB 
		<< " s, C:" << *timeC << " s, se ejecuto en " << *wait_time << " s)" << endl;
	sleep(*wait_time);

	delete timeB;
	delete timeC;
        delete[] tids;
	return wait_time;
	pthread_exit(wait_time);
}

void* func_E(void* arg) {
	ThreadInfo* tids = (ThreadInfo*) arg;
        int *timeA, *timeD;
        pthread_join(tids[0].tid, (void**)&timeA);
        pthread_join(tids[1].tid, (void**)&timeD);

        int* wait_time = new int(rand() % 3 + 1);
        cout << "Ejecucion de E (tiempo de espera A:" << *timeA
		<< " s, D:" << *timeD << " s, se ejecuto en " << *wait_time << " s)" << endl;
        sleep(*wait_time);

        delete timeA;
        delete timeD;
        delete[] tids;
        return wait_time;
        pthread_exit(wait_time);
}

int main() {
	srand(time(NULL));
	pthread_t tidA, tidB, tidC, tidD, tidE;
	int *timeA, *timeB, *timeC, *timeD, *timeE;
	       
        // A se ejecuta primero
	ThreadInfo* tidA_info = new ThreadInfo{0, 0};
	pthread_create(&tidA_info->tid, NULL, func_A, NULL);

	// B y C se ejecutan en paralelo
	ThreadInfo* tidB_info = new ThreadInfo{0, 0};
	ThreadInfo* tidC_info = new ThreadInfo{0, 0};
	pthread_create(&tidB_info->tid, NULL, func_B, NULL);
        pthread_create(&tidC_info->tid, NULL, func_C, NULL);

	// D debe esperar a B y C
	ThreadInfo* tidsBC = new ThreadInfo[2];
	tidsBC[0] = *tidB_info;
        tidsBC[1] = *tidC_info;
	ThreadInfo* tidD_info = new ThreadInfo{0, 0};
        pthread_create(&tidD_info->tid, NULL, func_D, tidsBC);

	// E debe esperar a A y D
	ThreadInfo* tidsAD = new ThreadInfo[2];
        tidsAD[0] = *tidA_info;
        tidsAD[1] = *tidD_info;
	pthread_create(&tidE, NULL, func_E, tidsAD);
	
 	// Sincronización final
	pthread_join(tidA_info->tid, (void**)&timeA);
	pthread_join(tidB_info->tid, (void**)&timeB);
	printf("trermino B");
        pthread_join(tidC_info->tid, (void**)&timeC);
	printf("trermino C");
	pthread_join(tidD_info->tid, (void**)&timeD);
	printf("termino D");
        pthread_join(tidE, (void**)&timeE);
	printf("termino E");

	delete timeA;
        delete timeB;
	delete timeC;
        delete timeD;
	delete timeE;
        delete tidB_info;
	delete tidC_info;
	delete tidA_info;
        delete tidD_info;
	printf("Finalizo todo correctamente\n");

	return 0;
}
