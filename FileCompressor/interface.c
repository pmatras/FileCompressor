#include <stdio.h>
#include <Windows.h>
#include <conio.h> 

#include "interface.h"
#include "filesSupport.h"
#include "dynamicStructures.h"

void printMenu() {

	printf("********************************************************\n");
	printf("**                                                    **\n");
	printf("** 1. Code your input file,                           **\n");
	printf("** 2. Count compression rate of your coded file,      **\n");
	printf("** 3. Decode your file,                               **\n");
	printf("** 4. Prepare file to coding,                         **\n");
	printf("** 9. Exit.                                           **\n"); 
	printf("**                                                    **\n");
	printf("********************************************************\n\n");
		
		

}

void readChoice(int* choice) {

	printf("Enter your choice: ");

	scanf("%d", choice); 
}

void menu(){               

	         
	enum Choices {codeFile = 1, compressRate, decodeFile, makeFile, exit = 9};

	int choice = 0;

	Huffman* head = NULL;

	Huffman* treeRoot = NULL;

	LettersCodes* codes = NULL;

	int lettersCount = 0;

	int bitsToDecode = 0;

	int alreadyCoded = 0;
	int alreadyDecoded = 0;
	int canDecode = 0;

	char usersInFile[50];
	char usersOutFile[50];

	do {

		fflush(stdin);

		printMenu();

		readChoice(&choice);

		switch (choice) {

		case(codeFile): {

			if (!alreadyCoded) {

				treeRoot = NULL;
				head = NULL;

				printf("\n\nEnter name of file to code with extension (*.txt): ");

				scanf("%s", usersInFile);

				printf("\n\nEnter name of file where coded file will be saved with extension (*.bin): ");

				scanf("%s", usersOutFile);

				head = fileAnalyze(usersInFile, &lettersCount);

				sortList(head); 

				//printList(head); //to check if made list is correct

				treeRoot = makeTreeFromList(head);

				if (lettersCount > 0) {

					codes = (LettersCodes*)calloc(lettersCount, sizeof(LettersCodes));
				}

				int tempCode = 0;

				makeCodesFromTree(treeRoot, lettersCount, tempCode, codes);

				//printLettersCodes(codes, lettersCount); //to check if assigned codes from Huffman's tree are correct

				bitsToDecode = makeEncodedFile(usersInFile, usersOutFile, codes, &lettersCount);

				if (bitsToDecode > 0) {

					printf("\n\nCompressed file saved as: %s\n\n", usersOutFile);

					printf("\nAfter compression of file spare %.2f percent of disk space.\n\n", countCompressionPercent(usersInFile, usersOutFile));

				}
				else {

					printf("Error occured!\n\n");
				}

				if (codes != NULL) {

					free(codes);

					codes = NULL;
				}
				

				alreadyCoded = 1;

				canDecode = 1;

				lettersCount = 0;

			}
			else {

				int codeNext = 0;

				printf("\n\nFile is already compressed or there was error before!\n\nDo you want to compress another? [0/1]: ");
				scanf("%d", &codeNext);

				if (codeNext) {

					if (treeRoot != NULL) {

						deallocateTree(treeRoot);

						treeRoot = NULL;
					}

					alreadyCoded = 0;

					printf("\nNow you can compress another file.\n\n");
				}
				
			}
			
			printf("\n\nEnter key to continue.\n");

			_getch();

			break;

		}

		case(compressRate): {

			char usersInFile[50];
			char usersOutFile[50];

			printf("\n\nEnter file name which was compressed with extension (*.txt): ");
			scanf("%s", usersInFile);

			printf("\nEnter file name, where compressed data are saved with extension (*.bin): ");
			scanf("%s", usersOutFile);
			
			printf("\nCompression rate equals %.2f percent.\n\n", 100 - countCompressionPercent(usersInFile, usersOutFile));


			printf("\nEnter key to continue.\n\n"); 

			_getch();


			break;
		}

		case(decodeFile): { 

			if (!alreadyDecoded && treeRoot) {

				char toDecodeFileName[50];

				printf("\nEnter file name to decode with extension (*.bin): ");
				scanf("%s", toDecodeFileName);

				char outFileName[50];

				printf("\n\nEnter file name, where decoded data will be saved with extension (*.txt): ");
				scanf("%s", outFileName);

				if (treeRoot != NULL) {

					makeDecodedFile(toDecodeFileName, outFileName, treeRoot, &bitsToDecode);

					printf("\n\nDecoded file saved as: %s! \n\n", outFileName);

					printf("After decompression decoded file is larger by %.2f percent than compressed file.", countDecompressionPercent(toDecodeFileName, outFileName));

				}
				else {

					printf("\n\nCode file first! \n\n");
				}

				alreadyDecoded = 1;
			}
			else {

				if (!canDecode) {

					printf("\n\nDidn't compress file yet! Compress any file and try again.\n\n");
				}
				else {

					int decodeNext = 0;

					printf("\n\nFile was decompressed yet!\n\nDo you want to compress file and then decompress another? [0/1]: ");
					scanf("%d", &decodeNext);

					if (decodeNext) {

						deallocateTree(treeRoot);

						treeRoot = NULL;

						alreadyDecoded = 0;

						printf("\n\nNow you can compress file and then decompress it.\n\n");
					}

				}
		
			}

			printf("\n\nEnter key to continue.\n");

			_getch();

			break;


		}

		case(makeFile): {

			char usersTextFile[50];

			printf("\n\nEnter file name with extension where your text file will be saved (*.txt): ");
			scanf("%s", usersTextFile);

			printf("\nEnter text to save in file [max 254 characters at once].\nTo save your entered text hit enter.\n");
			printf("To finish enter '@' once and hit enter.\n\n");

			printf("Enter key to continue.\n");
			_getch();

			system("cls");

			if (makeUsersFile(usersTextFile)) {

				printf("\n\nFile saved as: %s", usersTextFile);

			}
			else {

				printf("Error occured!\n");

			}

			printf("\n\nEnter key to continue.\n");

			_getch();

			break;
		}

		case(exit): {

			printf("Closing program...\n\n");

			break;
		}

		default: {

			printf("Incorrect choose, try again!\n\n"); 

			printf("\n\nEnter key to continue.\n");

			_getch();
		
			break;
		}

		}


		system("cls");



	} while (choice != exit);

	if (treeRoot != NULL) {

		deallocateTree(treeRoot);
		
	}


}
