#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;


string buffer, temp,string_result[20];
int  parsing_index=0,dynamic_index=0,check;



void show(string *string_result)
{
    for(int k=0;k<(parsing_index+1);k++)
       cout<<string_result[k]<<endl;
}

int main()
{



    
   cout<<"please input your word"<<endl;
    getline(cin,buffer);
    
    
  for(int i=0;i<buffer.length();i++)
    {    
      temp=buffer[i];
      check=temp.compare(" ");
    
    if(check==0) //parsing exclude last string
        {
          string_result[parsing_index].assign(buffer,dynamic_index,i+1-dynamic_index);
          dynamic_index=i+1;
          parsing_index++;
        }
    
    if(i==(buffer.length()-1)) //parsing exclude last string
       string_result[parsing_index].assign(buffer,dynamic_index,i+1-dynamic_index);  
      
    }



   char** char_result = new char*[20]; //string transform to char
      for(int i = 0; i <(parsing_index+1);i++) 
      { 
        char_result[i] = new char[string_result[i].length() +1];
        strcpy(char_result[i], string_result[i].c_str());
      }

      
   for(int k=0;k<(parsing_index+1);k++)
     cout<<char_result[k]<<endl;

     show(string_result); //show that how to use string function call

     free(char_result);
  
       system("pause");
        return 0;
}

