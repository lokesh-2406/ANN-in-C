# Artificial Neural Network in C

## Overview
This project implements a **Artificial Neural Network** in C, designed to recognize handwritten digits using the MNIST dataset. The network achieves accuracy ranging between **88% and 94%**, depending on training conditions. It includes functionality for training, testing, and predicting digit recognition.

### **Authors**
- **Aurindum Banerjee** (B23CS1006)
- **Lokesh Motwani** (B23CS1033)
- **Priyanshu Swain** (B23ME1050)
- **Rudraksha Dev Sharma** (B23PH1018)

This is submitted as part of the **ICS Spring 2024 Major Project**.

---

## Features
- **Train the Network:** Train the model on the MNIST training dataset.
- **Test Accuracy:** Evaluate the trained network on a test dataset.
- **Predict Digits:** Recognize a handwritten digit from a user-provided CSV file.

---

## Installation and Usage

### 1. Prerequisites
Ensure you have the following installed:
- GCC or any C compiler
- MNIST dataset in CSV format

### 2. Clone and Build
1. Clone or download the repository.
2. Place all source files (`main.c`, `neural.c`, `matrix.c`, `img.c`) and headers (`neural.h`, `matrix.h`, `img.h`) in the same directory.
3. Build the project using the provided `Makefile`.

```bash
make
```

### 3. Run the Program
Execute the compiled program:  
```bash
./main
```
### 4. Program Options
Upon running the program, choose from the following options:

`0`: Train the network using the MNIST training dataset.  
`1`: Test the network's accuracy using the MNIST test dataset.  
`2`: Predict a digit from a user-provided CSV file.
## Preparing Data
### Custom Digit Input
To use your own digit for prediction:

1. Visit [MNIST Draw Tool](https://www.ccom.ucsd.edu/~cdeotte/programs/MNIST.html).  
2. Draw your digit and click Classify.  
3. Download the file as CSV without labels.  
4. Place the downloaded file in the same directory as the executable.
5. When prompted in the program, enter the file name (e.g., digit.csv).

## Known Issues
**Cross-Platform Compatibility:** The mkdir() function in neural.c uses syntax specific to macOS/Linux.   On Windows, remove the second argument (0777) from mkdir() to resolve errors.  
 
**File Placement:** Ensure CSV files and the executable are in the same directory.

## Dependencies
This project depends on:

Standard libraries (math.h, stdio.h, etc.)
Custom modules: neural, matrix, img
To compile, you may use the provided Makefile.

## Contributing  
  Feel free to contribute by submitting pull requests or opening issues for bugs and feature requests.

## License
This project is for educational purposes and part of an academic submission. Please consult the authors before reusing.# ANN-in-C
