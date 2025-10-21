#include <stdio.h>
#include<time.h>
#include <stdlib.h>
void generateRandomArrayint(int arr[]) {
    for (int i = 0; i < 200000; i++) {
        arr[i] = rand();
    }
}
void generateRandomArrayfloat(double arr[]) {
    for (int i = 0; i < 200000; i++) {
        arr[i] = (double)rand();
    }
}