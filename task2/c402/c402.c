
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Pøikryl, prosinec 1994
**                                           Úpravy: Petr Pøikryl, listopad 1997
**                                                     Petr Pøikryl, kvìten 1998
**			  	                        Pøevod do jazyka C: Martin Tuèek, srpen 2005
**                                         Úpravy: Bohuslav Køena, listopad 2009
**                                         Upravy: Karel Masarik, rijen 2012
**                                         Finished by: David Kaspar, xkaspa34
**
** S vyu¾itím dynamického pøidìlování pamìti, implementujte NEREKURZIVNÌ
** následující operace nad binárním vyhledávacím stromem (pøedpona BT znamená
** Binary Tree a je u identifikátorù uvedena kvùli mo¾né kolizi s ostatními
** pøíklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vlo¾ení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní prùchod typu pre-order
**     BTInorder ....... nerekurzivní prùchod typu in-order
**     BTPostorder ..... nerekurzivní prùchod typu post-order
**     BTDisposeTree ... zru¹ v¹echny uzly stromu
**
** U v¹ech funkcí, které vyu¾ívají nìkterý z prùchodù stromem, implementujte
** pomocnou funkci pro nalezení nejlevìj¹ího uzlu v podstromu.
**
** Pøesné definice typù naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na nìj je typu tBTNodePtr. Jeden uzel obsahuje polo¾ku int Cont,
** která souèasnì slou¾í jako u¾iteèný obsah i jako vyhledávací klíè 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Pøíklad slou¾í zejména k procvièení nerekurzivních zápisù algoritmù
** nad stromy. Ne¾ zaènete tento pøíklad øe¹it, prostudujte si dùkladnì
** principy pøevodu rekurzivních algoritmù na nerekurzivní. Programování
** je pøedev¹ím in¾enýrská disciplína, kde opìtné objevování Ameriky nemá
** místo. Pokud se Vám zdá, ¾e by nìco ¹lo zapsat optimálnìji, promyslete
** si v¹echny detaily Va¹eho øe¹ení. Pov¹imnìte si typického umístìní akcí
** pro rùzné typy prùchodù. Zamyslete se nad modifikací øe¹ených algoritmù
** napøíklad pro výpoèet poètu uzlù stromu, poètu listù stromu, vý¹ky stromu
** nebo pro vytvoøení zrcadlového obrazu stromu (pouze popøehazování ukazatelù
** bez vytváøení nových uzlù a ru¹ení starých).
**
** Pøi prùchodech stromem pou¾ijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodu¹ení práce máte pøedem pøipraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
** Pozor! Je tøeba správnì rozli¹ovat, kdy pou¾ít dereferenèní operátor *
** (typicky pøi modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (napø. pøi vyhledávání). V tomto pøíkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, pou¾ijeme dereferenci.
**/

#include "errno.h"

#include "c402.h"

int solved;

void BTWorkOut(tBTNodePtr Ptr)
{{{
/*   ---------
** Pomocná funkce, kterou budete volat pøi prùchodech stromem pro zpracování
** uzlu urèeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr == NULL) 
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else 
    printf("Výpis hodnoty daného uzlu> %d\n", Ptr->Cont);
}}}
	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP(tStackP *S)
/*   ------
** Inicializace zásobníku.
**/
{{{
	S->top = 0;  
}}}

void SPushP(tStackP *S, tBTNodePtr ptr)
/*   ------
** Vlo¾í hodnotu na vrchol zásobníku.
**/
{{{
                 /* Pøi implementaci v poli mù¾e dojít k pøeteèení zásobníku. */
  if (S->top == MAXSTACK) 
    printf("Chyba: Do¹lo k pøeteèení zásobníku s ukazateli!\n");
  else {
		S->top++;  
		S->a[S->top] = ptr;
	}
}}}


tBTNodePtr STopPopP(tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
{{{
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top == 0)  {
		printf("Chyba: Do¹lo k podteèení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}
}}}


bool SEmptyP(tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{{{
  return (S->top == 0);
}}}


/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/
void SInitB(tStackB *S)
{{{
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;  
}}}


void SPushB(tStackB *S,bool val)
{{{
/*   ------
** Vlo¾í hodnotu na vrchol zásobníku.
**/
                 /* Pøi implementaci v poli mù¾e dojít k pøeteèení zásobníku. */
	if (S->top == MAXSTACK) 
		printf("Chyba: Do¹lo k pøeteèení zásobníku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top] = val;
	}	
}}}


bool STopPopB(tStackB *S)
{{{
/*   --------
** Odstraní prvek z vrcholu zásobníku a souèasnì vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem zpùsobí chybu. */
	if (S->top == 0) {
		printf("Chyba: Do¹lo k podteèení zásobníku pro boolean!\n");
		return (NULL);	
	}	
	else {  
		return (S->a[S->top--]); 
	}	
}}}


bool SEmptyB(tStackB *S)
{{{
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return (S->top==0);
}}}


/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit(tBTNodePtr *RootPtr)
{{{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze pøed prvním pou¾itím binárního
** stromu, proto¾e neuvolòuje uzly neprázdného stromu (a ani to dìlat nemù¾e,
** proto¾e pøed inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zru¹ení binárního stromu slou¾í procedura BTDisposeTree.
**	
** V¹imnìte si, ¾e zde se poprvé v hlavièce objevuje typ ukazatel na ukazatel,	
** proto je tøeba pøi práci s RootPtr pou¾ít dereferenèní operátor *.
**/
  
  /* Valid pointer is required. */
  if (RootPtr == NULL) {
    return;
  }

  *RootPtr = NULL;

  return;
}}}


void BTInsert(tBTNodePtr *RootPtr, int Content)
{{{
/*   --------
** Vlo¾í do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytváøený strom jako binární vyhledávací strom,
** kde uzly s hodnotou men¹í ne¾ má otec le¾í v levém podstromu a uzly vìt¹í
** le¾í vpravo. Pokud vkládaný uzel ji¾ existuje, neprovádí se nic (daná hodnota
** se ve stromu mù¾e vyskytnout nejvý¹e jednou). Pokud se vytváøí nový uzel,
** vzniká v¾dy jako list stromu. Funkci implementujte nerekurzivnì.
**/
  
  /* Valid pointer is needed. */
  if (RootPtr == NULL) {
    return;
  }

  /* Finding the correct place for insertion. */
  while (*RootPtr != NULL) {

    /* Already existing key. */
    if ((*RootPtr)->Cont == Content) {
      return;
    }
    /* Key has less sorting value. */
    else if (Content < (*RootPtr)->Cont) {
      RootPtr = &(*RootPtr)->LPtr;
    }
    /* Key has greater sorting value. */
    else {
      RootPtr = &(*RootPtr)->RPtr;
    }
  }
  
  *RootPtr = (tBTNodePtr) malloc(sizeof(struct tBTNode));

  /* Allocation failed, get the hell out of here! :D */
  if (*RootPtr == NULL) {
    errno = ENOMEM;
    perror("c402: error");
    exit(ENOMEM);
  }

  /* New node initialization. */
  (*RootPtr)->Cont = Content;
  (*RootPtr)->LPtr = NULL;
  (*RootPtr)->RPtr = NULL;

  return;
}}}


/*                                  PREORDER                                  */

void Leftmost_Preorder(tBTNodePtr ptr, tStackP *Stack)
{{{
/*   -----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Preorder nav¹tívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na nì is ulo¾íme do zásobníku.
**/
  
  /* Valid pointer is required. */
  if (Stack == NULL) {
    return;
  }
  
  while (ptr != NULL) {
    BTWorkOut(ptr);
    SPushP(Stack, ptr);
    ptr = ptr->LPtr;
  }

  return;
}}}


void BTPreorder(tBTNodePtr RootPtr)
{{{
/*   ----------
** Prùchod stromem typu preorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Preorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
  
  /* Valid pointer is required. */
  if (RootPtr == NULL) {
    return;
  }

  /* Helpful stack. */
  tStackP stack = {
    .top = 0,
  };

  Leftmost_Preorder(RootPtr, &stack);

  while (stack.top != 0) {
    RootPtr = STopPopP(&stack);

    if (RootPtr->RPtr != NULL) {
      Leftmost_Preorder(RootPtr->RPtr, &stack);
    }
  }

  return;
}}}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)
{{{
/*   ----------------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Inorder ukládáme ukazatele na v¹echny nav¹tívené uzly do
** zásobníku. 
**/

  /* Valid pointer is required. */
  if (Stack == NULL) {
    return;
  }
	
  while (ptr != NULL) {
    SPushP(Stack, ptr);
    ptr = ptr->LPtr;
  }

  return;
}}}

void BTInorder(tBTNodePtr RootPtr)
{{{
/*   ---------
** Prùchod stromem typu inorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Inorder a zásobníku ukazatelù. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
  
  /* Valid pointer is required. */
  if (RootPtr == NULL) {
    return;
  }

  /* Helpful stack. */
  tStackP stack = {
    .top = 0,
  };

	Leftmost_Inorder(RootPtr, &stack);

  while (stack.top != 0) {
    RootPtr = STopPopP(&stack);
    BTWorkOut(RootPtr);

    if (RootPtr->RPtr != NULL) {
      Leftmost_Inorder(RootPtr->RPtr, &stack);
    }
  }

  return;
}}}


/*                                 POSTORDER                                  */ 

void Leftmost_Postorder(tBTNodePtr ptr, tStackP *StackP, tStackB *StackB)
{{{
/*           --------
** Jde po levì vìtvi podstromu, dokud nenarazí na jeho nejlevìj¹í uzel.
**
** Pøi prùchodu Postorder ukládáme ukazatele na nav¹tívené uzly do zásobníku
** a souèasnì do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** nav¹tíven poprvé a ¾e se tedy je¹tì nemá zpracovávat. 
**/
  
  /* Valid pointers are required. */
  if (StackP == NULL || StackB == NULL) {
    return;
  }

  while (ptr != NULL) {
    SPushP(StackP, ptr);
    SPushB(StackB, true);
    ptr = ptr->LPtr;
  }
  
  return;
}}}

void BTPostorder(tBTNodePtr RootPtr)
{{{
/*           -----------
** Prùchod stromem typu postorder implementovaný nerekurzivnì s vyu¾itím funkce
** Leftmost_Postorder, zásobníku ukazatelù a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/
  
  /* Valid pointer is required. */
  if (RootPtr == NULL) {
    return;
  }
  
  /* Helpful stacks. */
  tStackP ptr_stack = {
    .top = 0,
  };

  tStackB bool_stack = {
    .top = 0,
  };
  
  /* Helpful variable to determine action in the tree passing. */
  bool go_right;
  
  Leftmost_Postorder(RootPtr, &ptr_stack, &bool_stack);

  while (ptr_stack.top != 0) {
    /* Restoring from stacks. */
    RootPtr = STopPopP(&ptr_stack);
    go_right = STopPopB(&bool_stack);
    
    /* Should we go right? And can we go right? */
    if (go_right == true && RootPtr->RPtr != NULL) {
      SPushP(&ptr_stack, RootPtr);
      SPushB(&bool_stack, false);
      Leftmost_Postorder(RootPtr->RPtr, &ptr_stack, &bool_stack);
    }
    /* We shouldn't/cant', just printing the content of node. */
    else {
      BTWorkOut(RootPtr);
    }
  }
  
  return;
}}}


void BTDisposeTree(tBTNodePtr *RootPtr)
{{{
/*   -------------
** Zru¹í v¹echny uzly stromu a korektnì uvolní jimi zabranou pamì».
**
** Funkci implementujte nerekurzivnì s vyu¾itím zásobníku ukazatelù.
**/

  /* Valid pointers are required. */
  if (RootPtr == NULL || *RootPtr == NULL) {
    return;
  }
  
  /* Helpful pointers. */
  tBTNodePtr p_node = NULL;
  tBTNodePtr *p_root = RootPtr;

  /* Helpful stack of pointers. */
  tStackP stack = {
    .top = 0,
  };


  /* Non-recursive deleting of binary tree. */
  while (*RootPtr != NULL) {

    /* Leaf reached, deletion. */
    if ((*RootPtr)->LPtr == NULL && (*RootPtr)->RPtr == NULL) {
      free((void *) *RootPtr);
      *RootPtr = NULL;
      
      /* Stack not empty, continue withdrawing. */
      if (stack.top != 0) {
        p_node = STopPopP(&stack);
        RootPtr = &p_node;
      }
    }
    /* Going left. */
    else if ((*RootPtr)->LPtr != NULL) {

      /*
       * Backing up current node in case the node has both children
       * or the stack has been depleted and we're not finished yet.
       */
      if ((*RootPtr)->RPtr != NULL || stack.top == 0) {
        SPushP(&stack, *RootPtr);
      }

      RootPtr = &(*RootPtr)->LPtr;
    }
    /* Going right. */
    else {
      /*
       * Backing up current node, 'cause we're not finished yet and the stack is
       * empty.
       */
      if (stack.top == 0) {
        SPushP(&stack, *RootPtr);
      }

      RootPtr = &(*RootPtr)->RPtr;
    }
  }
  
  /* Re-initializing of tree. */
  *p_root = NULL;

  return;
}}}

/* konec c402.c */

