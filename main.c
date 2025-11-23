#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// External assembly function
extern void imgCvtGrayInttoFloat(int* intPixels, float* floatPixels, int totalPixels);

// C implementation for comparison and verification
void imgCvtGrayInttoFloat_C(int* intPixels, float* floatPixels, int totalPixels) {
    for (int i = 0; i < totalPixels; i++) {
        floatPixels[i] = (float)intPixels[i] / 255.0f;
    }
}

// Function to get high-resolution time
double getTime() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

// Function to generate random pixel values
void generateRandomPixels(int* pixels, int size) {
    for (int i = 0; i < size; i++) {
        pixels[i] = rand() % 256;
    }
}

// Function to verify correctness of conversion
int verifyConversion(int* intPixels, float* floatPixels, int size) {
    const float epsilon = 0.0001f;
    for (int i = 0; i < size; i++) {
        float expected = (float)intPixels[i] / 255.0f;
        float actual = floatPixels[i];
        float diff = (expected - actual);
        if (diff < 0) diff = -diff;
        if (diff > epsilon) {
            printf("Verification failed at index %d: expected %.4f, got %.4f\n", 
                   i, expected, actual);
            return 0;
        }
    }
    return 1;
}

// Function to print image (for small sizes)
void printImage(int* intPixels, float* floatPixels, int height, int width) {
    printf("\nInput (int pixels):\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%3d", intPixels[i * width + j]);
            if (j < width - 1) printf(", ");
        }
        printf("\n");
    }
    
    printf("\nOutput (float pixels):\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%.2f", floatPixels[i * width + j]);
            if (j < width - 1) printf(", ");
        }
        printf("\n");
    }
}

// Function to benchmark the conversion
void benchmark(int height, int width, int iterations) {
    int totalPixels = height * width;
    
    // Allocate memory
    int* intPixels = (int*)malloc(totalPixels * sizeof(int));
    float* floatPixels = (float*)malloc(totalPixels * sizeof(float));
    
    if (!intPixels || !floatPixels) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Generate random pixel values
    generateRandomPixels(intPixels, totalPixels);
    
    // Warm-up run
    imgCvtGrayInttoFloat(intPixels, floatPixels, totalPixels);
    
    // Verify correctness
    if (verifyConversion(intPixels, floatPixels, totalPixels)) {
        printf("Verification passed for %dx%d image\n", height, width);
    } else {
        printf("Verification FAILED for %dx%d image\n", height, width);
        free(intPixels);
        free(floatPixels);
        return;
    }
    
    // Benchmark
    double totalTime = 0.0;
    for (int i = 0; i < iterations; i++) {
        double startTime = getTime();
        imgCvtGrayInttoFloat(intPixels, floatPixels, totalPixels);
        double endTime = getTime();
        totalTime += (endTime - startTime);
    }
    
    double avgTime = totalTime / iterations;
    printf("Average execution time for %dx%d (%d pixels) over %d iterations: %.9f seconds\n", 
           height, width, totalPixels, iterations, avgTime);
    printf("Average time in milliseconds: %.9f ms\n", avgTime * 1000.0);
    printf("Average time in microseconds: %.9f us\n", avgTime * 1000000.0);
    printf("Average time in nanoseconds: %.3f ns\n\n", avgTime * 1000000000.0);
    
    free(intPixels);
    free(floatPixels);
}

int main() {
    srand((unsigned int)time(NULL));
    
    printf("=== Grayscale Image Converter: uint8 to float ===\n\n");
    
    // Benchmark different sizes
    printf("Performance Benchmarking (30 iterations each)\n\n");
    
    printf("10x10 Image\n");
    benchmark(10, 10, 30);
    
    printf("100x100 Image\n");
    benchmark(100, 100, 30);
    
    printf("1000x1000 Image\n");
    benchmark(1000, 1000, 30);
    
    // Interactive mode
    printf("Enter height and width (or 0 0 to exit): ");
    
    while (1) {
        int h, w;
        if (scanf("%d %d", &h, &w) != 2) {
            printf("Invalid input. Please enter two integers.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (h == 0 || w == 0) {
            break;
        }
        
        if (h <= 0 || w <= 0 || h > 10000 || w > 10000) {
            printf("Invalid dimensions. Please enter positive values (max 10000x10000).\n");
            continue;
        }
        
        int total = h * w;
        int* intPixels = (int*)malloc(total * sizeof(int));
        float* floatPixels = (float*)malloc(total * sizeof(float));
        
        if (!intPixels || !floatPixels) {
            printf("Memory allocation failed\n");
            if (intPixels) free(intPixels);
            if (floatPixels) free(floatPixels);
            continue;
        }
        
        printf("Enter %d pixel values (0-255):\n", total);
        for (int i = 0; i < total; i++) {
            int val;
            if (scanf("%d", &val) != 1 || val < 0 || val > 255) {
                printf("Invalid pixel value. Using random values instead.\n");
                generateRandomPixels(intPixels, total);
                break;
            }
            intPixels[i] = val;
        }
        
        imgCvtGrayInttoFloat(intPixels, floatPixels, total);
        
        if (h <= 10 && w <= 10) {
            printImage(intPixels, floatPixels, h, w);
        }
        
        if (verifyConversion(intPixels, floatPixels, total)) {
            printf("Conversion verified successfully\n");
        }
        
        free(intPixels);
        free(floatPixels);
        
        printf("\nEnter height and width (or 0 0 to exit): ");
    }
    
    return 0;
}
