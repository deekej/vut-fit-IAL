
/* c016.c: **********************************************************}
{* T�ma:  Tabulka s Rozpt�len�mi Polo�kami
**                      Prvn� implementace: Petr P�ikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      �pravy: Karel Masa��k, ��jen 2012
**                      Finished by: David Kaspar (xkaspa34)
**
** Vytvo�ete abstraktn� datov� typ
** TRP (Tabulka s Rozpt�len�mi Polo�kami = Hash table)
** s explicitn� �et�zen�mi synonymy. Tabulka je implementov�na polem
** line�rn�ch seznam� synonym.
**
** Implementujte n�sleduj�c� procedury a funkce.
**
**  HTInit ....... inicializuje tabulku p�ed prvn�m pou�it�m
**  HTInsert ..... vlo�en� prvku
**  HTSearch ..... zji�t�n� p��tomnosti prvku v tabulce
**  HTDelete ..... zru�en� prvku
**  HTRead ....... p�e�ten� hodnoty prvku
**  HTClearAll ... zru�en� obsahu cel� tabulky (inicializace tabulky
**                 pot�, co ji� byla pou�ita)
**
** Definici typ� naleznete v souboru c016.h.
**
** Tabulka je reprezentov�na datovou strukturou typu tHTable,
** kter� se skl�d� z ukazatel� na polo�ky, je� obsahuj� slo�ky
** kl��e 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal�� synonymum 'ptrnext'. P�i implementaci funkc�
** uva�ujte maxim�ln� rozm�r pole HTSIZE.
**
** U v�ech procedur vyu��vejte rozptylovou funkci hashCode.  Pov�imn�te si
** zp�sobu p�ed�v�n� parametr� a zamyslete se nad t�m, zda je mo�n� parametry
** p�ed�vat jin�m zp�sobem (hodnotou/odkazem) a v p��pad�, �e jsou ob�
** mo�nosti funk�n� p��pustn�, jak� jsou v�hody �i nev�hody toho �i onoho
** zp�sobu.
**
** V p��kladech jsou pou�ity polo�ky, kde kl��em je �et�zec, ke kter�mu
** je p�id�n obsah - re�ln� ��slo.
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;


/*          -------
** Rozptylovac� funkce - jej�m �kolem je zpracovat zadan� kl�� a p�id�lit
** mu index v rozmez� 0..HTSize-1.  V ide�ln�m p��pad� by m�lo doj�t
** k rovnom�rn�mu rozpt�len� t�chto kl��� po cel� tabulce.  V r�mci
** pokus� se m��ete zamyslet nad kvalitou t�to funkce.  (Funkce nebyla
** volena s ohledem na maxim�ln� kvalitu v�sledku). }
*/
int hashCode(tKey key)
{{{
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}}}


/*
** Inicializace tabulky s explicitn� z�et�zen�mi synonymy.  Tato procedura
** se vol� pouze p�ed prvn�m pou�it�m tabulky.
*/
void htInit(tHTable* ptrht)
{{{
  /* Is there anything to initialize? */
  if (ptrht == NULL) {
    return;
  }

  /* Let's do it the *HARDCORE* way... xD */
  for (int i = 0; i < HTSIZE; i++) {
    (*ptrht)[i] = NULL;
  }
  
  return;
}}}


/* TRP s explicitn� z�et�zen�mi synonymy.
** Vyhled�n� prvku v TRP ptrht podle zadan�ho kl��e key.  Pokud je
** dan� prvek nalezen, vrac� se ukazatel na dan� prvek. Pokud prvek nalezen nen�, 
** vrac� se hodnota NULL.
**
*/
tHTItem* htSearch(tHTable* ptrht, tKey key)
{{{
  /* Valid pointers are required. */
  if (ptrht == NULL || key == NULL) {
    return NULL;
  }
  
  /* Helpful pointer. */
  tHTItem *p_item = (*ptrht)[hashCode(key)];

  /* Finding item with the given key. */
  while (p_item != NULL && strcmp(key, p_item->key) != 0x0) {
    p_item = p_item->ptrnext;
  }
  
  return p_item;
}}}


/* 
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vkl�d� do tabulky ptrht polo�ku s kl��em key a s daty
** data.  Proto�e jde o vyhled�vac� tabulku, nem��e b�t prvek se stejn�m
** kl��em ulo�en v tabulce v�ce ne� jedenkr�t.  Pokud se vkl�d� prvek,
** jeho� kl�� se ji� v tabulce nach�z�, aktualizujte jeho datovou ��st.
**
** Vyu�ijte d��ve vytvo�enou funkci htSearch.  P�i vkl�d�n� nov�ho
** prvku do seznamu synonym pou�ijte co nejefektivn�j�� zp�sob,
** tedy prove�te.vlo�en� prvku na za��tek seznamu.
**/
void htInsert(tHTable* ptrht, tKey key, tData data)
{{{
  /* Valid pointers are required. */
  if (ptrht == NULL || key == NULL) {
    return;
  }

  
  /* Helpful pointer. Initialized with address of item containing key. */
  tHTItem *p_item = htSearch(ptrht, key);
  
  /* Item with key found. Just overwriting its data. */
  if (p_item != NULL) {
    p_item->data = data;
    return;
  }
  

  /* No item with key found, creating new one. */
  p_item = (tHTItem *) malloc(sizeof(tHTItem));
  
  /* Memory allocation failed. Nothing to do --> enough with this bullsh*t. */
  if (p_item == NULL) {
    errno = ENOMEM;
    perror("c016: error");
    exit(ENOMEM);
  }


  /* We need hash index. AGAIN! */
  int hash_index = hashCode(key);

  /* Adding item to the start of the linked list of given index. */
  p_item->ptrnext = (*ptrht)[hash_index];
  (*ptrht)[hash_index] = p_item;

  /* Storing values. */
  p_item->key = key;
  p_item->data = data;

  return;
}}}


/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato funkce zji��uje hodnotu datov� ��sti polo�ky zadan� kl��em.
** Pokud je polo�ka nalezena, vrac� funkce ukazatel na polo�ku
** Pokud polo�ka nalezena nebyla, vrac� se funk�n� hodnota NULL
**
** Vyu�ijte d��ve vytvo�enou funkci HTSearch.
*/
tData* htRead(tHTable* ptrht, tKey key)
{{{
  tHTItem *p_item = htSearch(ptrht, key);

  return (p_item == NULL) ? NULL : &p_item->data;
}}}


/*
** TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura vyjme polo�ku s kl��em key z tabulky
** ptrht.  Uvoln�nou polo�ku korektn� zru�te.  Pokud polo�ka s uveden�m
** kl��em neexistuje, d�lejte, jako kdyby se nic nestalo (tj. ned�lejte
** nic).
**
** V tomto p��pad� NEVYU��VEJTE d��ve vytvo�enou funkci HTSearch.
*/

void htDelete(tHTable* ptrht, tKey key)
{{{
  /* Valid pointers are required. */
  if (ptrht == NULL || key == NULL) {
    return;
  }

  /* Hash index is needed more than once. */
  int hash_index = hashCode(key);

  /* Helpful pointers. */
  tHTItem *p_item, *p_item_prev;

  /* Assigning to start of the linked list. */
  p_item = (*ptrht)[hash_index];
  p_item_prev = (*ptrht)[hash_index];


  /* Trying to find the item with given key. */
  while (p_item != NULL && strcmp(key, p_item->key) != 0) {
    p_item_prev = p_item;
    p_item = p_item->ptrnext;
  }


  /* Nothing found, nothind to do. */
  if (p_item == NULL) {
    return;
  }
  /* Removing first item. */
  else if (p_item == p_item_prev) {
    (*ptrht)[hash_index] = p_item->ptrnext;
  }
  /* Removing item from linked list. */
  else {
    p_item_prev->ptrnext = p_item->ptrnext;
  }


  /* Freeing the item. */
  free((void *) p_item);

  return;
}}}


/* TRP s explicitn� z�et�zen�mi synonymy.
** Tato procedura zru�� v�echny polo�ky tabulky, korektn� uvoln� prostor,
** kter� tyto polo�ky zab�raly, a uvede tabulku do po��te�n�ho stavu.
*/
void htClearAll(tHTable* ptrht)
{{{
  if (ptrht == NULL) {
    return;
  }

  tHTItem *p_item;

  for (int i = 0; i < HTSIZE; i++) {
    while ((*ptrht)[i] != NULL) {
      p_item = (*ptrht)[i];
      (*ptrht)[i] = (*ptrht)[i]->ptrnext;
      free((void *) p_item);
    }
  }
  
  return;
}}}
