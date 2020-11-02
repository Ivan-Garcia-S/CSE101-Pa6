/*********************************************************************************
 * * * * Ivan Garcia-Sanchez, igarci33
 * * * * 2020 Spring CSE101 PA6
 * * * * DictionaryTest.c
 * * * *********************************************************************************/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Dictionary.h"

int main(int argc, char* argv[]){
   
   int i, n=7;
   Dictionary A = newDictionary(0);
   Dictionary B = newDictionary(1);
   char* word[] = { "a", "b", "c", "d", "e", "f", "g" };
   VAL_TYPE x;
   VAL_TYPE y;
   
   for(i=0; i<n; i++){
      insert(A, word[i], NULL);
   }

   for(i=n-1; i>=0; i--){
      insert(B, word[i], NULL);
   }

   printf("forward A:\n");
   for(x=beginForward(A); currentKey(A) != KEY_UNDEF; x=next(A)){
      printf("key: "KEY_FORMAT", val: "VAL_FORMAT"\n", currentKey(A), currentVal(A));
   }
   printf("\n\n");

   printf("reverse B:\n");
   for(y=beginReverse(B); currentKey(B) != KEY_UNDEF; y=prev(B)){
      printf("key: "KEY_FORMAT"\n", currentKey(B));
   }
   printf("\n\n");


   printf("Dictionary A (%sunique keys):\n", (getUnique(A)?"":"non-"));
   printf("pre order: ");
   printDictionary(stdout, A, "pre");
   printf("in order: ");
   printDictionary(stdout, A, "in");
   printf("\n");

   printf("Dictionary B (%sunique keys):\n", (getUnique(B)?"":"non-"));
   printf("post order: ");
   printDictionary(stdout, B, "post");
   printf("\n");
   
   for(i=0; i<n; i++){
      insert(A, word[i], VAL_UNDEF);
   }
   printf("After inserting duplicates: \n");
   printDictionary(stdout,A,"pre");
   printDictionary(stdout,A,"none");
   for(i=0; i<n; i++){
      delete(A, word[i]);
   }
   printf("After deletes: \n");
   printf("pre order: ");
   printDictionary(stdout, A, "pre");
   printf("in order: ");
   printDictionary(stdout, A, "in");
   printf("post order: ");
   printDictionary(stdout,A,"post");

   
   freeDictionary(&A);
   freeDictionary(&B);

   return(0);
}
