/*	LSculpt: Studs-out LEGO® Sculpture

	Copyright (C) 2010 Bram Lambrecht <bram@bldesign.org>

	http://lego.bldesign.org/LSculpt/

	This file (argpanel.cpp) is part of LSculpt.

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

#include <QVariant>

#include "argpanel.h"

ArgPanel::ArgPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArgPanel)
{
    ui->setupUi(this);
    connect(ui->runLSculptBtn, SIGNAL(clicked()), this, SIGNAL(runLSculptBtnClicked()));

	ui->unitsCombo->setItemData(0, QVariant(UNIT_LDU_ST));
	ui->unitsCombo->setItemData(1, QVariant(1.0));  // LDraw Units, default 1
	ui->unitsCombo->setItemData(2, QVariant(UNIT_LDU_MM));
	ui->unitsCombo->setItemData(3, QVariant(UNIT_LDU_CM));
	ui->unitsCombo->setItemData(4, QVariant(UNIT_LDU_M));
	ui->unitsCombo->setItemData(5, QVariant(UNIT_LDU_IN));
	ui->unitsCombo->setItemData(6, QVariant(UNIT_LDU_FT));
}

ArgPanel::~ArgPanel()
{
    delete ui;
}

ArgumentSet ArgPanel::getArguments(ArgumentSet defaults, char *infile)
{
	ArgumentSet args = defaults;  // Start with default argument set

	// Set each argument based on corresponding UI widget.  Order matches load_options.
	args.OPTS_COLOR = ui->colorCombo->currentIndex();
	args.OPTS_PART = ui->partCombo->currentIndex();

	// TODO: need UI widget for forcing a mesh file format
	if (infile != NULL)
		setFileFormat(&args, infile);

	args.OPTS_SCALE *= ui->unitsCombo->itemData(ui->unitsCombo->currentIndex()).toDouble();

	args.OPTS_OFFSET[0] = ui->offsetXDSpin->value();
	args.OPTS_OFFSET[1] = ui->offsetYDSpin->value();
	args.OPTS_OFFSET[2] = ui->offsetZDSpin->value();

	args.OPTS_CENTER = ui->centerCheck->isChecked();

	setStudsUpBaseArg(&args, ui->baseSpin->value());

	args.OPTS_UP = (ui->upYRadio->isChecked()) ? UP_Y : UP_Z;
	args.OPTS_STUDSUP = ui->studsUpCheck->isChecked();

	args.OPTS_ROT = ui->rotateDSpin->value();
	args.OPTS_ROT_SIN = sin(args.OPTS_ROT*PI/180.0);
	args.OPTS_ROT_COS = cos(args.OPTS_ROT*PI/180.0);

	args.OPTS_MAXITER = (ui->maxDetailRadio->isChecked()) ? 0 : OPTIM_MAX;

	if (ui->scaleRadio->isChecked())
	{
		if (ui->scaleDSpin->value() != 0.0)
			args.OPTS_SCALE /= ui->scaleDSpin->value();
	}
	else
	{
		args.OPTS_FIT = ui->lengthDSpin->value();
	}

	// TODO: need UI widget to set message output level
	args.OPTS_MESSAGE = MESSAGE_ERR; //MESSAGE_ALL;

	args.OPTS_NOFILL = ui->emptyCheck->isChecked();

	args.OPTS_GRID = ui->showGridCheck->isChecked();
	args.OPTS_MESH = ui->showMeshCheck->isChecked();
	args.OPTS_PLATES = ui->showPlatesCheck->isChecked();

#if 0  // Remove advanced argument widgets for now
	args.OPTS_MAXITER = ui->optimizeSpin->value();
	if (ui->noOptimizeCheck->isChecked())
		args.OPTS_MAXITER = 0;

	args.OP_ORN = ui->balanceADSpin->value();
	args.OP_DIR = ui->balanceBDSpin->value();
	args.OP_NBR = ui->balanceCDSpin->value();

	args.OP_THN = ui->weightADSpin->value();
	args.OP_NCT = ui->weightBDSpin->value();
	args.OP_SOR = ui->weightCDSpin->value();
	args.OP_BAK = ui->weightDDSpin->value();
	args.OP_BKO = ui->weightEDSpin->value();

#endif

	return args;
}

void ArgPanel::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
