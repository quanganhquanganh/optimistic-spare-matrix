#include <stdio.h>
#include <stdlib.h>
#include "Event.c"

enum Side{LEFT, RIGHT};

void add(int type, int idElementInGroup,
                int portID, 
                int endTime,
                int *root,
                int arr[320][7]
                );
/* From now on, an event has 7 fields about:
   + it's type
   + idElementInGroup is the index of element in each group Hosts, Ways, Switches Edge, Agg, Core
   + the portID where the event happens (= -1 in case event - A, B, C, G, H occurs on host)
   + endTime: the time of event to be finished.
   + index of father.
   + index of left
   + index of right
*/
void add(int type, int idElementInGroup,
                int portID, 
                int endTime,
                int *root,
                int arr[384][7]
                )
{
   /* Quy ước các event từ A đến G (tức type = 0..4) sẽ là 
      các event xảy ra trên hosts
      Các biến type < 0 sẽ là event xảy ra trên các edge switch
      Các biến type có bit cuối cùng là 1 sẽ là các event xảy ra trên Core switch
      Các biến type có bit cuối cùng là 0 sẽ là các event xảy ra trên Agg switch
   */
   int idNewNode = 0;
   if(type == A || type == B || type == C || type == H_HOST || type == G)
   {
      idNewNode = idElementInGroup*3 + type;
   }
   else if(type < 0)//Is event of edge switch
   {
      type *= -1;
      idNewNode = 16*3 + idElementInGroup*((4/2)*4 + 3*(4/2)) + type;
   }
   else{
      int isCoreSwitch = type & 1;
      type = type >> 1;
      idNewNode = 16*3 + (4*4/2)*((4/2)*4 + 3*(4/2)) + idElementInGroup*4 + type;
   }
   arr[idNewNode][0] = type;
   arr[idNewNode][1] = idElementInGroup;
   arr[idNewNode][2] = portID;
   arr[idNewNode][3] = endTime;
   arr[idNewNode][4] = -1;
   arr[idNewNode][5] = -1;
   arr[idNewNode][6] = -1;

   if(*root == -1)
   {
      arr[0][0] = type;
      arr[0][1] = idElementInGroup;
      arr[0][2] = portID;
      arr[0][3] = endTime;
      arr[0][4] = -1;
      arr[0][5] = -1;
      arr[0][6] = -1;

      *root = 0;
      return;
   }
   int left = idNewNode, right = idNewNode, temp = -1;
   int end_splay = 0;
   while (end_splay == 0) {
      if(endTime > arr[*root][3])//if(endTime > t->endTime)
      {
         temp = arr[*root][6];//temp = t->right;
         if(temp == -1)//if(temp == NULL)
         {
            //cas "zig"
            arr[left][6] = *root; //left->right = t;
            arr[*root][4] = left; //t->father = left;
            arr[right][5] = -1;//right->left = NULL;
            end_splay = 1;
         }
         else if(endTime < arr[temp][3])//if(endTime < temp->endTime)
         {
            //cas "zig-zag" simplifie
            arr[left][6] = *root;//left->right = t;
            arr[*root][4] = left;//t->father = left;
            left = *root;//left = t;
            *root = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[*root][6] = arr[temp][5]; //t->right = temp->left;
            if(arr[temp][5] == -1)//if (temp->left != NULL)
               arr[  arr[temp][5]  ][  4  ] = *root; //temp->left->father = t;
            arr[left][6] = temp;//left->right = temp;
            arr[temp][4] = left;//temp->father = left;
            arr[temp][5] = *root;//temp->left = t;
            arr[*root][4] = temp;//t->father = temp;
            left = temp;
            *root = arr[temp][6];//t = temp->right;
            if(*root == -1){//if (t == NULL) {
               arr[right][5] = -1;//right->left = NULL;
               end_splay = 1;
            }
         }
      }else {
         temp = arr[*root][5]; //temp = t->left;
         if(temp == -1){//if (temp == NULL) {
            //cas "zig"
            arr[right][5] = *root; //right->left = t;
            arr[*root][4] = right; //t->father = right;
            arr[left][6] = -1;     //left->right = NULL;
            end_splay = 1;
         }
         else if (endTime > arr[temp][3]) {//if (endTime > temp->endTime) {
            //cas "zig-zag" simplifie
            arr[right][5] = *root; //right->left = t;
            arr[*root][4] = right; //t->father = right;
            right = *root;//right = t;
            *root = temp;//t = temp;
         }
         else {
            //cas "zig-zig"
            arr[*root][5] = arr[temp][6]; //t->left = temp->right;
            if (arr[temp][6] != -1)//if (temp->right != NULL)
               arr[  arr[temp][6]  ][4] = *root; //temp->right->father = t;
            arr[right][5] = temp; //right->left = temp;
            arr[temp][4] = right; //temp->father = right;
            arr[temp][6] = *root; //temp->right = t;
            arr[*root][4] = temp; //t->father = temp;
            right = temp;         //right = temp;
            *root = arr[temp][5]; //t = temp->left;
            if(*root == -1) {//if (t == NULL) {
               arr[left][6] = -1; //left->right = NULL;
               end_splay = 1;
            }
         }
      }
   }

   temp = arr[idNewNode][5]; //temp = newNode->left;
   arr[idNewNode][5] = arr[idNewNode][6]; //newNode->left = newNode->right;
   arr[idNewNode][6] = temp; //newNode->right = temp;

   //return newNode;
   *root = idNewNode;

}
void show(int arr[384][7], int root);
void leaf(int arr[384][7], int root, enum Side side);

void show(int arr[384][7], int root)
{
   if(arr[root][0] != -1)
   {
      printf("\n===========> for event type = %d at end = %d in %d\n", 
               arr[root][0], arr[root][3], arr[root][1]);
      leaf(arr, arr[root][5], LEFT);
      leaf(arr, arr[root][6], RIGHT);
   }
   else{
      printf("===========> left NULL\n");
   }
}

void leaf(int arr[384][7], int root, enum Side side)
{
   printf("===========> ");
   if(side == LEFT)
   {
       printf("left ");
   }
   else{
       printf("right ");
   }
   if(arr[root][0] != -1)
   {
      printf("for event type = %d at end = %d in %d\n", 
               arr[root][0], arr[root][3], arr[root][1]);
      leaf(arr, arr[root][5], LEFT);
      leaf(arr, arr[root][6], RIGHT);
   }
   else{
      printf("NULL\n");
   }
}