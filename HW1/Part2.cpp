#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <fstream>

int size_of_mat;
int *temp_number;

// Global variable for shared memory ID
int shmID; 

int** readfile(const string );


void ProcessOne(int**,int);
void ProcessTwo(int**,int);
void ProcessThree(int**,int);
void ProcessFour(int**,int);


int *attachShm(int); // size of the share memory that you want to use
void detatchShm();
void releaseShm();


int main(int argc, char const *argv[])
{
	int sec,usec;
    
    int determinant;
	int **mat=reafile(argv[1]);
	struct timeval start_time,end_time;
	
	gettimeofday(&star,0);
	determinant=ProcessOne(mat,size_of_mat);
	gettimeofday(&end_time,0);
	sec= end_time.tv_sec - start_time.tv_sec;
	usec= end_time.tv_usec - start_time.tv_usec;
	cout<<"Caculating determinant using 1 process\n";
	cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant;

	gettimeofday(&star,0);
	determinant=ProcessOne(mat,size_of_mat);
	gettimeofday(&end_time,0);
	sec= end_time.tv_sec - start_time.tv_sec;
	usec= end_time.tv_usec - start_time.tv_usec;
	cout<<"Caculating determinant using 1 process\n";
	cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant;

	gettimeofday(&star,0);
	determinant=ProcessOne(mat,size_of_mat);
	gettimeofday(&end_time,0);
	sec= end_time.tv_sec - start_time.tv_sec;
	usec= end_time.tv_usec - start_time.tv_usec;
	cout<<"Caculating determinant using 1 process\n";
	cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant;

	gettimeofday(&star,0);
	determinant=ProcessOne(mat,size_of_mat);
	gettimeofday(&end_time,0);
	sec= end_time.tv_sec - start_time.tv_sec;
	usec= end_time.tv_usec - start_time.tv_usec;
	cout<<"Caculating determinant using 1 process\n";
	cout<<"Elapsed time : "<<sec + (usec/1000000.0)<<",determinant : "<<determinant;

	return 0;
}


/*Build the map*/
int** readfile(const string filename)
{
  fstream srcfile;
    srcfile.open(filename);
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



int *attachShm(int size)
{ 
   if((shmID = shmget(1111, sizeof(int)*size, IPC_CREAT | 0666)) == -1) 
   {
        perror("shmget: shmget failed");
        exit(EXIT_SUCCESS);
   }
   int *M;
    if((M = shmat(shmID, NULL, 0)) == (int*)-1) {
        perror("shmat: attach error");
        exit(EXIT_SUCCESS);
    }
   return M;
}

void detatchShm()
{
   if (shmdt(temp_number) == -1) 
   {
       fprintf(stderr, "shmdt failed\n");
       exit(EXIT_SUCCESS);
   }
}

void releaseShm()
{
	if(shmctl(shmID,IPC_RMID,0)==-1)
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_SUCCESS);
	}
}


/* Recursive function for finding determinant of matrix. 
   n is current dimension of mat[][]. */
void ProcessOne(int **mat, int n) 
{ 

    int D = 0; // Initialize result,D is the result
  
    //  Base case : if matrix contains single element 
    if (n == 1) 
        return D; 
  
    int temp[N][N]; // To store cofactors 
  
    int sign = 1;  // To store sign multiplier 
  
     // Iterate for each element of first row 
    for (int f = 0; f < n; f++) 
    { 
        // Getting Cofactor of mat[0][f] 
        getCofactor(mat, temp, 0, f, n); 
        D += sign * mat[0][f] * ProcessOne(temp, n - 1); 
  
        // terms are to be added with alternate sign 
        sign = -sign; 
    } 
    return D; 
}



void ProcessTwo(int **mat, int n) 
{ 
    int D = 0; // Initialize result,D is the result
    temp_number=attachShm(size_of_mat);
    //  Base case : if matrix contains single element 
    if (n == 1) 
        return D; 
  
    int temp[N][N]; // To store cofactors 

    pid_t pid;
    pid=fork();

    if(pid<0)
    {
    	fprintf(stderr, "fork fail");
    	exit(FAILURE);
    }

    else if(pid==0)
    {
       int sign=1;
       for(int f=0;f<n/2;f++)
       {
         getCofactor(mat, temp, 0, f, n); 
         temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
         sign = -sign; 
       }
       detatchShm();
       exit(0);
    }

    else
    {
      int sign;
      if(n%2==0) sign=-1; else sign=1;
      for(int f=n/2;f<n;f++)
       {
         getCofactor(mat, temp, 0, f, n); 
         temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
         sign = -sign; 
       }
       
       wait(NULL);
       for(int i=0;i<n;i++)
         D+=temp_number[i];
       detatchShm();
       releaseShm();
    }
    return D; 
}


void ProcessThree(int **mat, int n) 
{ 
    int D = 0; // Initialize result,D is the result
    int count_for_protect_attachShm=0;
    if(count_for_protect_attachShm==0)
    {
     temp_number=attachShm(size_of_mat);
     count_for_protect_attachShm++;
    }
    //  Base case : if matrix contains single element 
    if (n == 1) return D; 
  
    int temp[N][N]; // To store cofactors 
    pid_t pid;
    pid=fork();

    if(pid<0)
    {
     fprintf(stderr, "fork fail");
     exit(FAILURE);
    }

    else if(pid==0)
    {
      pid_t pid2;
      pid2=fork();
        if(pid<0)
         {
           fprintf(stderr, "fork fail");
           exit(FAILURE);
         }
        else if(pid==0)
        {
         int sign=1;
         for(int f=0;f<n/3;f++)
          {
           getCofactor(mat, temp, 0, f, n); 
           temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
           sign = -sign; 
          }
          detatchShm();
          exit(0);
        }
        else
        {
          int sign;
           if((n/3)%2==0) sign=-1; else sign=1;
           for(int f=n/3;f<2*n/3;f++)
          {
           getCofactor(mat, temp, 0, f, n); 
           temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
           sign = -sign; 
          }
         wait(NULL);
        }
        detatchShm();
        exit(0);
    }

    else
    {
      int sign;
      if((2*n/3)%2==0) sign=-1; else sign=1;
      for(int f=2*n/3;f<n;f++)
       {
         getCofactor(mat, temp, 0, f, n); 
         temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
         sign = -sign; 
       }
       
       wait(NULL);
       for(int i=0;i<n;i++)
         D+=temp_number[i];
       detatchShm();
       releaseShm();
    }
    return D; 
}


void ProcessFour(int **mat, int n) 
{ 
    int D = 0; // Initialize result,D is the result
    temp_number=attachShm(size_of_mat);
    //  Base case : if matrix contains single element 
    if (n == 1) 
        return D; 
  
    int temp[N][N]; // To store cofactors 
    pid_t pid;
    pid=fork();

    if(pid<0)
    {
     fprintf(stderr, "fork fail");
     exit(FAILURE);
    }

    else if(pid==0)
    {
      pid_t pid2;
      pid2=fork();
        if(pid<0)
         {
           fprintf(stderr, "fork fail");
           exit(FAILURE);
         }
        else if(pid==0)
        {
         int sign=1;
         for(int f=0;f<n/4;f++)
          {
           getCofactor(mat, temp, 0, f, n); 
           temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
           sign = -sign; 
          }
          detatchShm();
          exit(0);
        }
        else
        {
          int sign;
           if((n/4)%2==0) sign=-1; else sign=1;
           for(int f=n/4;f<2*n/4;f++)
          {
           getCofactor(mat, temp, 0, f, n); 
           temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
           sign = -sign; 
          }
         wait(NULL);
        }
       detatchShm();
       exit(0);
    }

    else
    {
    pid_t pid3;

    if(pid<0)
     {
       fprintf(stderr, "fork fail");
       exit(FAILURE);
     }
    
    else if(pid==0)
    {
     int sign;
      if((2*n/4)%2==0) sign=-1; else sign=1;
      for(int f=2*n/4;f<3*n/4;f++)
       {
         getCofactor(mat, temp, 0, f, n); 
         temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
         sign = -sign; 
       }
    }

    else
      {
       int sign;
        if((3*n/4)%2==0) sign=-1; else sign=1;
         for(int f=3*n/4;f<n;f++)
          {
           getCofactor(mat, temp, 0, f, n); 
           temp_number[f] = sign * mat[0][f] * ProcessTwo(temp, n - 1);  
           sign = -sign; 
          }
       wait(NULL);
       wait(NULL);
       for(int i=0;i<n;i++)
         D+=temp_number[i];
      }     
    detatchShm();
    releaseShm();
    }

    return D; 
}




void getCofactor(int mat[N][N], int temp[N][N], int p, int q, int n) 
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
  
 
