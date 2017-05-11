#include "Parser.h"
#include "TranseDigit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <string>
#include <QTextStream>
#include <QIODevice>

long int Hex_Int_To_Int(string& s);
string Hex_Int_Asc_To_Bin(string& s);

void Parser::PassOne()
{
    QString content = w.ui->InputTextEdit->toPlainText();
    QTextStream temp_text(&content, QIODevice::ReadOnly);
    QString ql;

    bool instru;
    string line, data;
    long int data_value, data_add;
    size_t pos;
    string::iterator str_iter;

    while(!temp_text.atEnd())
    {
        ql = temp_text.readLine();
        line = ql.toStdString();

        instru = true;

        if(line.length() == 0)            //   空行
            continue;

        DropComment(line);                // delete comments
        if(line.length() == 0)            //   空行
            continue;

        // .BaseAddre    默认地址一定是16进制数
        pos = (line.find(".BaseAddre") != string::npos) ? line.find(".BaseAddre"): line.find(".baseAddre");
        if(pos != string::npos)
        {
            line = line.substr(pos+11);
            data_add = GetAddr(line);
            if(pc > data_add)
            {
                cout << "Invalid Base Address declaration!" << endl;
                pc = 0;
                return;
            }
            pc = data_add;
            instru = false;
        }

        // DataAddre
        pos = (line.find(".DataAddre") != string::npos) ? line.find(".DataAddre"): line.find(".dataAddre");
        if(pos != string::npos)
        {
            line = line.substr(pos+11);
            data_add = GetAddr(line);
            if(pc > data_add)
            {
                cout << "Invalid Data Address declaration!" << endl;
                pc = 0;
                return;
            }
            pc = data_add;
            instru = false;
        }

        // Label
        InsertLabel(line, pc);

        //DB                         //  只能定义ascii 或 hex, hex 默认不做扩展
        pos = line.find(".db") != string::npos ? line.find(".db") : line.find(".DB");
        if(pos != string::npos)
        {

            line = line.substr(pos+4);
            str_iter = line.begin();
            data.clear();
            for(;str_iter != line.end(); ++str_iter)
            {
                if(*str_iter == ' ' || *str_iter == '\t')
                    continue;
                else if((*str_iter == ';' || *str_iter == ',') && data.length() != 0)
                {
                    data = DefByte(data);
                    DataBlock.insert(map<long int, string>::value_type(pc, data));
                    pc += (data.length() / 8);
                    data.clear();
                    if(*str_iter == ';')
                        break;
                }
                else data += *str_iter;
            }

            instru = false;
        }

        //.DW
        pos = line.find(".dw") != string::npos ? line.find(".dw") : line.find(".DW");
        if(pos != string::npos)
        {
            line = line.substr(pos+4);
            str_iter = line.begin();
            data.clear();
            data.clear();
            for(;str_iter != line.end(); ++str_iter)
            {
                if(*str_iter == ' ' || *str_iter == '\t')
                    continue;
                else if((*str_iter == ';' || *str_iter == ',') && data.length() != 0)
                {
                    data = DefWord(data);
                    DataBlock.insert(map<long int, string>::value_type(pc, data));
                    pc += (data.length() / 8);
                    data.clear();
                    if(*str_iter == ';')
                        break;
                }
                else data += *str_iter;
            }

            instru = false;
        }

        //.DD
        pos = line.find(".dd") != string::npos ? line.find(".dd") : line.find(".DD");
        if(pos != string::npos)
        {
            line = line.substr(pos+4);
            str_iter = line.begin();
            data.clear();
            data.clear();
            for(;str_iter != line.end(); ++str_iter)
            {
                if(*str_iter == ' ' || *str_iter == '\t')
                    continue;
                else if((*str_iter == ';' || *str_iter == ',') && data.length() != 0)
                {
                    data = DefD(data);
                    DataBlock.insert(map<long int, string>::value_type(pc, data));
                    pc += (data.length() / 8);
                    data.clear();
                    if(*str_iter == ';')
                        break;
                }
                else data += *str_iter;
            }

            instru = false;
        }


        //instruction
        if(instru && line.find(";") != string::npos) {
            if (line.find("la") != string::npos || line.find("li") != string::npos)
                pc += 8;
            else pc += 4;
        }

    }
    end_addr = pc;
}

/****************************************************************************************************/

void DropComment(string& s)
{
    size_t pos = s.find("//");
    if(pos != string::npos)
        s = s.substr(0, pos);
    pos = s.find("#");
    if(pos != string::npos)
        s = s.substr(0, pos);
}



void Parser::InsertLabel(string& s, long int loc)
{
    size_t  pos = s.find(":");
    if(pos == string::npos)
        return;

    string Label;
    string::iterator str_iter = s.begin();
    for(; str_iter != s.end() && (*str_iter == ' ' || *str_iter == '\t'); ++str_iter)
        ;
    for(; str_iter != s.end(); ++str_iter)
    {
        if(*str_iter == ':')
            break;
        Label += *str_iter;
    }
    s = s.substr(pos+1);

    symbolTable.insert(map<string, long int>::value_type(Label, loc));
}
