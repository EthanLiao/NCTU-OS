#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sys/time.h>
#include <cstring>
#include <vector>
#include <map>
using namespace std;


class FIFO
{
 map<string,int> pageforAccess;
 int pageSize,amtPageFault,amthit;
 int order;
public:
 FIFO();
 void setupperSize(int size){pageSize=size;};
 int getamtPageFault(){return amthit;}
 int getamthit(){return amtPageFault;}
 string resizeString(string str);
 void access(string str);
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
  order++;
  if(pageforAccess.size()<pageSize)
  {
    if(pageforAccess.find(mem)==pageforAccess.end()) {amthit++; return;}
    else
    {
      pageforAccess[mem]=order;
      amtPageFault++;
    }

  }
  else
  {
    if(pageforAccess.find(mem)!=pageforAccess.end()){amthit++;return;}
    else
    {
      pageforAccess.erase(pageforAccess.begin());
      pageforAccess[mem]=order;
      amtPageFault++;
    }
  }
}

class LRU
{
 map<string,int> pageforAccess;
 int pageSize,amtPageFault,amthit;
 int order;
public:
 LRU();
 void setupperSize(int size){pageSize=size;};
 int getamtPageFault(){return amthit;}
 int getamthit(){return amtPageFault;}
 string resizeString(string str);
 void access(string str);
 map<string,int> modifymap(map<string,int> page,string mem);
};

LRU::LRU(){pageSize=0;amtPageFault=0;amthit=0;order=0;}

string LRU:: resizeString(string str)
{
  string temp;
  for(int i=0;i<5;i++) temp+=str[i];
  return temp;
}

 map<string,int> LRU ::modifymap(map<string,int> page,string mem)
 {
   map<string,int> newmap;
   for(map<string,int>:: iterator it=pageforAccess.begin();it!=pageforAccess.find(mem);it++)newmap.insert(make_pair(it->first,it->second));
   for(map<string,int>:: iterator it=++pageforAccess.find(mem);it!=pageforAccess.end();it++)newmap.insert(make_pair(it->first,it->second));
   newmap.insert(make_pair(pageforAccess.find(mem)->first,pageforAccess.find(mem)->second));
   return newmap;
 }

void LRU::access(string str)
{

  string mem;
  mem=resizeString(str);
  order++;
  if(pageforAccess.size()<pageSize)
  {
   pageforAccess[mem]=order;
   amtPageFault++;
  }
  else
  {
    if(pageforAccess.find(mem)!=pageforAccess.end())
    {
      pageforAccess=modifymap(pageforAccess,mem);
      amthit++;
      return;
    }
    else
    {
      pageforAccess.erase(pageforAccess.begin());
      pageforAccess[mem]=order;
      amtPageFault++;
    }
  }
}

int main(int argc, char const *argv[])
{


  cout<<"FIFO---"<<"\n";
  cout<<"size"<<"   "<<"miss"<<"       "<<"hit"<<"   "<<"page fault ratio"<<"   "<<"\n";
  for(int amtpage=64;amtpage<=512;amtpage*=2)
  {
    //cout<<"test";
    fstream srcfile;
    srcfile.open("trace.txt");
    string str;
    FIFO obj;
    int amtAccess=0;
    obj.setupperSize(amtpage);
    while (srcfile>>str)
    {
     obj.access(str);
     amtAccess++;
    }
    cout<<amtpage<<"   "<<obj.getamtPageFault()<<"   "<<obj.getamthit()<<"   "<<(double)obj.getamtPageFault()/amtAccess<<"\n";
    srcfile.close();
  }

  cout<<"LRU---"<<"\n";
  cout<<"size"<<"   "<<"miss"<<"       "<<"hit"<<"   "<<"page fault ratio"<<"   "<<"\n";
  for(int amtpage=64;amtpage<=512;amtpage*=2)
  {
    //cout<<"test";
    fstream srcfile;
    srcfile.open("trace.txt");
    string str;
    LRU obj;
    int amtAccess=0;
    obj.setupperSize(amtpage);
    while (srcfile>>str)
    {
     obj.access(str);
     amtAccess++;
    }
    cout<<amtpage<<"   "<<obj.getamtPageFault()<<"   "<<obj.getamthit()<<"   "<<(double)obj.getamtPageFault()/amtAccess<<"\n";
    srcfile.close();
  }
  return 0;
}
