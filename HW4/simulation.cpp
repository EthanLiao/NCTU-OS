#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sys/time.h>
#include <cstring>
#include "0410893_pa4.cpp"
using namespace std;

typedef struct address
{
  string id;
  int time;
}page;

class FIFO : public BST
{
 BST bst;
 int pageSize,amtPageFault,amthit;
 int order;
 //friend class BST;
public:
 FIFO();
 string resizeString(string str);
 void access(string str);
 int getamtPageFault(){return amthit;}
 int getamthit(){return amtPageFault;}
 void setupperSize(int size){pageSize=size;};
};

FIFO::FIFO(){pageSize=0;amtPageFault=0;amthit=0;order=0;}

string FIFO:: resizeString(string str)
{
  string temp;
  for(int i=0;i<5;i++) temp+=str[i];
  return temp;
}

void FIFO::access(string str)
{

  string mem;
  mem=resizeString(str);
  page pageforAccess;
  pageforAccess.id = mem;
  pageforAccess.time = order;
  //cout<<pageforAccess.id<<" "<<pageforAccess.time<<"\n";
  //cout<<pageSize<<"\n";
  order++;
  //int count=0;
  if(bst.getsizeofTree()<pageSize)
  {bst.insert(pageforAccess);
   amtPageFault++;
  // cout<<count<<"\n";
   //count++;
   //cout<<(bst.searchRank(1))->id<<"\n";

  }
  else
  {
    cout<<"test";
    //cout<<bst.getsizeofTree();
    if(bst.searchid(pageforAccess.id)!=nullptr) {amthit++; return;}
    else { bst.Delete(searchRank(1)); bst.insert(pageforAccess); amtPageFault++; }
  }
}

class LRU : public BST
{
 friend class BST;
 BST bst;
 int pageSize,amtPageFault,amthit;
 int order;
 //friend class BST;
public:
 LRU();
 string resizeString(string str);
 void access(string str);
 int getamtPageFault(){return amthit;}
 int getamthit(){return amtPageFault;}
 void setupperSize(int size){pageSize=size;};
};

LRU::LRU(){pageSize=0;amtPageFault=0;amthit=0;order=0;}

string LRU:: resizeString(string str)
{
  string temp;
  for(int i=0;i<5;i++) temp+=str[i];
  return temp;
}

void LRU::access(string str)
{

  string mem;
  mem=resizeString(str);
  page pageforAccess;
  pageforAccess.id = mem;
  pageforAccess.time = order;
  //cout<<pageforAccess.id<<" "<<pageforAccess.time<<"\n";
  //cout<<pageSize<<"\n";
  order++;
  if(bst.getsizeofTree()<pageSize) {bst.insert(pageforAccess); amtPageFault++;}
  else
  {
    if(bst.searchid(pageforAccess.id)!=nullptr) {bst.Delete(bst.searchid(pageforAccess.id));bst.insert(pageforAccess); amthit++; return;}
    else { bst.Delete(bst.searchRank(1)); bst.insert(pageforAccess); amtPageFault++; }
  }
}


int main(int argc, char const *argv[])
{
  fstream srcfile;
  srcfile.open(argv[1]);
  FIFO obj;
  string str;
  cout<<"FIFO---"<<"\n";
  cout<<"size"<<"   "<<"miss"<<"  "<<"hit"<<"   "<<"page fault ratio"<<"   "<<"\n";
  int amtAccess=0;
  for(int amtpage=64;amtpage<=512;amtpage*=2)
  {
    obj.setupperSize(amtpage);
    while (srcfile>>str) {obj.access(str); amtAccess++;}
    cout<<amtpage<<"   "<<obj.getamtPageFault()<<"   "<<obj.getamthit()<<"   "<<obj.getamtPageFault()/amtAccess<<"\n";
  }
  srcfile.close();
  return 0;
}
