#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 12; // Факториал 12
    int chunk = N / size; // Предполагаем, что N делится на size без остатка
    
    int start = rank * chunk + 1;
    int end = (rank + 1) * chunk;
    
    long long local_prod = 1;
    for (int i = start; i <= end; i++) {
        local_prod *= i;
    }

    long long global_prod = 1;
    
    // Используем MPI_PROD для умножения локальных результатов
    MPI_Reduce(&local_prod, &global_prod, 1, MPI_LONG_LONG, MPI_PROD, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << N << "! = " << global_prod << endl;
    }

    MPI_Finalize();
    return 0;
}