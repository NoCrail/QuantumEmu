//
//  main.cpp
//  QuantumEmu
//
//  Created by Vladislav Dunaev on 06.04.2021.
//  Copyright © 2021 Vladislav Dunaev. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "Config.h"
#include <complex>
#include <sstream>
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
char binReverse(char);
void applyResult();
void quantumWalsh();
void quantumCNot(int,int);
void doInstructions(Config);

complex<double> arr[arraysize];

//int *pint = new int(1797783552);


int size = 0;
complex<double> res[arraysize];

int main(int argc, const char * argv[]) {
    Config config = readConfig();
    setUpData(config);
    bool debug = true;
    //cout << arr[0];
    //quantumX(1); //X(2)
    //quantumY(-1);
    //quantumZ(0);
    //quantumH(0);
    //quantumWalsh();
    //quantumCNot(1, 2);
    //std::string bin("1024");
    //int dec = std::stoi(bin, nullptr, 10); // dec = 13
    //cout << bitset<10>(32);
    //cout << dec;
    //cout << size;

    if(!debug){
        doInstructions(config);
    }
    cout << res[0];
    return 0;
    
    
}

void quantumCNot(int first, int second){ //управляемЫЙ управляЮЩИЙ
    //first--; second--;
    int len = log2(size);
    if(first<0) first = len-1;
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        if(ind[first]=='1') {
            ind[second]=binReverse(ind[second]);
        };
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

void quantumH(int first){ //какаято муть с first для 0 кубита -1 для 1 - 0 итд проверял пока только для кубита 1 по питону
    
    first--; //!!!!!!!! TODO TODO
    
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

void quantumZ(int first){ //какаято муть с first для 0 кубита -1 для 1 - 0 итд проверял пока только для кубита 1 по питону
    
    first--; //!!!!!!!! TODO TODO
    
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

void quantumY(int first){ //какаято муть с first для 0 кубита -1 для 1 - 0 итд проверял пока только для кубита 1 по питону
    
    first--; //!!!!!!!! TODO TODO
    
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

void quantumX(int first){ //какаято муть с first для 0 кубита -1 для 1 - 0 итд проверял пока только для кубита 1 по питону
    
    first--; //!!!!!!!! TODO TODO
    
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
    copy(begin(res), end(res), begin(arr));
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
    for(int i = 0; i<size; i++){
        getline (input, Text);
        vector<string> elems = split(Text, ' ');
        int ind = 0;
        double real = 0;
        double im = 0;
        for(auto e: elems){
            if(ind==0) real = stod(e); else im = stod(e);
            ind++;
            //cout << e << endl;
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
int binToDec(string b){ //эффективнее найти
    /*int pow = 1;
    int res = 0;
    int len = b.length()-1;
    for(int i = len; i>=0; i--){
        //cout << ((int)b[i]-'0')*pow << endl;
        res += ((int)b[i]-'0')*pow;
        pow *=2;
    }
    return res;*/
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

void doInstructions(Config config){
    string Text;
    ifstream input(config.get_instructionsPath()); // str path
    getline (input, Text);
    int size = stoi(Text);
    for(int i = 0; i<size; i++){
        getline (input, Text);
        vector<string> elems = split(Text, ' ');
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
        if (instruction=="X") quantumX(arg1);
        if (instruction=="Y") quantumY(arg1);
        if (instruction=="Z") quantumZ(arg1);
        if (instruction=="H") quantumH(arg1);
        if (instruction=="WH") quantumWalsh();
        if (instruction=="CN") quantumCNot(arg1, arg2);
        applyResult();
        
        //out << Text << endl;
    }
    input.close();
}

