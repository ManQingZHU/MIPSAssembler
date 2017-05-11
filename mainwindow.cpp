#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include "Parser.h"
#include <QObject>
#include "DisAssembly.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,&MainWindow::NotChange, this, &MainWindow::on_NotChange_triggered);
    connect(this,&MainWindow::AsmToBin, this, &MainWindow::on_AsmToBin_triggered);
    connect(this,&MainWindow::AsmToCoe, this, &MainWindow::on_AsmToCoe_triggered);
    connect(this,&MainWindow::BinToAsm, this, &MainWindow::on_BinToAsm_triggered);
    connect(this,&MainWindow::CoeToAsm, this, &MainWindow::on_CoeToAsm_triggered);
    connect(this,&MainWindow::CoeToBin, this, &MainWindow::on_CoeToBin_triggered);
    connect(this,&MainWindow::BinToCoe, this, &MainWindow::on_BinToCoe_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"));
    if(fileName.isEmpty())
        return;
    QFile fout(fileName);
    if(!fout.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("保存文件"),
                             tr("无法保存文件 %1:\n%2")
                             .arg(fileName)
                             .arg(fout.errorString()));
        return;
    }
    QTextStream out(&fout);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->OutputTextEdit->toPlainText();
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_actionsave_S_triggered()
{
    SaveFile();
}

void MainWindow::LoadFile(const QString &fileName)
{
    QFile fin(fileName);
    if(!fin.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("打开文件"),
                             tr("无法打开文件 %1:\n%2.")
                             .arg(fileName)
                             .arg(fin.errorString()));
        return;
    }
    QTextStream in(&fin);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->InputTextEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_actionopenFile_O_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        LoadFile(fileName);
    }
}

void MainWindow::on_TranspushButton_clicked()
{
    on_actionbuild_triggered();
}

void MainWindow::on_AsmToBin_triggered()
{
    Parser p(*this);
    p.PassOne();
    p.UpdateMemo();
    p.PassTwo();
    p.PrintCode();
}
void MainWindow::on_AsmToCoe_triggered()
{
    Parser p(*this);
    p.PassOne();
    p.UpdateMemo();
    p.PassTwo();
    p.PrintCOE();
}

void MainWindow::on_NotChange_triggered()
{
    QString qstr = ui->InputTextEdit->toPlainText();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->OutputTextEdit->setPlainText(qstr);
    QApplication::restoreOverrideCursor();
}

void MainWindow::on_BinToAsm_triggered()
{
    DisAssembly d(*this);
    d.DeBin();
    d.InsertLabel();
    d.OutputAsm();
}

void MainWindow::on_CoeToAsm_triggered()
{
    DisAssembly d(*this);
    d.DeCoe();
    d.InsertLabel();
    d.OutputAsm();
}

void MainWindow::on_CoeToBin_triggered()
{
    DisAssembly d(*this);
    d.ReadInCoe_forBin();
    d.CoeOutputBin();
}

void MainWindow::on_BinToCoe_triggered()
{
    Parser p(*this);
    p.ReadBin_forCoe();
    p.BinOtputCoe();
}

void MainWindow::on_InputCleanButton_clicked()
{
    ui->InputTextEdit->clear();
}

void MainWindow::on_OutputCleanButton_clicked()
{
    ui->OutputTextEdit->clear();
}

void MainWindow::on_openFile_button_clicked()
{
    on_actionopenFile_O_triggered();
}

void MainWindow::on_saveFile_button_clicked()
{
    on_actionsave_S_triggered();
}

void MainWindow::on_actionbuild_triggered()
{
    if(ui->Input_asm_check->isChecked())
    {
        if(ui->Output_asm_check->isChecked() )
            emit NotChange();
        else if(ui->Output_bin_check->isChecked())
            emit AsmToBin();
        else if(ui->Output_coe_check->isChecked())
            emit AsmToCoe();
        else {
            QMessageBox::critical(this, tr("汇编／反汇编"),
                                 tr("请选择一种输入和一种输出类型。"));
        }
    }
    else if(ui->Input_bin_check->isChecked())
    {
        if(ui->Output_bin_check->isChecked())
                    emit NotChange();
        else if(ui->Output_asm_check->isChecked())
            emit BinToAsm();
        else if(ui->Output_coe_check->isChecked())
            emit BinToCoe();
        else {
            QMessageBox::critical(this, tr("汇编／反汇编"),
                                 tr("请选择一种输入和一种输出类型。"));
        }
    }
    else if(ui->Input_coe_check->isChecked())
    {
        if(ui->Output_coe_check->isChecked())
            emit NotChange();
        else if(ui->Output_asm_check->isChecked())
            emit CoeToAsm();
        else if(ui->Output_bin_check->isChecked())
            emit CoeToBin();
        else {
            QMessageBox::critical(this, tr("汇编／反汇编"),
                                 tr("请选择一种输入和一种输出类型。"));
        }
    }
    else{
        QMessageBox::critical(this, tr("汇编／反汇编"),
                             tr("请选择一种输入和一种输出类型。"));
    }
}
