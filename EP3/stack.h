/*
 * stack.h 
 * INTERFACE: funcoes para manipular a pilha 
 */

#include "item.h"

void stackInit(int);
 int stackEmpty();
void stackPush(Item);
Item stackPop();
Item stackTop();
void stackFree();
void stackDump(); 
