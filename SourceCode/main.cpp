#include "welcomewindow.h"
#include "Parser.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icns("/appIcon/MyIcon.png");
    a.setWindowIcon(icns);
    MainWindow w;
    WelcomeWindow wel;
    if(wel.exec() == WelcomeWindow::Accepted)
    {
        w.show();
        return a.exec();
    }
    else return 0;
}
