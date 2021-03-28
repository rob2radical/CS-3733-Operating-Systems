#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct page_table
{
    int f_Num;
    int Vi;
};

int calculateBits(int M, int base);
int findEmptyFrame(int check[], int PM);
int findLRU(int arr[], int PM);

int main(int argc, char *argv[])
{ 
    unsigned long LA, PA; 
    int PageSize, VMSize, PMSize;
    unsigned int Pnum, Fnum, Dnum;
    char *SequenceFile, *OutputFile;
    FILE *inputFile, *writeFile;
    int j; 
    int CLK = 0;
    int cnt = 0;
    
    if(argv[0] == NULL) 
    { 
        fprintf(stderr, "ERROR: No Executeable Specified\n");
        exit(-1);
    } 
    else if(argv[1] == NULL)
    { 
        fprintf(stderr, "ERROR: No Bytes Per Page Specified\n");
        exit(-1);    
    } 
    else if(argv[2] == NULL)
    { 
        fprintf(stderr, "ERROR: Size of Virtual Memory not Specified\n");
        exit(-1);    
    } 
    else if(argv[3] == NULL)
    { 
        fprintf(stderr, "ERROR: Size of Physical Memory not Specified\n");
        exit(-1);    
    } 
    else if(argv[4] == NULL)
    { 
        fprintf(stderr, "ERROR: Sequence File not Specified\n");
        exit(-1);    
    } 
    else if(argv[5] == NULL)
    { 
        fprintf(stderr, "ERROR: Output File not Specified\n");
        exit(-1);    
    }
    else
    { 
        PageSize = calculateBits(atoi(argv[1]), 2);
        VMSize = calculateBits(atoi(argv[2]), 2);
        PMSize = calculateBits(atoi(argv[3]), 2);
        
        SequenceFile = argv[4];
        OutputFile = argv[5]; 
        unsigned int p, f, d; 
        d = (int)PageSize; 
        p = (int)VMSize - d;
        f = (int)PMSize - d;
        struct page_table PT[VMSize]; 
        int freeframes[PMSize - 1]; 
        int LRUcount[PMSize - 1];
        int revMap[PMSize];
        memset(revMap, 0, PMSize*sizeof(int)); 
        inputFile = fopen(SequenceFile, "r"); 
        writeFile = fopen(OutputFile, "w"); 
        
        for(j = 0; j < VMSize; j++) 
        { 
            PT[j].Vi = 0;

        } 
        while(fread(&LA, sizeof(unsigned long), 1, inputFile) == 1) 
        { 
            CLK++; 
            Dnum = (LA & 0x07F); 
            Pnum = (LA >> d); 
            
            if(PT[Pnum].Vi == 1) 
            { 
                Fnum = PT[Pnum].f_Num; 
                PA = (Fnum << d) + Dnum; 
                fwrite(&PA, sizeof(unsigned long), 1, writeFile); 
                LRUcount[Fnum] = CLK;
            } 
            else
            { 
                int x = findEmptyFrame(freeframes, PMSize); 
                cnt++; 

                if(x > 0) 
                { 
                    PT[Pnum].f_Num = x;
                    PT[Pnum].Vi = 1;

                    Fnum = PT[Pnum].f_Num;
                    PA = (Fnum << d) + Dnum;
                    fwrite(&PA, sizeof(unsigned long), 1, writeFile);
                    revMap[x] = Pnum;
                    LRUcount[Fnum] = CLK;
                    freeframes[x] = 0;
                } 
                else
                {
                    int index = findLRU(LRUcount, PMSize); 
                    PT[revMap[index]].Vi = 0;
                    PT[Pnum].f_Num = index;
                    PT[Pnum].Vi = 1;
                    Fnum = PT[Pnum].f_Num;
                    PA = (Fnum << d) + Dnum;
                    fwrite(&PA, sizeof(unsigned long), 1, writeFile);
                    LRUcount[Fnum] = CLK;
                    revMap[Fnum] = Pnum;
                }
            } 
        } 
        printf("Part 3 page faults: %d\n", cnt); 
        fclose(inputFile);
        fclose(writeFile); 
    } 
    return 0; 
}

int calculateBits(int M, int base) 
{ 
    return log(M) / log(base);
}

int findEmptyFrame(int check[], int PM) 
{ 
    int i;

    for(i = 1; i < PM; i++) 
    { 
        if(check[i] > 0) 
        { 
            return i;
        }
    } 
    return -1;
}

int findLRU(int arr[], int PM) 
{ 
    int i; 
    int min = 4500;
    int index = 0;

    for(i = 1; i < PM; i++) 
    { 
        if(min > arr[i]) 
        { 
            index = i;
            min = arr[i];
        }
    }
    return index;
}
    