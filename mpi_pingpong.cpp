#include <mpi.h>
#include <iostream>
#include <random>
#include <vector>
#include <iomanip>

using namespace std;

std::random_device rd;
std::mt19937 generator(rd());
std::uniform_int_distribution<int> distribution(1, 10000);

void process_communication(int rank) {
    	int value = distribution(generator);
	vector<double> RTT_val;

	for (int i = 0; i < 6; ++i) {
		double start_time, end_time, RTT;
		if (rank == 0) {
	    		start_time = MPI_Wtime(); // Inicia la medición

	                MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	                MPI_Recv(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    			end_time = MPI_Wtime(); // Termina la medición
			RTT = (end_time - start_time) * 1000;
			value++;
    		}

		else {
			start_time = MPI_Wtime(); // Inicia la medición
	    		MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	                value++;
	    
			MPI_Send(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	                end_time = MPI_Wtime(); // Termina la medición
			RTT = (end_time - start_time) * 1000;
     		}

		RTT_val.push_back(RTT);
	}

	// Gráfico del RTT que percibe cada nodo
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0){
		cout << "\n========================================\n";
		cout << "  Nodo " << rank << " - RTT por iteración (μs)\n";
		cout << "========================================\n";
		cout << "   Iteración      RTT (microsegundos)\n";
		cout << "----------------------------------------\n";
	
		for (int i = 0; i < 6; i++) {
			    cout << setw(6) << i << "      " << setw(6) << RTT_val[i] << " μs\n";
		}

		cout << "========================================\n";
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 1){
                cout << "\n========================================\n";
                cout << "  Nodo " << rank << " - RTT por iteración (μs)\n";
                cout << "========================================\n";
		cout << "   Iteración      RTT (microsegundos)\n";
		cout << "----------------------------------------\n";

		for (int i = 0; i < 6; i++) {
			cout << setw(6) << i << "      " << setw(6) << RTT_val[i] << " μs\n";
		}

		cout << "========================================\n";
	}
}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

        int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        process_communication(rank);

	MPI_Finalize();
        return 0;
}
