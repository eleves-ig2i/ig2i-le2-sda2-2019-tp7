#include "Huffman.h"


static void permuter(int * t, int a, int b);
static void inverser(code_t * c);
/* ------------------------------------------------------------------------- */
/**
* \fn		int compterCar(FILE *fp, huffmanTree_t arbre)
* \brief	Crée et initialise les feuilles de l'arbre d'Huffman
*			en comptant le nombre d'occurrences de chaque
*			symbole (caractères) présent dans un fichier
* \param	fp le fichier dans lequel on lit et compte les
*			symboles
* \param	arbre l'arbre d'Huffman (une table de noeuds)
* \return	le nombre de symboles lus dans le fichier
* \note		le fichier désigné par fp est supposé avoir été
*			ouvert préalablement en lecture. Il peut tout aussi
*			bien s'agir un fichier texte ou d'un fichier
*			contenant une image bitmap, ...
* \note		Initialement, les noeuds de l'arbre d'Huffman doivent
*			tous être initialisés avec un nombre d'occurrences nul.
*/
int compterCar (FILE *fp, huffmanTree_t arbre)
{
	int nbCar = 0;
	char c;
	if( fp == NULL)
		return 0;

	while( ( c = getc(fp) ) != EOF )
	{
		arbre[c].occ++;
		nbCar++;
	}

	return nbCar;
}


/* ------------------------------------------------------------------------- */
/**
* \fn		int nbcarDifferents(huffmanTree_t arbre)
* \brief	Compte le nombre de feuilles ayant un nombre
*			d'occurences strictement positif dans l'arbre d'Huffman
* \param	arbre l'arbre d'Huffman (une table de noeuds)
* \return	le nombre de symboles differents
* \note		les feuilles de l'arbre d'Huffman correspondent aux
*			noeuds d'indice 0 à NBMAX_ELT - 1
*/
int nbcarDifferents (huffmanTree_t arbre)
{
	int cpt = 0;
	for(int i = 0; i < NBMAX_ELT;i++)
	{
		if( arbre[i].occ != 0)
			cpt++;
	}
	return cpt;
}


/* ------------------------------------------------------------------------- */
/**
* \fn		oid afficherOccurrences(huffmanTree_t arbre)
* \brief	effectue l'affichage de l'arbre d'Huffman initial
*			c-à-d réduit à ses feuilles
* \param	arbre l'arbre d'Huffman (une table de noeuds)
* \return	néant
* \note		les feuilles de l'arbre d'Huffman correspondent aux
*			noeuds d'indice 0 à NBMAX_ELT - 1.
*			L'affichage est réalisé sous forme d'une table à 3 colonnes:
*			<ul>
*			<li>indice de la case : code Ascii du caractère</li>
*			<li>nombre d'occurrences</li>
*			<li>caractère, LR,CR, TAB ou hexa</li>
*			</ul>
*/
void afficherOccurrences(huffmanTree_t arbre)
{
	printf("Indice | Occurences | Caractère\n");
	printf("--------------------------\n");
	for(int i =0; i < NBMAX_ELT; i++)
	{
		if( arbre[i].occ != 0 )
		{
			printf("  %d\t|  %d\t|  '%c'  |\n",i,arbre[i].occ,i);
		}
	}
	printf("--------------------------\n");
}


/* ------------------------------------------------------------------------- */
/**
* \fn		void descendreMinimier(minimierIndirect_t *pTas,
*									huffmanTree_t arbre, int k)
* \brief	fait descendre l'élément d'indice k d'un minimier pour
*			rétablir sa propriété de minimier à partir de k.
* \param	pTas le pointeur sur le minimier indirect
* \param	arbre l'arbre d'Huffman contenant les noeuds dont
*			les indices sont les éléments du minimier, et
*			les nombres d'occurrences, les clés de comparaison
*			du minimier
* \param	k l'indice de l'élément à faire redescendre
* \return	néant
* \note		le minimier indirect est réorganisé
*/
void descendreMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre, int k)
{
	int i;
	while( existeFilsG(k,pTas->taille) )
	{
		i = FilsG(k);
		if( existeFilsD(k,pTas->taille) && ( arbre[ pTas->tab[ FilsG(k) ] ].occ > arbre[ pTas->tab[ FilsD(k) ] ].occ  ) )
			i = FilsD(k);

		if( arbre[ pTas->tab[i] ].occ < arbre[ pTas->tab[ k ] ].occ )
		{

			permuter(pTas->tab,i,k);
			k = i;
		}
		else
			break;
	}
}


/* ------------------------------------------------------------------------- */
/**
* \fn		void transformeEnMinimier(int n, minimierIndirect_t *pTas,
*										huffmanTree_t arbre)
* \brief	transforme les n premières composantes du tableau de la
*			structure pointée par pTas en minimier indirect.
* \param	n nombre de composantes à réorganiser en minimier
*			indirect
* \param	pTas le pointeur sur le minimier indirect
* \param	arbre l'arbre d'Huffman contenant les noeuds dont
*			les indices sont les éléments du minimier, et
*			les nombres d'occurrences, les clés de comparaison
*			du minimier
* \return	néant
* \note		le minimier indirect est réorganisé et sa taille est
*			mise à jour.
*/
void transformeEnMinimier (int n, minimierIndirect_t *pTas, huffmanTree_t arbre)
{
	for(int i = n/2; i >= 0; --i)
	{
		descendreMinimier(pTas,arbre,i);
	}
}


void creerMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre)
{
	int k = 0;
	for(int c = 0; c < NBMAX_ELT; c++)
	{
		if( arbre[c].occ != 0 )
			pTas->tab[ pTas->taille++ ] = c;
	}
	transformeEnMinimier(pTas->taille,pTas,arbre);
}


/* ------------------------------------------------------------------------- */
/**
* \fn		int afficherMinimier (const T_MinimierIndirect *pTas,
*                                       huffmanTree_t arbre)
* \brief	affiche le contenu d'un minimier indirect, c-à-d les
*			indices du tableau, le caractère (ou son code ASCII)
*			correspondant ainsi que le nombre d'occurrences de celui-ci
* \param	pTas le pointeur sur le minimier indirect
* \param	arbre l'arbre d'Huffman contenant les noeuds dont
*			les indices sont les éléments du minimier, et
*			les nombres d'occurrences, les clés de comparaison
*			du minimier
* \return	la taille du minimier
*/
int afficherMinimier (const minimierIndirect_t *pTas, huffmanTree_t arbre)
{
	printf("Indice | Occurences | Caractère\n");
	printf("--------------------------\n");
	for(int i =0; i < pTas->taille ; i++)
	{
		printf("  %d\t|  '%c'\t|  %d  |\n",i,pTas->tab[i],arbre[pTas->tab[i]].occ);
	}
	printf("--------------------------\n");
	return 0;
}


/* ------------------------------------------------------------------------- */
/**
* \fn		int extraireMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre)
* \brief	extrait l'élément minimum d'u minimier indirect et le
*			réorganise pour réablir sa proprieté de minimier
* \param	pTas le pointeur sur le minimier indirect
* \param	arbre l'arbre d'Huffman contenant les noeuds dont
*			les indices sont les éléments du minimier, et
*			les nombres d'occurrences, les clés de comparaison
*			du minimier
* \return	l'indice du noeud ayant le nombre d'occurrences minimal
*			-1 si le minimier était vide.
* \note		le minimier indirect est réorganisé et sa taille est
*			réduite d'une unité.
*/
int extraireMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre)
{
	if( pTas == NULL || pTas->taille == 0 )
		return -1;

	int caractereMin = pTas->tab[0];
	pTas->tab[0] = pTas->tab[pTas->taille-1];

	pTas->taille--;
	descendreMinimier(pTas,arbre,0);

	return caractereMin;
}


/**
 * \brief   construit le noeud de l'arbre d'Huffman vérifiant les propriétés suivantes :
 *          - le nombre d'occurences de ce noeud correspond à la somme des occurences des noeuds d'indice min1 et min2
 *          - l'indice de ce noeud dans l'arbre est égal à i.
 * \param i l'indice du noeud à créer
 * \param arbre l'arbre de Huffman en train d'être élaboré.
 * \param min1 code ascii du fils gauche du noeud d'indice i
 * \param min2 code ascii du fils droit du noeud d'indice i
 *
 * \note un exemple d'utilisation de cette fonction serait :
 *      for(int i = 255+nbcarDifferents(arbre); i != 255; i--)
 *          construireNoeudHuffman(i,arbre,extraireMinimier(pTas,arbre),extraireMinimier(pTas,arbre),extraireMinimier(pTas,arbre))
 */
void construireNoeudHuffman(int i, huffmanTree_t arbre, int min1, int min2)
{
	arbre[i].occ = arbre[min1].occ + arbre[min2].occ;

	arbre[min1].pere = -i;	// fils gauche
	arbre[min2].pere = i;	//fils droit

	printf("Création du noeud %d valant \t%d, père de ",i,arbre[i].occ);
	if( min1 < NBMAX_ELT )
		printf("'%c'",min1);
	else
		printf("%d",min1);

	printf(" et de ");

	if( min2 < NBMAX_ELT )
		printf("'%c'\n",min2);
	else
		printf("%d\n",min2);

}


/**
 * \brief Construit l'arbre de codage (de Huffman) à l'aide du minimier indirect pTas.
 * \param arbre l'arbre de codage à construire
 * \param pTas le minimier indirect.
 */
void construireArbreHuffman(huffmanTree_t arbre, minimierIndirect_t *pTas)
{
	for(int i = 255+nbcarDifferents(arbre)-1; i != 255; i--)
	{
		int filsG = extraireMinimier(pTas,arbre);
		int filsD = extraireMinimier(pTas,arbre);
		construireNoeudHuffman(i,arbre,filsG,filsD);
		insererPereDansMinimier(i,arbre,pTas);
	}
}

/**
 * \brief Remonte la valeur d'indice k dans le minimier indirect représenté par pTas si possible.
 * \param pTas le minimier indirect à traiter
 * \param a l'arbre de codage, permettant les comparaisons dans le minimier.
 * \param k un entier compris entre 0 et pTas->taille -1 inclus
 */
void remonterMinimier(minimierIndirect_t *pTas,huffmanTree_t  arbre , int k)
{
    if( pTas == NULL )
        return;

    while( existePere(k) && ( arbre[ pTas->tab[ Pere(k)] ].occ > arbre[ pTas->tab[ k ] ].occ ) )
    {
        permuter(pTas->tab,k,Pere(k));
        k = Pere(k);
    }
}


/**
 * \brief Insère l'indice d'un noeud père (interne) de l'arbre d'Huffman dans le minimier indirect.
 * \param indice l'indice du noeud père.
 * \param pTas le minimier indirect
 * \param arbre l'arbre d'huffman
 */
void insererPereDansMinimier(int indice, huffmanTree_t arbre, minimierIndirect_t *pTas )
{
	if( pTas == NULL)
		return;

	pTas->tab[ pTas->taille ] = indice;
	pTas->taille++;

	remonterMinimier(pTas,arbre,pTas->taille-1);
}


/* ------------------------------------------------------------------------- */
/**
* \fn		int afficherArbreHuffman (huffmanTree_t arbre)
* \brief	affiche le contenu d'un arbre d'Huffman
* \param	arbre l'arbre d'Huffman contenant les noeuds dont
*			les indices sont les éléments du minimier, et
*			les nombres d'occurrences, les clés de comparaison
*			du minimier
* \return	le nombre de noeuds de l'arbre d'Huffman
* \note		utilise les fonctions noFilsG() et noFilsD()
* \note		les éléments affichés sont les suivants:
*			+-----+---------+------+-------+-------+-------+
*			| No | Car. | Occ. | FilsG | FilsD | Pere |
*			+-----+---------+------+-------+-------+-------+
*/
int afficherArbreHuffman (huffmanTree_t arbre)
{
	int nbNoeuds = 0;
	printf("Indice | Caractère | Occurences | Pere\n");
	printf("--------------------------------\n");
	for(int i = 0; i < NBMAX_ELT; i++)
	{
		if( arbre[i].occ != 0)
		{
			nbNoeuds++;
			printf("  %d\t|  '%c'\t|  %d\t| %d\n",i,i,arbre[i].occ,arbre[i].pere);
		}
	}

	for(int i = NBMAX_ELT; i < NBMAX_NOEUDS; i++)
	{
		if( arbre[i].occ != 0)
		{
			nbNoeuds++;
			printf("  %d\t|  %d\t| %d\t| %d\n",i,i,arbre[i].occ,arbre[i].pere);
		}
	}

	return nbNoeuds;
}


/* ------------------------------------------------------------------------- */
/**
* \fn		void construireCodage (huffmanTree_t arbre, code_t codage[])
* \brief	construit la table de codage des NBMAX_ELT de l'alphabet
*			considéré, en parcourant l'arbre d'Huffman r
* \param	[in] arbre l'arbre d'Huffman contenant les noeuds constitués
*			de deux composantes : le nombre d'occurrences et
*			le lien avec le noeud père
* \param	[out] codage la table des MBMAX_ELT codes de longueur variable
* \return	néant
*/
void construireCodage (huffmanTree_t arbre, code_t codage[])
{
	for(int i = 0; i < NBMAX_ELT; i++)
	{
		if( arbre[i].occ != 0 )
		{
			codage[i] = encoderCaractere(i,arbre);
		}

	}
}

/**
 * \brief Encode le caractère de code ascii c, à l'aide de l'arbre de codage a.
 * \param c le code ascii du caractère à encoder
 * \param a l'arbre de codage.
 * \return le code associé à c, construit dans la fonction.
 */
code_t encoderCaractere(int c, huffmanTree_t a)
{
	code_t code;
	code.nbElts = 0;
	noeud_t noeudActuel = a[c];
	int indicePere;
	while(noeudActuel.pere != 0) {

		if (noeudActuel.pere > 0) {
			code.tab[code.nbElts++] = true;
		}
		else {
			code.tab[code.nbElts++] = false;
		}

		indicePere = abs(noeudActuel.pere);
		noeudActuel = a[ indicePere ];
	}

	inverser(&code);

	for(int i = 0; i < code.nbElts; i++)
	{
		if( code.tab[i] == true )
			code.toString[i] = '1';
		else
			code.toString[i] = '0';
	}
	code.toString[code.nbElts] = '\0';

	return code;
}

/* ------------------------------------------------------------------------- */
/**
* \fn		int afficherCodage (const code_t codage[])
* \brief	affiche la table de codage qui a été générée
* \param	[in] codage la table de codage
* \return	le nombre de codes effectivement générés
* \note		Les codes présents dans la table de codage
*			se caractérisent par une longueur > 0
* \note		l'affichage produit présente le code ASCII
*			initial, le caractère correspondant (ou
*			CR, LF, TAB ou code hexa) et le code de
*			longueur variable produit précédemment
*			+------+--------+----------------+
*			| No | Car. | Code |
*			+------+--------+----------------+
*/
int afficherCodage (const code_t codage[])
{
	printf("| No | Car. | Code |\n");
	printf("--------------------\n");
	int nbNoeuds = 0;
	for(int i = 0; i < NBMAX_CODAGE; i++)
	{
		if( codage[i].nbElts != 0)
		{
			printf(" %d\t| '%c'| ",i,i);
			afficherCode(codage[i]);
			printf("\n");
			nbNoeuds++;
		}
	}
	return nbNoeuds;
}

void afficherCode( code_t c)
{
	printf("%s",c.toString);
}

/* ------------------------------------------------------------------------- */
/**
* \fn		int valBit(unsigned int Mot, int i)
* \brief	retourne la valeur du i ème bit d'un mot
* \param	Mot le mot dont on teste la valeur d'un bit
* \param	i le n° du bit à tester entre 0 (bit de poids faible) et
*			31 (bit de poids fort)
* \return	1 ou 0 (la valeur du bit n°i de Mot)
* \note		>> représente l'opérateur de décalage à droite
*			& représente l'opérateur ET logique (bit à bit)
*/
int ValBit (unsigned int Mot, int i)
{
	return 0;
}


/**
 * \brief Code le message contenu dans le fichier f à l'aide de la table de codage c.
 * \param m le message à coder
 * \param c la table de codage utilisé.
 * @return le message codé.
 */
char * coderMessage( FILE *f, const code_t c[])
{
	if( f == NULL || c == NULL )
		return NULL;

	char * messageCode = malloc(1000*sizeof(char));
	rewind(f);	// initialise le curseur du fichier au départ.
	int caractere;

	strcpy(messageCode,"");
	while( ( caractere = getc(f) ) != EOF )
	{
		strcat(messageCode, c[caractere].toString);
	}
	return messageCode;
}


/**
 * \brief Renvoie true si morceauCode est égal à l'un des codes de la table de codage, false sinon.
 * \param morceauCode
 * \param c
 * \return l'indice (càd le caractère) dont le code correspond à morceauCode, -1 sinon.
 */
int comparerCode(const char * morceauCode, const code_t c[])
{
    if( morceauCode == NULL || c == NULL)
        return -1;

    for(int i = 0; i < NBMAX_ELT; i++)
    {
        if( c[i].nbElts != 0)
        {
            if( strcmp(morceauCode,c[i].toString) == 0 )
                return i;
        }
    }

    return -1;

}

/**
 * \brief Décode le message m à l'aide de la table c
 * \param m le message à décoder
 * \param c la table permettant de décoder le message.
 * \return le message décodé, NULL si m ou c vaut NULL
 */
char * decoderMessage( char * m, const code_t c[])
{
    if( m == NULL || c == NULL)
        return NULL;

    int iMessageDecode = 0;
    char * messageDecode = malloc(1000*sizeof(char));

    int iMessage = 0;
    while( m[iMessage] != '\0')
    {
        int iCode = 0;
        char morceauCode[NBMAX_CODE] = "";
        bool morceauCodeTraduit = false;
        while( m[iMessage] != '\0' && !morceauCodeTraduit)
        {
            int car = -1;
            morceauCode[iCode++] = m[iMessage++];
            if( ( car = comparerCode(morceauCode,c) ) != -1 )
            {
                messageDecode[iMessageDecode++]= (char)car;
                morceauCodeTraduit = true;
            }

        }
        // iMessage++;
    }

    messageDecode[iMessageDecode] = '\0';
    return messageDecode;
}

/* ------------------------------------------------------------------------- */

static void permuter(int * t, int a, int b)
{
	int aux = t[a];
	t[a] = t[b];
	t[b] = aux;
}

static void inverser(code_t * c)
{
	if( c == NULL)
		return;

	for(int i = 0; i < c->nbElts/2; i++)
	{
		bool aux = c->tab[i];
		c->tab[i] = c->tab[ c->nbElts - 1 - i];
		c->tab[ c->nbElts - 1 - i] = aux;
	}
}