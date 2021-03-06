
/* c201.c *********************************************************************}
{* T�ma: Jednosm�rn� line�rn� seznam
**
**                     N�vrh a referen�n� implementace: Petr P�ikryl, ��jen 1994
**                                          �pravy: Andrea N�mcov� listopad 1996
**                                                   Petr P�ikryl, listopad 1997
**                                P�epracovan� zad�n�: Petr P�ikryl, b�ezen 1998
**                                  P�epis do jazyka C: Martin Tu�ek, ��jen 2004
**	                                       �pravy: Bohuslav K�ena, z��� 2012
**
** Implementujte abstraktn� datov� typ jednosm�rn� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je cel� ��slo typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou typu tList.
** Definici konstant a typ� naleznete v hlavi�kov�m souboru c201.h.
** 
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu s v��e
** uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ tList:
**
**      InitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DisposeList ... zru�en� v�ech prvk� seznamu,
**      InsertFirst ... vlo�en� prvku na za��tek seznamu,
**      First ......... nastaven� aktivity na prvn� prvek,
**      CopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DeleteFirst ... zru�� prvn� prvek seznamu,
**      PostDelete .... ru�� prvek za aktivn�m prvkem,
**      PostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      Copy .......... vrac� hodnotu aktivn�ho prvku,
**      Actualize ..... p�ep��e obsah aktivn�ho prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal�� prvek seznamu,
**      Active ........ zji��uje aktivitu seznamu.
**
** P�i implementaci funkc� nevolejte ��dnou z funkc� implementovan�ch v r�mci
** tohoto p��kladu, nen�-li u funkce explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error()
{{{
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* This program has performed an illegal operation.\n");
    errflg = TRUE;                      /* glob�ln� prom�nn� -- p��znak chyby */
}}}


void InitList (tList *L)
{{{
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/
  
  L->First = NULL;
  L->Act = NULL;

  return;
}}}


void DisposeList (tList *L)
{{{
/*
** Zru�� v�echny prvky seznamu L a uvede seznam L do stavu, v jak�m se nach�zel
** po inicializaci. V�echny prvky seznamu L budou korektn� uvoln�ny vol�n�m
** operace free.
***/

  /* L->Act is used as auxiliary pointer. */
  while (L->First != NULL) {
    L->Act = L->First;
    L->First = L->First->ptr;
    free((void *) L->Act);
  }
  
  L->Act = NULL;

  return;
}}}


void InsertFirst (tList *L, int val)
{{{
/*
** Vlo�� prvek s hodnotou val na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci Error().
**/

  /* Valid pointer? */
  if (L == NULL) {
    return;
  }

  tElemPtr p_item_new;          /* Auxiliary pointer. */

  p_item_new = (tElemPtr) malloc(sizeof(struct tElem));

  /* Successful allocation? */
  if (p_item_new == NULL) {
    Error();
    return;
  }

  p_item_new->data = val;
  p_item_new->ptr = L->First;

  L->First = p_item_new;

  return;
}}}


void First (tList *L)
{{{
/*
** Nastav� aktivitu seznamu L na jeho prvn� prvek.
** Funkci implementujte jako jedin� p��kaz, ani� byste testovali,
** zda je seznam L pr�zdn�.
**/

  /* Valid pointer? */
  if (L == NULL) {
    return;
  }
  
  L->Act = L->First;

  return;
}}}


void CopyFirst (tList *L, int *val)
{{{
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci Error().
**/

  if (L == NULL) {
    return;
  }
  else if (L->First == NULL) {
    Error();
    return;
  }

  *val = L->First->data;

  return;
}}}


void DeleteFirst (tList *L)
{{{
/*
** Ru�� prvn� prvek seznamu L. Pokud byl ru�en� prvek aktivn�, aktivita seznamu
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je!
**/
  
  if (L == NULL || L->First == NULL) {
    return;
  }
  else if (L->Act == L->First) {
    L->Act = NULL;
  }
  
	tElemPtr p_item_del;          /* Auxiliary pointer. */

  p_item_del = L->First;
  L->First = L->First->ptr;
  free((void *) p_item_del);

  return;
}}}


void PostDelete (tList *L)
{{{
/* 
** Ru�� prvek seznamu L za aktivn�m prvkem. Pokud nen� seznam L aktivn�
** nebo pokud je aktivn� posledn� prvek seznamu L, nic se ned�je.
**/

  if (L == NULL || L->Act == NULL || L->Act->ptr == NULL) {
    return;
  }

  tElemPtr p_item_next;         /* Auxiliary pointer. */

  p_item_next = L->Act->ptr;
  L->Act->ptr = p_item_next->ptr;
  free((void *) p_item_next);

  return;
}}}


void PostInsert (tList *L, int val)
{{{
/*
** Vlo�� prvek s hodnotou val za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je!
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** zavol� funkci Error().
**/
  
  if (L == NULL || L->Act == NULL) {
    return;
  }

  tElemPtr p_item_new;          /* Auxiliary pointer. */

  p_item_new = (tElemPtr) malloc(sizeof(struct tElem));

  if (p_item_new == NULL) {
    Error();
    return;
  }

  p_item_new->data = val;
  p_item_new->ptr = L->Act->ptr;
  L->Act->ptr = p_item_new;

  return;
}}}


void Copy (tList *L, int *val)
{{{
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam nen� aktivn�, zavol� funkci Error().
**/

  if (L == NULL) {
    return;
  }
  else if (L->Act == NULL) {
    Error();
    return;
  }

  *val = L->Act->data;

  return;
}}}


void Actualize (tList *L, int val)
{{{
/*
** P�ep��e data aktivn�ho prvku seznamu L hodnotou val.
** Pokud seznam L nen� aktivn�, ned�l� nic!
**/

  if (L == NULL || L->Act == NULL) {
    return;
  }
	
  L->Act->data = val;

  return;	
}}}


void Succ (tList *L)
{{{
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** V�imn�te si, �e touto operac� se m��e aktivn� seznam st�t neaktivn�m.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/

  if (L == NULL || L->Act == NULL) {
    return;
  }
	
  L->Act = L->Act->ptr;

  return;
}}}

int Active (tList *L)
{{{
/*
** Je-li seznam L aktivn�, vrac� TRUE. V opa�n�m p��pad� vrac� FALSE.
** Tuto funkci implementujte jako jedin� p��kaz return. 
**/
  return (L->Act != NULL) ? TRUE : FALSE; 
}}}

/* Konec c201.c */
