#include<stdio.h>
#include <stdlib.h> 
#include<string.h>
#include<omp.h>
#include<time.h>
#include "sorts.h"
static int compareint(const void *a,const void *b){
    return (*(int*)a-*(int*)b);
}

static int compareDouble(const void *a,const void *b){
    if(*(double*)a<*(double*)b)return -1;
    else if(*(double*)a>*(double*)b)return 1;
    else return 0;
}
static void merge(void * base,int left,int mid,int right,size_t size,int(*compare)(const void*,const void*)){
    char* arr=(char*)base;
    int i,j,k;
    int n1=mid-left+1;
    int n2=right-mid;
    if(n1<=0 || n2<=0) return;
    char *L=malloc((size_t)n1*size);
    char *R=malloc((size_t)n2*size);
    if(!L || !R){
        fprintf(stderr, "malloc failed in merge\n");
        free(L); free(R);
        return;
    }
    for(i=0;i<n1;i++){
        memcpy(L+i*size,arr+(left+i)*size,size);
    }
    for(j=0;j<n2;j++){
        memcpy(R+j*size,arr+(mid+1+j)*size,size);
    }
    i=0;j=0;k=left;
    while(i<n1 && j<n2){
        int cmp = compare(L+i*size, R+j*size);
        if(cmp <= 0){
            memcpy(arr+k*size, L+i*size, size);
            i++;
        } else {
            memcpy(arr+k*size, R+j*size, size);
            j++;
        }
        k++;
    }
    while(i<n1){
        memcpy(arr+k*size,L+i*size,size);
        k++; i++;
    }
    while (j<n2){
        memcpy(arr+k*size,R+j*size,size);
        k++; j++;
    }
    free(L);
    free(R);
}

static void mergeSortRecu(void * base,int low,int high,int size,int(*compare)(const void*,const void*)){
    if(low>=high) return;
    int mid = low + (high - low) / 2;
    char *arr=(char*)base;
    const int TASK_THRESHOLD = 1000; // 小任务不拆分为 OpenMP task
    if(high - low > TASK_THRESHOLD){
        #pragma omp task shared(arr) firstprivate(low,mid,high,size,compare)
        mergeSortRecu(arr, low, mid, size, compare);
        #pragma omp task shared(arr) firstprivate(low,mid,high,size,compare)
        mergeSortRecu(arr, mid+1, high, size, compare);
        #pragma omp taskwait
    } else {
        mergeSortRecu(arr, low, mid, size, compare);
        mergeSortRecu(arr, mid+1, high, size, compare);
    }
    merge(arr, low, mid, high, size, compare);
}

// --- 文件读取与主流程 ---
static int *read_ints_from_file(const char *path, size_t *out_count){
    FILE *f = fopen(path, "r");
    if(!f) return NULL;
    size_t cap = 1024, n = 0;
    int *arr = malloc(cap * sizeof(int));
    char line[128];
    while(fgets(line, sizeof(line), f)){
        char *p = line;
        while(*p==' '||*p=='\t') p++;
        if(*p==0||*p=='\n'||*p=='\r') continue;
        int v = atoi(p);
        if(n>=cap){ cap*=2; arr=realloc(arr,cap*sizeof(int)); }
        arr[n++] = v;
    }
    fclose(f);
    *out_count = n;
    return arr;
}

static double *read_doubles_from_file(const char *path, size_t *out_count){
    FILE *f = fopen(path, "r");
    if(!f) return NULL;
    size_t cap = 1024, n = 0;
    double *arr = malloc(cap * sizeof(double));
    char line[128];
    while(fgets(line, sizeof(line), f)){
        char *p = line;
        while(*p==' '||*p=='\t') p++;
        if(*p==0||*p=='\n'||*p=='\r') continue;
        double v = atof(p);
        if(n>=cap){ cap*=2; arr=realloc(arr,cap*sizeof(double)); }
        arr[n++] = v;
    }
    fclose(f);
    *out_count = n;
    return arr;
}

static int is_sorted_int(const int *a, size_t n){
    for(size_t i=1;i<n;i++) if(a[i-1]>a[i]) return 0;
    return 1;
}
static int is_sorted_double(const double *a, size_t n){
    for(size_t i=1;i<n;i++) if(a[i-1]>a[i]) return 0;
    return 1;
}

static void print_usage(const char *prog){
    fprintf(stderr, "Usage: %s <input_file> <type>\n", prog);
    fprintf(stderr, "type: int | float\n");
}

#ifdef STANDALONE_MENCYSORT
int main(int argc, char **argv){
    if(argc<3){ print_usage(argv[0]); return 1; }
    const char *path = argv[1];
    const char *type = argv[2];
    size_t n=0;
    double time_ms=0.0;
    int correct=0;
    srand((unsigned)time(NULL));
    if(strcmp(type,"int")==0){
        int *arr = read_ints_from_file(path,&n);
        if(!arr){ fprintf(stderr, "Failed to open or parse %s\n", path); return 2; }
        double start_time = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            mergeSortRecu(arr,0,(int)n-1,sizeof(int),compareint);
        }
        double end_time = omp_get_wtime();
        time_ms = (end_time - start_time) * 1000.0;
        correct = is_sorted_int(arr,n);
        free(arr);
    } else if(strcmp(type,"float")==0){
        double *arr = read_doubles_from_file(path,&n);
        if(!arr){ fprintf(stderr, "Failed to open or parse %s\n", path); return 2; }
        double start_time = omp_get_wtime();
        #pragma omp parallel
        {
            #pragma omp single
            mergeSortRecu(arr,0,(int)n-1,sizeof(double),compareDouble);
        }
        double end_time = omp_get_wtime();
        time_ms = (end_time - start_time) * 1000.0;
        correct = is_sorted_double(arr,n);
        free(arr);
    } else {
        print_usage(argv[0]); return 3;
    }
    printf("TIME_MS:%.3f\n", time_ms);
    printf("CORRECT:%d\n", correct);
    return 0;
}

// Generic wrappers to match sorts.h declarations
void merge_sort_generic(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num == 0) return;
    mergeSortRecu(base, 0, (int)num - 1, (int)size, (int(*)(const void*,const void*))compare);
}

void merge_sort_parallel_generic(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num == 0) return;
    // parallel merge sort using OpenMP tasks as implemented
    #pragma omp parallel
    {
        #pragma omp single
        mergeSortRecu(base, 0, (int)num - 1, (int)size, (int(*)(const void*,const void*))compare);
    }
}