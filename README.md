# Parallel Sorting Using MPI with Bubble Sort and Even-Odd Transposition

## Project Overview

This project implements parallel sorting using Message Passing Interface (MPI) in the C programming language. The algorithm combines local Bubble Sort with the Even-Odd Transposition technique to achieve distributed sorting across multiple processes.

## Objectives

* Understand parallel computing concepts.
* Implement distributed sorting using MPI.
* Compare local and global sorting operations.
* Demonstrate inter-process communication.

## Technologies Used

* C Programming Language
* MPI (OpenMPI / MPICH)
* Ubuntu Linux
* GitHub Actions

## Algorithm

1. Generate an unsorted array.
2. Distribute data among processes using MPI_Scatter.
3. Perform local Bubble Sort on each process.
4. Execute Even-Odd Transposition phases.
5. Exchange data between neighboring processes.
6. Gather sorted data using MPI_Gather.
7. Display the final sorted array.

## Compilation

```bash
mpicc parallel_sort.c -o parallel_sort
```

## Execution

```bash
mpirun -np 4 ./parallel_sort 16
```

## Expected Output

The program displays the original unsorted array followed by the completely sorted array after parallel execution.

## Author

Muhammad Tayyab
BS Computer Science
