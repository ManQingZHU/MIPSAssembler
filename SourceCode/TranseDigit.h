//
// Created by 朱曼青 on 2017/3/30.
//

#ifndef ASSEMBLER_TRANSEDIGIT_H
#define ASSEMBLER_TRANSEDIGIT_H

#include <string>
#include <stack>
using namespace std;

string HexToBin(const string &s);   // don't deal with 0x
long int HexToInt(const string& s);  // don't deal with 0x
string IntToBin(long int i, int n);   // include sign-ext
string UnIntToBin(long int i, int n);
long int BinToUnInt(string &s);
int BinToInt(string &s);
string IntToStr(int i);
long int GetAddr(string& s);
string DefByte(string &s);
string DefWord(string &s);
string DefD(string &s);
string BreakCode(string& s);
string Shamt(string &s);
string Imm(string& s);
string UnImm(string& s);
string LiImm(string& s, int n);
string BreakInt(string& s);
string ImmInt(string &s);
string LuiImm(string &s);
string UnIntToHex(long int i);
string CoeToBin(string& s);
#endif //ASSEMBLER_TRANSEDIGIT_H
