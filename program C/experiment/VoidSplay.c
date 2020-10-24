#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

typedef struct node {
    enum TypesOfEvent type; //type of event
    
    int idElementInGroup;//id of element in group of hosts or switches
    int portID;
    int endTime;
    struct node *left;
    struct node *right;
    struct node *parent;
}node;

enum Side{LEFT, RIGHT};

typedef struct splay_tree {
  struct node *root;
}splay_tree;

node* new_node(int type, int idElementInGroup,
                int portID, 
                int endTime) {
  node *n = malloc(sizeof(node));
  n->type = type;
  n->idElementInGroup = idElementInGroup;
  n->portID = portID;
  n->endTime = endTime;
  n->parent = NULL;
  n->right = NULL;
  n->left = NULL;

  return n;
}

node* minimum(splay_tree *t) {
    node *x = t->root;
    if(x == NULL)
        return x;
    while(x->left != NULL)
        x = x->left;
    
    return x;
}


splay_tree* new_splay_tree() {
  splay_tree *t = malloc(sizeof(splay_tree));
  t->root = NULL;

  return t;
}



void left_rotate(splay_tree *t, node *x) {
  node *y = x->right;
  x->right = y->left;
  if(y->left != NULL) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == NULL) { //x is root
    t->root = y;
  }
  else if(x == x->parent->left) { //x is left child
    x->parent->left = y;
  }
  else { //x is right child
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(splay_tree *t, node *x) {
  node *y = x->left;
  x->left = y->right;
  if(y->right != NULL) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == NULL) { //x is root
    t->root = y;
  }
  else if(x == x->parent->right) { //x is left child
    x->parent->right = y;
  }
  else { //x is right child
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void splay(splay_tree *t, node *n) {
  while(n->parent != NULL) { //node is not root
    if(n->parent == t->root) { //node is child of root, one rotation
      if(n == n->parent->left) {
        right_rotate(t, n->parent);
      }
      else {
        left_rotate(t, n->parent);
      }
    }
    else {
      node *p = n->parent;
      node *g = p->parent; //grandparent

      if(g == NULL)
      {
        if(n->parent->left == n)
        {
          p->parent = n;
          p->left = n->right;
          if(p->left != NULL)
          {
            p->left->parent = p;
          }
          n->right = p;
          n->parent = NULL;
          if(t->root->right == n)
          {
            t->root->right = NULL;
          } 
          n->left = t->root;
        }
        else{
          if(n->parent->right == n)
          {
            p->parent = n;
            p->right = n->left;
            
            if(p->right != NULL)
            {
              p->right->parent = p;
            }
            n->left = p;
            n->parent = NULL;
            if(t->root->left == n)
            {
              t->root->left = NULL;
            } 
            n->right = t->root; 
          }
        }
        t->root = n;
      }
      else{
        if(n->parent->left == n && p->parent->left == p) { //both are left children
          right_rotate(t, g);
          right_rotate(t, p);
        }
        else if(n->parent->right == n && p->parent->right == p) { //both are right children
          left_rotate(t, g);
          left_rotate(t, p);
        }
        else if(n->parent->right == n && p->parent->left == p) {
          left_rotate(t, p);
          right_rotate(t, g);
        }
        else if(n->parent->left == n && p->parent->right == p) {
          right_rotate(t, p);
          left_rotate(t, g);
        }
      }
    }
  }
  /*int left;
    node * f;
    node * gf;
    node * ggf;
    while(n->parent != NULL)
    {
        f = n->parent;
        gf = f->parent;
        left = (n == f->left ? 1 : 0);
        if(left)
        {
            // cas du fils gauche
            if (gf == NULL) {
               // cas "zig", on fait la rotation de f (la racine) et e
               f->parent = n;
               f->left = n->right;
               if(f->left != NULL)
                  f->left->parent = f;
               n->right = f;
               n->parent = NULL;
            }
            else if (gf->right == f) {
               // cas "zig-zag", simplifie, pareil que le cas "zig"
               gf->right = n;

               f->parent = n;
               f->left = n->right;
               if(f->left != NULL)
                  f->left->parent = f;
               n->right = f;
               n->parent = gf;
            }
            else {
               // cas "zig-zig", on fait la rotation de gf avec
               // f, suivis de la rotation de e avec f
               ggf = gf->parent;

               gf->left = f->right;
               if(gf->left != NULL)
                  gf->left->parent = gf;
               f->right = gf;
               gf->parent = f;

               f->left = n->right;
               if(f->left != NULL)
                  f->left->parent = f;
               f->parent = n;
               n->right = f;

               // on rattache e a son nouveau pere
               n->parent = ggf;
               if(ggf != NULL)
                  if(ggf->left == gf)
                     ggf->left = n;
                  else
                     ggf->right = n;
            }
        }else
        {
            //cas du fils droit
            if(gf == NULL) {
               // cas "zig", on fait la rotation de f (la racine) et e

               f->parent = n;
               f->right = n->left;
               if(f->right != NULL)
                  f->right->parent = f;
               n->left = f;
               n->parent = NULL;
            }
            else if(gf->left == f) {
               // cas "zig-zag", simplifie, pareil que le cas "zig"
               gf->left = n;

               f->parent = n;
               f->right = n->left;
               if(f->right != NULL)
                  f->right->parent = f;
               n->left = f;
               n->parent = gf;
            }
            else {
               // cas "zig-zig", on fait la rotation de gf avec
               // f, suivis de la rotation de e avec f
               ggf = gf->parent;

               gf->right = f->left;
               if(gf->right != NULL)
                  gf->right->parent = gf;
               f->left = gf;
               gf->parent = f;

               f->right = n->left;
               if(f->right != NULL)
                  f->right->parent = f;
               f->parent = n;
               n->left = f;

               // on rattache e a son nouveau pere
               n->parent = ggf;
               if(ggf != NULL)
                  if(ggf->left == gf)
                     ggf->left = n;
                  else
                     ggf->right = n;
            }
        }
    }*/

}

void insert(splay_tree *t, node *n) {
  node *y = NULL;
  node *temp = t->root;
  while(temp != NULL) {
    y = temp;
    if(n->endTime < temp->endTime)
      temp = temp->left;
    else
      temp = temp->right;
  }
  n->parent = y;

  if(y == NULL) //newly added node is root
    t->root = n;
  else if(n->endTime < y->endTime)
    y->left = n;
  else
    y->right = n;

  splay(t, n);
}

node* maximum(splay_tree *t, node *x) {
  while(x->right != NULL)
    x = x->right;
  return x;
}

node* find(node *t, int key)
{
  if(t == NULL) return NULL;
  if(t->endTime < key)
  {
    return find(t->right, key);
  }
  if(t->endTime > key)
  {
    return find(t->left, key);
  }
  if(t->endTime == key)
    return t;
}


void delete(splay_tree *t, node *n) {
  splay(t, n);

  splay_tree *left_subtree = new_splay_tree();
  left_subtree->root = t->root->left;
  if(left_subtree->root != NULL)
    left_subtree->root->parent = NULL;

  splay_tree *right_subtree = new_splay_tree();
  right_subtree->root = t->root->right;
  if(right_subtree->root != NULL)
    right_subtree->root->parent = NULL;

  free(n);

  if(left_subtree->root != NULL) {
    node *m = maximum(left_subtree, left_subtree->root);
    splay(left_subtree, m);
    left_subtree->root->right = right_subtree->root;
    left_subtree->root->right->parent = left_subtree->root;
    t->root = left_subtree->root;
  }
  else {
    t->root = right_subtree->root;
  }
}

void delete_key(splay_tree *t, int key)
{
  node *temp = find(t->root, key);
  if(temp == NULL)
    return;
  delete(t, temp);
}

node* removeFirst(splay_tree *t)
{
    node *x = minimum(t);
    node *temp = new_node(A, 0, 0, 0);
    
    temp->type = x->type;
    temp->idElementInGroup = x->idElementInGroup;
    temp->portID = x->portID;
    temp->endTime = x->endTime;
    
    delete(t, x);
    return temp;
}

void inorder(splay_tree *t, node *n) {
  if(n != NULL) {
    inorder(t, n->left);
    //printf("%d\n", n->data);
    inorder(t, n->right);
  }
}

void leaf(node *t, enum Side side)
{
    printf("===========> ");
    if(side == LEFT)
    {
        printf("left ");
    }
    else{
        printf("right ");
    }
    if(t != NULL)
    {
        printf("for event type = %d at end = %d in %d\n", 
                t->type, t->endTime, t->
                            //idNode
                            idElementInGroup
                    );
        leaf(t->left, LEFT);
        leaf(t->right, RIGHT);
        printf("\n");
    }
    else{
        printf("NULL\n");
    }
}

void show(splay_tree * t)
{
    if(t != NULL)
    {
        printf("\n===========> for event type = %d at end = %d in %d\n", 
                t->root->type, t->root->endTime, t->root->
                                        //idNode
                                        idElementInGroup
                                    );
        leaf(t->root->left, LEFT);
        leaf(t->root->right, RIGHT);
        printf("\n");
    }
    else{
        printf("===========> left NULL\n");
    }
}

