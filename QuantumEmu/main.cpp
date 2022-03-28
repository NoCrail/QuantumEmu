//
//  main.cpp
//  QuantumEmu
//
//  Created by Vladislav Dunaev on 06.04.2021.
//  Copyright © 2021 Vladislav Dunaev. All rights reserved.
//

#include <iostream>
#include <mpi.h>
#include <fstream>
#include <string>
#include "Config.h"
#include <complex>
//#include <sstream>
#include <vector>


using namespace std;

Config readConfig();
string decToBin(int, int);
void setUpData(Config);
int binToDec(string);
void quantumX(int);
void quantumY(int);
void quantumZ(int);
void quantumH(int);
void quantumS(int);
char binReverse(char);
void quantumFT();
void applyResult();
void quantumWalsh();
void quantumCNot(int,int);
void doInstructions(Config);
void createResultFile(Config);
void quantumCP(int , int);

//complex<double> arr[arraysize];

int size = 0;
complex<double> *arr;
complex<double> *res;

int main(int argc, char **argv) {

    //cout << "work";
    Config config = readConfig();
    setUpData(config);
    bool debug = true;
    if(!debug){
        doInstructions(config);
    }
    MPI_Init(&argc,&argv);
    quantumFT();
    //createResultFile(config);
    applyResult();
    createResultFile(config);
    return 0;
}

void quantumFT(){

    int NRA = size;
    int NCA = size;
    int NCB = size;
    int MASTER = 0;
    int FROM_MASTER=1;
    int FROM_WORKER=2;


    int N = size;
    double nd = log2(N); 
    int n = (int)nd;
    double pow2 = pow(2.0f, n*1.0f);
    complex<double> zi = complex<double>(0, 1);
    complex<double> wn = exp(2*M_PI*zi/pow2);
    complex<double> wa[N*(N-1)];
    complex<double>	a[N][N];
    for(int i=0; i<=(N-1)*(N-1); i++) wa[i]=pow(wn, i);
    for(int i = 0; i<N; i++){
        for(int k = 0; k<N; k++)
            a[k][i]=wa[k*i];
    }


    int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	averow, extra, offset, /* used to determine rows sent to each worker */
	i, j, k, rc;           /* misc */
complex<double> c[NRA][NCB];           /* result matrix C */

MPI_Status status;

//MPI_Init(&argc,argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
  }
numworkers = numtasks-1;

printf("init done\n");
/**************************** master task ************************************/
   if (taskid == MASTER)
   {
      /* Measure start time */
      double start = MPI_Wtime();

      /* Send matrix data to the worker tasks */
      averow = NRA/numworkers;
      extra = NRA%numworkers;
      offset = 0;
      mtype = FROM_MASTER;
      for (dest=1; dest<=numworkers; dest++)
      {
         rows = (dest <= extra) ? averow+1 : averow;   	
         printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&arr, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         offset = offset + rows;
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
            
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         printf("Recived1\n");
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype, 
                  MPI_COMM_WORLD, &status);
         printf("Received results from task %d\n",source);
      }

      /* Print results */
      
      printf("******************************************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<NRA; i++)
      {
         printf("\n"); 
         for (j=0; j<NCB; j++) 
            cout << c[i][j];
            //printf("%6.2f   ", c[i][j]);
      }
      printf("\n******************************************************\n");
      

      /* Measure finish time */
      double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);
   }


/**************************** worker task ************************************/
   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&arr, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

      for (k=0; k<NCB; k++)
         for (i=0; i<rows; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<NCA; j++)
               c[i][k] = c[i][k] + a[i][j] * arr[k];
         }
      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
   }
   MPI_Finalize();
}

void quantumCNot(int first, int second){ //управляЮЩИЙ управляЕМЫЙ
    first--;
    second--;
    int len = log2(size);
    if(first<0) first = len-1;
    if(second<0) second = len-1;
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        if(ind[first]=='1') {
            ind[second]=binReverse(ind[second]);
            cout << binToDec(ind);
        };
        
        res[binToDec(ind)]=arr[i];
    }
}

void quantumCP(int first, int second){ //управляЮЩИЙ управляЕМЫЙ //TEST
    first--;
    second--;
    int len = log2(size);
    if(first<0) first = len-1;
    if(second<0) second = len-1;
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        if(ind[first]=='1') {
            ind[second]=binReverse(ind[second]);
        };
        cout << binToDec(ind);
        res[binToDec(ind)]=arr[i];
    }
}

void quantumWalsh(){
    int len = log2(size);
    for(int i=0; i<len; i++){
        quantumH(i);
        applyResult();
    }
}

void quantumS(int phase){
    int first = 2;
    int len = log2(size);
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        if(ind[first]=='1') {
            double ph = 1.0*phase;
            complex<double> add = exp(complex<double>(0,ph));
            //cout << arr[i];
            res[i]= add*arr[i];
        } else res[i]= arr[i];
    }
}

void quantumH(int first){
    first--;
    int len = log2(size);
    if(first<0) first = len-1;
    double h[2][2] = {{1/sqrt(2),1/sqrt(2)},{1/sqrt(2),-1/sqrt(2)}};
    
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        int vect[2]={1,0};
        if(ind[first]=='1') {vect[0]=0; vect[1]=1;};
        
        double vH[2];
        for(int j=0; j<2; j++){
            vH[j]=h[j][0]*vect[0] + h[j][1]*vect[1];
        }
        
        if(ind[first]=='1'){
            double temp = vH[1];
            vH[1]=vH[0];
            vH[0]=temp;
        }
        
        res[i]=0;
        res[i]+=complex<double>(vH[0],0)*arr[i];
        ind[first]=binReverse(ind[first]);
        res[i]+=complex<double>(vH[1],0)*arr[binToDec(ind)];
    }
}

void quantumZ(int first){
    first--;
    int len = log2(size);
    if(first<0) first = len-1;
    int z[2][2] = {{1,0},{0,-1}};
    
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        int vect[2]={1,0};
        if(ind[first]=='1') {vect[0]=0; vect[1]=1;};
        
        int vH[2];
        for(int j=0; j<2; j++){
            vH[j]=z[j][0]*vect[0] + z[j][1]*vect[1];
        }
        
        if(ind[first]=='1'){
            int temp = vH[1];
            vH[1]=vH[0];
            vH[0]=temp;
        }
        
        res[i]=0;
        res[i]+=complex<double>(vH[0],0)*arr[i];
        ind[first]=binReverse(ind[first]);
        res[i]+=complex<double>(vH[1],0)*arr[binToDec(ind)];
    }
}

void quantumY(int first){
    first--;
    int len = log2(size);
    if(first<0) first = len-1;
    complex<double> y[2][2] = {{complex<double>(0,0),complex<double>(0,-1)},{complex<double>(0,1),complex<double>(0,0)}};
    
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        int vect[2]={1,0};
        if(ind[first]=='1') {vect[0]=0; vect[1]=1;};
        
        complex<double> vH[2];
        for(int j=0; j<2; j++){
            vH[j]=y[j][0]*complex<double>(vect[0],0) + y[j][1]*complex<double>(vect[1],0);
        }
        
        if(ind[first]=='1'){
            complex<double> temp = vH[1];
            vH[1]=vH[0];
            vH[0]=temp;
        }
        
        res[i]=0;
        res[i]+=vH[0]*arr[i];
        ind[first]=binReverse(ind[first]);
        res[i]+=vH[1]*arr[binToDec(ind)];
    }
}

void quantumX(int first){
    first--;
    int len = log2(size);
    if(first<0) first = len-1;
    int x[2][2] = {{0,1},{1,0}};
    
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        int vect[2]={1,0};
        if(ind[first]=='1') {vect[0]=0; vect[1]=1;};
        
        int vH[2];
        for(int j=0; j<2; j++){
            vH[j]=x[j][0]*vect[0] + x[j][1]*vect[1];
        }
        
        if(ind[first]=='1'){
            int temp = vH[1];
            vH[1]=vH[0];
            vH[0]=temp;
        }
        
        res[i]=0;
        res[i]+=complex<double>(vH[0],0)*arr[i];
        ind[first]=binReverse(ind[first]);
        res[i]+=complex<double>(vH[1],0)*arr[binToDec(ind)];
    }
}

void applyResult(){
    for(int i = 0; i<size; i++){
        arr[i]=res[i]+complex<double>(0,0);
    }
    delete res;
    res = new complex<double> [size];
    
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    stringstream ss;
    ss.str(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void setUpData(Config config){
    string Text;
    ifstream input(config.get_dataPath()); // str path
    getline (input, Text);
    size = stoi(Text);
    res = new complex<double> [size];
    arr = new complex<double> [size];
    for(int i = 0; i<size; i++){
        getline (input, Text);
        vector<string> elems = split(Text, ' ');
        int ind = 0;
        double real = 0;
        double im = 0;
        for(auto e: elems){
            if(ind==0) real = stod(e); else im = stod(e);
            ind++;
        }
        arr[i]=complex<double>(real, im);
    }
    input.close();
}

string decToBin(int x, int len){ //bitset требует константу в аргумент
    string result = "";
    for(int i=0; i<len; i++){
        result = to_string(x % 2) + result;
        x /= 2;
    }
    return result;
}

int binToDec(string b){
    return stoi(b, nullptr, 2);
}

Config readConfig(){
    string Text;
    Config cfg;
    int linenmb = 0;
    ifstream ConfigFile("config"); // file in build folder
    while (getline (ConfigFile, Text)) {
       // cout << Text << endl;
        if (linenmb==0) cfg.set_instructionsPath(Text);
        if (linenmb==1) cfg.set_dataPath(Text);
        linenmb++;
    }
    ConfigFile.close();
    return cfg;
}
char binReverse(char x){
    if(x=='1') return '0'; else return '1';
}

void createResultFile(Config config){
    ofstream output(config.get_dataPath()+"-output");
    for(int i=0;i<size; i++){
        if(arr[i].imag()<0){
            output << arr[i].real()  << arr[i].imag() << "i" << endl;
        } else output << arr[i].real() << "+" << arr[i].imag() << "i" << endl;
    }
    output.close();
}

void doInstructions(Config config){
    string Text;
    ifstream input(config.get_instructionsPath()); // str path
    getline (input, Text);
    int size = stoi(Text);
    for(int i = 0; i<size; i++){
        getline (input, Text);
        vector<string> elems = split(Text, ' ');
        //почему split встроенного нет?
        int ind = 0;
        string instruction = "";
        int arg1 = -10;
        int arg2 = -10;
        for(auto e: elems){
            if(ind==0) instruction = e;
            if(ind==1) arg1 = stoi(e);
            if(ind==2) arg2 = stoi(e);
            ind++;
        }
        //почему switch не работает со string?
        if (instruction=="X") quantumX(arg1);
        if (instruction=="Y") quantumY(arg1);
        if (instruction=="Z") quantumZ(arg1);
        if (instruction=="H") quantumH(arg1);
        if (instruction=="WH") quantumWalsh();
        if (instruction=="CN") quantumCNot(arg1, arg2);
        if (instruction=="S") quantumS(arg1);
        applyResult();
    }
    createResultFile(config);
    delete arr;
    input.close();
}



