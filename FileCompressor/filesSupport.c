#include "dynamicStructures.h"
#include "filesSupport.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h> 
#include <Windows.h>

#include <string.h>
#include <stdint.h> //1 byte variable uint8_t

#include <io.h> //file's size

#define BYTE_SIZE 8
#define BYTE 1
#define MAX_LENGTH 255
#define ONE 1

Huffman* fileAnalyze(const char* inFileName, int* lettersCount) { 
	
	Huffman* head = NULL;
	
	FILE* inFile = fopen(inFileName, "r");

	if (inFile == NULL) {

		perror("\n\nError opening file: ");

		return NULL;
	}

	Huffman* current = head;

	char character = 0;

	while ((character = fgetc(inFile)) != EOF) { 

		head = addToList(&character, head); 

		if (head != current) {

			*lettersCount = *lettersCount + 1; 

			current = head; 
		}
	}

	fclose(inFile);


	return head;

}

int makeEncodedFile(const char* inFileName, const char* outFileName, const LettersCodes* codes, const int* lettersCount) { 
	
	if (codes == NULL) {

		return 0;
	}
																															
	FILE* inFile = fopen(inFileName, "r");
	FILE* outFile = fopen(outFileName, "wb");  

	if (inFile == NULL || outFile == NULL) {

		perror("\n\nError opening file: ");

		return 0;
	}
	
	char code[50] = "";

	char character; 

	uint8_t bitsBuffer = 0; //1 byte variable

	uint8_t bufferFullness = 0; 

	uint8_t bit = 0; 

	int encodedBitsCount = 0; 
	                          
							  
	while ((character = fgetc(inFile)) != EOF) { 

		for (int i = 0; i < lettersCount; ++i) {

			if (character == codes[i].letter) {

				strcpy(code, &codes[i].code);

				break;
			}
		}

		for (int i = 0; i < strlen(code); ++i) {

			bit = code[i] - '0';

			bitsBuffer = bitsBuffer | (bit << (7 - bufferFullness)) ; //MSB to LSB order

			bufferFullness++;

			if (bufferFullness == 8) {

				fwrite(&bitsBuffer, BYTE, ONE, outFile);

				bufferFullness = 0;

				bitsBuffer = 0;


			}

			encodedBitsCount++;
		}

			
	}

	if (bufferFullness > 0) { 

		fwrite(&bitsBuffer, sizeof(bitsBuffer), ONE, outFile); 
		
	}
	

	fclose(inFile);
	fclose(outFile);
	
	return encodedBitsCount; 
}


double countCompressionPercent(const char* inFileName, const char* outFileName) {

	int fn = 0;
	double inFileSize = 0;
	double outFileSize = 0;

	FILE* inFile = fopen(inFileName, "rb");

	FILE* outFile = fopen(outFileName, "rb");

	if (inFile != NULL && outFile != NULL) {

		fn = _fileno(inFile);

		inFileSize = _filelength(fn);

		fn = _fileno(outFile);

		outFileSize = _filelength(fn);


	}
	else {

		printf("\n\nError occured!\n\n");

		return 0;
	}


	fclose(inFile);
	fclose(outFile);

	double compressionRate = (1 - (outFileSize / inFileSize) ) * 100;

	return compressionRate;
}

int makeDecodedFile(const char* toDecodeFileName, const char* outFileName, const Huffman* root, const int* bitsToDecode) {

	FILE* inFile = fopen(toDecodeFileName, "rb");

	FILE* outFile = fopen(outFileName, "w");

	if (inFile == NULL || outFile == NULL) {

		perror("\n\nError opening file: \n\n");

		return 0;
	}

	int decodedBits = 0;

	uint8_t bit = 0; 

	uint8_t bitsBuffer = 0;  
	uint8_t tempBitsBuffer = 0; 
	uint8_t bitsReaded = 0; 

	Huffman* actualPosInTree = root;
	
	while(!feof(inFile)) {

		fread(&bitsBuffer, BYTE, ONE, inFile); 

		bitsReaded = BYTE_SIZE;

		while (bitsReaded > 0) { 

			tempBitsBuffer = bitsBuffer;

			bit = (tempBitsBuffer >> (bitsReaded - 1)) & 1; //MSB to LSB order

			decodedBits++;

			if (decodedBits > (*bitsToDecode)) { 

				break;
			}

			actualPosInTree = readLetterFromTree(actualPosInTree, bit); 
			                                                            
			if ((actualPosInTree != NULL) && (actualPosInTree->character != '@')) { 

				fputc(actualPosInTree->character, outFile);

				actualPosInTree = root;

			}

			bitsReaded--;

		}
				
	}

	
	fclose(inFile);
	fclose(outFile);
	

	return 1;
}

double countDecompressionPercent(const char* inFileName, const char* outFileName) {

	int fn = 0;
	double inFileSize = 0;
	double outFileSize = 0;

	FILE* inFile = fopen(inFileName, "rb");

	FILE* outFile = fopen(outFileName, "rb");

	if (inFile != NULL && outFile != NULL) {

		fn = _fileno(inFile);

		inFileSize = _filelength(fn);

		fn = _fileno(outFile);

		outFileSize = _filelength(fn);


	}
	else {

		printf("\n\nError occured! \n\n");

		return 0;
	}


	fclose(inFile);
	fclose(outFile);

	double compressionRate = (outFileSize / inFileSize - 1) * 100;

	return compressionRate;

}

int makeUsersFile(const char* fileName) {

	FILE* outFile = fopen(fileName, "w");

	if (outFile == NULL) {

		perror("\n\nError opening file: ");

		return 0;
	}

	char* usersText = (char*)calloc(MAX_LENGTH, BYTE);	

	do {

		fflush(stdin);

		fgets(usersText, 254, stdin);

		if (usersText != NULL && *usersText != '@' && *usersText != '\n') {

			fprintf(outFile, "%s", usersText);

			fflush(outFile); 

		}
		
	} while (*usersText != '@');


	fclose(outFile);

	free(usersText);	


	return 1;
}


