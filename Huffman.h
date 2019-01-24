#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------------------ */
/*      M A C R O - F O N C T I O N S  P O U R  M I N I M I E R             */
/* ------------------------------------------------------------------------ */
#define Pere(i)     ( (i-1) >> 1 )            // indice du p�re du noeud rang� � l�indice i
#define FilsG(i)    ( 2*i+1 ) 			// indice du fils gauche du noeud rang� � l�indice i
#define FilsD(i)    ( 2*(i+1) ) 			// indice du fils droit du noeud rang� � l�indice i
#define existePere(i)   (i > 0)		// indique si le noeud d�indice i a un p�re
#define existeFilsG(i , n)  ( 2*i < n-1 ) 	// indique si le noeud d�indice i a un fils gauche dans un tas de n �l�ments
#define existeFilsD(i , n)  ( 2*i < n-2 ) 	// indique si le noeud d�indice i a un fils droit dans un tas de n �l�ments
/*------------------------------------------------------------------------- */
typedef struct {
	unsigned int occ;	/* nombre d'occurrences */
	int pere;			/* indice du noeud père */
} noeud_t;

#define NBMAX_CODE 32
#define NBMAX_ELT 256
#define NBMAX_CODAGE NBMAX_ELT
#define NBMAX_NOEUDS (2*NBMAX_ELT-1)
typedef noeud_t huffmanTree_t[NBMAX_NOEUDS];

typedef struct {
	int taille;			/* Nb d'éléments */
	int tab[NBMAX_ELT+1];	/* Minimier */
} minimierIndirect_t;


typedef struct
{
    bool tab[NBMAX_CODE];
    size_t nbElts;
    char toString[NBMAX_CODE];
}code_t;


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
int compterCar (FILE *fp, huffmanTree_t arbre);
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
int nbcarDifferents (huffmanTree_t arbre) ;
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
void afficherOccurrences(huffmanTree_t arbre);
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
void descendreMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre, int k);
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
void transformeEnMinimier (int n, minimierIndirect_t *pTas, huffmanTree_t arbre);
void creerMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre);
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
int afficherMinimier (const minimierIndirect_t *pTas, huffmanTree_t arbre);
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
int extraireMinimier (minimierIndirect_t *pTas, huffmanTree_t arbre);
/* ------------------------------------------------------------------------- */


/**
 * \brief   construit le noeud de l'arbre d'Huffman vérifiant les propriétés suivantes :
 *          - le nombre d'occurences de ce noeud correspond à
 *            la somme des occurences des noeuds d'indice min1 et min2
 *          - l'indice de ce noeud dans l'arbre est égal à i.
 * \param i l'indice du noeud à créer
 * \param arbre l'arbre de Huffman en train d'être élaboré.
 * \param min1 code ascii de la 1ere racine du minimier.
 * \param min2 code ascii de la 2e racine du minimier.
 *
 * \note un exemple d'utilisation de cette fonction serait :
 *      for(int i = 255+nbcarDifferents(arbre); i != 255; i--)
 *          construireNoeudHuffman(i,arbre,extraireMinimier(pTas,arbre),extraireMinimier(pTas,arbreà)
 */
void construireNoeudHuffman(int i, huffmanTree_t arbre, int min1, int min2);


/**
 * \brief Construit l'arbre de codage (de Huffman) à l'aide du minimier indirect pTas.
 * \param arbre l'arbre de codage à construire
 * \param pTas le minimier indirect.
 */
void construireArbreHuffman(huffmanTree_t arbre, minimierIndirect_t *pTas);


/**
 * \brief Remonte la valeur d'indice k dans le minimier indirect représenté par pTas si possible.
 * \param pTas le minimier indirect à traiter
 * \param a l'arbre de codage, permettant les comparaisons dans le minimier.
 * \param k un entier compris entre 0 et pTas->taille -1 inclus
 */
void remonterMinimier(minimierIndirect_t *pTas, huffmanTree_t a, int k);

/**
 * \brief Insère l'indice d'un noeud père (interne) de l'arbre d'Huffman dans le minimier indirect.
 * \param indice l'indice du noeud père.
 * \param pTas le minimier indirect
 * \param arbre l'arbre d'huffman
 */
void insererPereDansMinimier(int indice, huffmanTree_t arbre, minimierIndirect_t *pTas );

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
int afficherArbreHuffman (huffmanTree_t arbre);
/* ------------------------------------------------------------------------- */

/**
 * \brief Encode le caractère de code ascii c, à l'aide de l'arbre de codage a.
 * \param c le code ascii du caractère à encoder
 * \param a l'arbre de codage.
 * \return le code associé à c.
 */
code_t encoderCaractere(int c, huffmanTree_t a);


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
void construireCodage (huffmanTree_t arbre, code_t codage[]);
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
int afficherCodage (const code_t codage[]);

void afficherCode( code_t c);

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
int ValBit (unsigned int Mot, int i);
/* ------------------------------------------------------------------------- */
/**
 * \brief Code le message contenu dans le fichier f à l'aide de la table de codage c.
 * \param m le message à coder
 * \param c la table de codage utilisé.
 * @return le message codé.
 */
char * coderMessage( FILE *f, const code_t c[]);


/**
 * \brief Renvoie true si morceauCode est égal à l'un des codes de la table de codage, false sinon.
 * \param morceauCode
 * \param c
 * \return l'indice (càd le caractère) dont le code correspond à morceauCode, -1 sinon.
 */
int comparerCode(const char * morceauCode, const code_t c[]);


/**
 * \brief Décode le message m à l'aide de la table c
 * \param m le message à décoder
 * \param c la table permettant de décoder le message.
 * \return le message décodé.
 */
char * decoderMessage( char * m, const code_t c[]);
