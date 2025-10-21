#include <stdio.h>
#include "sorts.h"

int compare_int(const void* a, const void* b){ return (*(int*)a) - (*(int*)b); }

int main(){
    int arr[] = {5,3,8,1,9,2,4,7,6};
    int n = sizeof(arr)/sizeof(arr[0]);
    quick_sort_iterative_generic(arr, n, sizeof(int), compare_int);
    for(int i=0;i<n;i++) printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
