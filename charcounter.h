#ifndef _CHARCOUNTER_H
#define _CHARCOUNTER_H

    #define MAXCHARNUM 256  // This demo Support ASCII only

    typedef struct
    {
        char data;      // The char
        int weight;     // How many times it appears
    } CNTRecord;

    typedef struct
    {
        CNTRecord *cr;  // An array containing appeared char and its weight
        int varChar;    // Variety of char, num of different chars appeared
    } CNTRetval;

    CNTRetval *charcounter(const char *fileName);

#endif