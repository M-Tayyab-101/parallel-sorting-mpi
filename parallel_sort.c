#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort(int arr[], int n)
{
    int i, j, temp;

    for(i = 0; i < n - 1; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void mergeLow(int local[], int received[], int n)
{
    int *temp = (int*)malloc(2 * n * sizeof(int));

    int i = 0, j = 0, k = 0;

    while(i < n && j < n)
    {
        if(local[i] <= received[j])
            temp[k++] = local[i++];
        else
            temp[k++] = received[j++];
    }

    while(i < n)
        temp[k++] = local[i++];

    while(j < n)
        temp[k++] = received[j++];

    for(i = 0; i < n; i++)
        local[i] = temp[i];

    free(temp);
}

void mergeHigh(int local[], int received[], int n)
{
    int *temp = (int*)malloc(2 * n * sizeof(int));

    int i = n - 1;
    int j = n - 1;
    int k = 2 * n - 1;

    while(i >= 0 && j >= 0)
    {
        if(local[i] >= received[j])
            temp[k--] = local[i--];
        else
            temp[k--] = received[j--];
    }

    while(i >= 0)
        temp[k--] = local[i--];

    while(j >= 0)
        temp[k--] = received[j--];

    for(i = 0; i < n; i++)
        local[i] = temp[i + n];

    free(temp);
}

int main(int argc, char *argv[])
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 16;

    if(argc > 1)
        N = atoi(argv[1]);

    if(N % size != 0)
    {
        if(rank == 0)
            printf("Array size must be divisible by number of processes.\n");

        MPI_Finalize();
        return 0;
    }

    int local_n = N / size;

    int *data = NULL;

    if(rank == 0)
    {
        data = (int*)malloc(N * sizeof(int));

        srand(time(NULL));

        printf("Original Array:\n");

        for(int i = 0; i < N; i++)
        {
            data[i] = rand() % 100;
            printf("%d ", data[i]);
        }
        printf("\n\n");
    }

    int *local_data = (int*)malloc(local_n * sizeof(int));

    MPI_Scatter(
        data,
        local_n,
        MPI_INT,
        local_data,
        local_n,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    bubbleSort(local_data, local_n);

    int *recv_buffer = (int*)malloc(local_n * sizeof(int));

    for(int phase = 0; phase < size; phase++)
    {
        int partner;

        if(phase % 2 == 0)
        {
            if(rank % 2 == 0)
                partner = rank + 1;
            else
                partner = rank - 1;
        }
        else
        {
            if(rank % 2 == 0)
                partner = rank - 1;
            else
                partner = rank + 1;
        }

        if(partner < 0 || partner >= size)
            continue;

        MPI_Sendrecv(
            local_data,
            local_n,
            MPI_INT,
            partner,
            0,
            recv_buffer,
            local_n,
            MPI_INT,
            partner,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );

        if(rank < partner)
            mergeLow(local_data, recv_buffer, local_n);
        else
            mergeHigh(local_data, recv_buffer, local_n);
    }

    MPI_Gather(
        local_data,
        local_n,
        MPI_INT,
        data,
        local_n,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    if(rank == 0)
    {
        printf("Sorted Array:\n");

        for(int i = 0; i < N; i++)
            printf("%d ", data[i]);

        printf("\n");

        free(data);
    }

    free(local_data);
    free(recv_buffer);

    MPI_Finalize();
    return 0;
}
