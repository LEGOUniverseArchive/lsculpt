#ifndef LSCULPTMAINWIN_H
#define LSCULPTMAINWIN_H

#include <QMainWindow>
#include <QTextEdit>

#include "argpanel.h"

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
	void resizeEvent(QResizeEvent *e);

private slots:
    void invokeLSculpt();
    void import();

private:
    Ui::LSculptMainWin *ui;
    ArgPanel *panel;
    QTextEdit *console;
	QWidget *ldvWin;

	void *pLDV;

    bool offerSave();

    QString currentFilename;
};

#endif // LSCULPTMAINWIN_H
