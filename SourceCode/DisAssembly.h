//
// Created by 朱曼青 on 2017/4/2.
//

#ifndef ASSEMBLER_DISASSEMBLY_H
#define ASSEMBLER_DISASSEMBLY_H

#include <string>
#include <vector>
#include <map>
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

extern class MainWindow;

class DisAssembly {
public:
    DisAssembly(MainWindow& mw):pc(0), Labelnum(0), w(mw){

    };
    virtual ~DisAssembly() { }
    void DeBin();
    void DeCoe();
    void InsertLabel();
    void OutputAsm();
    void ReadInCoe_forBin();
    void CoeOutputBin();
private:
    long int pc, end_pc;
    vector<string> Instru;
    int Labelnum;
    map<string, long int> LabelTable;
    MainWindow& w;
};


#endif //ASSEMBLER_DISASSEMBLY_H
