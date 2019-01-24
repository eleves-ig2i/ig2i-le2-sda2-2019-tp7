#include "Huffman.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @todo : finir message codé
 */
// IMPLEMENTATION EN LANGAGE C DE L ALGORITHME DE HUFFMAN POUR COMPRESSER DIVERSES DONNEES
char debug[20] = "Les_Corons.txt";

int main(int argc, char * argv[])
{
	FILE *fp;
	char filename[FILENAME_MAX + 1];
	huffmanTree_t arbreHuffman = { {0, 0} };
	minimierIndirect_t tas = {0, {0} };
	code_t codage[NBMAX_CODAGE];
	int n;

	/* ... */
	if (argc > 1)
		strcpy(filename, argv[1]);
	else
	{
		printf("File : ");
		fflush(stdout);
		//scanf("%[^\n]",filename);
		strcpy(filename,debug);
	}
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		fprintf(stderr, "Erreur d'ouverture du fichier %s\n", filename);
		return EXIT_FAILURE;
	}

	printf ("Le fichier %s contient %i caractères\n", filename, compterCar (fp, arbreHuffman));
	printf ("Le nombre de caractères différents est de %i\n", nbcarDifferents (arbreHuffman));
	afficherOccurrences(arbreHuffman);
	
	creerMinimier(&tas, arbreHuffman);
	printf("\nMinimier créé :\n");
	n = afficherMinimier(&tas, arbreHuffman);

    construireArbreHuffman(arbreHuffman,&tas);

    printf("\nArbre de codage créé :\n");
    afficherArbreHuffman(arbreHuffman);

    construireCodage(arbreHuffman,codage);

    printf("\nTable de codage créé :\n");
    afficherCodage(codage);

    char * messageCode = coderMessage(fp,codage);
    printf("Message codé : \n%s\n",messageCode);

    char * messageDecode = decoderMessage(messageCode,codage);
    printf("Message décodé : \n%s\n",messageDecode);

	free(messageCode);
	free(messageDecode);
    fclose(fp);
	return EXIT_SUCCESS;
}
