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

using namespace std;

Config readConfig();
string decToBin(int, int);
void setUpData(Config);
int binToDec(string);

complex<double> arr[arraysize];
//complex<double> arr2[9999999];
int main(int argc, const char * argv[]) {
    Config config = readConfig();
    setUpData(config);
    //cout << config.get_dataPath() << endl;
//    cout << decToBin(54, 7) << endl;
//    string st = decToBin(54, 7);
//    cout << binToDec(st) << endl;
    return 0;
}

void setUpData(Config config){
    string Text;
    ifstream input(config.get_dataPath()); // str path
    int count = getline (input, Text);
    while (getline (input, Text)) {
        cout << Text << endl;
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


