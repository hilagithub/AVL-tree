//
// Created by Hila Cohen on 07/03/2022.
//

#ifndef UNTITLED55_AVL_H
#define UNTITLED55_AVL_H

#include <stdbool.h>
#include <malloc.h>


typedef struct Node{
     struct Node* left;
     struct Node* right;
     int balance;
     int data;
}Node;



Node* total_insert(Node* root,Node* to_insert);
Node* total_deletion(Node* root,Node* to_del);


#endif //UNTITLED55_AVL_H
