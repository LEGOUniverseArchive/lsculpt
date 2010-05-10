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

#include "LDVLib.h"

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

	ldvWin = new QWidget(this);
	ldvWin->setMinimumWidth(panel->minimumWidth());
	ldvWin->setMinimumHeight(panel->minimumHeight());

	layout->addWidget(panel);
	layout->addWidget(ldvWin);
	center->setLayout(layout);

	LDVSetLDrawDir("C:\\LDraw");
	pLDV = LDVInit(ldvWin->winId());
	LDVGLInit(pLDV);

	statusBar()->showMessage("Welcome to LSculpt's new, partially finished UI");
	setCentralWidget(center);
	setWindowTitle("LSculpt");
}

LSculptMainWin::~LSculptMainWin()
{
	LDVDeInit(pLDV);
	delete ui;
}

void LSculptMainWin::invokeLSculpt()
{
	if (this->currentFilename.isEmpty())
	{
		this->statusBar()->showMessage("Open a 3D mesh before running LSculpt");
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

	// Copy redirected string buffer to status bar
	statusBar()->showMessage(buffer.str().c_str());

	// Reset cout & cerr
	cout.rdbuf(coutBuf);
	cerr.rdbuf(cerrBuf);

	LDVSetFilename(pLDV, outfile);
	if (isLoaded)
	{
		LDVLoadModel(pLDV, false);
	}
	else
	{
		LDVLoadModel(pLDV, true);
		isLoaded = true;
	}
}

void LSculptMainWin::import3DMesh()
{
	if (this->offerSave())
	{
		QString filename = QFileDialog::getOpenFileName(this, "Import 3D mesh", QString(), "3D mesh files (*.ply *.stl *.obj);;All Files (*)");
		if (!filename.isEmpty())
		{
			this->currentFilename = filename;
			this->statusBar()->showMessage("Loaded: " + filename);
			isLoaded = false;
			invokeLSculpt();
		}
	}
}

void LSculptMainWin::exportToLDraw()
{
	if (this->offerSave())
	{
		QString filename = QFileDialog::getSaveFileName(this, "Save LDraw File As", QString(), "LDraw files (*.dat *.ldr *.mpd);;All Files (*)");
		if (!filename.isEmpty())
		{
			QByteArray ba = filename.toLatin1();
			char *infile = ba.data();

			if (save_ldraw(infile))
				this->statusBar()->showMessage("Saved LDraw File: " + filename);
			else
				this->statusBar()->showMessage("Error Saving LDraw File: " + filename);
		}
	}
}

bool LSculptMainWin::offerSave()
{
	return true;  // TODO: check modified state of current file & throw up Save dialog, if modified.
}

void LSculptMainWin::resizeEvent(QResizeEvent *e)
{
	QMainWindow::resizeEvent(e);
	LDVSetSize(pLDV, ldvWin->width(), ldvWin->height());
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
