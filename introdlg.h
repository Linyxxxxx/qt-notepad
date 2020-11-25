#ifndef INTRODLG_H
#define INTRODLG_H

#include <QDialog>

namespace Ui {
class IntroDlg;
}

class IntroDlg : public QDialog
{
    Q_OBJECT

public:
    explicit IntroDlg(QWidget *parent = nullptr);
    ~IntroDlg();


private:
    Ui::IntroDlg *ui;
};

#endif // INTRODLG_H
