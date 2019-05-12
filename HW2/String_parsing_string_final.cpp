#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;

int  parsing_index=0,dynamic_index=0,check;

void show(string *result)
{
    for(int k=0;k<(parsing_index+1);k++)
       cout<<result[k]<<endl;
}

int main()
{

   string buffer,result[20],temp;

    
   cout<<"please input your word"<<endl;
   getline(cin,buffer);
    
    
  for(int i=0;i<buffer.length();i++)
    {    
      temp=buffer[i];
      check=temp.compare(" ");
    
    if(check==0) //parsing exclude last string
        {
          result[parsing_index].assign(buffer,dynamic_index,i+1-dynamic_index);
          dynamic_index=i;
          parsing_index++;
        }
    
    if(i==(buffer.length()-1)) //parsing for last string
       result[parsing_index].assign(buffer,dynamic_index,i+1-dynamic_index);  
      
    }
  
  for(int k=1;k<(parsing_index+1);k++) //erase the " "
    result[k].erase(0,1);
      
  for(int k=0;k<(parsing_index+1);k++)
    cout<<result[k]<<endl;
  
    show(result);
       system("pause");
        return 0;
}

