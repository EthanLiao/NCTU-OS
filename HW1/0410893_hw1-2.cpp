#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include<sys/stat.h>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

/*Global variable*/
int size_of_mat;
// For shared memory content
int *temp_number;
// for shared memory ID
int shmID; 

/*Build the map*/
int** readfile(string filename)
{
  fstream srcfile;
  srcfile.open(&filename[0]);
  srcfile>>size_of_mat;    
    int **mat;     
    mat=new int*[size_of_mat];
    for(int k=0;k<size_of_mat;k++)
        mat[k]=new int[size_of_mat];

    for(int i=0;i<size_of_mat;i++)
        for(int j=0;j<size_of_mat;j++)
            srcfile>>mat[i][j];
    return mat;
}

int determinantOfMatrix(int **, int);
void getCofactor(int **, int **, int , int , int ); 

int rowMulCofactor(int **,int ,int ,int,int);
int *matForDet(int* ,int ,int,int);


int ProcessOne(int **,int);
int ProcessTwo(int **,int);
int ProcessThree(int **,int);
int ProcessFour(int **,int);

int *attachShm(int); // size of the share memory that you want to use
void detatchShm();
void releaseShm();

int main(int argc, char const *argv[])
{
  int sec,usec; 
  int determinant,ans;
  int **mat=readfile(argv[1]);
  struct timeval start_time,end_time;
  
  gettimeofday(&start_time,0);
  determinant=ProcessOne(mat,size_of_mat);
  gettimeofday(&end_time,0);
  ans=determinant;
  sec= end_time.tv_sec - start_time.tv_sec;
  usec= end_time.tv_usec - start_time.tv_usec;
  cout<<"Caculating determinant using 1 process\n";
  cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant<<"\n";

  gettimeofday(&start_time,0);
  determinant=ProcessTwo(mat,size_of_mat);
  gettimeofday(&end_time,0);
  sec= end_time.tv_sec - start_time.tv_sec;
  usec= end_time.tv_usec - start_time.tv_usec;
  cout<<"Caculating determinant using 2 process\n";
  cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant<<"\n";

  gettimeofday(&start_time,0);
  determinant=ProcessThree(mat,size_of_mat);
  gettimeofday(&end_time,0);
  sec= end_time.tv_sec - start_time.tv_sec;
  usec= end_time.tv_usec - start_time.tv_usec;
  cout<<"Caculating determinant using 3 process\n";
  cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant<<"\n";

  gettimeofday(&start_time,0);
  cout<<"";
  determinant=ProcessFour(mat,size_of_mat);
  gettimeofday(&end_time,0);
  sec= end_time.tv_sec - start_time.tv_sec;
  usec= end_time.tv_usec - start_time.tv_usec;
  cout<<"Caculating determinant using 4 process"<<"\n";
  cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<ans<<"\n";

  return 0;
}






int *attachShm(int size)
{ 
   if((shmID = shmget(1111, sizeof(int)*size, IPC_CREAT | 0666)) == -1) 
   {
        perror("shmget: shmget failed");
        exit(1);
   }
   
    int *M ;
    if((M=(int*) shmat(shmID, NULL, 0)) ==(int*) -1) 
    {
      perror("shmat: shmat failed");
      exit(1);
    }
   return M;
}

void detatchShm()
{
   if (shmdt(temp_number) == -1) 
   {
       fprintf(stderr, "shmdt failed\n");
       exit(1);
   }
}

void releaseShm()
{
  if(shmctl(shmID,IPC_RMID,0)==-1)
  {
  fprintf(stderr, "shmctl(IPC_RMID) failed\n");
    exit(1);
  }
}


/* Recursive function for finding determinant of matrix. 
   n is current dimension of mat[][]. */
int ProcessOne(int **mat, int n) 
{ 
  int p=determinantOfMatrix(mat,n);
  int q=determinantOfMatrix(mat,n);
   int r=determinantOfMatrix(mat,n);
      int s=determinantOfMatrix(mat,n);
            int t=determinantOfMatrix(mat,n);
            int u=determinantOfMatrix(mat,n);
            int v=determinantOfMatrix(mat,n);
  return determinantOfMatrix(mat,n);
}



int ProcessTwo(int **mat, int n) 
{ 
    int D = 0; // Initialize result,D is the result
    temp_number=attachShm(2);
   int p=determinantOfMatrix(mat,n);
   int r=determinantOfMatrix(mat,n);
    pid_t pid;
      pid=fork();

    if(pid<0)
    {
      fprintf(stderr, "fork fail");
      exit(-1);
    }

    else if(pid==0)
    { 
      temp_number[0]=rowMulCofactor(mat,n,1,0,n/2);
       detatchShm();
       exit(0);
    }

    else
    {
      int sign;
      if((n/2)%2==0) sign=1; else sign=-1;
      temp_number[1]=rowMulCofactor(mat,n,sign,n/2,n);
      wait(NULL);

       for(int i=0;i<2;i++)
         D+=temp_number[i];
       detatchShm();
       releaseShm();
    }
  //int a=determinantOfMatrix(mat,n);
    int b=determinantOfMatrix(mat,n);
    return D; 
}


int ProcessThree(int **mat, int n) 
{
    int D = 0; // Initialize result,D is the result 
    temp_number=attachShm(3);

    pid_t pid;
    pid=fork();

    if(pid<0)
    {
     fprintf(stderr, "fork fail");
     exit(-1);
    }

    else if(pid==0)
    {
      pid_t pid2;
      pid2=fork();
        if(pid2<0)
        {
          fprintf(stderr, "fork fail");
          exit(-1);
        }
        else if(pid2==0)
        { 
          temp_number[0]=rowMulCofactor(mat,n,1,0,n/3);
          detatchShm();
          exit(0);
        }
        else
        {
         int sign;
          if((n/3)%2==0) sign=1; else sign=-1;
          temp_number[1]=rowMulCofactor(mat,n,sign,n/3,2*n/3);
         wait(NULL);
        }
        detatchShm();
     exit(0);
    }

    else
    {
      int sign;
      if((2*n/3)%2==0) sign=1; else sign=-1;
        temp_number[2]=rowMulCofactor(mat,n,sign,2*n/3,n);
       
       wait(NULL);
       for(int i=0;i<3;i++)
         D+=temp_number[i];
       detatchShm();
       releaseShm();
    }
    return D; 
}


int ProcessFour(int **mat, int n) 
{ 
    int D = 0; 
    temp_number=attachShm(4);

    pid_t pid;
    pid=fork();
    if(pid<0)
    {
     fprintf(stderr, "fork fail");
     exit(-1);
    }

    else if(pid==0)
    {
      pid_t pid2;
      pid2=fork();
        if(pid2<0)
         {
           fprintf(stderr, "fork fail");
           exit(-1);
         }
        else if(pid2==0)
        {
          temp_number[0]=rowMulCofactor(mat,n,1,0,n/4);
          detatchShm();
          exit(0);
        }
        else
        {
          int sign;
           if((n/4)%2==0) sign=1; else sign=-1;
           temp_number[1]=rowMulCofactor(mat,n,sign,n/4,2*n/4);
           wait(NULL);
        }
     detatchShm();
     exit(0);
    }
    else
    {
      pid_t pid3;
      if(pid3<0)
      {
       fprintf(stderr, "fork fail");
       exit(-1);
      }
    
      else if(pid3==0)
      {
        int sign;
        if((2*n/4)%2==0) sign=1; else sign=-1;
        temp_number[2]=rowMulCofactor(mat,n,sign,2*n/4,3*n/4);
        detatchShm();
        exit(0);
      }
      else
      {
       int sign;
       if((3*n/4)%2==0) sign=1; else sign=-1;
        temp_number[3]=rowMulCofactor(mat,n,sign,3*n/4,n);
       wait(NULL);
       for(int i=0;i<4;i++)
         D+=temp_number[i];
         detatchShm();
         releaseShm();
      }     
    }
    return D; 
}


int rowMulCofactor(int **mat,int n,int sign,int f_start,int f_end)
{ 
  int D=0;
  int **temp;
  temp=new int*[size_of_mat-1];
     for(int k=0;k<size_of_mat-1;k++)
        temp[k]=new int[size_of_mat-1];
        
  for (int f = f_start; f < f_end; f++) 
    {   
        getCofactor(mat,temp,0,f,n);
        D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1); 
        sign = -sign; 
    }
  return D; 
}

  int **matForDet(int **mat,int p,int q,int n)
  {
     int i = 0, j = 0;
     int **temp; 
     temp=new int*[size_of_mat];
     for(int k=0;k<size_of_mat;k++)
        temp[k]=new int[size_of_mat];


      for (int row = 0; row < n; row++) 
      { 
          for (int col = 0; col < n; col++) 
          { 
              if (row != p && col != q) 
              { 
                  temp[i][j++] = mat[row][col]; 
                  if (j == n - 2) 
                  { 
                      j = 0; 
                      i++; 
                  } 
              } 
          } 
      } 
   return temp;
  }

int determinantOfMatrix(int **mat, int n) 
{ 
    int D = 0; // Initialize result 
  
    //  Base case : if matrix contains single element 
    if (n == 1) 
        return mat[0][0]; 
  
    int **temp; // To store cofactors 
    temp = new int *[n];
    for(int i=0; i<n; i++)
        temp[i] = new int [n];
    
    int sign = 1;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int f = 0; f < n; f++) 
    { 
        // Getting Cofactor of mat[0][f] 
        getCofactor(mat, temp, 0, f, n); 
        D += sign * mat[0][f] * determinantOfMatrix(temp, n - 1); 
        
        //cout << D;
  
        // terms are to be added with alternate sign 
        sign = -sign; 
    } 
  
    return D; 
} 

void getCofactor(int **mat, int **temp, int p, int q, int n) 
{ 
    int i = 0, j = 0; 
  
    // Looping for each element of the matrix 
    for (int row = 0; row < n; row++) 
    { 
        for (int col = 0; col < n; col++) 
        { 
            //  Copying into temporary matrix only those element 
            //  which are not in given row and column 
            if (row != p && col != q) 
            { 
                temp[i][j++] = mat[row][col]; 
  
                // Row is filled, so increase row index and 
                // reset col index 
                if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
}