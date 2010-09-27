/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (lsculptmainwin.cpp) is part of LSculpt.

	LSculpt is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	LSculpt is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see http://www.gnu.org/licenses/  */

#include <iostream>
#include <sstream>
#include <QDir>
#include <QFile>
#include <QURL>
#include <QWidget>
#include <QTextBrowser>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QByteArray>
#include <QProgressDialog>
#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>

#include "LSculpt_functions.h"
#include "lsculptmainwin.h"
#include "ui_lsculptmainwin.h"
#include "argpanel.h"
#include "aboutdialog.h"

#include "LDVLib.h"

using namespace std;

#define lsculpt_version "0.5.0"

LSculptMainWin::LSculptMainWin(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LSculptMainWin)
{
	this->appPath = QApplication::applicationDirPath() + "/";
	this->LDVPath =  this->appPath + "LDVLib/";

	QDir dir(this->LDVPath);
	if (!dir.exists())
	{
		// Check if LDVLib path exists.  LDVLib path contains empty.ldr, PARTS & P folders and all the parts LSculpt needs.
		// Might want to check for existence of all necessary folders & parts too, eventually.
		QMessageBox::warning(this, tr("LSculpt"),
		                     tr("LSculpt cannot find the LDraw parts it needs to build models.\n"
		                     "Try reinstalling LSculpt to fix this.\n"
		                     "LSculpt will now terminate"),
		QMessageBox::Ok);
		exit(1);
	}

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

	QByteArray ba = this->LDVPath.toAscii();
	LDVSetLDrawDir(ba.data());
	pLDV = LDVInit(ldvWin->winId());
	LDVGLInit(pLDV);

  statusBar()->showMessage("Adjust the settings or import a 3D file to get started...");
	setCentralWidget(center);
	setWindowModified(false);
	QString title = QString("LSculpt %1 [*]").arg(lsculpt_version);
	setWindowTitle(title);

	this->defaults = getDefaultArgumentSet();

	QString iniFile = this->appPath + "LSculpt.ini";
	this->settings = new QSettings(iniFile, QSettings::IniFormat);
	loadSettings();
}

LSculptMainWin::~LSculptMainWin()
{
	LDVDeInit(pLDV);
	delete ui;
}

// Ugly global variable, for now.  Cleaner than getting a non-static C++ function callback into low-level LSculpt
QProgressDialog *progress;
void incrProgress(const char * label)
{
	progress->setLabelText(label);
	progress->setValue(progress->value() + 1);
}

void LSculptMainWin::initProgressDialog()
{
	int maxSteps = 7 + 2;  // Keep this relatively in sync with the # of progress_cb calls in main_wrapper
	progress = new QProgressDialog("Clearing Preview", "Abort Update", 0, maxSteps, this->ldvWin, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	progress->setWindowModality(Qt::WindowModal);
	progress->setWindowTitle("Updating...");
	progress->setMinimumDuration(0);
	progress->setValue(1);  // Force immediate display
}

int LSculptMainWin::invokeLSculpt()
{
	if (this->currentFilename.isEmpty())
	{
		this->statusBar()->showMessage("Open a 3D mesh before running LSculpt");
		return EXIT_FAILURE;
	}

	initProgressDialog();
	incrProgress("Begin Update");

	QString emptyLDrawFilename = QString(this->LDVPath + "empty.ldr");
	if (!QFile::exists(emptyLDrawFilename))  // Check if empty file exists - need to give LDView an empty file to begin with
	{
		QFile empty(emptyLDrawFilename);
		empty.open(QIODevice::WriteOnly);
		empty.close();
	}

	QByteArray ba = emptyLDrawFilename.toAscii();
	LDVSetFilename(pLDV, ba.data());
	LDVLoadModel(pLDV, false);

	// setup options suited for display of LSculpt models:
	LDVSetSeamWidth(pLDV, 0.0);
	LDVSetTextureStuds(pLDV, false);

	// Setup input & output filename
	ba = this->currentFilename.toAscii();
	char *infile = ba.data();
	char outfile[80] = "";

	// Redirect cout & cerr to local string buffer
	streambuf *coutBuf = cout.rdbuf();
	streambuf *cerrBuf = cerr.rdbuf();

	stringbuf buffer;
	cout.rdbuf(&buffer);
	cerr.rdbuf(&buffer);

	// Build set of arguments from UI, pass to LSculpt, then run LSculpt
	ArgumentSet args = panel->getArguments(this->defaults, infile);
	setArgumentSet(args);
	setOutFile(args, infile, outfile);
	int res = main_wrapper(infile, outfile, incrProgress);

	// Copy redirected string buffer to status bar
	statusBar()->showMessage(buffer.str().c_str());

	// Reset cout & cerr
	cout.rdbuf(coutBuf);
	cerr.rdbuf(cerrBuf);

	if (res == EXIT_FAILURE)  // Total failure on model import - go no further
	{
		setWindowModified(false);
		progress->setValue(progress->maximum());  // Ensure progress goes away
		return EXIT_FAILURE;
	}

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

	setWindowModified(true);
	progress->setValue(progress->maximum());  // Ensure progress goes away
	if (statusBar()->currentMessage().isEmpty())
		statusBar()->showMessage("Model loaded successfully");
	return EXIT_SUCCESS;
}

void LSculptMainWin::closeEvent(QCloseEvent *event)
{
	if (offerSave())
	{
		this->saveSettings();
		event->accept();
	}
	else
		event->ignore();
}

void LSculptMainWin::import3DMesh()
{
	if (!offerSave())
		return;

	QString filename = QFileDialog::getOpenFileName(this, "Import 3D mesh", QString(), "3D mesh files (*.ply *.stl *.obj);;All Files (*)");
	if (!filename.isEmpty())
	{
		currentFilename = filename;
		statusBar()->showMessage("Loaded: " + filename);
		isLoaded = false;
		if (invokeLSculpt() != EXIT_FAILURE)
		{
			QString title = QString("LSculpt %1 - %2 [*]").arg(lsculpt_version).arg(filename);
			setWindowTitle(title);
			setWindowModified(true);
		}
	}
}

// Returns true if successfully saved, false otherwise
bool LSculptMainWin::exportToLDraw()
{
	QString filename = QFileDialog::getSaveFileName(this, "Save LDraw File As", QString(), "LDraw files (*.dat *.ldr *.mpd);;All Files (*)");
	if (filename.isEmpty())
		return false;

	QByteArray ba = filename.toAscii();
	char *infile = ba.data();

	if (save_ldraw(infile))
	{
		this->statusBar()->showMessage("Saved LDraw File: " + filename);
		setWindowModified(false);
		return true;
	}

	this->statusBar()->showMessage("Error Saving LDraw File: " + filename);
	return false;
}

// Returns true if we should proceed with whatever is checking save state, false if we should cancel.
// Will prompt user for save if necessary.
bool LSculptMainWin::offerSave()
{
	if (!this->isWindowModified())
		return true;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this,
	                              tr("LSculpt - Unsaved Changes"),
	                              tr("Save unsaved changes?"),
	                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	if (reply == QMessageBox::Yes)
		return this->exportToLDraw();
	return (reply == QMessageBox::No);
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

void LSculptMainWin::loadSettings()
{
	defaults.OPTS_COLOR_NONE = settings->value("LDrawColors/COLOR_NONE", defaults.OPTS_COLOR_NONE).toInt();
	defaults.OPTS_COLOR_0 = settings->value("LDrawColors/COLOR_0", defaults.OPTS_COLOR_0).toInt();
	defaults.OPTS_COLOR_1 = settings->value("LDrawColors/COLOR_1", defaults.OPTS_COLOR_1).toInt();
	defaults.OPTS_COLOR_2 = settings->value("LDrawColors/COLOR_2", defaults.OPTS_COLOR_2).toInt();
	defaults.OPTS_COLOR_3 = settings->value("LDrawColors/COLOR_3", defaults.OPTS_COLOR_3).toInt();
	defaults.OPTS_COLOR_4 = settings->value("LDrawColors/COLOR_4", defaults.OPTS_COLOR_4).toInt();
	defaults.OPTS_COLOR_5 = settings->value("LDrawColors/COLOR_5", defaults.OPTS_COLOR_5).toInt();

	defaults.OPTS_COLOR_PLATES = settings->value("LDrawColors/COLOR_PLATES", defaults.OPTS_COLOR_PLATES).toInt();
	defaults.OPTS_COLOR_MESH = settings->value("LDrawColors/COLOR_MESH", defaults.OPTS_COLOR_MESH).toInt();
	defaults.OPTS_COLOR_GRID = settings->value("LDrawColors/COLOR_GRID", defaults.OPTS_COLOR_GRID).toInt();
	
	restoreGeometry(settings->value("MainWindow/Geometry").toByteArray());
	restoreState(settings->value("MainWindow/State").toByteArray());
	ldvWin->resize(settings->value("MainWindow/LDVWinSize", QSize(400, 320)).toSize());
}

void LSculptMainWin::saveSettings()
{

//  Sample code to write an array to .ini file
//	int colors[5] = {1,2,3,4,5};
//	
//	settings->beginWriteArray("LDrawColorArray");
//	for (int i = 0; i < 5; i++) {
//		settings->setArrayIndex(i);
//		settings->setValue("color", colors[i]);
//	}
//	settings->endArray();

	ArgumentSet args = panel->getArguments(this->defaults);  // Pull arguments from panel so that, in the future, can save any settings made by user

	settings->setValue("LDrawColors/COLOR_NONE", args.OPTS_COLOR_NONE);
	settings->setValue("LDrawColors/COLOR_0", args.OPTS_COLOR_0);
	settings->setValue("LDrawColors/COLOR_1", args.OPTS_COLOR_1);
	settings->setValue("LDrawColors/COLOR_2", args.OPTS_COLOR_2);
	settings->setValue("LDrawColors/COLOR_3", args.OPTS_COLOR_3);
	settings->setValue("LDrawColors/COLOR_4", args.OPTS_COLOR_4);
	settings->setValue("LDrawColors/COLOR_5", args.OPTS_COLOR_5);

	settings->setValue("LDrawColors/COLOR_PLATES", QVariant(args.OPTS_COLOR_PLATES));
	settings->setValue("LDrawColors/COLOR_MESH", QVariant(args.OPTS_COLOR_MESH));
	settings->setValue("LDrawColors/COLOR_GRID", QVariant(args.OPTS_COLOR_GRID));

	settings->setValue("MainWindow/Geometry", QVariant(saveGeometry()));
	settings->setValue("MainWindow/State", QVariant(saveState()));
	settings->setValue("MainWindow/LDVWinSize", QVariant(ldvWin->size()));
}

void LSculptMainWin::showHelpFile()
{
	QTextBrowser *tb = new QTextBrowser(0);
	tb->setSource(QUrl::fromLocalFile(this->appPath + "readme.html"));
	tb->resize(950, 800);
	tb->show();
}

void LSculptMainWin::showAboutDialog()
{
	AboutDialog *d = new AboutDialog(lsculpt_version);
	d->show();
}
