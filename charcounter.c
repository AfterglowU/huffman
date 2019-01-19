#include <stdio.h>
#include <stdlib.h>
#include "charcounter.h"

// Caculate the weight of each char (ASCII ONLY) in the input file.
CNTRetval *charcounter(const char *fileName)
{
    int varChar = 0;
    CNTRecord *cr = NULL;
    CNTRetval *cret = NULL;

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "charcounter: Can't find input file %s .\n", fileName);
        exit(EXIT_FAILURE);
    }

    cr = (CNTRecord *)malloc(sizeof(CNTRecord) * MAXCHARNUM);
    cret = (CNTRetval *)malloc(sizeof(CNTRetval));
    for (int i = 0; i < MAXCHARNUM; i++)
    {
        cr[i].data = (char) i;
        cr[i].weight = 0;
    }

    for(char c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        cr[c].weight++;
    }

    // Move all appeared char(s) to the beginning of cr
    for (int i = 0, j = 0; i < MAXCHARNUM; i++)
    {
        if (cr[i].weight != 0)
        {
            cr[j++] = cr[i];
            varChar++;
        }
    }

    cret->cr = cr;
    cret->varChar = varChar;

    fclose(fp);

    return cret;
}
