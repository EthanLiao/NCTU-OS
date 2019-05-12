#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include <queue>
#include <cstring>
using namespace std;

struct Node
{
  int key;
  string id;
  int leftSize;
  Node *leftchild;
  Node *rightchild;
  Node *parent;
};


class BST
{

  Node *root;
  int sizeofTree;
  //int upperSize; // set the upper size if ineed

public:
  BST();
Node *searchNode(int num);
Node *searchid(string name);
int searchLevel(int num);
template<class usrtype>
void insert(usrtype usr);
void Delete(Node* node);
Node *findMax(Node* node);
Node* findMin(Node* innode);
Node *searchRank(int rank);
void modifyLeftsize(Node* targetnode);
int getsizeofTree();
//void setupperSize(int num);
};

BST::BST(){root=nullptr; sizeofTree=0;}
Node *BST::searchNode(int num)
{
  Node *node;
  node=root;
  while(node!=nullptr)
  {
    if(node->key==num) return node;
    else
    {
      if(num<node->key) node=node->leftchild;
      else node=node->rightchild;
    }
  }
  //not found
  return node;
}
//using level order traversal
Node *BST::searchid(string name)
{
  //cout<<"test";
  //empty tree
  if(!root) return nullptr;
  Node *node=root;
  queue<Node*> q;
  q.push(node);
  while(!q.empty())
  {
    Node* p = q.front(); q.pop();
    string tmp=p->id;
    if(strcmp(&name[0],&tmp[0])==0) return p;
    if (p->leftchild)  q.push(p->leftchild);
    if (p->rightchild) q.push(p->rightchild);
  }
  return nullptr;
}

int BST::searchLevel(int num)
{
  Node *node=root;
  int level=0;
  while(node!=nullptr)
  {
    if(node->key==num) return level;
    else
    {
      if(num<node->key) {node=node->leftchild; level++;}
      else { node=node->rightchild; level++;}
    }
  }
  //not found
  return -1;
}

template<class usrtype>
void BST::insert(usrtype usr)
{
  //cout<<"insert";
  //cout<<usr.time<<"\n";
  if(searchid(usr.id)!=nullptr) return;

  Node *newnode,*current,*prev;
  newnode=new Node;
  newnode->key=usr.time;
  int num=usr.time;
  newnode->id=usr.id;
  newnode->leftSize=1;
  newnode->leftchild=newnode->rightchild=newnode->parent=nullptr;
  sizeofTree++;
  //cout<<sizeofTree<<"\n";
  //cout<<newnode->id<<"\n";

  /*Insert a node*/
  //find the insert position
  if(root==nullptr) root=newnode;
  else
  {
    current=root;
    // Search the node position
    while(current!=nullptr)
    {
      prev=current;
      if(num>current->key) current=current->rightchild;
      else {current->leftSize++;  current=current->leftchild;}
    }
    //insert the node
    newnode->parent=prev;
    if(num>prev->key) prev->rightchild=newnode;
    else prev->leftchild=newnode;
  }
}
void BST::Delete(Node* node)
{
  // No data in BST
  if(root==nullptr) return;
  Node *delnode;
  //BST doesn't have the key
  if((delnode=searchid(node->id))==nullptr) return;

  //Case1 : delnode is leaf
  if(delnode->rightchild==nullptr && delnode->leftchild==nullptr)
  {
    if(delnode==root)
    {
      delete delnode;
      root=nullptr;
    }
    else
    {
      if(delnode->key>delnode->parent->key)
      {
        modifyLeftsize(delnode);
        delnode->parent->rightchild=nullptr;
      }
      else if(delnode->key<delnode->parent->key)
      {
        modifyLeftsize(delnode);
        delnode->parent->leftchild=nullptr;
      }
      delete delnode;
    }
    sizeofTree--;
  }
  //Case2 : delnode has one-child
  else if(delnode->rightchild==nullptr && delnode->leftchild!=nullptr)
  {

    if(delnode==root)
    {
      Node *alternode;
      if(!(alternode=findMax(delnode->leftchild)))
      {
        root=delnode->leftchild;
        root->parent=nullptr;
        delete delnode;
      }
      else
      {
        modifyLeftsize(alternode);
        alternode->parent->rightchild=nullptr;
        delnode->id=alternode->id;
        delnode->key=alternode->key;
        delete alternode;
      }
    }
    else
    {
      modifyLeftsize(delnode);
      if(delnode->key>delnode->parent->key)
      {
        delnode->parent->rightchild=delnode->leftchild;
        delnode->leftchild->parent=delnode->parent;
      }
      if(delnode->key<delnode->parent->key)
      {
        delnode->parent->leftchild=delnode->leftchild;
        delnode->leftchild->parent=delnode->parent;
      }
    }
    delete delnode;
    sizeofTree--;
  }
  else if(delnode->rightchild!=nullptr && delnode->leftchild==nullptr)
  {

    if(delnode==root)
    {
      Node *alternode;
      if(!(alternode=findMin(delnode->rightchild)))
      {
        root=delnode->rightchild;
        root->parent=nullptr;
        delete delnode;
      }
      else
      {
        modifyLeftsize(alternode);
        alternode->parent->leftchild=nullptr;
        delnode->id=alternode->id;
        delnode->key=alternode->key;
        delete alternode;
      }
    }
    else
    {
      modifyLeftsize(delnode);
      if(delnode->key>delnode->parent->key)
      {
       delnode->parent->rightchild=delnode->rightchild;
       delnode->rightchild->parent=delnode->parent;
      }
      if(delnode->key<delnode->parent->key)
      {
       delnode->parent->leftchild=delnode->rightchild;
       delnode->rightchild->parent=delnode->parent;
      }
      delete delnode;
    }

    sizeofTree--;
  }
  //Case3 : delnode has two-child
  else if(delnode->rightchild!=nullptr && delnode->leftchild!=nullptr)
  {

    /*if(delnode==root && delnode->leftchild->leftchild==nullptr && delnode->leftchild->rightchild==nullptr&& delnode->rightchild->leftchild==nullptr && delnode->rightchild->rightchild==nullptr)
    {
      Node* temp;
      delnode->key=delnode->leftchild->key;
      delnode->leftchild=nullptr;
      delnode->leftSize=1;
      delete delnode->leftchild;
    }*/
    if(delnode==root)
    {
      Node *alternode;
      alternode=findMax(delnode->leftchild);
      modifyLeftsize(alternode);
      root->id=alternode->id;
      root->key=alternode->key;
      alternode->parent->rightchild=nullptr;
      delete alternode;
    }
    else
    {
      Node *alternode;
      alternode=findMax(delnode->leftchild);
      if(alternode->key>alternode->parent->key)
      {
        modifyLeftsize(alternode);
        alternode->parent->rightchild=alternode->leftchild;
      }
      else if(alternode->key<alternode->parent->key)
      {
        modifyLeftsize(alternode);
        alternode->parent->leftchild=alternode->leftchild;
      }
      delnode->key=alternode->key;
      delnode->id=alternode->id;
      delete alternode;
    }
      sizeofTree--;
  }
}

Node* BST::findMax(Node* innode)
{
  Node* node;
  node=innode;
  while(node!=nullptr&&node->rightchild!=nullptr) node=node->rightchild;
  return node;
}

Node* BST::findMin(Node* innode)
{
  Node* node;
  node=innode;
  while(node!=nullptr&&node->leftchild!=nullptr) node=node->leftchild;
  return node;
}

Node* BST::searchRank(int rank)
{
  for(Node* node=root;node;)
  {
    if(rank==node->leftSize) return node;
    if(rank<node->leftSize) node=node->leftchild;
    else
    {
      rank-=node->leftSize;
      node=node->rightchild;
    }
  }
  return 0;
}

void BST::modifyLeftsize(Node* targetnode)
{
  Node* target=targetnode;
  Node* current=root;
  while (current!=target)
  {
    if(target->key < current->key){current->leftSize--; current=current->leftchild;}
    if(target->key > current->key) current=current->rightchild;
  }
}

int BST::getsizeofTree()
{
return sizeofTree;
}





/*BST for FIFO*/
/*
void BST:: setupperSize(int num)
{
  upperSize=num;
}
*/
