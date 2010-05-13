/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (argpanel.h) is part of LSculpt.

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

#ifndef ARGPANEL_H
#define ARGPANEL_H

#include <QWidget>
#include "ui_argpanel.h"
#include "LSculpt_functions.h"

class ArgPanel : public QWidget {
    Q_OBJECT
public:
    ArgPanel(QWidget *parent = 0);
    ~ArgPanel();
	ArgumentSet getArguments(char *infile);

signals:
    void runLSculptBtnClicked();

public slots:

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArgPanel *ui;

};

#endif // ARGPANEL_H
