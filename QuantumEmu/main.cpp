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


complex<double> arr[arraysize];
int size = 0;
//complex<double> arr2[9999999];
int main(int argc, const char * argv[]) {
    Config config = readConfig();
    setUpData(config);

    return 0;
}

//Метод стоит переименовать в чтото связанное с Х
void quantumMatrix(int first){ //какаято муть с first для 0 кубита -1 для 1 - 0 итд
    int len = log2(size);
    int x[2][2] = {{0,1},{1,0}};
    
    for(int i = 0; i<size; i++){
        string ind = decToBin(i, len);
        int vect[2]={1,0};
        if(ind[first]=='1') {vect[0]=0; vect[1]=1;};
        
        int vH[2];
        for(int j=0; j<2; j++){
            vH[j]=x[j][0]*vect[0] + x[j][1]*vect[1];
        }
    }
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
        //out << Text << endl;
    }
    input.close();
}
string decToBin(int x, int len){
    string result = "";
    for(int i=0; i<len; i++){
        result = to_string(x % 2) + result;
        x /= 2;
    }
    return result;
}
int binToDec(string b){
    int pow = 1;
    int res = 0;
    int len = b.length()-1;
    for(int i = len; i>=0; i--){
        cout << ((int)b[i]-'0')*pow << endl;
        res += ((int)b[i]-'0')*pow;
        pow *=2;
    }
    return res;
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



