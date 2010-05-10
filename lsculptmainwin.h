#ifndef LSCULPTMAINWIN_H
#define LSCULPTMAINWIN_H

#include <QMainWindow>

#include "argpanel.h"

namespace Ui {
    class LSculptMainWin;
}

class LSculptMainWin : public QMainWindow {
    Q_OBJECT
public:
    LSculptMainWin(QWidget *parent = 0);
    ~LSculptMainWin();
	void closeEvent(QCloseEvent *event);

protected:
    void changeEvent(QEvent *e);
	void resizeEvent(QResizeEvent *e);

private slots:
    void invokeLSculpt();
	void import3DMesh();
	bool exportToLDraw();

private:
    Ui::LSculptMainWin *ui;
    ArgPanel *panel;
	QWidget *ldvWin;

	void *pLDV;
	bool isLoaded;

    bool offerSave();
	void initProgressDialog();
    QString currentFilename;
};

#endif // LSCULPTMAINWIN_H
