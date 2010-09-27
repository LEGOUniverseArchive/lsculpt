#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(char *version, QWidget *parent) :
	QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

	// Change text box background color to same as parent (ie. transparent)
	QColor bg = this->palette().color(QPalette::Background);
	QPalette p = ui->textEdit->palette();
	p.setColor(QPalette::Base, bg);
    ui->textEdit->setPalette(p);

	ui->titleLabel->setText(QString("LSculpt %1").arg(version));

    //ui->textEdit->setHtml(QString("<a href=\"http://google.com\">LSculpt Website</a>"));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
