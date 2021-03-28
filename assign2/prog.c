#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define BUFSIZE 30 

typedef struct PCB
{ 
    int ProcId;
    int ProcPR;
    int CPUburst;
    int Reg[8];
    int queueEnterClock;
    int waitingTime;
    struct PCB *pNext;  
    struct PCB *pPrev; 
}PCB;

int CPUreg[8] = {0}; 
PCB *pHead = NULL; 
PCB *pTail = NULL;
PCB *pPrev = NULL; 
int CLOCK = 0; 
int Total_waiting_time = 0; 
int Total_turnaround_time = 0; 
int Total_job = 0;

void createPCB(PCB **temp, int ProcId, int ProcPR, int CPUBurst) 
 { 
     int i;
     *temp = malloc(sizeof(PCB));
     (*temp)->pNext = NULL;
     (*temp)->pPrev = NULL;
     (*temp)->ProcId = ProcId;
     (*temp)->ProcPR = ProcPR;
     (*temp)->CPUburst = CPUBurst;
     (*temp)->queueEnterClock = 0;
     (*temp)->waitingTime = 0;

     for(i = 0; i < 8; i++) 
     { 
         (*temp)->Reg[i] = (*temp)->ProcId;
     }
     return;

 } 

 void insertPCB(PCB *temp, PCB **pHead) 
 { 
     PCB *p;
     p = *pHead;
     if(*pHead == NULL) 
     { 
         *pHead = temp; 
     }
     else
     { 
         while(p->pNext != NULL) 
         { 
             p = p->pNext;
         }
         temp->pPrev = p;
         p->pNext = temp;
     } 
     return; 
}

void insertRR(PCB **head, PCB *new)
{ 
	if (*head == NULL) 
    {
		*head = new;
		pTail = new;
	}
    else
    {
		//pTail->pNext = new;
		pTail = new;
        pTail->pNext = NULL;
    }
	return;
}

int FIFO_Scheduling(PCB **pHead) 
{ 
    int i;
    while(*pHead != NULL) 
    { 
        PCB *pTemp;
        pTemp = *pHead;
        for(i = 0; i < 8; i++) 
        { 
            CPUreg[i] = pTemp->Reg[i];
        } 
        for(i = 0; i < 8; i++) 
        { 
            CPUreg[i] += 1;
        }
        for(i = 0; i < 8; i++) 
        { 
            pTemp->Reg[i] = CPUreg[i] - 1;
        }
        pTemp->waitingTime = pTemp->waitingTime + CLOCK - pTemp->queueEnterClock;
        Total_waiting_time = Total_waiting_time + pTemp->waitingTime;
        CLOCK = CLOCK + pTemp->CPUburst;
        Total_turnaround_time = Total_turnaround_time + CLOCK; 
        Total_job = Total_job + 1;
        printf("Process %d is completed at %d ms\n", pTemp->ProcId, CLOCK);
        *pHead = (*pHead)->pNext;
        free(pTemp); 
    }
    printf("Average Waiting time = %.2f ms     (%d/%d)\n", (double)Total_waiting_time/(double)Total_job, Total_waiting_time, Total_job);
    printf("Average Turnaround time = %.2f ms (%d/%d)\n", (double)Total_turnaround_time/(double)Total_job, Total_turnaround_time, Total_job);
    printf("Throughput = %.2f jobs per ms      (%d/%d)\n", (double)Total_job/(double)CLOCK, Total_job, CLOCK); 
    return 1; 

} 

void swap(PCB *Swap1, PCB *Swap2) 
{  
    PCB *tmp;
    createPCB(&tmp, Swap1->ProcId, Swap1->ProcPR, Swap1->CPUburst);

    Swap1->ProcId = Swap2->ProcId;
    Swap1->ProcPR = Swap2->ProcPR;
    Swap1->CPUburst = Swap2->CPUburst;
    Swap2->ProcId = tmp->ProcId;
    Swap2->ProcPR = tmp->ProcPR;
    Swap2->CPUburst = tmp->CPUburst;
    free(tmp);

    return;
}

void SJF_Scheduling(PCB **pHead) 
{ 
    int swapped;
    PCB *p;
    int i;

    if(*pHead == NULL) 
    { 
        return;
    }
    do 
    { 
        swapped = 0;
        p = *pHead;

        while(p->pNext != NULL) 
        { 
            if(p->CPUburst > p->pNext->CPUburst) 
            { 
                swap(p, p->pNext);
                swapped = 1;
            }
            p = p->pNext;
        }
        //p = *pHead;
    }while(swapped != 0);
    p = *pHead; 

    for(i = 0; i < 8; i++) 
    { 
            CPUreg[i] = p->Reg[i];
    } 
    for(i = 0; i < 8; i++) 
    { 
        CPUreg[i] += 1;
    }
    for(i = 0; i < 8; i++) 
    { 
        p->Reg[i] = CPUreg[i] - 1;
    }
    FIFO_Scheduling(&p);
    free(p);

    return;
}

void PR_Scheduling(PCB **pHead) 
{ 
    int swapped;
    PCB *p;
    int i;

    if(*pHead == NULL) 
    { 
        return;
    }
    do 
    { 
        swapped = 0;
        p = *pHead;

        while(p->pNext != NULL) 
        { 
            if(p->ProcPR < p->pNext->ProcPR) 
            { 
                swap(p, p->pNext);
                swapped = 1;
            }
            p = p->pNext;
        }
        //p = *pHead;
    }while(swapped != 0);
    p = *pHead;

    for(i = 0; i < 8; i++) 
    { 
        CPUreg[i] = p->Reg[i];
    } 
    for(i = 0; i < 8; i++) 
    { 
        CPUreg[i] += 1;
    } 
    for(i = 0; i < 8; i++) 
    { 
        p->Reg[i] = CPUreg[i] - 1;
    }
    FIFO_Scheduling(&p);
    free(p);

    return;
} 

void RR_Scheduling(PCB **pHead, int quantum) 
{ 
    int i; 
    int isGreater = 0;
    while(*pHead != NULL) 
    { 
        isGreater = 0;
        PCB *pTemp;
        pTemp = *pHead;
        for(i = 0; i < 8; i++) 
        { 
            CPUreg[i] = pTemp->Reg[i];
        } 
        for(i = 0; i < 8; i++) 
        { 
            CPUreg[i] += 1;
        }
        for(i = 0; i < 8; i++) 
        { 
            pTemp->Reg[i] = CPUreg[i] - 1;
        } 
        if(pTemp->CPUburst <= quantum) 
        { 
            isGreater = 0;
            pTemp->waitingTime = pTemp->waitingTime + CLOCK - pTemp->queueEnterClock; 
            Total_waiting_time = Total_waiting_time + pTemp->waitingTime; 
            CLOCK = CLOCK + pTemp->CPUburst; 
            Total_turnaround_time = Total_turnaround_time + CLOCK; 
            Total_job = Total_job + 1; 
            printf("Process %d is completed at %d ms\n", pTemp->ProcId, CLOCK); 
            *pHead = (*pHead)->pNext; 
            free(pTemp); 
        } 
        else 
        { 
            pTemp->waitingTime = pTemp->waitingTime + CLOCK - pTemp->queueEnterClock; 
            CLOCK = CLOCK + quantum; 
            pTemp->CPUburst = pTemp->CPUburst - quantum; 
            pTemp->queueEnterClock = CLOCK;
            *pHead = (*pHead)->pNext;
            isGreater = 1;
        }
        if(isGreater) 
        { 
            PCB *copy;
            PCB **tmp;
            copy = *pHead;
            *tmp = (*pHead)->pNext;
            copy->pNext = NULL;
            insertRR(pHead, copy);
            pTail->pNext = NULL;
            *pHead = *tmp;
            //*pHead = (*pHead)->pNext;


        }
    }
        printf("Average Waiting time = %.2f ms     (%d/%d)\n", (double)Total_waiting_time/(double)Total_job, Total_waiting_time, Total_job); 
        printf("Average Turnaround time = %.2f ms (%d/%d)\n", (double)Total_turnaround_time/(double)Total_job, Total_turnaround_time, Total_job); 
        printf("Throughput = %.2f jobs per ms      (%d/%d)\n", (double)Total_job/(double)CLOCK, Total_job, CLOCK); 
        return; 
}

int main(int argc, char *argv[]) 
{
    FILE *input;
    int pID;
    int pPriority;
    int pCPUTime;
    PCB *temp;
    PCB *pHead = NULL;
    PCB *p; 

    if(argc == 5 && strcmp(argv[1], "-alg") == 0) 
    { 
        if(strcmp(argv[2], "FIFO") == 0) 
        { 
            if(fopen(argv[4], "r") != NULL) 
            { 
                input = fopen(argv[4], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[4]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[2]); 
                
                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3) 
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime); 
                    insertPCB(temp, &pHead); 
                } 
                FIFO_Scheduling(&pHead); 
                fclose(input);
            } 
            else
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[4]); 
                exit(-1);
            } 
        }

        else if(strcmp(argv[2], "SJF") == 0) 
        { 
            if(fopen(argv[4], "r") != NULL) 
            { 
                input = fopen(argv[4], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[4]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[2]); 
                
                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3) 
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime); 
                    insertPCB(temp, &pHead); 
                } 
                SJF_Scheduling(&pHead); 
                fclose(input);
            } 
            else 
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[4]); 
                exit(-1); 
            } 
        }
        else if(strcmp(argv[2], "PR") == 0) 
        { 
            if(fopen(argv[4], "r") != NULL) 
            { 
                input = fopen(argv[4], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[4]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[2]); 
                
                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3) 
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime); 
                    insertPCB(temp, &pHead); 
                } 
                PR_Scheduling(&pHead); 
                fclose(input);
            }
            else
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[4]);
                exit(-1);
            } 
        }  
        
    } 
    else if(argc == 5 && strcmp(argv[1], "-input") == 0) 
    { 
        if(strcmp(argv[4], "FIFO") == 0) 
        { 
            if(fopen(argv[2], "r") != NULL) 
            { 
                input = fopen(argv[2], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[2]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[4]); 
                
                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3) 
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime); 
                    insertPCB(temp, &pHead); 
                } 
                FIFO_Scheduling(&pHead);
                fclose(input);
            } 
            else 
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[2]); 
                exit(-1);
            } 
        } 
        else if(strcmp(argv[4], "SJF") == 0)
        { 
            if(fopen(argv[2], "r") != NULL) 
            { 
                input = fopen(argv[2], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[2]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[4]); 
                
                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3) 
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime); 
                    insertPCB(temp, &pHead); 
                } 
                SJF_Scheduling(&pHead); 
                fclose(input);
            }
            else 
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[4]); 
                exit(-1);
            }
            
        } 
        else if(strcmp(argv[4], "PR") == 0)
        { 
            if(fopen(argv[2], "r") != NULL) 
            { 
                input = fopen(argv[2], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[2]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[4]); 
                
                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3) 
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime); 
                    insertPCB(temp, &pHead); 
                } 
                PR_Scheduling(&pHead); 
                fclose(input);
            }
            else 
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[4]); 
                exit(-1);
            }
            
        }
    } 
    else if(argc == 7 && strcmp(argv[1], "-alg") == 0)
    { 
        if(strcmp(argv[2], "RR") == 0)
        { 
            if(fopen(argv[6], "r") != NULL)
            { 
                input = fopen(argv[6], "r"); 
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[6]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[2]);
                int quantum = atoi(argv[4]);

                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3)
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime);
                    //printf("hello from parseInput %d\n", temp->CPUburst);
                    insertPCB(temp, &pHead);
                }
                RR_Scheduling(&pHead, quantum); 
                fclose(input);
            }
            else
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[6]); 
                exit(-1);
            }
            
        }
        
    } 
    else if(argc == 7 && strcmp(argv[1], "-input") == 0)
    { 
        if(strcmp(argv[4], "RR") == 0) 
        { 
            if(fopen(argv[2], "r") != NULL) 
            { 
                input = fopen(argv[2], "r");
                printf("Student Name: Robert Quintanilla\n"); 
                printf("Input File Name : %s\n", argv[2]); 
                printf("CPU Scheduling Alg : %s\n\n",argv[4]);
                int quantum = atoi(argv[6]);

                while(fscanf(input, "%d %d %d\n", &pID, &pPriority, &pCPUTime) == 3)
                { 
                    createPCB(&temp, pID, pPriority, pCPUTime);
                    insertPCB(temp, &pHead);
                }
                RR_Scheduling(&pHead, quantum); 
                fclose(input);
            }
            else
            { 
                fprintf(stderr, "ERROR: Could not open/find %s\n", argv[6]); 
                exit(-1);  
            } 
        } 
    } 
    return 0; 
}