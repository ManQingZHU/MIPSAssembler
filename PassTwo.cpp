#include "Parser.h"
#include "Reference.h"
#include "TranseDigit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <string>
#include <QTextStream>
#include <QIODevice>

void Parser::PassTwo()
{
    QString content = w.ui->InputTextEdit->toPlainText();
    QTextStream temp_text(&content, QIODevice::ReadOnly);
    QString ql;

    pc = 0;
    string line, op, BinCode;
    int op_id;
    string rs, rt, rd, code, shamt, imm, Label;
    string::iterator str_iter;
    map<long int, string>::const_iterator m_li_str_iter;
    map<long int, string>::const_iterator m_li_str_iter_end = DataBlock.end();
    size_t  pos;

    while(!temp_text.atEnd())
    {
        ql = temp_text.readLine();
        line = ql.toStdString();

        if(line.length() == 0)
            continue;

        DropComment(line);                // delete comments

        if(line.find(".") != string::npos)   // data declaration
        {
            m_li_str_iter = DataBlock.find(pc);
            if(m_li_str_iter != m_li_str_iter_end)                // adjust pc
                pc += ((m_li_str_iter->second).length() / 8);

            continue;
        }

        pos = line.find(":");             // delete label
        if(pos != string::npos)
            line = line.substr(pos+1);

        str_iter = line.begin();
        op.clear();
        BinCode.clear();
        for(;str_iter != line.end() && (*str_iter == ' ' || *str_iter == '\t'); ++str_iter)
            ;
        for(;str_iter != line.end(); ++str_iter)
        {
            if(*str_iter == ' ' || *str_iter == '\t' || *str_iter == ';')
                break;
            op += *str_iter;        // get opcode
        }
        if(instru_code.find(op) == instru_code.end())
            continue;

        if(instru_code[op].size() == 4)
        {
            op_id = Instru_id[op];

            if(op_id == 49 || op_id == 50)
            {
                rt.clear();
                rd.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rt += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    rd += *str_iter;
                }
                BinCode = (instru_code[op])[0] + Reg_code[rt] + Reg_code[rd] + (instru_code[op])[3];
            }
            else if(op_id == 31)
            {
                rt.clear();
                imm.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rt += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    imm += *str_iter;
                }
                BinCode = (instru_code[op])[0] + (instru_code[op])[1] + Reg_code[rt] + Imm(imm);
            }
            else if(op_id >= 22 && op_id <= 25)
            {
                rs.clear();
                rt.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rs += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    rt += *str_iter;
                }
                BinCode = (instru_code[op])[0] + Reg_code[rs] + Reg_code[rt] + (instru_code[op])[3];
            }
            else if(op_id >= 26 && op_id <= 33)
            {
                rt.clear();
                rs.clear();
                imm.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rt += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rs += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    imm += *str_iter;
                }
                if(op_id >= 28 && op_id <= 30)
                    BinCode = (instru_code[op])[0] + Reg_code[rs] + Reg_code[rt] + UnImm(imm);
                else BinCode = (instru_code[op])[0] + Reg_code[rs] + Reg_code[rt] + Imm(imm);
            }
            else if(op_id >= 1 && op_id <= 8)
            {
                rt.clear();
                imm.clear();
                rs.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rt += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == '(') break;
                    imm += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ')') break;
                    rs += *str_iter;
                }

                BinCode = (instru_code[op])[0] + Reg_code[rs] + Reg_code[rt] + Imm(imm);
            }
            else{
                rs.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rs += *str_iter;
                }
                str_iter++;
                if(op_id == 34 || op_id == 35)
                {
                    rt.clear();
                    for (; str_iter != line.end(); ++str_iter) {
                        if (*str_iter == ' ' || *str_iter == '\t') continue;
                        if (*str_iter == ',') break;
                        rt += *str_iter;
                    }
                    str_iter++;
                }
                Label.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    Label += *str_iter;
                }
                BinCode = (instru_code[op])[0] + Reg_code[rs]
                        + ((op_id == 34 || op_id == 35)? Reg_code[rt]:(instru_code[op])[2])
                        + BranchOff(Label, pc);
            }
        }
        else if(instru_code[op].size() == 5)
        {
            rd.clear();
            op_id = Instru_id[op];
            if(op_id >= 44 && op_id <= 47)
            {
                for(;str_iter != line.end(); ++str_iter)
                {
                    if(*str_iter == ' ' || *str_iter == '\t') continue;
                    if(*str_iter == ';') break;
                    rd += *str_iter;
                }
                BinCode = (instru_code[op])[0] + (instru_code[op])[1] + (instru_code[op])[2]
                        + Reg_code[rd] + (instru_code[op])[4];
            }
            else if(op_id == 54 || op_id == 42 ||op_id == 43) {
                rs.clear();
                if (op_id == 54) {
                    for (; str_iter != line.end(); ++str_iter) {
                        if (*str_iter == ' ' || *str_iter == '\t') continue;
                        if (*str_iter == ',') break;
                        rd += *str_iter;
                    }
                    str_iter++;
                    for (; str_iter != line.end(); ++str_iter) {
                        if (*str_iter == ' ' || *str_iter == '\t') continue;
                        if (*str_iter == ';') break;
                        rs += *str_iter;
                    }
                } else if (op_id == 42) {
                    for (; str_iter != line.end(); ++str_iter) {
                        if (*str_iter == ' ' || *str_iter == '\t') continue;
                        if (*str_iter == ',') break;
                        rs += *str_iter;
                    }
                    str_iter++;
                    for (; str_iter != line.end(); ++str_iter) {
                        if (*str_iter == ' ' || *str_iter == '\t') continue;
                        if (*str_iter == ';') break;
                        rd += *str_iter;
                    }
                } else {
                    for (; str_iter != line.end(); ++str_iter) {
                        if (*str_iter == ' ' || *str_iter == '\t') continue;
                        if (*str_iter == ';') break;
                        rs += *str_iter;
                    }
                }

                BinCode = (instru_code[op])[0] + Reg_code[rs] + (instru_code[op])[2]
                          + ((op_id == 43) ? (instru_code[op])[3] : Reg_code[rd])
                          + (instru_code[op])[4];
            }
            else if(op_id >= 9 && op_id <= 18)
            {
                rd.clear();
                rs.clear();
                rt.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rd += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rs += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    rt += *str_iter;
                }

                BinCode = (instru_code[op])[0] + Reg_code[rs] + Reg_code[rt] + Reg_code[rd] + (instru_code[op])[4];
            }
            else{
                rd.clear();
                rt.clear();
                shamt.clear();
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rd += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ',') break;
                    rt += *str_iter;
                }
                str_iter++;
                for (; str_iter != line.end(); ++str_iter) {
                    if (*str_iter == ' ' || *str_iter == '\t') continue;
                    if (*str_iter == ';') break;
                    shamt += *str_iter;
                }
                BinCode = (instru_code[op])[0] + Reg_code[rt] + Reg_code[rd] + Shamt(shamt) +(instru_code[op])[4];
            }
        }
        else if(instru_code[op].size() == 2)
        {
            Label.clear();
            for (; str_iter != line.end(); ++str_iter) {
                if (*str_iter == ' ' || *str_iter == '\t') continue;
                if (*str_iter == ';') break;
                Label += *str_iter;
            }

            BinCode = (instru_code[op])[0] + JumpOff(Label);
        }
        else if(instru_code[op].size() == 7)
        {
            rd.clear();
            imm.clear();
            op_id = Instru_id[op];
            for (; str_iter != line.end(); ++str_iter) {
                if (*str_iter == ' ' || *str_iter == '\t') continue;
                if (*str_iter == ',') break;
                rd += *str_iter;
            }
            str_iter++;
            for (; str_iter != line.end(); ++str_iter) {
                if (*str_iter == ' ' || *str_iter == '\t') continue;
                if (*str_iter == ';') break;
                imm += *str_iter;
            }
            if(op_id == 55) {
                BinCode = (instru_code[op])[0] + Reg_code[rd] + LiImm(imm,1)
                        + (instru_code[op])[3] + Reg_code[rd] + Reg_code[rd] + LiImm(imm, 2);
            }
            else{
                BinCode = (instru_code[op])[0] + (instru_code[op])[1] + LaImm(imm,1)
                          + (instru_code[op])[3] + Reg_code[rd] + (instru_code[op])[5] + LaImm(imm, 2);
            }
        }
        else if(instru_code[op].size() == 3)
        {
            code.clear();
            for(;str_iter != line.end(); ++str_iter)
            {
                if(*str_iter == ' ' || *str_iter == '\t') continue;
                if(*str_iter == ';') break;
                code += *str_iter;
            }
            BinCode = (instru_code[op])[0] + BreakCode(code) + (instru_code[op])[2];
        }
        else if(instru_code[op].size() == 1)
        {
            BinCode = (instru_code[op])[0];
        }

        // store into memory
        for(unsigned long j = 0; j <= BinCode.size() - 8; j = j+8) {
            Memory[pc++] = BinCode.substr(j, 8);
        }

    }

}
