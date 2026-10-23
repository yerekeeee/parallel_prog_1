#include <mpi.h>
#include <iostream>
#include <vector>
using namespace std;
int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int total_pixels = 800; // Условное разрешение
    int chunk_size = total_pixels / size;
    
    vector<int> image;
    vector<int> local_pixels(chunk_size);

    if (rank == 0) {
        image.resize(total_pixels, 0); // Изначально пустое изображение
    }

    // В данном случае scatter не обязателен, если данные генерируются с нуля,
    // но он нужен, если мы распределяем веса нейросетей.

    // Вычисляем свою порцию пикселей (Ray Tracing)
    for (int i = 0; i < chunk_size; i++) {
        // Условно: сложные вычисления освещения
        local_pixels[i] = (rank + 1) * 255; 
    }

    // Собираем куски обратно в целую картинку на нулевом процессе
    MPI_Gather(local_pixels.data(), chunk_size, MPI_INT, 
               image.data(), chunk_size, MPI_INT, 
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Рендеринг завершен. Собрано " << image.size() << " пикселей." << endl;
    }

    MPI_Finalize();
    return 0;
}