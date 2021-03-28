#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

typedef struct merge
{
    double *FirstHalf;
    double *SecondHalf;
    double *myVal;
} MergeArray;

// Global Vars
int ArraySize;
int ArrayHalfSize;

void *mergeThread(void *args) 
{ 
    int i, j;
    MergeArray *myMerge = (struct merge *)args;

    for(i = 0; i < ArrayHalfSize; i++) 
    { 
        myMerge->myVal[i] = myMerge->FirstHalf[i];
    }
    ArraySize = ArrayHalfSize + ArrayHalfSize;

    for(i = 0, j = ArrayHalfSize; j < ArraySize && i < ArrayHalfSize; i++, j++) 
    { 
        myMerge->myVal[j] = myMerge->SecondHalf[i];
    }
    return NULL;
}

void *selectionSortThreadB(void *args) 
{ 
    double *arr; 
    double *ptrArr;
    arr = (double*)args;
    ptrArr = (double*)malloc(ArraySize * sizeof(double));
    int i;
    int j; 
    double temp;

    for(i = 1; i < ArraySize; i++) 
    { 
        temp = arr[i];
        j = i - 1;
        while(j >= 0 && arr[j] > temp) 
        { 
            arr[j + 1] = arr[j]; 
            j = j - 1;
        } 
        arr[j + 1] = temp;
    }
    ptrArr = arr;
    pthread_exit((void*)ptrArr);
}

void *selectionSortSecondHalf(void *args) 
{ 
    double *myarr, *myptrretSecondHalf; 
    myarr = (double *)args; 
    myptrretSecondHalf = (double *)malloc(ArrayHalfSize * sizeof(double)); 
    
    if(myptrretSecondHalf == NULL) 
    { 
        fprintf(stderr, "Could not allocate memory in selectionSortSecondtHalf\n"); 
        exit(-1); 
    } 
    int i, j; 
    int min; 
    double temp; 
    
    for (i = ArrayHalfSize; i < ArraySize; i++) 
    { 
        min = i; 
        for(j = i + 1; j < ArraySize; j++) 
        { 
            if(myarr[j] < myarr[min]) 
            { 
                min = j; 
            } 
        } 
        temp = myarr[i]; 
        myarr[i] = myarr[min]; 
        myarr[min] = temp; 
    }
    myptrretSecondHalf = myarr;
    pthread_exit(myptrretSecondHalf);
}


void *selectionSortFirstHalf(void *args) 
{
    double *myarr, *myptrretFirstHalf; 
    myarr = (double *)args; 
    myptrretFirstHalf = (double *)malloc(ArrayHalfSize * sizeof(double)); 
    
    if(myptrretFirstHalf == NULL) 
    { 
        fprintf(stderr, "Could not allocate memory in selectionSortFirstHalf\n"); 
        exit(-1); 
    } 
    int i, j; 
    int min; 
    double temp; 
    
    for (i = 0; i < ArrayHalfSize; i++) 
    { 
        min = i; 
        for(j = i + 1; j < ArrayHalfSize; j++) 
        { 
            if(myarr[j] < myarr[min]) 
             min = j; 
        } 
        temp = myarr[i]; 
        myarr[i] = myarr[min]; 
        myarr[min] = temp; 
    }
    myptrretFirstHalf = myarr; 
    pthread_exit(myptrretFirstHalf); 
}
int main(int argc, char *argv[]) 
{ 
    if(argc != 2) 
    { 
        fprintf(stderr, "ERROR: Please provide the correct number of arguments (file, size of array)\n");
        exit(-1);
    }
    else
    { 
        ArraySize = atoi(argv[1]);
        ArrayHalfSize = (ArraySize / 2);
        clock_t start, end, start2, end2;
        double RandomNum; 
        double *ThreadBlock; 
        double *ThreadArrayHalf1; 
        double *ThreadArrayHalf2; 
        pthread_t tid, tid1, tid2, tid3; 
        double A[ArraySize]; 
        double B[ArraySize]; 
        //double C[ArraySize]; 
        double A_First_Half[ArrayHalfSize]; 
        double A_Second_Half[ArrayHalfSize];
        int i;

        srand(time(NULL)); // generate seed for rand nums based on time

        for(i = 0; i < ArraySize; i++) 
        { 
            RandomNum = ((double) rand()*(1000.0+1.0)/(double)RAND_MAX+1.0);
            A[i] = RandomNum;
        }
        for(i = 0; i < ArraySize; i++) 
        { 
            B[i] = A[i];
        }
        start = clock();
        pthread_create(&tid, NULL, selectionSortThreadB, (void*)B);
        pthread_join(tid, (void**)&ThreadBlock);
        end = clock() - start;
        printf("Sorting is done in %.2fms when one thread is used\n", end * 1000.0 / CLOCKS_PER_SEC); 
        
//*******Two-Threaded Option***************************//
        for(i = 0; i < ArrayHalfSize; i++) 
        { 
            A_First_Half[i] = A[i];
        }
        for(i = ArrayHalfSize; i < ArraySize; i++) 
        { 
            A_Second_Half[i] = A[i];
        }  
        start2 = clock();
        pthread_create(&tid1, NULL, selectionSortFirstHalf, A_First_Half);
        pthread_create(&tid2, NULL, selectionSortSecondHalf, A_Second_Half);
        pthread_join(tid1, (void**)&ThreadArrayHalf1); 
        pthread_join(tid2, (void**)&ThreadArrayHalf2);
        MergeArray threadMerge; 
        threadMerge.myVal = (double*)malloc(ArraySize * sizeof(double));
        threadMerge.FirstHalf = ThreadArrayHalf1; 
        threadMerge.SecondHalf = ThreadArrayHalf2; 
        pthread_create(&tid3, NULL, mergeThread, (void*)&threadMerge); 
        pthread_join(tid3, NULL);
        end2 = clock() - start2;  
        printf("Sorting is done in %.2fms when two threads are used\n", end2 * 1000.0 / CLOCKS_PER_SEC);
        free(threadMerge.myVal);
    } 
    return 0;
}