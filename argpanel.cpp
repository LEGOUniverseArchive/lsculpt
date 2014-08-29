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
#include <QMessageBox>

#include "argpanel.h"

ArgPanel::ArgPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArgPanel)
{
    ui->setupUi(this);
    connect(ui->runLSculptBtn, SIGNAL(clicked()), this, SIGNAL(runLSculptBtnClicked()));
    connect(ui->hideLDViewBtn, SIGNAL(clicked()), this, SIGNAL(hideLDViewBtnClicked()));
    connect(ui->scaleRadio, SIGNAL(toggled(bool)), this, SLOT(switchToScale()));
    connect(ui->lengthRadio, SIGNAL(toggled(bool)), this, SLOT(switchToSize()));
    connect(ui->unitsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(switchUnits(int)));
    connect(ui->sliderX, SIGNAL(sliderMoved(int)), this, SLOT(sliderToBoxX()));
    connect(ui->sliderY, SIGNAL(sliderMoved(int)), this, SLOT(sliderToBoxY()));
    connect(ui->sliderZ, SIGNAL(sliderMoved(int)), this, SLOT(sliderToBoxZ()));
    connect(ui->nudgeX, SIGNAL(textEdited(QString)), this, SLOT(boxToSliderX()));
    connect(ui->nudgeY, SIGNAL(textEdited(QString)), this, SLOT(boxToSliderY()));
    connect(ui->nudgeZ, SIGNAL(textEdited(QString)), this, SLOT(boxToSliderZ()));

	ui->unitsCombo->setItemData(0, QVariant(UNIT_LDU_ST));
	ui->unitsCombo->setItemData(1, QVariant(1.0));  // LDraw Units, default 1
	ui->unitsCombo->setItemData(2, QVariant(UNIT_LDU_MM));
	ui->unitsCombo->setItemData(3, QVariant(UNIT_LDU_CM));
	ui->unitsCombo->setItemData(4, QVariant(UNIT_LDU_M));
	ui->unitsCombo->setItemData(5, QVariant(UNIT_LDU_IN));
	ui->unitsCombo->setItemData(6, QVariant(UNIT_LDU_FT));

    ui->scaleRadio->setToolTip(SCALETIPR);
    ui->lengthRadio->setToolTip(SIZETIPR);
    this->switchToSize();

    ui->nudgeX->setValidator(new QDoubleValidator(-1.0,1.0,1000,0));
    ui->nudgeY->setValidator(new QDoubleValidator(-1.0,1.0,1000,0));
    ui->nudgeZ->setValidator(new QDoubleValidator(-1.0,1.0,1000,0));
    this->sliderToBoxX();
    this->sliderToBoxY();
    this->sliderToBoxZ();
}

ArgPanel::~ArgPanel()
{
    delete ui;
}

void ArgPanel::sliderToBoxX(){
    ui->nudgeX->setText(QString::number((double)ui->sliderX->value()/(double)ui->sliderX->maximum(),'f',2));
}

void ArgPanel::sliderToBoxY(){
    ui->nudgeY->setText(QString::number((double)ui->sliderY->value()/(double)ui->sliderY->maximum(),'f',2));
}

void ArgPanel::sliderToBoxZ(){
    ui->nudgeZ->setText(QString::number((double)ui->sliderZ->value()/(double)ui->sliderZ->maximum(),'f',2));
}

void ArgPanel::boxToSliderX(){
    double val;
    val = ui->nudgeX->text().toDouble();
    val = val > 1.0 ? 1.0 : (val < -1.0 ? -1.0 : val);
    if(val >= 1.0 || val <= -1.0) ui->nudgeX->setText(QString::number(val,'f',2));
    ui->sliderX->setValue((int)(val * (double)ui->sliderX->maximum()));
}
void ArgPanel::boxToSliderY()
{
    double val;
    val = ui->nudgeY->text().toDouble();
    val = val > 1.0 ? 1.0 : (val < -1.0 ? -1.0 : val);
    if(val >= 1.0 || val <= -1.0) ui->nudgeY->setText(QString::number(val,'f',2));
    ui->sliderY->setValue((int)(val * (double)ui->sliderY->maximum()));
}
void ArgPanel::boxToSliderZ()
{
    double val;
    val = ui->nudgeZ->text().toDouble();
    val = val > 1.0 ? 1.0 : (val < -1.0 ? -1.0 : val);
    if(val >= 1.0 || val <= -1.0) ui->nudgeZ->setText(QString::number(val,'f',2));
    ui->sliderZ->setValue((int)(val * (double)ui->sliderZ->maximum()));
}

void ArgPanel::switchToScale()
{
    ui->scaleLabel->setText(SCALELABEL);
    ui->scaleLabel->setToolTip(SCALETIP);
    ui->scaleDSpin->setToolTip(SCALETIP);
    ui->unitsLabel->setText(SCALEUNITS);
    ui->unitsLabel->setToolTip(SCALETIPU);
    ui->unitsCombo->setToolTip(SCALETIPU);
    ui->scaleDSpin->setValue(SCALEDFLT);
    this->switchUnits(ui->unitsCombo->currentIndex());
}

void ArgPanel::switchToSize()
{
    ui->scaleLabel->setText(SIZELABEL);
    ui->scaleLabel->setToolTip(SIZETIP);
    ui->scaleDSpin->setToolTip(SIZETIP);
    ui->unitsLabel->setText(SIZEUNITS);
    ui->unitsLabel->setToolTip(SIZETIPU);
    ui->unitsCombo->setToolTip(SIZETIPU);
    ui->scaleDSpin->setValue(SIZEDFLT * ui->unitsCombo->currentData().toDouble() / UNIT_LDU_ST);
    this->switchUnits(ui->unitsCombo->currentIndex());
}

void ArgPanel::switchUnits(int newindex)
{
    static int previndex = 0;

    /*
    int places;
    for( places = 1;
         places < 15 && (scalesize*pow(10,places) != (double) qRound(scalesize*pow(10,places)));
         places++
    );
    ui->scaleDSpin->setDecimals(places);
    */

    if(ui->lengthRadio->isChecked() || 1 ){ // or mesh is loaded
        ui->scaleDSpin->setValue(
            ui->scaleDSpin->value() *
            ui->unitsCombo->itemData(newindex).toDouble() /
            ui->unitsCombo->itemData(previndex).toDouble()
        );
    }

    if(ui->lengthRadio->isChecked()){
        double newmax = (double)(2 << (sizeof(SpCubeCoord)*CHAR_BIT)) *
                ui->unitsCombo->itemData(newindex).toDouble() / UNIT_LDU_ST;
        ui->scaleDSpin->setDecimals(5 - (int)(log10(newmax)));
        ui->scaleDSpin->setMaximum(newmax);
    } else {
        ui->scaleDSpin->setDecimals(6);
        ui->scaleDSpin->setMaximum(10000);
    }

    previndex = newindex;
}

void ArgPanel::toggleLDViewBtn(bool hide)
{
    if(hide) ui->hideLDViewBtn->setText(HIDEBUTTON);
    else ui->hideLDViewBtn->setText(SHOWBUTTON);
}

void ArgPanel::disableLDViewBtn()
{
    ui->hideLDViewBtn->setEnabled(false);
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
        args.OPTS_FIT = ui->scaleDSpin->value();
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
