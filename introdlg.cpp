#include "introdlg.h"
#include "ui_introdlg.h"

IntroDlg::IntroDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IntroDlg)
{
    ui->setupUi(this);
}

IntroDlg::~IntroDlg()
{
    delete ui;
}
