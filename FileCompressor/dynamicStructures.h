#ifndef DYNAMICSTRUCTURES_H_
#define DYNAMICSTRUCTURES_H_

#include <stdlib.h>
#include <stdint.h> //1 byte variable uint8_t

typedef struct Huffman { 

	char character;
	int count;

	struct Huffman* next; 

	struct Huffman* left;
	struct Huffman* right; 

} Huffman;

typedef struct LettersCodes {

	char letter;
	char code[50];

} LettersCodes;

Huffman* addToList(const char* character, Huffman* head);

int sortList(const Huffman* head);

void printList(Huffman* head); 

Huffman* makeTreeFromList(Huffman* listRoot);

void makeCodesFromTree(const Huffman* root, const int lettersCount, int tempCode, LettersCodes* codes);

void printLettersCodes(const LettersCodes* codes, const int lettersCount);

Huffman* readLetterFromTree(Huffman* current, const uint8_t bit);

void deallocateTree(Huffman* root);



#endif 
