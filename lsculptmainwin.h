#ifndef LSCULPTMAINWIN_H
#define LSCULPTMAINWIN_H

#include <QMainWindow>

namespace Ui {
    class LSculptMainWin;
}

class LSculptMainWin : public QMainWindow {
    Q_OBJECT
public:
    LSculptMainWin(QWidget *parent = 0);
    ~LSculptMainWin();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::LSculptMainWin *ui;
};

#endif // LSCULPTMAINWIN_H
