#ifndef ARGPANEL_H
#define ARGPANEL_H

#include <QWidget>
#include "ui_argpanel.h"

class ArgPanel : public QWidget {
    Q_OBJECT
public:
    ArgPanel(QWidget *parent = 0);
    ~ArgPanel();

signals:

public slots:

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ArgPanel *ui;

};

#endif // ARGPANEL_H
