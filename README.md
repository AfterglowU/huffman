# huffman

## Basis

### Intro

* A simple huffman encoding and decoding tool runs on Linux, supporting ASCII only.

### Some limitations

* Runs on Linux:<br>Since I use [getopt](http://man7.org/linux/man-pages/man3/getopt.3.html) to parse the command-line arguments, this tool can only runs on Linux.

* Support ASCII only: <br>"simple"

----

## How to use it

### 1. Build

``` nohighlight
$ make
```

### 2. Encode

``` nohighlight
$ ./huffman -e [inFileName] -o [outFileName]
```

P.S. Recommend to use .hfm as file suffix of the encoded file.

### 3. Decode

``` nohighlight
$ ./huffman -d [inFileName] -o [outFileName]
```

### 4. Help information

Use [-h] option to see the help information.

``` nohighlight
$ ./huffman -h
Usage: huffman [-edovh]
   [-e]                 Encode mode, endode the input file.
   [-d]                 Decode mode, dedode the input file.
   [-o] [outFileName]   Specify the output file.
   [-v]                 Print verbose info, only works in Encode mode.
   [-h]                 Print this message.
```

### note

* You can use the attached testfile to run the commands above.

----

## Encoded file format

Part         | Contant
:----------- | :------
Header       | 1. File identifier 'H' 'F' 'M' <br> 2. Number of chars in the original file <br> 3. Offset to the encoded data
Huffman Tree | The huffman tree of the original file.
Encoded data | The encoded data.