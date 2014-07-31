/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (lsculptmainwin.h) is part of LSculpt.

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

#ifndef LSCULPTMAINWIN_H
#define LSCULPTMAINWIN_H

#include <QMainWindow>
#include <QSettings>

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
	int invokeLSculpt();
    int hidePreview();
	void import3DMesh();
	bool exportToLDraw();
	void showHelpFile();
	void showAboutDialog();

private:
    Ui::LSculptMainWin *ui;
    ArgPanel *panel;
    ArgumentSet defaults;  // Local copy of default arguments, which we can munge through INI file
	QWidget *ldvWin;

	void *pLDV;
	bool isLoaded;
    bool hideLDView;
	QString appPath;
	QString LDVPath;
	QSettings *settings;

    bool offerSave();
	void initProgressDialog();
    QString currentFilename;
	void loadSettings();
	void saveSettings();
};

#endif // LSCULPTMAINWIN_H
