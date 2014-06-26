/*
Aditya Modi
Roll no. 12045
Under Prof. Sumit Ganguly
CS210 Advanced track project
Splay tree implementation by using bottom-up splaying routine
Updation is done by modifying regular search tree updation routines.

For beginning insert no of initial nodes.
After inserting a few nodes use following procedure:

input "l n" for searching the number n
input "i n" for inserting the number n
input "d n" for deleting the number n
input "e" for ending the sequence
*/

#include <stdio.h>
#include <algorithm>
#include <iostream>
using namespace std;
struct node{
	struct node *left, * right, * parent;
	int key;
};

struct tree{
	node * root;
};

#define left_parent(t) {if ((t)->left!=NULL) (t)->left->parent=(t);}
#define right_parent(t) {if ((t)->right!=NULL) (t)->right->parent=(t);}

void right_rotate(node * x){
	node * y,*p;
	p=x->parent;
	y=x->right;
	x->right=p;
	x->parent=p->parent;
	if(y!=NULL)	y->parent=p;
	p->left=y;
	p->parent=x;
}

void left_rotate(node * x){
	node * y,*p;
	p=x->parent;
	y=x->left;
	x->left=p;
	x->parent=p->parent;
	if(y!=NULL) y->parent=p;
	p->right=y;
	p->parent=x;
}

void single_rotate(node * curr){
	if (curr==curr->parent->left){
		right_rotate(curr);
	}
	else left_rotate(curr);
}

void double_rotate(node * curr){
	node * p,*g;
	p=curr->parent;
	g=p->parent;
	if(curr==p->left && p==g->left){
		right_rotate(p);
		right_rotate(curr);
	}
	else if(curr==p->right && p==g->right){
		left_rotate(p);
		left_rotate(curr);
	}
	else if(curr==p->right && p==g->left){
		left_rotate(curr);
		right_rotate(curr);
	}
	else{
		right_rotate(curr);
		left_rotate(curr);
	}
}


void splay(node * t,tree *T){
	node * father;
	father= t->parent;
	while(father!=NULL){
		if(father->parent==NULL){
			single_rotate(t);
		}
		else double_rotate(t);
		father=t->parent;
	}
	T->root=t;
}

int insert(int num,tree *T){
	node * newnode;
	newnode=(node *) calloc(1,sizeof(node));
	newnode->key=num;
	node * t=T->root;
	node * prev=NULL;
	while(t!=NULL){
		prev=t;
		if(num==t->key) return 0;
		else if(num<t->key) t=t->left;
		else t=t->right;
	}
	newnode->parent=prev;
	if(prev==NULL) T->root=newnode;
	else if(newnode->key < prev->key) prev->left=newnode;
	else prev->right=newnode;
	splay(newnode,T);
	return 1;
}

void tree_delete(int i,tree * T){
	node * t=T->root;
	node * prev=NULL;
	node *y,*x;
	int flag=0;
	if(T->root==NULL) return;
	while(t!=NULL && t->key!=i){
		prev=t;
		if(i<t->key) t=t->left;
		else t=t->right;
	}
	if(t==NULL && prev!=NULL) splay(prev,T);
	else{
		if(t->left==NULL && t->right==NULL){
			y=t->parent;
			if(t!=T->root){
				if(t==y->left){
					y->left=NULL;
				}
				else y->right==NULL;
				free(t);
				splay(y,T);
			}
			else T->root=NULL;
		}
		else if(t->left==NULL || t->right==NULL){
			y=t->parent;
			if(t!=T->root){
				if(t->left!=NULL){
					if(t==y->left) y->left=t->left;
					else y->right=t->left;
				}
				else{
					if(t==y->left) y->left=t->right;
					else y->right=t->right;
				}
				splay(y,T);
			}
			else{
				if(t->left!=NULL){
					T->root=t->left;
				}
				else{
					T->root=t->right;
				}
			}
			free(t);
		}
		else{
			y=t->parent;
			x=t->right;
			prev=x;
			while(x->left!=NULL){
				prev=x;
				x=x->left;
			}
			prev->parent->left=prev->right;
			if(prev->right!=NULL) prev->right->parent=prev->parent;
			prev->right=t->right;
			prev->left=t->left;
			prev->right->parent=prev;
			prev->left->parent=prev;
			if(y!=NULL)	splay(y,T);
			else T->root=prev;
			free(t);
		}
	}
}

int lookup(int i, tree *T){
	node * t=T->root;
	node * prev=NULL;
	int flag=0;
	while(t!=NULL && t->key!=i){
		prev=t;
		if(i<t->key) t=t->left;
		else t=t->right;
	}
	if(t==NULL && T->root==NULL) return 0;
	else if (t==NULL) splay(prev,T);
	else{
		splay(t,T);
		flag=1;
	}
	return flag;
}

int main(){
	tree *T;
	T=(tree *)calloc(1,sizeof(tree));
	int num,n,ret;
	char q;
	printf("Enter the number of initial keys: \n");
	scanf("%d",&n);
	printf("Enter the keys (please don't insert equal keys): \n");
	for(int i=0;i<n;i++){
		scanf("%d",&num);
		insert(num,T);
	}
	printf("\n\nKeys inserted\nTree root key is %d\nNow enter the desired updation or search query (Don't insert already existing keys):\n",T->root->key);
	scanf("%c",&q);
	while(q!='e'){
		scanf("%d",&num);
		if(q=='l'){
			ret=lookup(num,T);
			if(ret==1){
				printf("Found;\nTree root key = %d\n\nEnter query:\n",T->root->key);
			}
			else printf("Not found;\nTree root key = %d\n\nEnter query:\n",T->root->key);
		} 
		else if(q=='i'){
			ret=insert(num,T);
			if(ret==0) printf("Key already exists;\nTree root key = %d\n\nEnter query:\n",T->root->key);
			else printf("Key inserted;\nTree root key = %d\n\nEnter query:\n",T->root->key);
		}
		else if(q=='d'){
			tree_delete(num,T);
			printf("Tree root key = %d\n\nEnter query:\n",T->root->key);
		}
		else if(q!='\n') printf("Invalid query.\n\nPlease enter again:\n");
		scanf("%c",&q);
		//printf("%c\n",q);
	}
	return 0;
}