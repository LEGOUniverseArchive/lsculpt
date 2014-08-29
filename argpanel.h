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
#include <QLabel>
#include <QSignalMapper>
#include <QString>
#include "LSculpt_functions.h"
#include "ui_argpanel.h"

#define SHOWBUTTON "Show Preview"
#define HIDEBUTTON "Hide Preview"
#define SCALELABEL "Scale Factor:"
#define SIZELABEL  "Output Size:"
#define SIZEUNITS  "Output Units:"
#define SCALEUNITS "Mesh Units:"
#define SIZETIPR   "Use the desired size of the LEGO conversion"
#define SCALETIPR  "Use the size data from the mesh and scale it up or down"
#define SIZETIPU   "Measure the size of the output in these units"
#define SCALETIPU  "Interpret the mesh using these Measurement units"
#define SIZETIP    "Grow or shrink the mesh until this is the length of its longest side"
#define SCALETIP   "Grow or shrink the mesh by this multiplication factor"
#define SCALEDFLT  1.0
#define SIZEDFLT   32.0

class ArgPanel : public QWidget {
    Q_OBJECT
public:
    ArgPanel(QWidget *parent = 0);
    ~ArgPanel();
	ArgumentSet getArguments(ArgumentSet defaults, char *infile = NULL);
    void toggleLDViewBtn(bool hide);
    void disableLDViewBtn(void);

signals:
    void runLSculptBtnClicked();
    void hideLDViewBtnClicked();

public slots:
    void switchToScale();
    void switchToSize();
    void sliderToBoxX();
    void sliderToBoxY();
    void sliderToBoxZ();
    void boxToSliderX();
    void boxToSliderY();
    void boxToSliderZ();
    void switchUnits(int unitindex);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArgPanel *ui;

};

#endif // ARGPANEL_H
