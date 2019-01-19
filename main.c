#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "huffman.h"

int verbose = 0;            // Doesn't show huffman table by default
struct globalArgs_t
{
    int mode;               // 0: encode, 1: decode
    const char *outFileName;// -o option
    const char **inputFiles;// input files
    int numInputFiles;      // # of input files
} globalArgs;
static const char *optString = "hvedo:";
extern char *optarg;
extern int optind, opterr, optopt;

void display_usage();

int main(int argc, char const *argv[])
{
    char opt;

    // Initialize globalArgs
    globalArgs.mode = 0;
    globalArgs.outFileName = NULL;
    globalArgs.inputFiles = NULL;
    globalArgs.numInputFiles = 0;

    // Parse the command-line arguments
    while ((opt = getopt(argc, argv, optString)) != EOF)
    {
        switch (opt)
        {
            case 'h':
                display_usage();
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                verbose = 1;
                break;
            case 'e':
                globalArgs.mode = 0;
                break;
            case 'd':
                globalArgs.mode = 1;
                break;
            case 'o':
                globalArgs.outFileName = optarg;
                break;
            case '?':
                display_usage();
                exit(EXIT_FAILURE);
            default:
                // Never reach here
                break;
        }
    }
    globalArgs.inputFiles = argv + optind;
    globalArgs.numInputFiles = argc - optind;

    // Check whether user have specified (at least) an input file or not,
    // and eliminate extra input files.
    if (globalArgs.inputFiles == NULL || globalArgs.numInputFiles == 0)
    {
        fprintf(stderr, "No input file, you should specify one.\n");
        display_usage();
        exit(EXIT_FAILURE);
    }
    else if (globalArgs.numInputFiles > 1)
    {
        printf("More than 1 input files, files listed below are ignored:\n");
        for (int i = 1; i < globalArgs.numInputFiles; i++)
        {   
            printf("\t%s\n", globalArgs.inputFiles[i]);
        }
    }

    // Check whether user have specified the output file or not.
    if (globalArgs.outFileName == NULL)
    {
        fprintf(stderr, "No output file, you should specify one.\n");
        display_usage();
        exit(EXIT_FAILURE);
    }

    // Encode or decode
    switch (globalArgs.mode)
    {
        case 0:
            Huffman_encode(globalArgs.inputFiles[0], globalArgs.outFileName);
            break;
        case 1:
            Huffman_decode(globalArgs.inputFiles[0], globalArgs.outFileName);
            break;
        default:
            // Never reach here.
            break;
    }
    
    return EXIT_SUCCESS;
}

void display_usage()
{
    printf("Usage: huffman [-edovh]\n");
    printf("   [-e]                 Encode mode, endode the input file.\n");
    printf("   [-d]                 Decode mode, dedode the input file.\n");
    printf("   [-o] [outFileName]   Specify the output file.\n");
    printf("   [-v]                 Print verbose info, only works in Encode mode.\n");
    printf("   [-h]                 Print this message.\n");
}
