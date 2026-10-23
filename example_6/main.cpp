#include <iostream>
#include <vector>
#include <mpi.h>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 10000;
    int chunk = N / size;
    vector<int> data;
    vector<int> local_data(chunk);

    if (rank == 0) {
        data.resize(N);
        for(int i=0; i<N; i++) data[i] = rand() % 100000;
        data[555] = 999999; // Гарантированно самый большой элемент
    }

    // Раздаем куски массива
    MPI_Scatter(data.data(), chunk, MPI_INT, 
                local_data.data(), chunk, MPI_INT, 
                0, MPI_COMM_WORLD);

    // Ищем максимум в своем куске
    int local_max = -1;
    for (int i = 0; i < chunk; i++) {
        if (local_data[i] > local_max) local_max = local_data[i];
    }

    // Выбираем глобальный максимум
    int global_max;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Глобальный максимум: " << global_max << endl;
    }

    MPI_Finalize();
    return 0;
}