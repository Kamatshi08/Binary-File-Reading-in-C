#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

// Function to read complex numbers from a binary file
double complex* read_complex_binary_file(const char* filename, int* length) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Each complex number consists of two double values (real and imaginary)
    int num_elements = filesize / (2 * sizeof(double));
    *length = num_elements;

    // Allocate memory for the complex array
    double complex *data = (double complex*) malloc(num_elements * sizeof(double complex));
    if (!data) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read the data
    for (int i = 0; i < num_elements; i++) {
        double real, imag;
        fread(&real, sizeof(double), 1, file);
        fread(&imag, sizeof(double), 1, file);
        data[i] = real + imag * I;
    }

    fclose(file);
    return data;
}

// Function to compute the correlation
double complex correlate(double complex *reference, double complex *signal, int length) {
    double complex result = 0;
    for (int i = 0; i < length; i++) {
        result += reference[i] * conj(signal[i]);
    }
    return result;
}

int main() {
    const char* filename = "/mnt/c/work/1khz_reference_samples.bin";
    int length;

    // Read the complex numbers from the binary file
    double complex *reference = read_complex_binary_file(filename, &length);
    double complex *signal = read_complex_binary_file(filename, &length);  // Assuming signal data is also in the same file

    if (reference == NULL || signal == NULL) {
        fprintf(stderr, "Failed to read complex data from file.\n");
        return 1;
    }

    // Compute the correlation
    double complex result = correlate(reference, signal, length);
    printf("Correlation result: %f + %fi\n", creal(result), cimag(result));

    // Free the allocated memory
    free(reference);
    free(signal);

    return 0;
}
