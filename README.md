# LBYARCH_MP2

## How to Run

You may use

```
nasm -f win64 asmfunc.asm -o asmfunc.o
gcc main.c asmfunc.o -o main.exe
main.exe
```

or the .bat file created

```
.\build_and_run.bat
```

## Grayscale Image Converter: uint8 to float

This project implements a grayscale image converter that transforms integer pixel values (0-255) to floating-point representation (0.0-1.0) using x86-64 assembly with scalar SIMD instructions.

## Performance Benchmarking Results

### Execution Times (30 iterations)

| Image Size | Total Pixels | Average Time (seconds) | Average Time (ms) | Average Time (μs) |
|------------|--------------|------------------------|-------------------|-------------------|
| 10×10      | 100          | 0.000000373           | 0.000373          | 0.373             |
| 100×100    | 10,000       | 0.000015043           | 0.015043          | 15.043            |
| 1000×1000  | 1,000,000    | 0.001519663           | 1.519663          | 1519.663          |

### Analysis

The execution time scales linearly with the number of pixels, demonstrating O(n) complexity. The assembly implementation processes approximately 658,000 pixels per millisecond on average, showing efficient conversion through scalar SIMD operations (`cvtsi2ss`, `mulss`).

Each test used randomized pixel values from 0-255 and was ran for 30 iterations to obtain average execution times. All outputs were verified for correctness against a C reference implementation using the formula f = i / 255. This ensures both performance and accuracy of the assembly implementation.

## Program Output

![Program Output and Correctness Check](Images/Program%20Output%20and%20Correctness%20Check.png)

### Demo Video Link: https://drive.google.com/file/d/1BcxSiD0_J7iF4FWvCVe0JcI-hLYtMQ__/view?usp=sharing
