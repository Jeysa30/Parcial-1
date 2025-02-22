#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* A(void* arg) {
	    printf("A\n");
	        return NULL;
}

void* B(void* arg) {
	    pthread_t tidA = *(pthread_t*)arg;
	        pthread_join(tidA, NULL);  // Espera a que termine A
		    printf("B\n");
		        return NULL;
}

void* C(void* arg) {
	    printf("C\n");
	        return NULL;
}

void* D(void* arg) {
	    pthread_t *tids = (pthread_t*)arg;
	        pthread_join(tids[0], NULL);  // Espera a B
		    pthread_join(tids[1], NULL);  // Espera a C
		        printf("D\n");
			    return NULL;
}

void* E(void* arg) {
	    pthread_t tidD = *(pthread_t*)arg;
	        pthread_join(tidD, NULL);  // Espera a D
		    printf("E\n");
		        return NULL;
}

int main() {
	pthread_t tidA, tidB, tidC, tidD, tidE;

        pthread_create(&tidA, NULL, A, NULL);
	pthread_create(&tidC, NULL, C, NULL);
        pthread_create(&tidB, NULL, B, &tidA);
	pthread_join(tidA, NULL);  // Garantiza que A haya terminado antes de continuar

        pthread_t tidsBC[2] = {tidB, tidC};
	pthread_create(&tidD, NULL, D, tidsBC);
				        
        pthread_create(&tidE, NULL, E, &tidD);
	pthread_join(tidD, NULL);
        pthread_join(tidE, NULL);

	return 0;
}
