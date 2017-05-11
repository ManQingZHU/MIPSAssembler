#include "Parser.h"
#include "TranseDigit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <iostream>

void Parser::UpdateMemo()
{
    long int index = 0;
    map<long int, string>::iterator m_iter;
    map<long int, string>::iterator m_iter_end = DataBlock.end();
    string temp;
    unsigned long int j;
    while(index != end_addr)
    {
        m_iter = DataBlock.find(index);
        if(m_iter == m_iter_end) {
            Memory.push_back("00000000");
            index++;
            continue;
        }

        temp = m_iter->second;

        if(temp.length() == 8) {
            Memory.push_back(temp);
            index++;
        }
        else for(j = 0; j <= temp.size() - 8; j = j+8)
            {
                Memory.push_back(temp.substr(j, 8));
                index ++;
            }

    }

}

void Parser::PrintCode() {
    w.ui->OutputTextEdit->clear();
    string line;
    QString ql;
    for(int i = 0; i < end_addr; i++) {
        if(i && i % 4 == 0)
        {
            ql = QString::fromStdString(line);
            line.clear();
            w.ui->OutputTextEdit->appendPlainText(ql);
        }
        line += Memory[i];
    }
}

void Parser::PrintCOE()
{
   w.ui->OutputTextEdit->clear();
    QString ql;
    ql = "memory_initialization_radix=16;";
    w.ui->OutputTextEdit->appendPlainText(ql);
    ql = "memory_initialization_vector=";
    w.ui->OutputTextEdit->appendPlainText(ql);

    int temp;
    string line, word;
    int i;
    for(i = 0; i < end_addr; i++)
    {
        if(i && i % 4 == 0)
            line += ", ";
        if(i && i % 32 == 0)
        {
            ql = QString::fromStdString(line);
            line.clear();
            w.ui->OutputTextEdit->appendPlainText(ql);
        }

        temp = BinToUnInt(Memory[i]);
        if(temp == 0)
            word = "00";
        else word = UnIntToHex(temp);
        if(word.size() < 2)
            word = '0' + word;
        line += word;
    }
    line += ";";
    ql = QString::fromStdString(line);
    w.ui->OutputTextEdit->appendPlainText(ql);
}

void Parser::ReadBin_forCoe()
{
    QString content = w.ui->InputTextEdit->toPlainText();
    QTextStream temp_text(&content, QIODevice::ReadOnly);
    QString ql;

    string line;

    while(!temp_text.atEnd())
    {
        ql = temp_text.readLine();
        line = ql.toStdString();

        Memory.push_back(line);
        pc ++;
    }
    end_addr = pc;
}

void Parser::BinOtputCoe()
{
    w.ui->OutputTextEdit->clear();
     QString ql;
     ql = "memory_initialization_radix=16;";
     w.ui->OutputTextEdit->appendPlainText(ql);
     ql = "memory_initialization_vector=";
     w.ui->OutputTextEdit->appendPlainText(ql);

     long int temp;
     string line, word;
     int i;
     for(i = 0; i < end_addr; i++)
     {
         if(i)
             line += ", ";

         if(i && i % 8 == 0)
         {
             ql = QString::fromStdString(line);
             line.clear();
             w.ui->OutputTextEdit->appendPlainText(ql);
         }

         temp = BinToUnInt(Memory[i]);
         if(temp == 0)
             word = "00000000";
         else word = UnIntToHex(temp);
         while(word.size() < 8)
             word = '0' + word;
         line += word;
     }
     line += ";";
     ql = QString::fromStdString(line);
     w.ui->OutputTextEdit->appendPlainText(ql);
}
