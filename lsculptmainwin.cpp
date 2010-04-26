#include "lsculptmainwin.h"
#include "ui_lsculptmainwin.h"
#include "argpanel.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>

LSculptMainWin::LSculptMainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LSculptMainWin)
{
    ui->setupUi(this);


    QWidget *center = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(center);

    ArgPanel *panel = new ArgPanel(this);
    QTextEdit *console = new QTextEdit("LSculpt output goes here...", this);
    console->setReadOnly(true);

    layout->addWidget(panel);
    layout->addWidget(console);

    center->setLayout(layout);
    this->setCentralWidget(center);
    this->setWindowTitle("LSculpt");
}

LSculptMainWin::~LSculptMainWin()
{
    delete ui;
}

void LSculptMainWin::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
