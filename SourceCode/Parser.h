#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

extern class MainWindow;

class Parser {
public:
    Parser(MainWindow& mw):pc(0), w(mw)
    {
        Memory.clear();
        symbolTable.clear();
        DataBlock.clear();
    };
    virtual ~Parser(){ }
    void PassOne();
    void UpdateMemo();
    void PassTwo();
    void PrintCode();
    void PrintCOE();

    void ReadBin_forCoe();
    void BinOtputCoe();

private:
    string BranchOff(string& s, long int pc);
    string JumpOff(string& s);
    string LaImm(string& s, int n);
    void InsertLabel(string& s, long int loc);
    long int pc, end_addr;
    vector<string> Memory;   // store binary code 8-bit
    map<string, long int> symbolTable;  // Label and constant
    map<long int, string> DataBlock; // string is 8-bit, 16-bit, 32-bit binary code
    MainWindow& w;
};

void DropComment(string& s);

#endif //ASSEMBLER_PARSER_H
