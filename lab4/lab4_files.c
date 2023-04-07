#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// This program creates the Fibonacci series with n terms
// It then writes it to a file in ASCII format and to a different file in binary format

// USAGE: lab4_files n output_ascii output_bin

//Benjamin Griepp

int main (int argc, char **argv) {
	long int ascii_size, binary_size;
	if (argc != 4) {
		fprintf(stderr, "Wrong number of command-line arguments: %d\n", argc);
		return -1;
	}
	
	int n = atoi(argv[1]);

	// TODO: complete the file opening code below
	FILE* out_ascii = fopen(argv[2], "w");
	if (out_ascii == NULL) {
		printf("%s", "The ASCII output file cannot be opened!");
		exit(-1);
	}

	FILE* out_bin = fopen(argv[3], "wb");
	if (out_bin == NULL) {
		printf("%s", "The binary output file cannot be opened!");
		exit(-1);
	}

	// TODO: compute the first n terms of the Fibonacci series
	// no need to use recursion or store the entire sequence 
	int f1 =1, f2 =1;

	// TODO: write first two terms to the files
	// ASCII file: a human-readable 32-bit integer and a newline character per line
	// binary file: a 32-bit integer for each number in the series

	for (int i = 2; i < n; i++)
	{
		int tmp = f1 + f2;
		f1 = f2;
		f2 = tmp;

		// TODO: write new number to files
		fprintf(out_ascii, "%d %d\n", f1, f2);
		fwrite(&f1, sizeof(int), 1, out_bin);
		fwrite(&f2, sizeof(int), 1, out_bin);
	}

	// TODO: close files
	// TODO: compute the size of the two files in bytes
	// print the sizes in stdout

	fseek(out_ascii, 0L, SEEK_END);
    ascii_size = ftell(out_ascii);

	fseek(out_bin, 0L, SEEK_END);
    binary_size = ftell(out_bin);

	printf("ASCII File is %ld bytes, Binary File is %ld bytes\n", ascii_size, binary_size);

	fclose(out_ascii);
	fclose(out_bin);

	return 0;
}

