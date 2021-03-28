#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

struct page_table
{
    int f_Num;
    int Vi;
};

int findEmptyFrame(int check[]);
int findLRU(int arr[]);

int main(int argc, char *argv[])
{ 
    unsigned long LA, PA;
    struct page_table PT[32];
    unsigned int p = 5, f = 3, d = 7;
    unsigned int pNum, fNum, dNum;
    int CLK = 0;
    int freeframes[8] = {0, 1, 1, 1, 1, 1, 1, 1};
    int LRUcount[8] = {0};
    int revMap[8] = {-1};
    int j;
    int cnt = 0; 
    char *inputFile;
    char *outputFile;

    for(j = 0; j < 33; j++) 
    { 
        PT[j].Vi = 0;
    }

    if(argv[1] != NULL) 
    { 
        inputFile = argv[1];
    }
    else
    {
        fprintf(stderr, "ERROR: Input file was not specified or could not be found\n");
        exit(-1);
    }

    if(argv[2] != NULL) 
    { 
        outputFile = argv[2];
    }
    else
    { 
        fprintf(stderr, "ERROR: Output file was not specified or could not be found\n");
        exit(-1);
    }

    FILE *inFile, *writeFile;
    inFile = fopen(inputFile, "r");
    writeFile = fopen(outputFile, "w");

    if(inFile == NULL) 
    { 
        fprintf(stderr, "ERROR: Could not open input file\n");
        exit(-1);
    }
    while(fread(&LA, sizeof(unsigned long), 1, inFile) == 1) 
    { 
        CLK++;
        dNum = (LA & 0x07F);
        pNum = (LA >> d);

        if(PT[pNum].Vi == 1) 
        { 
            fNum = PT[pNum].f_Num;
            PA = (fNum << d) + dNum;
            fwrite(&PA, sizeof(unsigned long), 1, writeFile);
            LRUcount[fNum] = CLK;
        }
        else
        {
            int x = findEmptyFrame(freeframes);
            cnt++;

            if(x > 0) 
            { 
                PT[pNum].f_Num = x;
                PT[pNum].Vi = 1;

                fNum = PT[pNum].f_Num;
                PA = (fNum << d) + dNum;
                fwrite(&PA, sizeof(unsigned long), 1, writeFile);
                revMap[x] = pNum;
                LRUcount[fNum] = CLK;
                freeframes[x] = 0;
            }
            else
            { 
                int frameIndex = findLRU(LRUcount);
                PT[revMap[frameIndex]].Vi = 0;
                PT[pNum].f_Num = frameIndex;
                PT[pNum].Vi = 1;
                fNum = PT[pNum].f_Num;
                PA = (fNum << d) + dNum;
                fwrite(&PA, sizeof(unsigned long), 1, writeFile);
                LRUcount[fNum] = CLK;
                revMap[fNum] = pNum;
                
            }
        }
    }
    printf("Part 2 page faults: %d\n", cnt);
    fclose(inFile);
    fclose(writeFile);
    return 0;
}

int findEmptyFrame(int check[]) 
{ 
    int i;

    for(i = 1; i < 8; i++) 
    { 
        if(check[i] > 0) 
        { 
            return i;
        }
    }
    return -1;
}

int findLRU(int arr[])
{ 
    int i;
    int min = 4500;
    int index = 0;

    for(i = 1; i < 8; i++) 
    { 
        if(min > arr[i]) 
        { 
            index = i;
            min = arr[i];
        }
    }
    return index;
}