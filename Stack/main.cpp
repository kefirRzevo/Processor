#include <stdio.h>
#include <stdlib.h>
#include "include/stack.h"


int main() 
{
   stack stk = {};
   stack stk1 = {};
   //stk.capacity = -2;
   StackCtor(&stk);
   StackDump(&stk);
   StackPush(&stk, 11);
   StackPush(&stk, 13);
   StackPush(&stk, 17);
   StackDump(&stk);
   StackPush(&stk, 23);
   StackPush(&stk, 29);
   StackPush(&stk, 31);
   StackDump(&stk);
   double a = 0;
   StackPop(&stk, &a);
   StackPop(&stk, &a);
   StackPop(&stk, &a);
   //stk.LeftCanary = 7;
   StackPop(&stk, &a);
   StackPop(&stk, &a);
   StackPop(&stk, &a);
   StackPop(&stk, &a);
   
   printf("%lg\n", a);
   StackDump(&stk);
   StackDump(&stk1);
   StackCtor(&stk1);
   StackDump(&stk1);
   return 0;
}