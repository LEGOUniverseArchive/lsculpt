#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileDialog>

#include "lsculptmainwin.h"
#include "ui_lsculptmainwin.h"
#include "argpanel.h"

LSculptMainWin::LSculptMainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LSculptMainWin)
{
    ui->setupUi(this);

    QWidget *center = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(center);

    panel = new ArgPanel(this);
    console = new QTextEdit("LSculpt output goes here...", this);
    console->setReadOnly(true);

    layout->addWidget(panel);
    layout->addWidget(console);

    center->setLayout(layout);
    setCentralWidget(center);
    setWindowTitle("LSculpt");
}

LSculptMainWin::~LSculptMainWin()
{
    delete ui;
}

void LSculptMainWin::import()
{
    if (this->offerSave())
    {
        QString filename = QFileDialog::getOpenFileName(this, "Import 3D mesh", QString(), "3D mesh files (*.ply *.stl);;All Files (*)");
        if (!filename.isEmpty())
        {
            this->currentFilename = filename;
            this->console->setText("Loaded: " + filename);
        }
    }
}

bool LSculptMainWin::offerSave()
{
    return true;  // TODO: check modified state of current file & throw up Save dialog, if modified.
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
