#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]) 
{ 
    unsigned long LA;
    unsigned long PA;
    unsigned int PageTable[32] = {2, 4, 1, 7, 3, 5, 6, 0};
    unsigned int p = 5;
    unsigned int f = 3;
    unsigned int d = 7;
    unsigned int pNum;
    unsigned int fNum;
    unsigned int dNum;
    char *file;
    char *outputFile;
    int i;
    int p1Test = 0;

    if(argv[1] != NULL) 
    { 
        file = argv[1];
    }
    else
    {
        fprintf(stderr, "ERROR: Input file was not specified\n");
        exit(-1);
    }

    if(argv[2] != NULL) 
    { 
        outputFile = argv[2];
    }
    else
    {
        fprintf(stderr, "ERROR: Output file was not specified\n");
        exit(-1);
    } 
    for(i = 0; i < argc; i++) 
    { 
        if(strncmp("part1-out-test", argv[i], 14) == 0) 
        { 
            p1Test = 1;
        }
    }

    FILE *Fp;
    FILE *writeTo; 

    if(Fp != NULL && writeTo != NULL) 
    { 
        Fp = fopen(file, "r");
        writeTo = fopen(outputFile, "w"); 
        int cnt = 0;

        while(fread(&LA, sizeof(unsigned long), 1, Fp) == 1) 
        { 
            dNum = (LA & 0x7F);
            pNum = (LA >> d);
            fNum = PageTable[pNum];
            PA = (fNum << d) + dNum; 

            if(p1Test != 0) 
            { 
                printf("The LA is %lx and the corresponding PA is %lx\n", LA, PA); 
            }
            fwrite(&PA, sizeof(unsigned long), 1, writeTo);
            cnt++;
        } 
        if(p1Test != 0) 
        { 
            printf("Total number of pages = %d\n", cnt);
        }
        fclose(Fp);
        fclose(writeTo);
    }
    else
    {
        fprintf(stderr, "ERROR: Could not open given file\n");
        exit(-1);
    }
}