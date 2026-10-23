#include <mpi.h>
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total_simulations = 1000000; // Миллион сценариев рынка
    int local_simulations = total_simulations / size;
    
    double local_profit = 0.0;
    
    // Каждый узел симулирует свою часть сценариев независимо
    srand(rank + 1234); // Разный "seed" для каждого процесса
    for (int i = 0; i < local_simulations; i++) {
        // Симуляция изменения цены (условно)
        double random_fluctuation = ((double)rand() / RAND_MAX) * 2.0 - 1.0; 
        if (random_fluctuation > 0) local_profit += random_fluctuation;
    }

    double global_profit = 0.0;
    
    // Сбор результатов (редукция)
    MPI_Reduce(&local_profit, &global_profit, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Ожидаемая общая прибыль по всем сценариям: " << global_profit << endl;
    }

    MPI_Finalize();
    return 0;
}
