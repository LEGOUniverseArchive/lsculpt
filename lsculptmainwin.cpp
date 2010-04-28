#include <iostream>
#include <sstream>
#include <QWidget>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QByteArray>

#include "LSculpt_functions.h"
#include "lsculptmainwin.h"
#include "ui_lsculptmainwin.h"
#include "argpanel.h"

using namespace std;

LSculptMainWin::LSculptMainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LSculptMainWin)
{
    ui->setupUi(this);

    QWidget *center = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(center);

    panel = new ArgPanel(this);
    connect(panel, SIGNAL(runLSculptBtnClicked()), this, SLOT(invokeLSculpt()));

    console = new QTextEdit("Welcome to LSculpt's new, totally unfinished UI", this);
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

void LSculptMainWin::invokeLSculpt()
{
    if (this->currentFilename.isEmpty())
    {
        console->append("Open a 3D mesh before running LSculpt.");
        return;
    }

	// Setup input & output filename (output is
	QByteArray ba = this->currentFilename.toLatin1();
	char *infile = ba.data();
	char outfile[80] = "";

    // Redirect cout & cerr to local string buffer
    streambuf *coutBuf = cout.rdbuf();
    streambuf *cerrBuf = cerr.rdbuf();

    stringbuf buffer;
    cout.rdbuf(&buffer);
	cerr.rdbuf(&buffer);

	ArgumentSet args = panel->getArguments(infile);
	setArgumentSet(args);
	setOutFile(args, infile, outfile);
	main_wrapper(infile, outfile);

    // Copy redirected string buffer to console text editor
    console->append(buffer.str().c_str());

    // Reset cout & cerr
    cout.rdbuf(coutBuf);
    cerr.rdbuf(cerrBuf);
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
