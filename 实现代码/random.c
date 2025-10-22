#include <stdlib.h>

void generateRandomArrayint(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
}

void generateRandomArrayfloat(double arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = (double)rand();
    }
}
