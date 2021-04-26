//
//  Config.cpp
//  QuantumEmu
//
//  Created by Vladislav Dunaev on 08.04.2021.
//  Copyright © 2021 Vladislav Dunaev. All rights reserved.
//
#include <iostream>

#define arraysize 9999999 // 26 кубитов 9

using namespace std;
class Config {
public:
    string get_dataPath(){
        return dataPath;
    }
    string get_instructionsPath(){
        return dataPath;
    }
    void set_dataPath(string path){
        dataPath=path;
    }
    void set_instructionsPath(string path){
        instructionsPath=path;
    }
private:
    string dataPath;
    string instructionsPath;
};
