#include "DisAssembly.h"
#include "Reference.h"
#include "TranseDigit.h"
#include "ui_mainwindow.h"
#include <QString>
#include <string>
#include <QTextStream>
#include <QIODevice>

void DisAssembly::DeBin()
{
    QString content = w.ui->InputTextEdit->toPlainText();
    QTextStream temp_text(&content, QIODevice::ReadOnly);
    QString ql;

    string Bincode;
    string rs, rt, rd, opcode, func, shamt, imm, instruct, BreCode, Label;
    int op_id;
    map<string, string>::iterator m_iter;
    map<string, string>::iterator m_iter_end = Code_instru.end();

    while(!temp_text.atEnd())
    {
        Bincode.clear();
        ql = temp_text.readLine();
        Bincode = ql.toStdString();

        if(Bincode.size() == 0)
            continue;

        opcode.clear();
        func.clear();

        opcode = Bincode.substr(0, 6);
        func = Bincode.substr(26,6);
        m_iter = Code_instru.find(opcode);
        if(m_iter == m_iter_end || (m_iter->first == "010000"))
            m_iter = Code_instru.find(opcode+"+"+func);
        if(m_iter != m_iter_end)
        {
            instruct.clear();
            instruct = m_iter->second;

            if(instru_code[instruct].size() == 1)
                ;
            else if(instru_code[instruct].size() == 3){
                BreCode.clear();
                BreCode = Bincode.substr(6, 20);
                instruct += ' ';
                instruct += BreakInt(BreCode);
            }
            else if(instru_code[instruct].size() == 5)
            {
                op_id = Instru_id[instruct];
                if(op_id >= 44 && op_id <= 47)
                {
                    rd.clear();
                    rd = Bincode.substr(16,5);
                    instruct += ' ';
                    instruct += Code_reg[rd];
                }
                else if(op_id == 43)
                {
                    rs.clear();
                    rs = Bincode.substr(6,5);
                    instruct += ' ';
                    instruct += Code_reg[rs];
                }
                else if(op_id == 42)
                {
                    rs.clear();
                    rd.clear();
                    rs = Bincode.substr(6,5);
                    rd = Bincode.substr(16,5);
                    instruct += ' ';
                    instruct += Code_reg[rd];
                    instruct += ", ";
                    instruct += Code_reg[rs];
                }
                else if(op_id >= 9 && op_id <= 18)
                {
                    rs.clear();
                    rd.clear();
                    rt.clear();
                    rs = Bincode.substr(6,5);
                    rt = Bincode.substr(11,5);
                    rd = Bincode.substr(16,5);

                    instruct += ' ';
                    instruct += Code_reg[rd];
                    instruct += ", ";
                    instruct += Code_reg[rs];
                    instruct += ", ";
                    instruct += Code_reg[rt];
                }
                else{
                    rt.clear();
                    rd.clear();
                    shamt.clear();
                    rt = Bincode.substr(11,5);
                    rd = Bincode.substr(16,5);
                    shamt = Bincode.substr(21,5);

                    instruct += ' ';
                    instruct += Code_reg[rd];
                    instruct += ", ";
                    instruct += Code_reg[rt];
                    instruct += ", ";
                    instruct += ImmInt(shamt);
                }
            }
            else if(instru_code[instruct].size() == 4)
            {
                op_id = Instru_id[instruct];
                if(op_id == 31)
                {
                    rt.clear();
                    imm.clear();
                    rt = Bincode.substr(11,5);
                    imm = Bincode.substr(16, 16);

                    instruct += ' ';
                    instruct += Code_reg[rt];
                    instruct += ", ";
                    instruct += LuiImm(imm);
                }
                else if(op_id >= 26 && op_id <= 33)
                {
                    rs.clear();
                    rt.clear();
                    imm.clear();
                    rs = Bincode.substr(6,5);
                    rt = Bincode.substr(11,5);
                    imm = Bincode.substr(21, 16);

                    instruct += ' ';
                    instruct += Code_reg[rt];
                    instruct += ", ";
                    instruct += Code_reg[rs];
                    instruct += ", ";
                    instruct += ImmInt(imm);
                }
                else if(op_id >= 49 && op_id <= 50)
                {
                    rd.clear();
                    rt.clear();
                    rt = Bincode.substr(11,5);
                    rd = Bincode.substr(16,5);

                    instruct += ' ';
                    instruct += Code_reg[rt];
                    instruct += ", ";
                    instruct += Code_reg[rd];
                }
                else if(op_id >= 22 && op_id <= 25)
                {
                    rs.clear();
                    rt.clear();
                    rs = Bincode.substr(6,5);
                    rt = Bincode.substr(11,5);

                    instruct += ' ';
                    instruct += Code_reg[rt];
                    instruct += ", ";
                    instruct += Code_reg[rs];
                }
                else if(op_id >= 1 && op_id <= 8)
                {
                    rs.clear();
                    rt.clear();
                    imm.clear();
                    rs = Bincode.substr(6,5);
                    rt = Bincode.substr(11,5);
                    imm = Bincode.substr(21, 16);

                    instruct += ' ';
                    instruct += Code_reg[rt];
                    instruct += ", ";
                    instruct += ImmInt(imm);
                    instruct += '('+Code_reg[rs]+')';
                }
                else{
                    rs.clear();
                    imm.clear();
                    rs = Bincode.substr(6,5);
                    imm = Bincode.substr(21, 16);
                    if(op_id == 34 || op_id == 35)
                    {
                        rt.clear();
                        rt = Bincode.substr(11,5);
                    }

                    instruct += ' ';
                    instruct += Code_reg[rs];
                    instruct += ", ";
                    if(op_id == 34 || op_id == 35)
                    {
                        instruct += Code_reg[rt];
                        instruct += ", ";
                    }
                    Label.clear();
                    Label = "Label"+ IntToStr(Labelnum);
                    instruct += Label;
                    LabelTable.insert(map<string, long int>::value_type(Label, (pc+4+ (BinToInt(imm) << 2))) );
                    Labelnum++;
                }
            }
            else{
                imm.clear();
                imm = Bincode.substr(6, 26);
                instruct += ' ';
                Label.clear();
                Label = "Label"+ IntToStr(Labelnum);
                instruct += Label;
                LabelTable.insert(map<string, long int>::value_type(Label, (((pc+4)&0xF0000000 )+ (BinToInt(imm) << 2))) );
                Labelnum++;
            }
            Instru.push_back(instruct);
            pc += 4;
        }
    }
    end_pc = pc;
}

void DisAssembly::DeCoe()
{
    QString content = w.ui->InputTextEdit->toPlainText();
    QTextStream temp_text(&content, QIODevice::ReadOnly);
    QString ql;

    string line;
    string Bincode;
    string rs, rt, rd, opcode, func, shamt, imm, instruct, BreCode, Label;
    int op_id;
    string::iterator str_iter;
    map<string, string>::iterator m_iter;
    map<string, string>::iterator m_iter_end = Code_instru.end();

    while(!temp_text.atEnd()) {
        line.clear();
        ql = temp_text.readLine();
        line = ql.toStdString();

        if (line.size() == 0)
            continue;
        if (line[0] == 'm')
            continue;

        for (str_iter = line.begin(); str_iter != line.end(); ++str_iter) {

            if (*str_iter == ' ')
                continue;
            else if (*str_iter == ',' || *str_iter == ';') {
                Bincode = CoeToBin(Bincode);

                opcode.clear();
                func.clear();

                opcode = Bincode.substr(0, 6);
                func = Bincode.substr(26, 6);
                m_iter = Code_instru.find(opcode);
                if (m_iter == m_iter_end || (m_iter->first == "010000"))
                    m_iter = Code_instru.find(opcode + "+" + func);
                if (m_iter != m_iter_end) {
                    instruct.clear();
                    instruct = m_iter->second;

                    if (instru_code[instruct].size() == 1);
                    else if (instru_code[instruct].size() == 3) {
                        BreCode.clear();
                        BreCode = Bincode.substr(6, 20);
                        instruct += ' ';
                        instruct += BreakInt(BreCode);
                    } else if (instru_code[instruct].size() == 5) {
                        op_id = Instru_id[instruct];
                        if (op_id >= 44 && op_id <= 47) {
                            rd.clear();
                            rd = Bincode.substr(16, 5);
                            instruct += ' ';
                            instruct += Code_reg[rd];
                        } else if (op_id == 43) {
                            rs.clear();
                            rs = Bincode.substr(6, 5);
                            instruct += ' ';
                            instruct += Code_reg[rs];
                        } else if (op_id == 42) {
                            rs.clear();
                            rd.clear();
                            rs = Bincode.substr(6, 5);
                            rd = Bincode.substr(16, 5);
                            instruct += ' ';
                            instruct += Code_reg[rd];
                            instruct += ", ";
                            instruct += Code_reg[rs];
                        } else if (op_id >= 9 && op_id <= 18) {
                            rs.clear();
                            rd.clear();
                            rt.clear();
                            rs = Bincode.substr(6, 5);
                            rt = Bincode.substr(11, 5);
                            rd = Bincode.substr(16, 5);

                            instruct += ' ';
                            instruct += Code_reg[rd];
                            instruct += ", ";
                            instruct += Code_reg[rs];
                            instruct += ", ";
                            instruct += Code_reg[rt];
                        } else {
                            rt.clear();
                            rd.clear();
                            shamt.clear();
                            rt = Bincode.substr(11, 5);
                            rd = Bincode.substr(16, 5);
                            shamt = Bincode.substr(21, 5);

                            instruct += ' ';
                            instruct += Code_reg[rd];
                            instruct += ", ";
                            instruct += Code_reg[rt];
                            instruct += ", ";
                            instruct += ImmInt(shamt);
                        }
                    } else if (instru_code[instruct].size() == 4) {
                        op_id = Instru_id[instruct];
                        if (op_id == 31) {
                            rt.clear();
                            imm.clear();
                            rt = Bincode.substr(11, 5);
                            imm = Bincode.substr(16, 16);

                            instruct += ' ';
                            instruct += Code_reg[rt];
                            instruct += ", ";
                            instruct += LuiImm(imm);
                        } else if (op_id >= 26 && op_id <= 33) {
                            rs.clear();
                            rt.clear();
                            imm.clear();
                            rs = Bincode.substr(6, 5);
                            rt = Bincode.substr(11, 5);
                            imm = Bincode.substr(21, 16);

                            instruct += ' ';
                            instruct += Code_reg[rt];
                            instruct += ", ";
                            instruct += Code_reg[rs];
                            instruct += ", ";
                            instruct += ImmInt(imm);
                        } else if (op_id >= 49 && op_id <= 50) {
                            rd.clear();
                            rt.clear();
                            rt = Bincode.substr(11, 5);
                            rd = Bincode.substr(16, 5);

                            instruct += ' ';
                            instruct += Code_reg[rt];
                            instruct += ", ";
                            instruct += Code_reg[rd];
                        } else if (op_id >= 22 && op_id <= 25) {
                            rs.clear();
                            rt.clear();
                            rs = Bincode.substr(6, 5);
                            rt = Bincode.substr(11, 5);

                            instruct += ' ';
                            instruct += Code_reg[rt];
                            instruct += ", ";
                            instruct += Code_reg[rs];
                        } else if (op_id >= 1 && op_id <= 8) {
                            rs.clear();
                            rt.clear();
                            imm.clear();
                            rs = Bincode.substr(6, 5);
                            rt = Bincode.substr(11, 5);
                            imm = Bincode.substr(21, 16);

                            instruct += ' ';
                            instruct += Code_reg[rt];
                            instruct += ", ";
                            instruct += ImmInt(imm);
                            instruct += '(' + Code_reg[rs] + ')';
                        } else {
                            rs.clear();
                            imm.clear();
                            rs = Bincode.substr(6, 5);
                            imm = Bincode.substr(21, 16);
                            if (op_id == 34 || op_id == 35) {
                                rt.clear();
                                rt = Bincode.substr(11, 5);
                            }

                            instruct += ' ';
                            instruct += Code_reg[rs];
                            instruct += ", ";
                            if (op_id == 34 || op_id == 35) {
                                instruct += Code_reg[rt];
                                instruct += ", ";
                            }
                            Label.clear();
                            Label = "Label" + IntToStr(Labelnum);
                            instruct += Label;
                            LabelTable.insert(
                                    map<string, long int>::value_type(Label, (pc + 4 + (BinToInt(imm) << 2))));
                            Labelnum++;
                        }
                    } else {
                        imm.clear();
                        imm = Bincode.substr(6, 26);
                        instruct += ' ';
                        Label.clear();
                        Label = "Label" + IntToStr(Labelnum);
                        instruct += Label;
                        LabelTable.insert(map<string, long int>::value_type(Label, (((pc + 4) & 0xF0000000) +
                                                                                    (BinToInt(imm) << 2))));
                        Labelnum++;
                    }
                    Instru.push_back(instruct);
                    pc += 4;
                }
                Bincode.clear();
            }
            else Bincode += *str_iter;
        }
    }
        end_pc = pc;
    }

void DisAssembly::OutputAsm()
    {
        w.ui->OutputTextEdit->clear();
        QString ql;
        int index = 0;
        while(index != end_pc/4)
        {
            ql = QString::fromStdString(Instru[index]+";");
            w.ui->OutputTextEdit->appendPlainText(ql);
            index++;
        }
    }

void DisAssembly::InsertLabel()
    {
        int index = 0;
        string Label;
        while(index != Labelnum)
        {
            Label.clear();
            Label = "Label" + IntToStr(index);
            Instru[(LabelTable[Label])/4] = Label + ": " + Instru[(LabelTable[Label])/4];
            index++;
        }
    }
void DisAssembly::ReadInCoe_forBin()
{
    QString content = w.ui->InputTextEdit->toPlainText();
    QTextStream temp_text(&content, QIODevice::ReadOnly);
    QString ql;

    string line;
    string Bincode;
    string::iterator str_iter;
    while(!temp_text.atEnd()) {
        line.clear();
        ql = temp_text.readLine();
        line = ql.toStdString();

        if (line.size() == 0)
            continue;
        if (line[0] == 'm')
            continue;

        for (str_iter = line.begin(); str_iter != line.end(); ++str_iter) {

            if (*str_iter == ' ')
                continue;
            else if (*str_iter == ',' || *str_iter == ';') {
                Bincode = CoeToBin(Bincode);
                Instru.push_back(Bincode);
                pc += 4;
                Bincode.clear();
            }
            else Bincode += *str_iter;
        }
    }
    end_pc = pc;
}
void DisAssembly::CoeOutputBin()
{
    w.ui->OutputTextEdit->clear();
    QString ql;
    int index = 0;
    while(index != end_pc/4)
    {
        ql = QString::fromStdString(Instru[index]);
        w.ui->OutputTextEdit->appendPlainText(ql);
        index++;
    }
}
