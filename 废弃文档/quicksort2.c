#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
// add read-from-file and CLI support
#include "sorts.h"
static void swap(void* a,void*b ,size_t size){
    void *temp=malloc(size);
    memcpy(temp,a,size);
    memcpy(a,b,size);
    memcpy(b,temp,size);
    free(temp);
}

static int compare(const void *a,const void *b){
    return (*(int*)a-*(int*)b);
}

static int compareDouble(const void *a,const void *b){
    if(*(double*)a<*(double*)b)return -1;
    else if(*(double*)a>*(double*)b)return 1;
    else return 0;
}

static int pivotpos(void *base,int low,int high,size_t size,int pivotIndex,int(*compare)(const void*,const void*)){
    char *arr=(char*)base;
    swap(arr+pivotIndex*size,arr+high*size,size);
    void *pivot=arr+high*size;
    int i= low-1;
    for(int j=low;j<high;j++){
        if(compare((char*)base+j*size,pivot)<0){
            i++;
            swap(arr+i*size,arr+j*size,size);
        }
    }   
    swap(arr+(i+1)*size,arr+high*size,size);
    return i+1;
}

static int randomIndex(int low, int high){
    return low+rand()%(high-low+1);
}

static int midIndex(void *base,int low, int high,size_t size,int(*compare)(const void*,const void*)){
    int mid= low+(high-low)/2;
    char *arr=(char*)base;
   if(compare(arr+low*size,arr+mid*size)>0)
    swap(arr+low*size, arr+mid*size,size);
   if(compare(arr+low*size,arr+high*size)>0)
    swap(arr+low*size,arr+high*size,size);
   if(compare(arr+mid*size,arr+high*size)>0)
    swap(arr+mid*size,arr+high*size,size);
   return mid;
}

static void quickSortRecursiveThree(void *base,int low,int high,size_t size,int(*compare)(const void*,const void*)){
    if(low<high){
        int pivotIndex = pivotpos(base,low,high,size,midIndex(base,low,high,size,compare),compare);
        quickSortRecursiveThree(base,low,pivotIndex-1,size,compare);
        quickSortRecursiveThree(base,pivotIndex+1,high,size,compare);
    }
}

static void quickSortRecursiveRandom(void *base,int low,int high,size_t size,int(*compare)(const void*,const void*)){
    if(low<high){
        int pivotIndex = pivotpos(base,low,high,size,randomIndex(low,high),compare);
        quickSortRecursiveRandom(base,low,pivotIndex-1,size,compare);
        quickSortRecursiveRandom(base,pivotIndex+1,high,size,compare);
    }
}

// Provide wrappers matching sorts.h
void quick_sort_generic(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num == 0) return;
    // use recursive three median pivot by default
    quickSortRecursiveThree(base, 0, (int)num - 1, size, (int(*)(const void*,const void*))compare);
}

void quick_sort_median_generic(void* base, size_t num, size_t size, CompareFunc compare) {
    // same as quick_sort_generic for this file
    quick_sort_generic(base, num, size, compare);
}

// placeholder third algorithm -> use qsort as fallback
void your_third_sort_generic(void* base, size_t num, size_t size, CompareFunc compare) {
    if (num == 0) return;
    qsort(base, num, size, (int(*)(const void*,const void*))compare);
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
    fprintf(stderr, "Usage: %s <input_file> <mode> <type>\n", prog);
    fprintf(stderr, "mode: rec_rand | rec_three\n");
    fprintf(stderr, "type: int | float\n");
}

#ifdef STANDALONE_QUICKSORT2
int main(int argc, char **argv){
    if(argc<4){ print_usage(argv[0]); return 1; }
    const char *path = argv[1];
    const char *mode = argv[2];
    const char *type = argv[3];
    size_t n=0;
    clock_t start, end;
    int correct=0;
    double time_ms=0.0;
    srand((unsigned)time(NULL));
    if(strcmp(type,"int")==0){
        int *arr = read_ints_from_file(path,&n);
        if(!arr){ fprintf(stderr, "Failed to open or parse %s\n", path); return 2; }
        start = clock();
        if(strcmp(mode,"rec_rand")==0){
            quickSortRecursiveRandom(arr,0,(int)n-1,sizeof(int),compare);
        } else if(strcmp(mode,"rec_three")==0){
            quickSortRecursiveThree(arr,0,(int)n-1,sizeof(int),compare);
        } else {
            print_usage(argv[0]); free(arr); return 3;
        }
        end = clock();
        time_ms = (double)(end-start) * 1000.0 / CLOCKS_PER_SEC;
        correct = is_sorted_int(arr,n);
        free(arr);
    } else if(strcmp(type,"float")==0){
        double *arr = read_doubles_from_file(path,&n);
        if(!arr){ fprintf(stderr, "Failed to open or parse %s\n", path); return 2; }
        start = clock();
        if(strcmp(mode,"rec_rand")==0){
            quickSortRecursiveRandom(arr,0,(int)n-1,sizeof(double),compareDouble);
        } else if(strcmp(mode,"rec_three")==0){
            quickSortRecursiveThree(arr,0,(int)n-1,sizeof(double),compareDouble);
        } else {
            print_usage(argv[0]); free(arr); return 3;
        }
        end = clock();
        time_ms = (double)(end-start) * 1000.0 / CLOCKS_PER_SEC;
        correct = is_sorted_double(arr,n);
        free(arr);
    } else {
        print_usage(argv[0]); return 4;
    }
    printf("TIME_MS:%.3f\n", time_ms);
    printf("CORRECT:%d\n", correct);
    return 0;
}
