# Knuth-Morris-Pratt (KMP) Algorithm

This project implements the Knuth-Morris-Pratt (KMP) algorithm for searching a particular pattern in a given DNA sequence. The algorithm is implemented using various parallel programming paradigms such as OpenMP, MPI, and CUDA.

## Project Details

- The project includes implementations of the KMP algorithm in OpenMP, MPI, and CUDA.
- The KMP algorithm efficiently searches for a pattern in a DNA sequence by utilizing the concept of the longest proper prefix suffix (LPS) array.
- The OpenMP implementation uses parallelism to speed up the search process by dividing the sequence into multiple blocks and searching them concurrently.
- The MPI implementation distributes the search among multiple processes, where each process searches a portion of the sequence, and the results are combined to find the pattern occurrences.
- The CUDA implementation utilizes GPU parallelism to accelerate the search process by exploiting the computational power of the GPU.

## File Structure

- `OpenMP_KMP.cpp`: Contains the implementation of the KMP algorithm using OpenMP parallelism.
- `OpenMP_KMP_Parallel.cpp`: Contains the implementation of the KMP algorithm with parallelization using OpenMP, dividing the sequence into blocks for concurrent searching.
- `MPI_KMP_Serial.cpp`: Contains the serial implementation of the KMP algorithm using MPI for message passing.
- `MPI_KMP_Parallel.cpp`: Contains the parallel implementation of the KMP algorithm using MPI for message passing, distributing the search among multiple processes.
- `CUDA_KMP.cu`: Contains the CUDA implementation of the KMP algorithm, utilizing GPU parallelism.

## Usage

- Compile the desired implementation using the respective compiler and flags.
- Execute the compiled binary with the necessary command-line arguments, such as the DNA sequence file and the pattern to search for.
- The program will output the indices where the pattern is found in the DNA sequence.
- For the OpenMP and MPI implementations, the number of threads/processes can be specified to control the parallel execution.

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please feel free to create an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

