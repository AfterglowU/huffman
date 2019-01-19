#include <stdio.h>
#include <stdlib.h>
#include "charcounter.h"
#include "huffman.h"

extern int verbose;

HFMTree *Huffman_createTree(const char *fileName)
{
    CNTRetval *cret;
    int varChar;
    int min1, min2;
    int lchild, rchild;
    HFMTree *ht;

    // Init huffman tree (via return value of charcoutner)
    cret = charcounter(fileName);
    varChar = cret->varChar;
    HFMNode *nodes = (HFMNode *)malloc(sizeof(HFMNode) * (2 * varChar - 1));
    for (int i = 0 ; i < varChar; i++)
    {
        nodes[i].data = cret->cr[i].data;
        nodes[i].weight = cret->cr[i].weight;
        nodes[i].parent = -1;
        nodes[i].lchild = -1;
        nodes[i].rchild = -1;
    }
    for (int i = varChar; i < 2 * varChar - 1; i++)
    {
        nodes[i].data = 0;
        nodes[i].weight = 0;
        nodes[i].parent = -1;
        nodes[i].lchild = -1;
        nodes[i].rchild = -1;
    }
    free(cret->cr);
    free(cret);

    // Create huffman tree
    for (int k = varChar; k < 2 * varChar - 1; k++)
    {
        min1 = min2 = 2147483647;
        lchild = rchild = -1;
        for (int i = 0; i < k; i++)
        {
            if (nodes[i].parent == -1)
            {
                if (nodes[i].weight < min1)
                {
                    min2 = min1;
                    rchild = lchild;
                    min1 = nodes[i].weight;
                    lchild = i;
                }
                else if (nodes[i].weight < min2)
                {
                    min2 = nodes[i].weight;
                    rchild = i;
                }
            }
        }
        nodes[lchild].parent = nodes[rchild].parent = k;
        nodes[k].weight = min1 + min2;
        nodes[k].lchild = lchild;
        nodes[k].rchild = rchild;
    }

    ht = (HFMTree *)malloc(sizeof(HFMTree));
    ht->nodes = nodes;
    ht->varChar = varChar;

    return ht;
}

HFMTable *Huffman_genCode(HFMTree ht)
{
    HFMTable *hfmtab;
    int varChar = ht.varChar;

    // Init huffman table
    hfmtab = (HFMTable *)malloc(sizeof(HFMTable) * MAXCHARNUM);
    for (int i = 0, p = 0; i < MAXCHARNUM; i++)
    {
        hfmtab[i].hfmcode = 0;
        hfmtab[i].len = 0;
    }

    // Generate huffman code of each leaf node
    for (int i = 0, p = 0, c = 0; i < varChar; i++)
    {
        c = i;
        while ((p = ht.nodes[c].parent) != -1)
        {
            if (ht.nodes[p].lchild == c)
            {
                hfmtab[ht.nodes[i].data].hfmcode <<= 1;
            }
            else
            {
                hfmtab[ht.nodes[i].data].hfmcode <<= 1;
                hfmtab[ht.nodes[i].data].hfmcode |= 1;
            }
            hfmtab[ht.nodes[i].data].len++;

            c = p;
        }
    }
    
    return hfmtab;
}

void Huffman_encode(const char *inFileName, const char *outFileName)
{
    FILE *fin, *fout;
    HFMTree *hfmtree;
    HFMTable *hfmtab;
    HFMFILEHEADER hfmheader;

    if ((fin = fopen(inFileName, "r")) == NULL)
    {
        fprintf(stderr, "Can't find input file %s .\n", inFileName);
        exit(EXIT_FAILURE);
    }

    if ((fout = fopen(outFileName, "wb")) == NULL)
    {
        fprintf(stderr, "Failed to create output file %s .\n", outFileName);
        exit(EXIT_FAILURE);
    }

    // Generate huffman map table
    hfmtree = Huffman_createTree(inFileName);
    hfmtab = Huffman_genCode(*hfmtree);

    if (verbose)
    {
        for (int i = 0; i < MAXCHARNUM; i++)
        {
            if (hfmtab[i].len != 0)
            {
                switch(i)
                {
                    case 9:
                        printf("[HT]:    ");
                        break;
                    case 10:
                        printf("[LF]:    ");
                        break;
                    case 11:
                        printf("[VT]:    ");
                        break;
                    case 13:
                        printf("[CR]:    ");
                        break;
                    default:
                        printf("[%c] :    ", (char) i);
                        break;
                }
                for (int j = 0; j < hfmtab[i].len; j++)
                {
                    printf("%d", (hfmtab[i].hfmcode >> j) & 1);
                }
                printf("\n");
            }
        }
    }

    // 1. Output the header of hfm file
    hfmheader.hfmType[0] = 'H';
    hfmheader.hfmType[1] = 'F';
    hfmheader.hfmType[2] = 'M';
    hfmheader.numChar = hfmtree->nodes[2 * hfmtree->varChar - 2].weight;
    hfmheader.offset = sizeof(HFMFILEHEADER) + sizeof(HFMNode) * (2 * hfmtree->varChar - 1);
    fwrite(&hfmheader, sizeof(HFMFILEHEADER), 1, fout);
    
    // 2. Output huffman tree
    for (int i = 0; i < 2 * hfmtree->varChar - 1; i++)
    {
        fwrite(&hfmtree->nodes[i], sizeof(HFMNode), 1, fout);
    }

    // 3. Output huffman code of the input file
    char byteout = 0;
    int bitcnt = 0;
    for (char c = fgetc(fin); c != EOF; c = fgetc(fin))
    {
        for (int i = 0; i < hfmtab[c].len; i++)
        {
            byteout <<= 1;
            byteout |= ((hfmtab[c].hfmcode >> i) & 1);
            bitcnt++;

            if (bitcnt == 8)
            {
                fwrite(&byteout, 1, 1, fout);
                byteout = 0;
                bitcnt = 0;
            }
        }
    }
    // Deal with tail
    if (bitcnt != 0)
    {
        byteout <<= (8 - bitcnt);
        fwrite(&byteout, 1, 1, fout);
    }

    free(hfmtree->nodes);
    free(hfmtree);
    free(hfmtab);

    fclose(fin);
    fclose(fout);
}

void Huffman_decode(const char *inFileName, const char *outFileName)
{
    FILE *fin, *fout;
    HFMNode *nodes;
    int nNode, nChar;
    HFMFILEHEADER hfmheader;

    if ((fin = fopen(inFileName, "rb")) == NULL)
    {
        fprintf(stderr, "Can't find input file %s.\n", inFileName);
        exit(EXIT_FAILURE); 
    }

    if ((fout = fopen(outFileName, "w")) == NULL)
    {
        fprintf(stderr, "Failed to create output file %s .\n", outFileName);
        exit(EXIT_FAILURE);
    }

    fread(&hfmheader, sizeof(HFMFILEHEADER), 1, fin);
    if (hfmheader.hfmType[0] != 'H' 
        || hfmheader.hfmType[1] != 'F' 
        || hfmheader.hfmType[2] != 'M' )
    {
        fprintf(stderr, "The input file %s is not huffman encoded.\n", inFileName);
        exit(EXIT_FAILURE);
    }

    nodes = (HFMNode *)malloc(hfmheader.offset - sizeof(HFMFILEHEADER));
    nNode = (hfmheader.offset - sizeof(HFMFILEHEADER)) / sizeof(HFMNode);
    nChar = hfmheader.numChar;
    fread(nodes, (hfmheader.offset - sizeof(HFMFILEHEADER)), 1, fin);

    int bytein;
    int now = nNode - 1;    // root node
    int cnt = 0;
    while((bytein = fgetc(fin)) != EOF)
    {
        for (int i = 7; i >= 0; i--)
        {
            now = ((bytein >> i) & 1) ? nodes[now].rchild : nodes[now].lchild;
            if (cnt >= nChar)
            {
                break;
            }
            else if (nodes[now].lchild == -1 && nodes[now].rchild == -1)
            {
                fwrite(&nodes[now].data,1,1,fout);
                now = nNode - 1;
                cnt++;
            }
        }
    }

    free(nodes);

    fclose(fin);
    fclose(fout);
}