#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/types.h> 
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>


using namespace std;


string buffer,string_result[20],string_temp;
char char_temp[20];
int parsing_index=0,dynamic_index=0,check;
char** char_result = new char*[20]; //string transform to char

   
void READ_LINE()
{
    getline(cin,buffer);
    
    
  for(int i=0;i<buffer.length();i++)
    {    
      string_temp=buffer[i];
      check=string_temp.compare(" ");
    
    if(check==0) //parsing exclude last string
        {
          string_result[parsing_index].assign(buffer,dynamic_index,i+1-dynamic_index);
          dynamic_index=i+1;
          parsing_index++;
        }
    
    if(i==(buffer.length()-1)) //parsing exclude last string
       string_result[parsing_index].assign(buffer,dynamic_index,i+1-dynamic_index);  
      
    }
       for(int i = 0; i <(parsing_index+1);i++) 
       { 
         char_result[i] = new char[string_result[i].length() +1];
         strcpy(char_result[i], string_result[i].c_str());
       } 
}


int EXCUTE1()  // Push the parsing char_result into OS
  {
    pid_t pid1,pid2;
    pid1 = fork();
    
    if(pid1<0)
      {
        fprintf(stderr, "Fork Failed");
        exit(-1);
      }
		
	 else if(pid1==0)     //parent waits for first child to exit
      {  
		     pid2=fork(); 
      	if(pid2)     //child
      		exit;
        else         //granchild
         {
         	strcpy(char_result[2],"NULL");
         	execvp(char_result[0],char_result);
         }
      }
    else
      { 
        wait(NULL);
        return 1;
      }
  }


int EXCUTE2()  // Push the parsing char_result into OS
  {
    pid_t pid;
    pid=fork();

    if(pid<0)
      {
        fprintf(stderr, "Fork Failed");
        exit(-1);
      }
    
    else if(pid==0)
      {
        execvp(char_result[0], char_result);
      }

    else 
      {
         wait(NULL);
         cout<<"Child complete"<<endl;
         return 1;
      }  
  }


int main()
  {
    string content;
    int status;
    do
    {
      cout<<">"<<endl;
      READ_LINE();
      if(strcmp(char_result[2],"&")==0)
         status=EXCUTE1();
      else
         status=EXCUTE2();
    } while (status);  
     
     free(char_result);  
     return 0;
  }
