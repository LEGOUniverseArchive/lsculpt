#ifndef LSCULPT_CONSOLE

#include <QtGui/QApplication>
#include "lsculptmainwin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LSculptMainWin w;
    w.show();
    return a.exec();
}
#endif  // LSCULPT_CONSOLE
