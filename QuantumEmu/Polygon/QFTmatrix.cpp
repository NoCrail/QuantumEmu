#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex>
#include <math.h> 

using namespace std;

int main (int argc, char **argv)
{
    int N = 4;
    double nd = log2(N); 
    int n = (int)nd;
    double pow2 = pow(2.0f, n*1.0f);
    complex<double> zi = complex<double>(0, 1);
    complex<double> wn = exp(2*M_PI*zi/pow2);
    complex<double>	a[N][N];
    for(int i = 0; i<N; i++){
        for(int k = 0; k<N; k++)
            a[k][i]=pow(wn, k*i);
    }

    printf("******************************************************\n");
      printf("Matrix:\n");
      for (int i=0; i<N; i++)
      {
         printf("\n"); 
         for (int j=0; j<N; j++) 
            cout << a[i][j];
      }
      printf("\n******************************************************\n");

}