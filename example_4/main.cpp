#include <iostream>
#include <mpi.h>
using namespace std;
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int ping_pong_count = 0;
    int limit = 5;

    while (ping_pong_count < limit) {
        if (rank == ping_pong_count % 2) {
            // Моя очередь бросать
            ping_pong_count++;
            int target = (rank + 1) % 2;
            MPI_Send(&ping_pong_count, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
            cout << "Процесс " << rank << " отправил число " << ping_pong_count << " процессу " << target << endl;
        } else {
            // Моя очередь ловить
            MPI_Recv(&ping_pong_count, 1, MPI_INT, (rank + 1) % 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cout << "Процесс " << rank << " получил число " << ping_pong_count << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
