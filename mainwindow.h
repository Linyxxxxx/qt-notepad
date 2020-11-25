#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class QLineEdit;
class QDialog;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 

    void newFile(); // 新建操作
    bool isSaved();    // 判断是否需要保存
    bool save();    // 保存操作
    bool saveAs();  // 另存为操作
    bool saveFile(const QString &fileName); // 保存文件
    bool openFile(const QString &fileName); // 打开文件
    void showIntro();   //显示简介框
    void showFind();    //显示搜索框

private slots:
    void on_actionNew_triggered();

    void on_actionSave_S_triggered();

    void on_actionSave_as_A_triggered();

    void on_actionOpen_O_triggered();

    void on_actionClose_C_triggered();

    void on_actionExit_X_triggered();

    void on_actionIntro_I_triggered();

    void on_actionCopy_C_triggered();

    void on_actionPaste_V_triggered();

    void on_actionCut_X_triggered();

    void on_actionUndo_Z_triggered();

    void on_actionRedo_Y_triggered();

    void on_actionFind_F_triggered();

    void findText();

    void on_actionFont_N_triggered();

    void on_actionColor_M_triggered();

private:
    Ui::MainWindow *ui;

    bool isUntitled=true;    // 为真表示文件没有保存过，为假表示文件已经被保存过了

    QString curFile;    // 保存当前文件的路径

    QLineEdit *findLineEdit;
    QDialog *findDlg;

protected:
    void closeEvent(QCloseEvent *event);    //关闭文件时弹出询问框

};
#endif // MAINWINDOW_H
