#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
  TreeMap * map = (TreeMap*) malloc(sizeof(TreeMap));
  if(map==NULL)
    return NULL;
  
  map->root = NULL;
  map->current = NULL;
  map->lower_than = lower_than;
  return map;

}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  if(searchTreeMap(tree, key)!=NULL)
    return;
  TreeNode* parent = NULL;
  TreeNode* current = tree->root;

  while(current!=NULL)
  {
    parent = current;
    if(tree->lower_than(key, current->pair->key))
      current = current->left;
      
    else
      current=current->right;
  }
  TreeNode* nuevo = createTreeNode(key, value);
  nuevo->parent = parent;

  if(parent==NULL)
    tree->root=nuevo;
    
  else if (tree->lower_than(key, parent->pair->key))
    parent->left = nuevo;
    
  else
    parent->right = nuevo;

  tree->current=nuevo;
}
TreeNode * minimum(TreeNode * x)
{
  if(x==NULL)
    return NULL;
  
  while(x->left!=NULL)
    x=x->left;
    
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
  if(tree==NULL)
    return;
  
  //caso 1
  if(node->left==NULL && node->right==NULL)
  {
    if(node->parent!=NULL)
    {
      if(node->parent->left==node)
      {
        node->parent->left=NULL;
      }
      else
      {
        node->parent->right=NULL;
      }
    }
    else
    {
      tree->root=NULL;
    }
  free(node);
  return;
  }
  //caso 2

  if(node->left==NULL && node->right==NULL)
  {
    TreeNode* auxiliar = (node->left != NULL) ? node->left : node->right;

    if(node->parent!=NULL)
    {
      if(node->parent->left==node)
        node->parent->left=auxiliar;
      else
        node->parent->right=auxiliar;
      if(auxiliar != NULL)
      {
        auxiliar->parent = node->parent;
      }
    }
    
   
  }
    
  //caso 3
  else
  {
    TreeNode* minimoDerecha = minimum(node->right);
    node->pair->key = minimoDerecha->pair->key;
    node->pair->value = minimoDerecha->pair->value;
    removeNode(tree, minimoDerecha);
  }
  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
  if(tree==NULL)
    return NULL;
  TreeNode* current = tree->root; 
  while(current!=NULL)
  {
    if(is_equal(tree,current->pair->key, key))
    {
      tree->current = current;
      return current->pair;
    }
    else if(tree->lower_than(key,current->pair->key))
    {
      current = current->left;
    }
    else
    {
      current = current->right;
    }
  }
  return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) 
{
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
  if(tree==NULL)
    return NULL;

  TreeNode * minNode = minimum(tree->root);
  tree->current = minNode;
  return minNode->pair;
  
}

Pair * nextTreeMap(TreeMap * tree) {
  if(tree==NULL)
    return NULL;

  TreeNode * sucesor = NULL;
  if(tree->current->right != NULL)
    sucesor = minimum(tree->current->right);
  
  else
  {
    TreeNode * current = tree->current;
    TreeNode * parent = tree->current->parent;

    while(parent!=NULL && current == parent->right)
      {
        current=parent;
        parent=parent->parent;
      }
    sucesor=parent;
  }
  if(sucesor!=NULL)
  {
    tree->current = sucesor;
    return sucesor->pair;
  }
  else
  {
    tree->current=NULL;
    return NULL;
  }
}
