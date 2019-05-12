#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <sys/time.h>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <limits.h>
using namespace std;


class LRU
{
private :
int pagefault,pageSize;
public:
  LRU();
  int pageFaults(string pages[], int n, int capacity);
  void access(vector<string> v);
  void setpageSize(int size){pageSize=size;};
  int getamtPageFault(){return pagefault;}

};

LRU::LRU(){pageSize=0;pagefault=0;}

void LRU:: access(vector<string> v)
{
  string *pages=&v[0];
  pagefault=pageFaults(pages,v.size(),pageSize);
}

int LRU:: pageFaults(string pages[], int n, int capacity)
{
  // To represent set of current pages. We use
  // an unordered_set so that we quickly check
  // if a page is present in set or not
  unordered_set<string> s;

  // To store least recently used indexes
  // of pages.
  unordered_map<string, int> indexes;

  // Start from initial page
  int page_faults = 0;
  for (int i=0; i<n; i++)
  {
      // Check if the set can hold more pages
      if (s.size() < capacity)
      {
          // Insert it into set if not present
          // already which represents page fault
          if (s.find(pages[i])==s.end())
          {
              s.insert(pages[i]);

              // increment page fault
              page_faults++;
          }

          // Store the recently used index of
          // each page
          indexes[pages[i]] = i;
      }

      // If the set is full then need to perform lru
      // i.e. remove the least recently used page
      // and insert the current page
      else
      {
          // Check if current page is not already
          // present in the set
          if (s.find(pages[i]) == s.end())
          {
              // Find the least recently used pages
              // that is present in the set
              int lru = INT_MAX;
              string val;
              for (unordered_set<string>::iterator it=s.begin(); it!=s.end(); it++)
              {
                  if (indexes[*it] < lru)
                  {
                      lru = indexes[*it];
                      val = *it;
                  }
              }

              // Remove the indexes page
              s.erase(val);

              // insert the current page
              s.insert(pages[i]);

              // Increment page faults
              page_faults++;
          }

          // Update the current page index
          indexes[pages[i]] = i;
      }
  }

  return page_faults;
}

class FIFO
{
private :
int pagefault,pageSize;
public:
  FIFO();
  int pageFaults(string pages[], int n, int capacity);
  void access(vector<string> v);
  void setpageSize(int size){pageSize=size;};
  int getamtPageFault(){return pagefault;}

};

FIFO::FIFO(){pageSize=0;pagefault=0;}

void FIFO:: access(vector<string> v)
{
  string *pages=&v[0];
  pagefault=pageFaults(pages,v.size(),pageSize);
}

int FIFO:: pageFaults(string pages[], int n, int capacity)
{
  unordered_set<string> s;
   queue<string> indexes;
   int page_faults = 0;
   for (int i=0; i<n; i++)
   {
    if (s.size() < capacity)
    {
        if (s.find(pages[i])==s.end())
        {
            s.insert(pages[i]);
            page_faults++;
            indexes.push(pages[i]);
        }
    }
    else
    {
        if (s.find(pages[i]) == s.end())
        {
            string val = indexes.front();
            indexes.pop();
            s.erase(val);
            s.insert(pages[i]);
            indexes.push(pages[i]);
            page_faults++;
        }
    }
}

return page_faults;
}



 string resizeString(string str)
 {
     string temp;
     for(int i=0;i<5;i++) temp+=str[i];
     return temp;
 }

int main(int argc, char const *argv[])
{
  /*read file*/
  vector<string> v;
  fstream srcfile;
  int amtAccess=0;
  srcfile.open("trace.txt");
  string str;

  while (srcfile>>str)
  {
   v.push_back(resizeString(str));
   amtAccess++;
  }
  srcfile.close();

 /*page replacement algo*/
 cout<<"FIFO---"<<"\n";
 cout<<"size"<<"   "<<"miss"<<"       "<<"hit"<<"   "<<"page fault ratio"<<"   "<<"\n";
 for(int amtpage=64;amtpage<=512;amtpage*=2)
 {
   FIFO obj;
   obj.setpageSize(amtpage);
   obj.access(v);
   cout<<amtpage<<"   "<<obj.getamtPageFault()<<"   "<<amtAccess-obj.getamtPageFault()<<"   "<<(double)obj.getamtPageFault()/amtAccess<<"\n";
 }

  cout<<"LRU---"<<"\n";
  cout<<"size"<<"   "<<"miss"<<"       "<<"hit"<<"   "<<"page fault ratio"<<"   "<<"\n";
  for(int amtpage=64;amtpage<=512;amtpage*=2)
  {
    LRU obj;
    obj.setpageSize(amtpage);
    obj.access(v);
    cout<<amtpage<<"   "<<obj.getamtPageFault()<<"   "<<amtAccess-obj.getamtPageFault()<<"   "<<(double)obj.getamtPageFault()/amtAccess<<"\n";
  }
  return 0;

}
