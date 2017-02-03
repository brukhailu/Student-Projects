#include "gestion_tree.h"
#include <unistd.h>

/**
 * Calcule le maximum et le minimum utilis�s par les points d'une liste des cha�nes.
 *
 * @param C
 * Liste des cha�nes.
 * @param xmin
 * Abscisse minimum.
 * @param ymin
 * Ordonn� minimum.
 * @param xmax
 * Abscisse maximum.
 * @param ymax
 * Ordonn� maximum.
 */
void chaineCoordMinMax(Chaines *C, double *xmin, double *ymin, double *xmax, double *ymax)
{
  if (!C) return;
  if (!C->chaines) return;

  CellChaine *parcoursChaines = C->chaines;
  CellPoint *parcoursPoints = parcoursChaines->points;

  *xmin = parcoursPoints->x;
  *ymin = parcoursPoints->y;
  *xmax = parcoursPoints->x;
  *ymax = parcoursPoints->y;

  while (parcoursChaines) {
    parcoursPoints = parcoursChaines->points;
    while (parcoursPoints) {
      if (parcoursPoints->x > *xmax) *xmax = parcoursPoints->x;
      if (parcoursPoints->y > *ymax) *ymax = parcoursPoints->y;
      if (parcoursPoints->x < *xmin) *xmin = parcoursPoints->x;
      if (parcoursPoints->y < *ymin) *ymin = parcoursPoints->y;
      parcoursPoints = parcoursPoints->suivant;
    }
    parcoursChaines = parcoursChaines->suivant;
  }
}

/**
 * Cr�e un arbre quaternaire.
 * 
 * @param xc
 * Centre de l'arbre quaternaire.
 * @param yc
 * Centre de l'arbre quaternaire.
 * @param coteX
 * Longueur de l'arbre quaternaire.
 * @param coteY
 * Largeur de l'arbre quaternaire.
 * @return 
 * Arbre quaternaire nouvellement cr��.
 */
ArbreQuat *creerArbreQuat(double xc, double yc, double coteX, double coteY)
{
  ArbreQuat *treeReturn = (ArbreQuat *) malloc (sizeof(ArbreQuat));

  treeReturn->xc = xc;
  treeReturn->yc = yc;
  treeReturn->coteX = coteX;
  treeReturn->coteY = coteY;

  treeReturn->noeud = NULL;
  treeReturn->so = NULL;
  treeReturn->se = NULL;
  treeReturn->no = NULL;
  treeReturn->ne = NULL;

  return treeReturn;
}

/**
 * Indique o� doit aller le point dans l'arbre. 
 *
 * @param noeudADeterminer
 * Noeud duquel il faut trouver l'emplacement.
 * @param AQ
 * Arbre quaternaire parent dans lequel on va placer le noeud.
 * @return 
 * La direction o� doit aller le point.
 *   0 : Sud-Ouest. 
 *   1 : Nord-Ouest. 
 *   2 : Sud-Est.
 *   3 : Nord-Est. 
 */
int determinerOrientation(int x, int y, ArbreQuat *AQ)
{
  if (x < AQ->xc)
    if (y < AQ->yc) 
      return 0;
    else 
      return 1;
  else 
    if (y < AQ->yc) 
      return 2;
    else 
      return 3;
}

/**
 * Ins�re un noeud dans l'arbre au bon endroit recursivement.
 * 
 * @param n
 * Noeud � ins�rer.
 * @param a
 * Arbre pr�sent.
 * @param parent
 * Arbre parent de l'arbre pr�sent.
 * @return
 * Arbre quaternaire final dans lequel le noeud � �t� ins�r�.
 */
ArbreQuat *insererNoeudArbre(Noeud *n, ArbreQuat *a, ArbreQuat *parent)
{
  double xc, yc;
  Noeud *noeudTmp;
 
  if (a) {
    if (!(a->noeud)) {
      printf("Oh yeah !\n");
      switch (determinerOrientation(n->x, n->y, a)) {
      case 0: a->so = insererNoeudArbre(n, a->so, a); break;
      case 1: a->no = insererNoeudArbre(n, a->no, a); break;
      case 2: a->se = insererNoeudArbre(n, a->se, a); break;
      case 3: a->ne = insererNoeudArbre(n, a->ne, a); break;	
      }
    }
    else {
      printf("Oh yeah ! 2\n");
      noeudTmp = a->noeud;
      a->noeud = NULL;
      printf("%p %p %p %p\n", a->so, a->no, a->se, a->ne);
      printf("On va caser %d et %d.\n", n->num, noeudTmp->num);
      switch (determinerOrientation(n->x, n->y, a)) {
      case 0: a->so = insererNoeudArbre(n, a->so, a); break;
      case 1: a->no = insererNoeudArbre(n, a->no, a); break;
      case 2: a->se = insererNoeudArbre(n, a->se, a); break;
      case 3: a->ne = insererNoeudArbre(n, a->ne, a); break;	
      }    
      
      switch (determinerOrientation(noeudTmp->x, noeudTmp->y, a)) {
      case 0: a->so = insererNoeudArbre(noeudTmp, a->so, a); break;
      case 1: a->no = insererNoeudArbre(noeudTmp, a->no, a); break;
      case 2: a->se = insererNoeudArbre(noeudTmp, a->se, a); break;
      case 3: a->ne = insererNoeudArbre(noeudTmp, a->ne, a); break;	
      }
    }
    return a;
  }

  else {
    printf("Tu rentres parfois la ?\n");
    printf("Le noeud : %d\n", n->num);
    switch (determinerOrientation(n->x, n->y, parent)) {
    case 0: 
      xc = (parent->xc) - (parent->coteX)/4; 
      yc = (parent->yc) - (parent->coteY)/4; 
      break;
    case 1: 
      xc = (parent->xc) - (parent->coteX)/4; 
      yc = (parent->yc) + (parent->coteY)/4; 
      break;
    case 2: 
      xc = (parent->xc) + (parent->coteX)/4; 
      yc = (parent->yc) - (parent->coteY)/4; 
      break;
    case 3: 
      xc = (parent->xc) + (parent->coteX)/4; 
      yc = (parent->yc) + (parent->coteY)/4; 
      break;
    }
    a = creerArbreQuat(xc, yc, parent->coteX/2, parent->coteY/2);
    a->noeud = n;
    printf("Et je sors du else !\n");
    return a;
  }
}

/**
 * Retourne le noeud correspondant � pt.
 * Si le noeud n'existe pas dans l'arbre, 
 * il est cr�� et ajout� dans l'arbre et 
 * le r�seau.
 *
 * @param pt
 * Point recherch�.
 * @param R
 * R�seau dans lequel le noeud peut �tre pr�sent.
 * @param aptr
 * Arbre dans lequel on recherche le noeud.
 * @param parent
 * Le p�re de aptr.
 * @return
 * Le noeud que l'on cherche.
 */
Noeud *chercherNoeudArbre(CellPoint* pt, Reseau* R, ArbreQuat** aptr, ArbreQuat* parent)
{
  Noeud *noeudAdd;

  if (!(*aptr)) {
    printf("Je ne suis pas la hein ?\n");
    sleep(10);
    noeudAdd = CreateNoeud((R->nbNoeuds)++, pt->x, pt->y, NULL);
    *aptr = insererNoeudArbre(noeudAdd, *aptr, parent);
    R->noeuds = AddCellNoeud(noeudAdd, R->noeuds);
    return noeudAdd;
  } else {
    printf("Plutot la ?\n");
    sleep (5);
    if ((*aptr)->noeud) {
      printf("Ce if ?\n");
      sleep (5);
      if (((*aptr)->noeud->x == pt->x) && ((*aptr)->noeud->y == pt->y)) {
	printf("A nouveau un if.\n");
	sleep (5);
	return (*aptr)->noeud;
      }
      else {
	printf("A nouveau un else.\n");
	sleep (5);
	noeudAdd = CreateNoeud((R->nbNoeuds)++, pt->x, pt->y, NULL);
	printf("Ca a marche ?\n%p\n", *aptr);
	sleep (5);
	*aptr = insererNoeudArbre(noeudAdd, *aptr, parent);
	printf("Ma seg fault.\n");
	sleep (5);
	R->noeuds = AddCellNoeud(noeudAdd, R->noeuds);
	return noeudAdd;
      }
    } else {
      printf("Ou le else.\n");
      switch (determinerOrientation(pt->x, pt->y, *aptr)) {
      case 0: return chercherNoeudArbre(pt, R, &((*aptr)->so), *aptr);
      case 1: return chercherNoeudArbre(pt, R, &((*aptr)->no), *aptr);
      case 2: return chercherNoeudArbre(pt, R, &((*aptr)->se), *aptr);
      case 3: return chercherNoeudArbre(pt, R, &((*aptr)->ne), *aptr);
      }
    }   
  }
}

/**
 * Recr�e un r�seau � partir de cha�nes.
 *
 * @param C
 * Liste des cha�nes du r�seau.
 * @return
 * R�seau recr�� � partir des cha�nes.
 */
Reseau *recreeReseauArbre (Chaines *C)
{
  double xmin, xmax, ymin, ymax;
  ArbreQuat *arbreQuaternaire;
  Reseau *reseauReturn = CreateReseau(0, C->gamma, NULL, NULL); 
  Noeud *temporaire1, *temporaire2, *noeudVoisins1, *noeudVoisins2;
  CellChaine *parcoursChaines = C->chaines;
  CellPoint *parcoursPoints;
  CellCommodite *commoditesDernier = NULL;

  chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
  printf("Affiche les dimensions de l'arbre : %lf, %lf, %lf, %lf.\n", xmin, ymin, xmax, ymax);
  arbreQuaternaire = creerArbreQuat((xmax - xmin)/2, 
				    (ymax - ymin)/2, 
				    (xmax - xmin), 
				    (ymax - ymin));
  while (parcoursChaines) {
    printf("Ici ?\n");
    parcoursPoints = parcoursChaines->points;
    temporaire1 = chercherNoeudArbre(parcoursPoints, 
				     reseauReturn, 
				     &arbreQuaternaire, 
				     NULL);
    noeudVoisins1 = temporaire1;
    printf("Le noeud est : %d\n", temporaire1->num);
    parcoursPoints = parcoursPoints->suivant;
    while (parcoursPoints && parcoursPoints->suivant) {
      printf("Et la ?\n");
      noeudVoisins2 = chercherNoeudArbre(parcoursPoints, 
					 reseauReturn, 
					 &arbreQuaternaire, 
					 NULL); 
      printf("On est la ?\n");
      noeudVoisins2->voisins = AddCellNoeud(noeudVoisins1, noeudVoisins2->voisins);
      noeudVoisins1->voisins = AddCellNoeud(noeudVoisins2, noeudVoisins1->voisins);
      noeudVoisins1 = noeudVoisins2;
      parcoursPoints = parcoursPoints->suivant;
    }
    temporaire2 = chercherNoeudArbre(parcoursPoints, 
				     reseauReturn, 
				     &arbreQuaternaire, 
				     NULL);
    commoditesDernier = AddCellCommodite(temporaire1, 
					 temporaire2, 
					 &(reseauReturn->commodites), 
					 commoditesDernier);
    parcoursChaines = parcoursChaines->suivant;
  }
  return reseauReturn;
}

