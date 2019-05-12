#include <cstdlib>
#include <iostream>
#include <cstring>
using namespace std;

int main()
{

   int  parsing_index1=0,parsing_index2=0;

   char buffer[20];
   int  buffer_index=0,result_index=0;
   int  i=0,j=0,k,check1,check2;
   char result[5][20];
    
    cout<<"please input your word"<<endl;

    cin.getline(buffer,20);

    while(check1 != 0)
      {
        result[parsing_index1][parsing_index2]=buffer[buffer_index];
        parsing_index2 ++;
        if(result[parsing_index1][parsing_index2]=' ')
            {
                parsing_index1++;
                parsing_index2=0;
            }             
        buffer_index++;
        check1=strcmp(*(result+parsing_index1)+parsing_index2,"/0");
      }

    while(i<=parsing_index1)
      {  
        if(check2==0)            
              strcpy(*(result+i)+j,"/0");
              j=0;
       }  
       check2=strcmp(*(result+i)+j," ") ;  
       i++;
       j++; 
            
    for(int k=0;k<parsing_index1;k++)
      cout<<result[k]<<endl;
    
    return 0;
}

