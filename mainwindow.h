#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "DisAssembly.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class Parser;
    friend class DisAssembly;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void SaveFile();
    void LoadFile(const QString &fileName);
    void ReadIn();

private slots:
    void on_actionsave_S_triggered();
    void on_actionopenFile_O_triggered();
    void on_TranspushButton_clicked();

    void on_InputCleanButton_clicked();

    void on_OutputCleanButton_clicked();

    void on_openFile_button_clicked();

    void on_saveFile_button_clicked();

    void on_actionbuild_triggered();

public slots:
    void on_AsmToBin_triggered();
    void on_AsmToCoe_triggered();
    void on_NotChange_triggered();
    void on_BinToAsm_triggered();
    void on_CoeToAsm_triggered();
    void on_CoeToBin_triggered();
    void on_BinToCoe_triggered();

signals:
    void NotChange();
    void AsmToBin();
    void AsmToCoe();
    void BinToAsm();
    void BinToCoe();
    void CoeToAsm();
    void CoeToBin();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
