#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h> 
#include <time.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <semaphore.h> 
#define BUFFER_SIZE 512

struct PCB
{ 
    int PID; 
    int PR; 
    int numCPUBurst, numIOBurst; 
    int *CPUBurst, *IOBurst;
    int CPUindex, IOindex; 
    struct timespec ts_begin, ts_end;
    struct PCB *pPrev, *pNext; 
    double end; 
    double WaitTime; 
    time_t start;
    time_t timeEnterReadyQ;
    struct timespec ts_beginUtil; 
    struct timespec ts_endUtil;
    struct timespec beginTime; 
    struct timespec endTime;    
}; 

struct PCB *Ready_Q;
struct PCB *IO_Q = NULL;

int file_read_done = 0;
int cpu_sch_done = 0;
int io_sys_done = 0;
int cpu_busy = 0;
int io_busy = 0;
sem_t sem_CPU;
sem_t sem_IO;

struct PCB *pHead = NULL;
struct PCB *pHead_IO;
struct timespec ts;
struct timespec ts_begin, ts_end;

char *inputFile;
double totalCPU;
double totalThroughput;
double CPU_Util;
double totalWait;
double waitTime;
int process = 0;
double turnAroundTime;
int ALG;
int quantumTime = 0;
int totalProcesses = 0;
double throughput;
int TotalSleepTime = 0;
double totalTime;
pthread_mutex_t lock; 

void Insert_Into_Ready_Queue_W_Clock(struct PCB **pHead, int *CPUBurst, int *IOBurst, int numCPUBurst, int numIOBurst, int PR, int CPUindex, int IOindex)
{
    Ready_Q = (struct PCB *)malloc(sizeof(struct PCB));
    Ready_Q->CPUBurst = (int *)malloc(numCPUBurst * sizeof(int));
    Ready_Q->IOBurst = (int *)malloc(numIOBurst * sizeof(int));
    clock_gettime(CLOCK_MONOTONIC, &Ready_Q->ts_begin);
    Ready_Q->start = clock_gettime(CLOCK_MONOTONIC, &Ready_Q->beginTime); 
    int i;

    for(i = 0; i < numCPUBurst; i++)
    {
        Ready_Q->CPUBurst[i] = CPUBurst[i];
    }
    for(i = 0; i < numIOBurst; i++)
    {
        Ready_Q->IOBurst[i] = IOBurst[i];
    }
    struct PCB *pLast = *pHead;
    Ready_Q->numCPUBurst = numCPUBurst;
    Ready_Q->numIOBurst = numIOBurst;
    Ready_Q->PR = PR;
    Ready_Q->CPUindex = CPUindex;
    Ready_Q->IOindex = IOindex;
    Ready_Q->pNext = NULL;

    if(*pHead == NULL)
    {
        Ready_Q->pPrev = NULL;
        *pHead = Ready_Q;
        return;
    }
    while(pLast->pNext != NULL) 
    { 
        pLast = pLast->pNext; 
    }
    pLast->pNext = Ready_Q;
    Ready_Q->pPrev = pLast;
    return;
} 

void Insert_Into_Ready_Queue(struct PCB **pHead, int *CPUBurst, int *IOBurst, int numCPUBurst, int numIOBurst, int PR, int CPUindex, int IOindex)
{
    Ready_Q = (struct PCB *)malloc(sizeof(struct PCB));
    Ready_Q->CPUBurst = (int *)malloc(numCPUBurst * sizeof(int));
    Ready_Q->IOBurst = (int *)malloc(numIOBurst * sizeof(int)); 
    int i;

    for(i = 0; i < numCPUBurst; i++)
    {
        Ready_Q->CPUBurst[i] = CPUBurst[i];
    }

    for(i = 0; i < numIOBurst; i++)
    {
        Ready_Q->IOBurst[i] = IOBurst[i];
    }
    struct PCB *pLast = *pHead;
    Ready_Q->numCPUBurst = numCPUBurst;
    Ready_Q->numIOBurst = numIOBurst;
    Ready_Q->PR = PR;
    Ready_Q->CPUindex = CPUindex;
    Ready_Q->IOindex = IOindex;
    Ready_Q->pNext = NULL;
    if (*pHead == NULL)
    {
        Ready_Q->pPrev = NULL;
        *pHead = Ready_Q;
        return;
    }
    while(pLast->pNext != NULL) 
    { 
        pLast = pLast->pNext; 
    }
    pLast->pNext = Ready_Q;
    Ready_Q->pPrev = pLast;
    return;
} 

void Insert_Into_IO_Queue(struct PCB **pHead, int *CPUBurst, int *IOBurst, int numCPUBurst, int numIOBurst, int PR, int CPUindex, int IOindex)
{
    IO_Q = (struct PCB *)malloc(sizeof(struct PCB));
    IO_Q->CPUBurst = (int *)malloc(numCPUBurst * sizeof(int));
    IO_Q->IOBurst = (int *)malloc(numIOBurst * sizeof(int));
    struct PCB *pLast = *pHead;
    int i; 

    for(i = 0; i < numCPUBurst; i++)
    {
        IO_Q->CPUBurst[i] = CPUBurst[i];
    }
    for(i = 0; i < numIOBurst; i++)
    {
        IO_Q->IOBurst[i] = IOBurst[i];
    }
    IO_Q->numCPUBurst = numCPUBurst;
    IO_Q->numIOBurst = numIOBurst;
    IO_Q->PR = PR;
    IO_Q->CPUindex = CPUindex;
    IO_Q->IOindex = IOindex;
    IO_Q->pNext = NULL;

    if(*pHead == NULL)
    {
        IO_Q->pPrev = NULL;
        *pHead = IO_Q;
        return;
    }
    while(pLast->pNext != NULL) 
    { 
        pLast = pLast->pNext; 
    }
    pLast->pNext = IO_Q;
    IO_Q->pPrev = pLast;
    return;
} 

void Terminate_PCB(struct PCB **pHead)
{
    struct PCB *current = *pHead;
    if(*pHead != NULL)
    {
        *pHead = current->pNext;

        free(current);
    }
}

struct PCB *remove_IO_Head(struct PCB **pHead)
{
    struct PCB *current = *pHead;
    if(*pHead != NULL)
    {
        *pHead = current->pNext;
        return current;
    }
}

struct PCB *Remove_From_Ready_Queue(struct PCB **pHead)
{
    struct PCB *pCurr; 
    pCurr = *pHead;
    if(*pHead != NULL)
    {
        *pHead = pCurr->pNext;
        return pCurr;
    }
} 

struct PCB *FindHighestPR(struct PCB **pHead)
{
    struct PCB *maxPR; 
    struct PCB *pTemp;
    pTemp = maxPR = *pHead;
    while(pTemp != NULL)
    {
        if(pTemp->PR > maxPR->PR) 
        { 
            maxPR = pTemp; 
        }
        pTemp = pTemp->pNext;
    }
    return maxPR;
}

struct PCB *FindMinCPUTime(struct PCB **pHead)
{
    struct PCB *minCPUTime; 
    struct PCB *pTemp;
    pTemp = minCPUTime = *pHead;
    int i;

    while(pTemp != NULL)
    {
        for(i = 0; i < pTemp->numCPUBurst; i++)
        {
            if(pTemp->CPUBurst < minCPUTime->CPUBurst) 
            { 
                minCPUTime = pTemp; 
            }
        }
        pTemp = pTemp->pNext;
    }
    return minCPUTime;
}

void PrintPerformanceMetrics()
{ 
    printf("--------------------------------------------------------\n");
    printf("Input File Name:                   %s\n", inputFile);
    if(ALG == 1)
    {
        printf("CPU Scheduling Alg:                FIFO\n");
    }
    if(ALG == 2)
    {
        printf("CPU Scheduling Alg:                SJF\n");
    }
    if(ALG == 3)
    {
        printf("CPU Scheduling Alg:                PR\n");
    }
    if(ALG == 4)
    {
        printf("CPU Scheduling Alg:                RR quantum: %d\n", quantumTime);
    }
    printf("CPU Utilization:                   %.3lf%%\n", totalCPU);
    printf("Throughput:                        %.3lf processes / ms\n", totalThroughput);
    printf("Avg. Turnaround Time:              %.3lfms\n", (turnAroundTime)*1000);
    printf("Avg. Waiting time in Ready Queue:  %.3lfms\n", (totalWait / totalProcesses) * 1000); 
    printf("--------------------------------------------------------\n");
} 

void *File_Read_Thread(void *args) 
{ 
    char *inputFile = (char*)args; 
    FILE *pFile; 
    pFile = fopen(inputFile, "r"); 

    if(pFile == NULL) 
    { 
        fprintf(stderr, "ERROR: Could not open given input file %s\n", inputFile); 
        exit(-1);
    } 
    char inputBuffer[BUFFER_SIZE]; 

    while(fgets(inputBuffer, BUFFER_SIZE, pFile) != NULL) 
    { 
        char *temp[BUFFER_SIZE]; 
        int PR;
        int CPUBurstCount; 
        int IOBurstCount;  
        int sleepTime; 
        int count = 0; 
        int i = 0; 
        char *token = strtok(inputBuffer, " "); 

        while(token != NULL) 
        { 
            temp[i++] = token; 
            count++; 
            token = strtok(NULL, " ");
        } 
        if(strncmp("proc", temp[0], 4) == 0) 
        { 
            process++; 
            pthread_mutex_lock(&lock); 
            CPUBurstCount = (atoi(temp[2]) / 2) + 1; 
            IOBurstCount = (atoi(temp[2]) / 2); 
            int *IOBurst = (int*)malloc(IOBurstCount * sizeof(int)); 
            int *CPUBurst = (int*)malloc(CPUBurstCount * sizeof(int)); 
            int i; 
            int j; 

            for(i = 3, j = 0; i < count; i += 2, j++) 
            { 
                CPUBurst[j] = atoi(temp[i]); 
            } 
            for(i = 4, j = 0; i < count; i += 2, j++) 
            { 
                IOBurst[j] = atoi(temp[i]); 
            } 
            PR = atoi(temp[1]);
            Insert_Into_Ready_Queue_W_Clock(&pHead, CPUBurst, IOBurst, CPUBurstCount, IOBurstCount, PR, 0, 0); 
            sem_post(&sem_CPU); 
            pthread_mutex_unlock(&lock); 
        } 
        else if(strncmp("sleep", temp[0], 5) == 0)
        {
            sleepTime = atoi(temp[1]);
            usleep(sleepTime * 1000);
            totalTime += sleepTime;
            TotalSleepTime++;
        }
        else if(strncmp("stop", temp[0], 4) == 0)
        {
            break;
        }
    }
    file_read_done = 1;
    return NULL; 
} 

void *CPU_Thread(void *args) 
{
    while(1) 
    { 
        if(Ready_Q == NULL && !cpu_busy && IO_Q == NULL && !io_busy && file_read_done == 1) 
        { 
            break;
        } 
        if(ALG == 1) // If given alg is FIFO 
        { 
            int res = sem_timedwait(&sem_CPU, &ts);
            if(res == -1 && errno == ETIMEDOUT) 
            { 
                continue;
            }
            pthread_mutex_lock(&lock);
            cpu_busy = 1; 
            struct PCB *temp = (struct PCB *)malloc(sizeof(struct PCB)); 
            temp = pHead; 
            Remove_From_Ready_Queue(&pHead); 
            clock_gettime(CLOCK_MONOTONIC, &temp->ts_beginUtil); 
            usleep((temp->CPUBurst[temp->CPUindex]) * 1000);
            totalTime += (temp->CPUBurst[temp->CPUindex]); 
            clock_gettime(CLOCK_MONOTONIC, &temp->ts_end); 
            waitTime = temp->ts_end.tv_sec - temp->ts_begin.tv_sec; 
            waitTime += (temp->ts_end.tv_nsec - temp->ts_begin.tv_nsec); 
            temp->CPUindex++; 

            if(temp->CPUindex >= temp->numCPUBurst) 
            { 
                clock_gettime(CLOCK_MONOTONIC, &temp->ts_endUtil); 
                totalCPU = (temp->ts_endUtil.tv_sec - temp->ts_beginUtil.tv_sec);
                totalCPU += (temp->ts_endUtil.tv_nsec - temp->ts_beginUtil.tv_nsec) / 1000000000.0;
                totalWait += (waitTime) / 10000000000.0;
                waitTime = 0;
                totalProcesses++;
                turnAroundTime += ((temp->ts_end.tv_nsec - temp->ts_begin.tv_nsec) / 10000000000.0);
                Terminate_PCB(&temp);
                cpu_busy = 0; 

                if(totalProcesses == process)
                {
                    clock_gettime(CLOCK_MONOTONIC, &ts_end);
                    throughput = ts_end.tv_sec - ts_begin.tv_sec;
                    throughput += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;
                    totalThroughput =  ((totalProcesses+TotalSleepTime) / throughput)/1000;
                    totalCPU = ((throughput / totalTime) * 1000) * 100;
                    PrintPerformanceMetrics();
                    exit(1);
                }
            }
            else
            {
                Insert_Into_IO_Queue(&pHead_IO, temp->CPUBurst, temp->IOBurst, temp->numCPUBurst, temp->numIOBurst, temp->PR, temp->CPUindex, temp->IOindex);
                cpu_busy = 0;
                sem_post(&sem_IO);
            }
            pthread_mutex_unlock(&lock);
        } 
        else if(ALG == 2) // If given alg is SJF
        { 
            int res = sem_timedwait(&sem_CPU, &ts);
            if(res == -1 && errno == ETIMEDOUT)
            {
                continue;
            }
            pthread_mutex_lock(&lock);
            cpu_busy = 1;
            struct PCB *temp;
            temp = FindMinCPUTime(&pHead);
            Remove_From_Ready_Queue(&pHead);
            usleep((temp->CPUBurst[temp->CPUindex]) * 1000);
            totalTime += temp->CPUBurst[temp->CPUindex];
            clock_gettime(CLOCK_MONOTONIC, &temp->ts_end);
            waitTime = temp->ts_end.tv_sec - temp->ts_begin.tv_sec;
            waitTime += (temp->ts_end.tv_nsec - temp->ts_begin.tv_nsec);
            temp->CPUindex++;
            if(temp->CPUindex >= temp->numCPUBurst)
            {
                totalWait += (waitTime) / 10000000000.0;
                waitTime = 0;
                totalProcesses++;
                temp->end = clock_gettime(CLOCK_MONOTONIC, &temp->endTime);
                turnAroundTime += (temp->endTime.tv_nsec - temp->beginTime.tv_nsec) / 10000000000.0;
                Terminate_PCB(&temp);
                cpu_busy = 0;
                if(totalProcesses == process)
                {
                    clock_gettime(CLOCK_MONOTONIC, &ts_end);
                    throughput = ts_end.tv_sec - ts_begin.tv_sec;
                    throughput += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;
                    totalThroughput =  ((totalProcesses+TotalSleepTime) / throughput)/1000;
                    totalCPU = ((throughput / totalTime) * 1000) * 100;
                    PrintPerformanceMetrics();
                    exit(1);
                }
            }
            else
            {
                Insert_Into_IO_Queue(&pHead_IO, temp->CPUBurst, temp->IOBurst, temp->numCPUBurst, temp->numIOBurst, temp->PR, temp->CPUindex, temp->IOindex);
                cpu_busy = 0;
                sem_post(&sem_IO);
            }
            pthread_mutex_unlock(&lock);
        } 
        else if(ALG == 3) // If given alg is PR
        { 
            int res = sem_timedwait(&sem_CPU, &ts);
            if(res == -1 && errno == ETIMEDOUT)
            {
                continue;
            }
            pthread_mutex_lock(&lock);
            cpu_busy = 1;
            struct PCB *temp;
            temp = FindHighestPR(&pHead);
            Remove_From_Ready_Queue(&pHead);
            usleep((temp->CPUBurst[temp->CPUindex]) * 1000);
            totalTime+= temp->CPUBurst[temp->CPUindex];
            clock_gettime(CLOCK_MONOTONIC, &temp->ts_end);
            waitTime = temp->ts_end.tv_sec - temp->ts_begin.tv_sec;
            waitTime += (temp->ts_end.tv_nsec - temp->ts_begin.tv_nsec);
            temp->CPUindex++;
            if(temp->CPUindex >= temp->numCPUBurst)
            {
                totalWait += (waitTime) / 10000000000.0;
                waitTime = 0;
                totalProcesses++;
                clock_gettime(CLOCK_MONOTONIC, &temp->endTime);
                turnAroundTime += (temp->endTime.tv_nsec - temp->beginTime.tv_nsec) / 10000000000.0;
                Terminate_PCB(&temp);
                cpu_busy = 0;
                if(totalProcesses == process)
                {
                    clock_gettime(CLOCK_MONOTONIC, &ts_end);
                    throughput = ts_end.tv_sec - ts_begin.tv_sec;
                    throughput += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;
                    totalThroughput =  ((totalProcesses + TotalSleepTime) / throughput)/1000;
                    totalCPU = ((throughput / totalTime) * 1000) * 100;
                    PrintPerformanceMetrics();
                    exit(1);
                }
            }
            else
            {
                Insert_Into_IO_Queue(&pHead_IO, temp->CPUBurst, temp->IOBurst, temp->numCPUBurst, temp->numIOBurst, temp->PR, temp->CPUindex, temp->IOindex);
                cpu_busy = 0;
                sem_post(&sem_IO);
            }
            pthread_mutex_unlock(&lock);
        } 
        else if(ALG == 4) // If given alg is RR
        { 
            int res = sem_timedwait(&sem_CPU, &ts);
            if(res == -1 && errno == ETIMEDOUT)
            {
                continue;
            }
            pthread_mutex_lock(&lock);

            cpu_busy = 1;
            struct PCB *temp;
            struct PCB *DeleteNode = (struct PCB *)malloc(sizeof(struct PCB));
            temp = pHead;
            Remove_From_Ready_Queue(&pHead);
            clock_gettime(CLOCK_MONOTONIC, &temp->ts_end);
            waitTime = temp->ts_end.tv_sec - temp->ts_begin.tv_sec;
            waitTime += (temp->ts_end.tv_nsec - temp->ts_begin.tv_nsec);
            if(quantumTime < temp->CPUBurst[temp->CPUindex])
            {
                usleep(((temp->CPUBurst[temp->CPUindex]) - quantumTime) * 1000);
                temp->CPUBurst[temp->CPUindex] = ((temp->CPUBurst[temp->CPUindex]) - quantumTime);
                totalTime += ((temp->CPUBurst[temp->CPUindex]) - quantumTime);
            }
            else
            {
                temp->CPUBurst[temp->CPUindex] = ((temp->CPUBurst[temp->CPUindex]) - quantumTime);
                temp->CPUindex++;
            }
            if(temp->CPUindex >= temp->numCPUBurst)
            {
                totalWait += (waitTime) / 10000000000.0;
                waitTime = 0;
                totalProcesses++;
                temp->end = clock_gettime(CLOCK_MONOTONIC, &temp->endTime);
                turnAroundTime += (temp->endTime.tv_nsec - temp->beginTime.tv_nsec) / 10000000000.0;
                Terminate_PCB(&temp);
                cpu_busy = 0;
                if(totalProcesses == process)
                {
                    clock_gettime(CLOCK_MONOTONIC, &ts_end);
                    throughput = ts_end.tv_sec - ts_begin.tv_sec;
                    throughput += (ts_end.tv_nsec - ts_begin.tv_nsec) / 1000000000.0;
                    totalThroughput =  ((totalProcesses) / throughput)/1000;
                    totalCPU = ((throughput / totalTime) * 1000) * 100;
                    PrintPerformanceMetrics();
                    exit(1);
                }
            }
            else
            {
                Insert_Into_IO_Queue(&pHead_IO, temp->CPUBurst, temp->IOBurst, temp->numCPUBurst, temp->numIOBurst, temp->PR, temp->CPUindex, temp->IOindex);
                cpu_busy = 0;
                sem_post(&sem_IO);
            }
            pthread_mutex_unlock(&lock);
        }
    }
    cpu_sch_done = 1;
    return NULL;    
}

void *IO_Thread(void *args) 
{  
    while(1)
    {
        if(Ready_Q == NULL && !cpu_busy && IO_Q == NULL && file_read_done == 1)
        {
            break;
        }
        int res = sem_timedwait(&sem_IO, &ts);
        if(res == -1 && errno == ETIMEDOUT)
        {
            continue;
        }
        pthread_mutex_lock(&lock);
        struct PCB *temp = (struct PCB *)malloc(sizeof(struct PCB));
        io_busy = 1;
        temp = remove_IO_Head(&pHead_IO);
        usleep((temp->IOBurst[temp->IOindex]) * 1000);
        totalTime += temp->IOBurst[temp->IOindex];
        temp->IOindex++;
        Insert_Into_Ready_Queue(&pHead, temp->CPUBurst, temp->IOBurst, temp->numCPUBurst, temp->numIOBurst, temp->PR, temp->CPUindex, temp->IOindex);
        io_busy = 0;
        sem_post(&sem_CPU);
        pthread_mutex_unlock(&lock);
    }
    io_sys_done = 1;
} 

int main(int argc, char *argv[]) 
{
    int inputCNT = 0;
    int algoARGS = 0;
    pthread_t tid1; 
    pthread_t tid2;
    pthread_t tid3;
    int i;

    if(argc < 4) 
    { 
        fprintf(stderr, "ERROR, Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n"); 
        exit(-1);
    } 
    sem_init(&sem_CPU, 0, 0); 
    sem_init(&sem_IO, 0, 0);
    pthread_mutex_init(&lock, NULL); 
    ts.tv_sec = 1; 

    for(i = 0; i < argc; i++) 
    { 
        if(strcmp(argv[0], "prog") != 0)
        { 
            fprintf(stderr, "ERROR, Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n"); 
            exit(-1);
        } 
        if(strcmp(argv[1], "-alg") != 0) 
        { 
            fprintf(stderr, "ERROR, Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n"); 
            exit(-1);
        } 

        if(strcmp(argv[3], "-input") != 0)
        { 
            if(strcmp(argv[5], "-input") == 0) 
            { 
                break;
            } 
            break;
            
        } 
        break;
    }

    if(argc == 5 && strcmp(argv[1], "-alg") == 0) 
    { 
        if(strcmp(argv[2], "FIFO") == 0) 
        {
            ALG = 1; 
            inputFile = argv[4]; 
            struct PCB *process = (struct PCB*)malloc(sizeof(struct PCB)); 
            process = pHead; 
            clock_gettime(CLOCK_MONOTONIC, &ts_begin); 
            pthread_create(&tid1, NULL, File_Read_Thread, (void *)inputFile); 
            pthread_create(&tid2, NULL, CPU_Thread, (void *)&process);
            pthread_create(&tid3, NULL, IO_Thread, (void *)&process);
            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
            pthread_join(tid3, NULL);
        } 
        else if(strcmp(argv[2], "SJF") == 0)
        { 
            ALG = 2; 
            inputFile = argv[4]; 
            struct PCB *process = (struct PCB*)malloc(sizeof(struct PCB)); 
            process = pHead; 
            clock_gettime(CLOCK_MONOTONIC, &ts_begin); 
            pthread_create(&tid1, NULL, File_Read_Thread, (void *)inputFile); 
            pthread_create(&tid2, NULL, CPU_Thread, (void *)&process);
            pthread_create(&tid3, NULL, IO_Thread, (void *)&process);
            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
            pthread_join(tid3, NULL);
        } 
        else if(strcmp(argv[2], "PR") == 0)
        { 
            ALG = 3; 
            inputFile = argv[4]; 
            struct PCB *process = (struct PCB*)malloc(sizeof(struct PCB)); 
            process = pHead; 
            clock_gettime(CLOCK_MONOTONIC, &ts_begin); 
            pthread_create(&tid1, NULL, File_Read_Thread, (void *)inputFile); 
            pthread_create(&tid2, NULL, CPU_Thread, (void *)&process);
            pthread_create(&tid3, NULL, IO_Thread, (void *)&process);
            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
            pthread_join(tid3, NULL);
        } 
    } 
    else if(argc == 7 && strcmp(argv[1], "-alg") == 0) 
    { 
        if(strcmp(argv[2], "RR") == 0) 
        { 
            ALG = 4; 
            quantumTime = atoi(argv[4]);
            inputFile = argv[6];
            struct PCB *process = (struct PCB*)malloc(sizeof(struct PCB)); 
            process = pHead; 
            clock_gettime(CLOCK_MONOTONIC, &ts_begin); 
            pthread_create(&tid1, NULL, File_Read_Thread, (void *)inputFile); 
            pthread_create(&tid2, NULL, CPU_Thread, (void *)&process);
            pthread_create(&tid3, NULL, IO_Thread, (void *)&process);
            pthread_join(tid1, NULL);
            pthread_join(tid2, NULL);
            pthread_join(tid3, NULL);
        }
        
    } 
    else
    { 
        fprintf(stderr, "ERROR, Usage: prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [input_file_name.txt]\n"); 
        exit(-1);   
    }
}