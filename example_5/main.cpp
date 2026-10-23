#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Номер текущего процесса
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Общее количество процессов

    int N = 4;                 // Размер матрицы N x N
    int local_N = N / size;    // Количество строк матрицы для каждого процесса

    vector<int> matrix;                    // Полная матрица — хранится на процессе 0
    vector<int> input_vector(N, 2);        // Вектор из N элементов, каждый равен 2
    vector<int> local_matrix(local_N * N); // Часть матрицы, полученная процессом
    vector<int> local_result(local_N, 0);  // Результаты для строк, обрабатываемых процессом
    vector<int> global_result;             // Полный результат — собирается на процессе 0

    // Процесс 0 создаёт матрицу из единиц и выделяет память под итоговый результат
    if (rank == 0) {
        matrix.resize(N * N, 1);
        global_result.resize(N);
    }

    // Раздаём процессы строки матрицы
    MPI_Scatter(matrix.data(), local_N * N, MPI_INT,
                local_matrix.data(), local_N * N, MPI_INT,
                0, MPI_COMM_WORLD);

    // Рассылаем вектор всем процессам
    MPI_Bcast(input_vector.data(), N, MPI_INT, 0, MPI_COMM_WORLD);

    // Каждый процесс умножает полученные строки матрицы на вектор
    for (int i = 0; i < local_N; i++) {
        for (int j = 0; j < N; j++) {
            local_result[i] += local_matrix[i * N + j] * input_vector[j];
        }
    }

    // Собираем частичные результаты на процессе 0
    MPI_Gather(local_result.data(), local_N, MPI_INT,
               global_result.data(), local_N, MPI_INT,
               0, MPI_COMM_WORLD);

    // Процесс 0 выводит результат
    if (rank == 0) {
        cout << "Результат умножения: ";
        for (int val : global_result) {
            cout << val << " ";
        }
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}