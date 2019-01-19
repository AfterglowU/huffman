#ifndef _HUFFMAN_H
#define _HUFFMAN_H

    typedef struct
    {
        char data;
        int weight;
        int parent;
        int lchild;
        int rchild;
    } HFMNode;

    typedef struct
    {
        HFMNode *nodes; // The huffman tree (an array), containing (2 * varChar - 1) nodes
        int varChar;    // Variety of char, num of different chars appeared
    } HFMTree;

    typedef struct
    {
        int hfmcode;    // Containing the huffman code of the coresponding char
                        // *** stored in reverse order: [leaf <- ... <- root] ***
        int len;        // Length of the huffman code
    } HFMTable;

    typedef struct
    {
        char hfmType[3];    // The header field used to identify hfm file; "HFM"
        int numChar;        // Total num of chars in the original file
        int offset;         // Offset to encoded data
    } HFMFILEHEADER;

    void Huffman_encode(const char *inFileName, const char *outFileName);
    void Huffman_decode(const char *inFileName, const char *outFileName);

#endif