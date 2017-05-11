//
// Created by 朱曼青 on 2017/3/26.
//

#ifndef ASSEMBLER_REFERENCE_H
#define ASSEMBLER_REFERENCE_H

#include <string>
#include <map>
#include <vector>
using namespace std;

extern map<string, string> Reg_code;

extern map<string, vector<string> > instru_code;

extern map<string, int> Instru_id;

extern map<string, string> Code_reg;

extern map<string, string> Code_instru;

#endif //ASSEMBLER_REFERENCE_H
