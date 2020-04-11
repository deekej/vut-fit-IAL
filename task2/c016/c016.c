
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Polo¾kami
**                      První implementace: Petr Pøikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masaøík, øíjen 2012
**                      Finished by: David Kaspar (xkaspa34)
**
** Vytvoøete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Polo¾kami = Hash table)
** s explicitnì øetìzenými synonymy. Tabulka je implementována polem
** lineárních seznamù synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pøed prvním pou¾itím
**  HTInsert ..... vlo¾ení prvku
**  HTSearch ..... zji¹tìní pøítomnosti prvku v tabulce
**  HTDelete ..... zru¹ení prvku
**  HTRead ....... pøeètení hodnoty prvku
**  HTClearAll ... zru¹ení obsahu celé tabulky (inicializace tabulky
**                 poté, co ji¾ byla pou¾ita)
**
** Definici typù naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelù na polo¾ky, je¾ obsahují slo¾ky
** klíèe 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal¹í synonymum 'ptrnext'. Pøi implementaci funkcí
** uva¾ujte maximální rozmìr pole HTSIZE.
**
** U v¹ech procedur vyu¾ívejte rozptylovou funkci hashCode.  Pov¹imnìte si
** zpùsobu pøedávání parametrù a zamyslete se nad tím, zda je mo¾né parametry
** pøedávat jiným zpùsobem (hodnotou/odkazem) a v pøípadì, ¾e jsou obì
** mo¾nosti funkènì pøípustné, jaké jsou výhody èi nevýhody toho èi onoho
** zpùsobu.
**
** V pøíkladech jsou pou¾ity polo¾ky, kde klíèem je øetìzec, ke kterému
** je pøidán obsah - reálné èíslo.
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;


/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíè a pøidìlit
** mu index v rozmezí 0..HTSize-1.  V ideálním pøípadì by mìlo dojít
** k rovnomìrnému rozptýlení tìchto klíèù po celé tabulce.  V rámci
** pokusù se mù¾ete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
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
** Inicializace tabulky s explicitnì zøetìzenými synonymy.  Tato procedura
** se volá pouze pøed prvním pou¾itím tabulky.
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


/* TRP s explicitnì zøetìzenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíèe key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vkládá do tabulky ptrht polo¾ku s klíèem key a s daty
** data.  Proto¾e jde o vyhledávací tabulku, nemù¾e být prvek se stejným
** klíèem ulo¾en v tabulce více ne¾ jedenkrát.  Pokud se vkládá prvek,
** jeho¾ klíè se ji¾ v tabulce nachází, aktualizujte jeho datovou èást.
**
** Vyu¾ijte døíve vytvoøenou funkci htSearch.  Pøi vkládání nového
** prvku do seznamu synonym pou¾ijte co nejefektivnìj¹í zpùsob,
** tedy proveïte.vlo¾ení prvku na zaèátek seznamu.
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
** TRP s explicitnì zøetìzenými synonymy.
** Tato funkce zji¹»uje hodnotu datové èásti polo¾ky zadané klíèem.
** Pokud je polo¾ka nalezena, vrací funkce ukazatel na polo¾ku
** Pokud polo¾ka nalezena nebyla, vrací se funkèní hodnota NULL
**
** Vyu¾ijte døíve vytvoøenou funkci HTSearch.
*/
tData* htRead(tHTable* ptrht, tKey key)
{{{
  tHTItem *p_item = htSearch(ptrht, key);

  return (p_item == NULL) ? NULL : &p_item->data;
}}}


/*
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vyjme polo¾ku s klíèem key z tabulky
** ptrht.  Uvolnìnou polo¾ku korektnì zru¹te.  Pokud polo¾ka s uvedeným
** klíèem neexistuje, dìlejte, jako kdyby se nic nestalo (tj. nedìlejte
** nic).
**
** V tomto pøípadì NEVYU®ÍVEJTE døíve vytvoøenou funkci HTSearch.
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


/* TRP s explicitnì zøetìzenými synonymy.
** Tato procedura zru¹í v¹echny polo¾ky tabulky, korektnì uvolní prostor,
** který tyto polo¾ky zabíraly, a uvede tabulku do poèáteèního stavu.
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
