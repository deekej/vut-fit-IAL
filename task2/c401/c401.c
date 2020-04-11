/* c401.c: **********************************************************}
{* T�ma: Rekurzivn� implementace operac� nad BVS
**                                         Vytvo�il: Petr P�ikryl, listopad 1994
**                                         �pravy: Andrea N�mcov�, prosinec 1995
**                                                      Petr P�ikryl, duben 1996
**                                                   Petr P�ikryl, listopad 1997
**                                  P�evod do jazyka C: Martin Tu�ek, ��jen 2005
**                                         �pravy: Bohuslav K�ena, listopad 2009
**                                         Upravy: Karel Masarik, rijen 2012
**                                    Finished by: David Kaspar (xkaspa34), 2012
**
** Implementujte rekurzivn�m zp�sobem operace nad bin�rn�m vyhled�vac�m
** stromem (BVS; v angli�tin� BST - Binary Search Tree).
**
** Kl��em uzlu stromu je jeden znak (obecn� j�m m��e b�t cokoliv, podle
** �eho se vyhled�v�). U�ite�n�m (vyhled�van�m) obsahem je zde integer.
** Uzly s men��m kl��em le�� vlevo, uzly s v�t��m kl��em le�� ve stromu
** vpravo. Vyu�ijte dynamick�ho p�id�lov�n� pam�ti.
** Rekurzivn�m zp�sobem implementujte n�sleduj�c� funkce:
**
**   BSTInit ...... inicializace vyhled�vac�ho stromu
**   BSTSearch .... vyhled�v�n� hodnoty uzlu zadan�ho kl��em
**   BSTInsert .... vkl�d�n� nov� hodnoty
**   BSTDelete .... zru�en� uzlu se zadan�m kl��em
**   BSTDispose ... zru�en� cel�ho stromu
**
** ADT BVS je reprezentov�n ko�enov�m ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje kl�� (typu char), podle
** kter�ho se ve stromu vyhled�v�, vlastn� obsah uzlu (pro jednoduchost
** typu int) a ukazatel na lev� a prav� podstrom (LPtr a RPtr). P�esnou definici typ� 
** naleznete v souboru c401.h.
**
** Pozor! Je t�eba spr�vn� rozli�ovat, kdy pou��t dereferen�n� oper�tor *
** (typicky p�i modifikaci) a kdy budeme pracovat pouze se samotn�m ukazatelem 
** (nap�. p�i vyhled�v�n�). V tomto p��kladu v�m napov� prototypy funkc�.
** Pokud pracujeme s ukazatelem na ukazatel, pou�ijeme dereferenci.
**/

#include <errno.h>

#include "c401.h"


int solved;


void BSTInit(tBSTNodePtr *RootPtr)
{{{
/*   -------
** Funkce provede po��te�n� inicializaci stromu p�ed jeho prvn�m pou�it�m.
**
** Ov��it, zda byl ji� strom p�edan� p�es RootPtr inicializov�n, nelze,
** proto�e p�ed prvn� inicializac� m� ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Program�tor vyu��vaj�c� ADT BVS tedy mus� zajistit, aby inicializace
** byla vol�na pouze jednou, a to p�ed vlastn� prac� s BVS. Proveden�
** inicializace nad nepr�zdn�m stromem by toti� mohlo v�st ke ztr�t� p��stupu
** k dynamicky alokovan� pam�ti (tzv. "memory leak").
**	
** V�imn�te si, �e se v hlavi�ce objevuje typ ukazatel na ukazatel.	
** Proto je t�eba p�i p�i�azen� p�es RootPtr pou��t dereferen�n� oper�tor *.
** Ten bude pou�it i ve funkc�ch BSTDelete, BSTInsert a BSTDispose.
**/

  if (RootPtr == NULL) {
    return;
  }

  *RootPtr = NULL;

  return;
}}}


int BSTSearch(tBSTNodePtr RootPtr, char K, int *Content)
{{{
/*  ---------
** Funkce vyhled� uzel v BVS s kl��em K.
**
** Pokud je takov� nalezen, vrac� funkce hodnotu TRUE a v prom�nn� Content se
** vrac� obsah p��slu�n�ho uzlu.�Pokud p��slu�n� uzel nen� nalezen, vrac� funkce
** hodnotu FALSE a obsah prom�nn� Content nen� definov�n (nic do n� proto
** nep�i�azujte).
**
** P�i vyhled�v�n� v bin�rn�m stromu bychom typicky pou�ili cyklus ukon�en�
** testem dosa�en� listu nebo nalezen� uzlu s kl��em K. V tomto p��pad� ale
** probl�m �e�te rekurzivn�m vol�n� t�to funkce, p�i�em� nedeklarujte ��dnou
** pomocnou funkci.
**/

  if (RootPtr == NULL) {
    return FALSE;
  }

  if (K == RootPtr->Key) {
    *Content = RootPtr->BSTNodeCont;
    return TRUE;
  }
  else if (K < RootPtr->Key && RootPtr->LPtr != NULL) {
    return BSTSearch(RootPtr->LPtr, K, Content);
  }
  else if (K > RootPtr->Key && RootPtr->RPtr != NULL) {
    return BSTSearch(RootPtr->RPtr, K, Content);
  }

  return FALSE;
}}}


void BSTInsert(tBSTNodePtr* RootPtr, char K, int Content)
{{{
/*   ---------
** Vlo�� do stromu RootPtr hodnotu Content s kl��em K.
**
** Pokud ji� uzel se zadan�m kl��em ve stromu existuje, bude obsah uzlu
** s kl��em K nahrazen novou hodnotou. Pokud bude do stromu vlo�en nov�
** uzel, bude vlo�en v�dy jako list stromu.
**
** Funkci implementujte rekurzivn�. Nedeklarujte ��dnou pomocnou funkci.
**
** Rekurzivn� implementace je m�n� efektivn�, proto�e se p�i ka�d�m
** rekurzivn�m zano�en� ukl�d� na z�sobn�k obsah uzlu (zde integer).
** Nerekurzivn� varianta by v tomto p��pad� byla efektivn�j�� jak z hlediska
** rychlosti, tak z hlediska pam�ov�ch n�rok�. Zde jde ale o �koln�
** p��klad, na kter�m si chceme uk�zat eleganci rekurzivn�ho z�pisu.
**/
  
  /* Valid pointer required. */
  if (RootPtr == NULL) {
    return;
  }


  /* Skipping inserting in case of empty tree. */
  if (*RootPtr != NULL) {

    /* Already existing key. */
    if (K == (*RootPtr)->Key) {
      (*RootPtr)->BSTNodeCont = Content;
      return;
    }
    /* Given key has less sorting value. */
    else if (K < (*RootPtr)->Key && (*RootPtr)->LPtr != NULL) {
      BSTInsert(&(*RootPtr)->LPtr, K, Content);
      return;
    }
    /* Given key has greater sorting value. */
    else if (K > (*RootPtr)->Key && (*RootPtr)->RPtr != NULL) {
      BSTInsert(&(*RootPtr)->RPtr, K, Content);
      return;
    }

  }

  
  /* Leaf of tree reached. New leaf must be created. */
  tBSTNodePtr p_node = (tBSTNodePtr) malloc(sizeof(struct tBSTNode));
  
  /* Allocation failed, nothing to do -> get the hell out!!! xD */
  if (p_node == NULL) {
    errno = ENOMEM;
    perror("c401: error");
    exit(ENOMEM);
  }

  
  /* Initializing leaf with new content. */
  p_node->Key = K;
  p_node->BSTNodeCont = Content;
  p_node->LPtr = NULL;
  p_node->RPtr = NULL;
  
  /* Assigning leaf to proper pointer of the parent. */
  if (*RootPtr == NULL) {
    *RootPtr = p_node;
  }
  else if (K < (*RootPtr)->Key) {
    (*RootPtr)->LPtr = p_node;
  }
  else {
    (*RootPtr)->RPtr = p_node;
  }
  
  return;
}}}


void ReplaceByRightmost(tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr)
{{{
/*   ------------------
** Pomocn� funkce pro vyhled�n�, p�esun a uvoln�n� nejprav�j��ho uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kter�ho bude p�esunuta hodnota
** nejprav�j��ho uzlu v podstromu, kter� je ur�en ukazatelem RootPtr.
** P�edpokl�d� se, �e hodnota ukazatele RootPtr nebude NULL (zajist�te to
** testov�n�m p�ed vol�n� t�to funkce). Tuto funkci implementujte rekurzivn�. 
**
** Tato pomocn� funkce bude pou�ita d�le. Ne� ji za�nete implementovat,
** p�e�t�te si koment�� k funkci BSTDelete(). 
**/
  
  /* Valid pointers are required. */
  if (RootPtr == NULL || *RootPtr == NULL || PtrReplaced == NULL) {
    return;
  }
  

  /* Helpful pointer of parent of node. */
  tBSTNodePtr p_parent = PtrReplaced;

  /* Assigning left pointer as a starting node. */
  *RootPtr = (*RootPtr)->LPtr;

  
  /* Finding rightmost node and its parent. */
  while ((*RootPtr)->RPtr != NULL) {
    p_parent = *RootPtr;
    *RootPtr = (*RootPtr)->RPtr;
  }

  /*
   * Setting pointers of node (which should be "moved") to properly point to new
   * location. The new pointing is determined by the position of the node in the
   * tree.
   */
  if (p_parent == PtrReplaced) {                  /* No rightmost node. */

    /* Assigning new pointers. */
    if (PtrReplaced->LPtr != *RootPtr) {
      (*RootPtr)->LPtr = PtrReplaced->LPtr;
    }

    if (PtrReplaced->RPtr != *RootPtr) {
      (*RootPtr)->RPtr = PtrReplaced->RPtr;
    }
  }
  else {

    /* Parent inherits in case the rightmost had any children. */
    if ((*RootPtr)->LPtr != NULL) {
      p_parent->RPtr = (*RootPtr)->LPtr;
    }
    else {
      p_parent->RPtr = NULL;
    }
    
    /* Assigning new pointers. */
    (*RootPtr)->LPtr = PtrReplaced->LPtr;
    (*RootPtr)->RPtr = PtrReplaced->RPtr;
  }

  free((void *) PtrReplaced);     /* Disposing of node. */
  PtrReplaced = *RootPtr;         /* Moving node. */

  return;
}}}


void BSTDelete(tBSTNodePtr *RootPtr, char K)
{{{
/*   ---------
** Zru�� uzel stromu, kter� obsahuje kl�� K.
**
** Pokud uzel se zadan�m kl��em neexistuje, ned�l� funkce nic. 
** Pokud m� ru�en� uzel jen jeden podstrom, pak jej zd�d� otec ru�en�ho uzlu.
** Pokud m� ru�en� uzel oba podstromy, pak je ru�en� uzel nahrazen nejprav�j��m
** uzlem lev�ho podstromu. Pozor! Nejprav�j�� uzel nemus� b�t listem.
**
** Tuto funkci implementujte rekurzivn� s vyu�it�m d��ve deklarovan�
** pomocn� funkce ReplaceByRightmost.
**/

  /* Valid pointers are required. */
  if (RootPtr == NULL || *RootPtr == NULL) {
    return;
  }

  /* Key found. */
  if (K == (*RootPtr)->Key) {
  
    /* Leaf of the tree. */
    if ((*RootPtr)->LPtr == NULL && (*RootPtr)->RPtr == NULL) {
      free((void *) *RootPtr);
      *RootPtr = NULL;
    }
    /* Node contains both children. */
    else if ((*RootPtr)->LPtr != NULL && (*RootPtr)->RPtr != NULL) {
      ReplaceByRightmost(*RootPtr, &(*RootPtr));
    }
    /* Node contains only one child. */
    else {
      tBSTNodePtr p_node = *RootPtr;      /* Helpful pointer. */
      
      /* Parent is inheriting grandchildren. */
      if ((*RootPtr)->LPtr != NULL) {
        *RootPtr = (*RootPtr)->LPtr;
      }
      else {
        *RootPtr = (*RootPtr)->RPtr;
      }
      
      /* Freeing child. */
      free((void *) p_node);
    }

  }
  /* Given key has less sorting value. */
  else if (K < (*RootPtr)->Key && (*RootPtr)->LPtr != NULL) {
    BSTDelete(&(*RootPtr)->LPtr, K);
  }
  /* Given key has greater sorting value. */
  else if (K > (*RootPtr)->Key && (*RootPtr)->RPtr != NULL) {
    BSTDelete(&(*RootPtr)->RPtr, K);
  }
  
  return;
}}}


void BSTDispose(tBSTNodePtr *RootPtr)
{{{
/*   ----------
** Zru�� cel� bin�rn� vyhled�vac� strom a korektn� uvoln� pam�.
**
** Po zru�en� se bude BVS nach�zet ve stejn�m stavu, jako se nach�zel po
** inicializaci. Tuto funkci implementujte rekurzivn� bez deklarov�n� pomocn�
** funkce.
**/
  
  /* Valid pointer are required. */
  if (RootPtr == NULL || *RootPtr == NULL) {
    return;
  }

  
  /* In-order recursive calling. */
  if ((*RootPtr)->LPtr != NULL) {
    BSTDispose(&(*RootPtr)->LPtr);
  }

  if ((*RootPtr)->RPtr != NULL) {
    BSTDispose(&(*RootPtr)->RPtr);
  }

  
  /* Leaf reached, freiing. */
  free((void *) (*RootPtr));
  *RootPtr = NULL;

  return;
}}}

/* konec c401.c */

