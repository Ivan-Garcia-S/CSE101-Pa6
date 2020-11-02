/*********************************************************************************
 * * * Ivan Garcia-Sanchez, igarci33
 * * * 2020 Spring CSE101 PA6
 * * * Dictionary.c
 * * *********************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include <stdbool.h>
#include "Dictionary.h"

	
typedef struct NodeObj{
   KEY_TYPE key;
   VAL_TYPE value;
   char* color;
   struct NodeObj* left;
   struct NodeObj* right;
   struct NodeObj* parent;
} NodeObj;

typedef NodeObj* Node;

Node newNode(KEY_TYPE k, VAL_TYPE v){
   Node N = malloc(sizeof(NodeObj));
   assert(N != NULL);
   N->key = k;
   N->value = v;
   N->parent = NULL;
   N->left = NULL;
   N->right = NULL;
   return N;	
}

void freeNode(Node* pN){
   if( pN!=NULL && *pN!=NULL ){
      free(*pN);
      *pN = NULL;
   }
}

typedef struct DictionaryObj{
   Node root;
   Node current;
   Node nil;
   int unique;
   int size;
} DictionaryObj;
	
typedef DictionaryObj* Dictionary;

Dictionary newDictionary(int unique){
   Dictionary D = malloc(sizeof(DictionaryObj));
   D->nil = newNode(KEY_UNDEF,VAL_UNDEF);
   D->root = D->nil;
   D->current = D->nil;
   D->nil->color = "b";
   D->unique = unique;
   D->size = 0;
   return D;
}

void freeDictionary(Dictionary* pD){
   if( pD!=NULL && *pD!=NULL ){
      if( size(*pD) == 0 ) 
      //if(pD!=NULL && *pD!=NULL){
         makeEmpty(*pD);
      //}
      freeNode(&(*pD)->nil);
      (*pD)->nil = NULL;
      free(*pD);
      *pD = NULL;
   }
}	
	
int isEmpty(Dictionary D) {
   if(size(D) == 0){
      return 1;
   }
   return 0;
}
	

int size(Dictionary D) {
   return D->size;
}

int getUnique(Dictionary D){
   return D->unique;
}

Node TreeMinimum(Dictionary D, Node x){
    if(x != D->nil){
       while(x->left != D->nil){
          x = x->left;
       }
    }
    return x;
}

Node TreeMaximum(Dictionary D, Node x){
   if(x != D->nil){
      while(x->right != D->nil){
         x = x->right;
      }
   }
   return x;
}

Node TreePredecessor(Dictionary D, Node x){
   if(x == D->nil){
      return D->nil;
   }
   
   else if(x->left != D->nil){
      x = TreeMaximum(D,x->left);
      return x; 
   }
   
   else{
      Node y = x->parent;
      while (x == y->left){
         x = y;
         y = y->parent;
      }
      return y;
   }
}

Node TreeSuccessor(Dictionary D,Node x){
   if(x == D->nil){
      return D->nil;
   }
   else if(x->right != D->nil){
      x = TreeMinimum(D,x->right);
      return x;
   }
   else{
      Node y = x->parent;
      while(y != D->nil && x == y->right){
         x = y;
         y = y->parent;
      }
      return y;
   }
}

Node TreeSearch(Dictionary D, Node x, KEY_TYPE k){
   if(x == D->nil || KEY_CMP(x->key,k) == 0){
      return x;
   }
   else if(KEY_CMP(k, x->key) < 0){
      return TreeSearch(D,x->left,k);
   }
   else{ 
      return TreeSearch(D,x->right,k);
   }
}

void LeftRotate(Dictionary D, Node x){
   Node y = x->right;
   x->right = y->left;
   if(y->left != D->nil){
      y->left->parent = x;
   }

   y->parent = x->parent;
   if(x->parent == D->nil){
      D->root = y;
   }
   else if(x == x->parent->left){
      x->parent->left = y;
   }
   else{
      x->parent->right = y;
   }
   y->left = x;
   x->parent = y;
}

void RightRotate(Dictionary D, Node x){
   Node y = x->left;
   x->left = y->right;
   if(y->right != D->nil){
      y->right->parent = x;
   }
   
   y->parent = x->parent;
   if(x->parent == D->nil){
      D->root = y;
   }
   else if(x == x->parent->right){
      x->parent->right = y;
   }
   else{
      x->parent->left = y;
   }
   
   y->right = x;
   x->parent = y;
}

void InsertFixUp(Dictionary D,Node z){
   while(strcmp(z->parent->color,"r") == 0){
      Node y = NULL;
      if(z->parent == z->parent->parent->left){
         y = z->parent->parent->right;
         if(strcmp(y->color,"r") == 0){
            z->parent->color = "b";         
            y->color = "b";            
            z->parent->parent->color = "r";
            z = z->parent->parent;
         }                 
         else{
            if(z == z->parent->right){
               z = z->parent;                    
               LeftRotate(D, z);      
            }          
            z->parent->color = "b";              
            z->parent->parent->color = "r";         
            RightRotate(D, z->parent->parent);     
         }
      }
      else{
         y = z->parent->parent->left;
         if(strcmp(y->color,"r") == 0){
            z->parent->color = "b";              
            y->color = "b";                
            z->parent->parent->color = "r";         
            z = z->parent->parent; 
         }                
         else{ 
            if(z == z->parent->left){
               z = z->parent;                 
               RightRotate(D, z);                
            }
            z->parent->color = "b";            
            z->parent->parent->color = "r";         
            LeftRotate(D, z->parent->parent);      
         }
      }
   }
   D->root->color = "b";
}

void Transplant(Dictionary D, Node u, Node v){
   if(u->parent == D->nil){
      D->root = v;
      if(v != D->nil){
         v->parent = D->nil;
      }
   }
   else if(u == u->parent->left){
      u->parent->left = v;
   }
   else{
      u->parent->right = v;
   }
   if(v != D->nil){
      v->parent = u->parent;
   }
}

void RB_Transplant(Dictionary D, Node u, Node v){
   if(u->parent == D->nil){
      D->root = v;
   }
   else if(u == u->parent->left){
      u->parent->left = v;
   }
   else{ 
      u->parent->right = v;
   }
   v->parent = u->parent;
}

void RB_DeleteFixUp(Dictionary D, Node x){
   Node w;
   while(x != D->root && strcmp(x->color,"b") == 0){
      if(x == x->parent->left){
         w = x->parent->right;
         if(strcmp(w->color,"r") == 0){
            w->color = "b";
            x->parent->color = "r";
            LeftRotate(D, x->parent);
            w = x->parent->right;
         }
         if(strcmp(w->left->color,"b") == 0 && strcmp(w->right->color,"b") == 0){
            w->color = "r";
            x = x->parent;
         }
         else{
            if(strcmp(w->right->color,"b") == 0){
               w->left->color = "b";
               w->color = "r";
               RightRotate(D, w);
               w = x->parent->right;
            }
            w->color = x->parent->color;
            x->parent->color = "b";
            w->right->color = "b";
            LeftRotate(D, x->parent);
            x = D->root;
         }
      }
      else{
         w = x->parent->left;
         if(strcmp(w->color,"r") == 0){
            w->color = "b";
            x->parent->color = "r";
            RightRotate(D, x->parent);
            w = x->parent->left;
         }
         if(strcmp(w->right->color,"b") == 0 && strcmp(w->left->color, "b") == 0){
            w->color = "r";
            x = x->parent;
         }
         else{
            if(strcmp(w->left->color,"b") == 0){
               w->right->color = "b";
               w->color = "r";
               LeftRotate(D, w);
               w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = "b";
            w->left->color = "b";
            RightRotate(D, x->parent);
            x = D->root;
         }
      }
      x->color = "b";
   }
}

VAL_TYPE lookup(Dictionary D,KEY_TYPE k) {
   if(D == NULL){
      exit(1);
   }

   Node n = TreeSearch(D,D->root,k);
   if(n == D->nil){
      return VAL_UNDEF;
   }
   else{
      return n->value;
   }
}


void delete(Dictionary D, KEY_TYPE k){
   Node z = TreeSearch(D,D->root,k);
   if(z == D->nil){
      exit(1);
   }
   else{
      if(z == D->current){
         D->current = D->nil;
      } 
      Node x = NULL;
      Node y = z;
      char* yOrigColor = y->color;
      if(z->left == D->nil){
         x = z->right;
         RB_Transplant(D, z, z->right);
      }
      else if(z->right == D->nil){
         x = z->left;
         RB_Transplant(D, z, z->left);
      }
      else{
         y = TreeMinimum(D,z->right);
         yOrigColor = y->color;
         x = y->right;
         if(y->parent == z){
            x->parent = y;
         }
         else{
            RB_Transplant(D, y, y->right);
            y->right = z->right;
            y->right->parent = y;
         }
         RB_Transplant(D, z, y);
         y->left = z->left;
         y->left->parent = y;
         y->color = z->color;
      }
      if(strcmp(yOrigColor,"b") == 0){
         RB_DeleteFixUp(D, x);
      }
      D->size -= 1;
      freeNode(&z);
      z = NULL;
   }
}

void PreOrderTreeWalk(Dictionary D,FILE* out, Node x){
   if(x != D->nil){
      fprintf(out,"%s\n",x->key);
      PreOrderTreeWalk(D,out,x->left);
      PreOrderTreeWalk(D,out,x->right);
   }
}


void InOrderTreeWalk(Dictionary D,FILE* out, Node x){
   if(x != D->nil){
      InOrderTreeWalk(D,out,x->left);
      fprintf(out,"%s\n",x->key);
      InOrderTreeWalk(D,out,x->right);
   }
}

void PostOrderTreeWalk(Dictionary D,FILE* out, Node x){
   if(x != D->nil){
      PostOrderTreeWalk(D,out,x->left);
      PostOrderTreeWalk(D,out,x->right);
      fprintf(out,"%s\n",x->key);
   }
}

void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v){
   if((getUnique(D) && lookup(D,k) == VAL_UNDEF) || !getUnique(D)){
      Node y = D->nil;
      Node x = D->root;
      Node z = newNode(k,v);
      while(x != D->nil){
         y = x;
         if(KEY_CMP(z->key,x->key) < 0){
            x = x->left;
         }
         else{
            x = x->right;
         }
      }
      z->parent = y;
      if(y == D->nil){
         D->root = z;
      }
      else if(KEY_CMP(z->key,y->key) < 0){
         y->left = z;
      }
      else{
         y->right = z;
      }
      z->left = D->nil;
      z->right = D->nil;
      z->color = "r";
      InsertFixUp(D,z);
      D->size += 1;
   }
   else{
      printf("Dictionary error: calling insert on existing key: %s\n",k);
      exit(1);
   }
}
	
	
void makeEmpty(Dictionary D) {
   //if(!isEmpty(D)){
      while(D->root != D->nil){
         delete(D,D->root->key);
      }
      D->size = 0;
      D->current = D->nil;
      D->root = D->nil;
   //}
}
	
VAL_TYPE beginForward(Dictionary D){
   D->current = TreeMinimum(D,D->root);
   return currentVal(D); 
}

VAL_TYPE beginReverse(Dictionary D){
   D->current = TreeMaximum(D,D->root);
  if(D->current == D->nil){
     return VAL_UNDEF;
  }
   return currentVal(D);
}

KEY_TYPE currentKey(Dictionary D){
   if(D->current == D->nil){
      return KEY_UNDEF;
   }
   return D->current->key;
}

VAL_TYPE currentVal(Dictionary D){
   if(D->current == D->nil){
      return VAL_UNDEF;
   }
   return D->current->value;
}

VAL_TYPE next(Dictionary D){
  D->current = TreeSuccessor(D,D->current);
  if(D->current == D->nil){
     return VAL_UNDEF;
  }
  return currentVal(D);
}

VAL_TYPE prev(Dictionary D){
  D->current = TreePredecessor(D,D->current);
  if(D->current == D->nil){
     return VAL_UNDEF;
  }
  return currentVal(D);
}

void printDictionary(FILE* out, Dictionary D, const char* ord){
   Node x = D->root;
   if(strcmp(ord,"pre") == 0){
      PreOrderTreeWalk(D,out,x);
   }
   else if(strcmp(ord,"in") == 0){
      InOrderTreeWalk(D,out,x);
   }
   if(strcmp(ord,"post") == 0){
      PostOrderTreeWalk(D,out,x);
   }
}

