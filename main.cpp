#include "LogAnalyse.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LogAnalyse w;
    w.show();
    return a.exec();
}
