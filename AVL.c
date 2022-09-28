#include <stdio.h>
#include "AVL.h"


enum rotation{RR,RL,LL,LR};


int max(int x, int y){
    if(x>=y){
        return x;
    }
    return y;
}



void rebalance(Node *node){
    if(!node){
        return;
    }

    //leaf
    if(!node->left && !node->right){
        node->balance=0;
        return ;

    }
    //has 2 children
    else if(node->left && node->right){
        rebalance(node->left);
        rebalance(node->right);
        node->balance= max(node->left->balance,node->right->balance) + 1;

    }
    //has left child only
    else if(node->left && !node->right){
        rebalance(node->left);
        node->balance=node->left->balance + 1;


    }
    //has right child
    else if(!node->left && node->right){
        rebalance(node->right);
        node->balance=node->right->balance + 1;
    }

}


int get_balance(Node* node){
    if(!node){
        return -1;
    }
    else{
        return node->balance;
    }

}

int rotation(Node* node){

    if (get_balance(node->right)>get_balance(node->left)){
        if(!node->right->left ){
            return RR;

        }
        else if (!node->right->right){
            return RL;
        }
        else if(node->right->right->balance>node->right->left->balance){
            return RR;
        }
        else if(node->right->left>=node->right->right){
            return RL;
        }
    }
    if(get_balance(node->left)> get_balance(node->right))
    {
        if (!node->left->right)
        {
            return LL;

        } else if (!node->left->left)
        {
            return LR;
        } else if (node->left->left->balance >
                   node->left->right->balance)
        {
            return LL;
        } else if (node->left->right->balance >
                   node->left->left->balance)
        {
            return LR;
        }
    }



}


Node* find_node(Node* root,int data){
      if(!root){
          return NULL;
      }
      if(root->data==data){
         return root;
      }
      else if(data>=root->data){
          find_node(root->right,data);
      }
      else if(data<root->data){
          find_node(root->left,data);
      }
}


Node* find_father(Node* unbalanced, Node* root){
    if(!root || !unbalanced){
        return NULL;

    }

    if(root->data==unbalanced->data){
        return NULL ;
    }

    if(root->right->data==unbalanced->data ||
       root->left->data==unbalanced->data){
        return root ;
    }


    if(root->data>unbalanced->data){

        find_father(unbalanced, root->left);
    }
    if(root->data<unbalanced->data){

        find_father(unbalanced, root->right);
    }

}


Node* find_unbalance_node(Node* root){
    if(!root || (!(*root).left && !(*root).right) ||
       root->balance<2 ){
        return NULL;
    }


    ///if there is no right child-root is inbalanced
    if((*root).left && !(*root).right){
        if((*root).balance==2 && (!(*root).left || !(*root).right))
        {
            return root;
        }
        find_unbalance_node((*root).left);

    }

    ///if there is no left child-root is inbalanced
    if(!(*root).left && (*root).right){
        if((*root).balance==2 && (!(*root).left || !(*root).right))
        {
            return root;
        }
        find_unbalance_node((*root).right);


    }

    ///if left child has greater balance
    if((*root).left->balance- (*root).right->balance==2){
        Node* check_right= find_unbalance_node((*root).left);
        if(check_right){
            return find_unbalance_node((*root).left);
        }
        return root;

    }


    ///if right child has greater balance
    if((*root).left->balance- (*root).right->balance==-2){
        Node* check_right= find_unbalance_node((*root).right);
        if(check_right){
            return find_unbalance_node((*root).right);
        }
        return root;

    }

    ///left child tree has Unbalanced tree
    if((*root).left->balance- (*root).right->balance>2 )
    {
        find_unbalance_node((*root).left);
    }
    ///right child tree has Unbalanced tree
    if((*root).left->balance- (*root).right->balance<-2 )
    {
        find_unbalance_node((*root).right);
    }

    return NULL;

}

Node* rotate(Node* root){
    if(!root)
    {
        return NULL;
    }
    else
     {
        Node* node= find_unbalance_node(root);
        Node* father= find_father(node, root);
        if(!node){
            return root;
        }
         printf("rotate unbalance %d\n",root->data);
        enum rotation rot=rotation(node);

         ///LL
         if(rot==LL){
             if(!father){
                 root=((node)->left);
             }
             Node* tmp=node;
             Node* right_child=tmp->left->right;
             node=(node)->left;

             if(!right_child){
                 (node)->right= malloc(sizeof(Node));
             }
             (node)->right=tmp;
             tmp->left=right_child;
             if(father && father->data>node->data){
                 father->left=node;
             }
             else if (father && father->data<node->data){
                 father->right=node;
             }
         }
         ///RR
         else if(rot==RR){
             if(!father){
                 root=((node)->right);
             }
             Node* left_child=node->right->left;
             Node* tmp=node;
             node=(node)->right;

             if(!left_child){
                 (node)->left= malloc(sizeof(Node));
             }
             (node)->left=tmp;
             tmp->right=left_child;
             if(father && father->data>node->data){
                 father->left=node;
             }
             else if (father && father->data<node->data){
                 father->right=node;
             }



         }
         ///RL
         else if(rot==RL){

             Node* new_node=node->right->left;
             Node* left_subt=node->right->left->left;
             Node* right_subt=node->right->left->right;
             Node* prev_father=node;
             Node* prev_right=node->right;

             ///new_node should have 2 children
             if(!left_subt){
                 new_node->left= malloc(sizeof(Node));
                 free(prev_father->right);


             }
             if(!right_subt){
                 new_node->right= malloc(sizeof(Node));
                 free(prev_right->left);


             }
             new_node->left= prev_father;
             new_node->right=prev_right;

             ///concat to rest-"upper"  tree
             if(father && father->data>node->data){
                 father->left=new_node;
             }
             else if (father && father->data<node->data){
                 father->right=new_node;
             }

             ///if there are not sub tree of new_node- free memory
             ///left sub tree
             if(!left_subt){
                 new_node->left->right=NULL;

             }
             else if(left_subt){
                 new_node->left->right=left_subt;
             }
             ///right sub-tree
             if(!right_subt){
                 new_node->right->left=NULL;

             }
             else if (right_subt){
                 new_node->right->left=right_subt;

             }
             ///update root
             if(!father){
                 root=new_node;
             }

         }

         ///LR
         else if(rot==LR){
             Node* new_node=node->left->right;
             Node* right_subt=node->left->right->right;
             Node* left_subt=node->left->right->left;
             Node* prev_father=node;
             Node* prev_left=node->left;
             prev_father->left=right_subt;
             prev_left->right=left_subt;
             if(!right_subt){
                 new_node->right= malloc(sizeof(Node));

             }
             if(!left_subt){
                 new_node->left= malloc(sizeof(Node));

             }
             new_node->right= prev_father;
             new_node->left=prev_left;

             ///concat to rest-"upper"  tree
             if(father && father->data>new_node->data){
                 father->left=new_node;
             }
             else if (father && father->data<new_node->data){
                 father->right=new_node;
             }

             ///if there are not sub tree of new_node- free memory
             ///left sub tree
             if(!left_subt){
                 free(new_node->left->right);

             }
             else if(left_subt){
                 new_node->left->right=left_subt;

             }
             ///right sub tree
             if(!right_subt){
                 free(new_node->right->left);

             }
             else if (right_subt){
                 new_node->right->left=right_subt;

             }
             ///update root
             if(!father){
                 root=new_node;
             }



         }
     }
    return root;
}





Node* insert(Node* node, Node* node_to_insert , Node* root){

    if(!node_to_insert){
        return root;
    }

    //ifthis is the first node to insert
    if (!root )
    {
        Node* r= malloc(sizeof(Node));
        if(!r){
            return NULL;
        }
        r->data=node_to_insert->data;
        r->left=NULL;
        r->right=NULL;
        if(!node){
            node= malloc(sizeof(Node));
        }
        if(!node){
            return NULL;
        }
        return r;

    }


    if (node_to_insert->data >= node->data)
    {
        if (!(node)->right)
        {
            (node)->right = malloc(sizeof(Node));
            (node)->right->data = node_to_insert->data;
            (node)->right->right = NULL;
            (node)->right->left = NULL;

            return root;
        }
        else
        {
            insert((node)->right, node_to_insert, root);

        }


    }
    else if (node_to_insert->data < (node)->data)
    {
            if (!(node)->left)
            {
                (node)->left = malloc(sizeof(Node));
                (node)->left->data = node_to_insert->data;
                (node)->left->right = NULL;
                (node)->left->left = NULL;

                return root;
            }
            else
            {
                insert((node)->left,node_to_insert, root);

            }


    }


}



Node* total_insert(Node* root,Node* to_insert){
    root= insert(root,to_insert,root);
    rebalance(root);
    root= rotate(root);
    rebalance(root);
    return root;

}

Node* max_left_tree(Node* to_delete){
    Node* node=to_delete->left;
    while(node->right){
        node=node->right;
    }
    return node;
}

Node* delete(Node* root,Node* to_delete){
    if(!root || !to_delete){
        return NULL;
    }
    Node* father= find_father(to_delete,root);
    Node* node=root;
    Node* left_child=to_delete->left;
    Node* right_child=to_delete->right;
    if(!father){
        return NULL;
    }

    bool is_left_of_father=(father->data>to_delete->data);
    ///no children of to_delete
    if(!left_child && !right_child ){
        ///to delete is left child of father
        if(is_left_of_father){
           free(father->left);
        }
        ///right child of father
        else{
            free(father->right);
        }
    }
    ///only right child to_delete
    else if(!left_child){
        Node* tmp;
        ///left child of father
        if(is_left_of_father){
            tmp=father->left;
            father->left=right_child;
            free(tmp);
        }
        ///right child of father
        else{
            tmp=father->right;
            father->right=right_child;
            free(tmp);
        }

    }
    ///only left child of to_delete
    else if(!right_child){
        Node* tmp;
        ///left child of father
        if(is_left_of_father){
            tmp=father->left;
            father->left=left_child;
            free(tmp);
        }
            ///right child of father
        else{
            tmp=father->right;
            father->right=left_child;
            free(tmp);
        }


    }
    ///2 children
    else{
        Node* max_left_sub=max_left_tree(to_delete);
        int origin_to_del= to_delete->data;
        to_delete->data=max_left_sub->data;
        max_left_sub->data=origin_to_del;
        delete(root,max_left_sub);

    }
    return root;

}

Node* total_deletion(Node* root,Node* to_del){
    rebalance(root);
    root=delete(root,to_del);
    rebalance(root);
    root=rotate(root);
    while(root && find_unbalance_node(root)){
        rebalance(root);
        root=rotate(root);

    }
    rebalance(root);
    return root;

}



void print_tree(Node* root){
     if(root){
         printf("%d %d \n",root->data,root->balance);
         print_tree(root->left);
         print_tree(root->right);
     }
     return;

}


int main() {
    Node* n9= malloc(sizeof(Node));
    n9->left=NULL;
    n9->right=NULL;
    n9->data=9;

    Node* n15= malloc(sizeof(Node));
    n15->left=NULL;
    n15->right=NULL;
    n15->data=15;

    Node* n11= malloc(sizeof(Node));
    n11->left=NULL;
    n11->right=NULL;
    n11->data=11;

    Node* n20= malloc(sizeof(Node));
    n20->left=NULL;
    n20->right=NULL;
    n20->data=20;

    Node* n2= malloc(sizeof(Node));
    n2->left=NULL;
    n2->right=NULL;
    n2->data=2;

    Node* n4= malloc(sizeof(Node));
    n4->left=NULL;
    n4->right=NULL;
    n4->data=4;

    Node* n8= malloc(sizeof(Node));
    n8->left=NULL;
    n8->right=NULL;
    n8->data=8;

    Node* n7= malloc(sizeof(Node));
    n7->left=NULL;
    n7->right=NULL;
    n7->data=7;

    Node* n13= malloc(sizeof(Node));
    n13->left=NULL;
    n13->right=NULL;
    n13->data=13;

    Node* n3= malloc(sizeof(Node));
    n3->left=NULL;
    n3->right=NULL;
    n3->data=3;

    Node* n26= malloc(sizeof(Node));
    n26->left=NULL;
    n26->right=NULL;
    n26->data=26;

    Node* n21= malloc(sizeof(Node));
    n21->left=NULL;
    n21->right=NULL;
    n21->data=21;

    Node* n30= malloc(sizeof(Node));
    n30->left=NULL;
    n30->right=NULL;
    n30->data=30;

    Node* n10= malloc(sizeof(Node));
    n10->left=NULL;
    n10->right=NULL;
    n10->data=10;

      ///test1
//    Node* root=insert(NULL, n20, NULL);
//    root=insert(root, n15, root);
//    root=total_insert(root, n20);
//    root=total_insert(root, n8);
//    root=total_insert(root, n7);
//    root=total_insert(root, n13);
//    root=total_insert(root,n7);
//    print_tree(root);
//    root=total_deletion(root,root->left);
//    rebalance(root);

    ///test 1.5
//    Node* root= total_insert(NULL,n20);
//    root= total_insert(root,n4);
//    root= total_insert(root,n26);
//    root= total_insert(root,n3);
//    root= total_insert(root,n9);
//    root= total_insert(root,n15);

      ///test2
//    Node* root= total_insert(NULL,n20);
//    root= total_insert(root,n4);
//    root= total_insert(root,n26);
//    root= total_insert(root,n3);
//    root= total_insert(root,n9);
//    root= total_insert(root,n21);
//    root= total_insert(root,n30);
//    root= total_insert(root,n2);
//    root= total_insert(root,n7);
//    root= total_insert(root,n11);
//    root= total_insert(root,n15);

      ///test3
//    Node* root= total_insert(NULL,n20);
//    root= total_insert(root,n4);
//    root= total_insert(root,n26);
//    root= total_insert(root,n3);
//    root= total_insert(root,n9);
//    root= total_insert(root,n8);

     /// /test4

//    Node* root= total_insert(NULL,n20);
//    root= total_insert(root,n4);
//    root= total_insert(root,n26);
//    root= total_insert(root,n3);
//    root= total_insert(root,n9);
//    root= total_insert(root,n21);
//    root= total_insert(root,n30);
//    root= total_insert(root,n2);
//    root= total_insert(root,n7);
//    root= total_insert(root,n11);
//    root= total_insert(root,n8);

//
//    rebalance(root);
//
//    print_tree(root);






    return 0;
}