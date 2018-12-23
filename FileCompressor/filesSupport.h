#ifndef FILESSUPPORT_H_
#define FILESSUPPORT_H_

#include "dynamicStructures.h"


Huffman* fileAnalyze(const char* inFileName, const int* lettersCount);

int makeEncodedFile(const char* inFileName, const char* outFileName, const LettersCodes* codes, const int* lettersCount); 

double countCompressionPercent(const char* inFileName, const char* outFileName);

int makeDecodedFile(const char* toDecodeFileName, const char* outFileName, const Huffman* root, const int* bitsToDecode);

double countDecompressionPercent(const char* inFileName, const char* outFileName);

int makeUsersFile(const char* fileName);


#endif 