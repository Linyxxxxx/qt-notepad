#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "introdlg.h"

#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QDialog>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    isUntitled = true;  // 初始化文件为未保存状态

    curFile = tr("Untitled.txt");   // 初始化文件名为"Untitled.txt"

    setWindowTitle(curFile);    // 初始化窗口标题为文件名

    showFind();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*函数定义部分*/


//新建文档
void MainWindow::newFile()
{
   if (isSaved())
   {
       isUntitled = true;
       curFile = tr("Untitiled.txt");
       setWindowTitle(curFile);
       ui->textEdit->clear();
       ui->textEdit->setVisible(true);
   }
}

//询问是否需要保存
bool MainWindow::isSaved()
{
    if(ui->textEdit->document()->isModified())  //文件已经被修改
    {

        QMessageBox box;    //弹出提示
        box.setWindowTitle("Warning!");
        box.setIcon(QMessageBox::Warning);
        box.setText("Didn't save, save or not?");

        //选择按钮
        QPushButton *yesBtn=box.addButton(tr("Yes (&Y)"),QMessageBox::YesRole);
        QPushButton *noBtn=box.addButton(tr("No (&N)"), QMessageBox::NoRole);
        QPushButton *cancelBtn=box.addButton("Cancel",QMessageBox::RejectRole);
        box.exec(); //暂停 让用户选择

        if(box.clickedButton()==yesBtn)return save();
        else if(box.clickedButton()==noBtn)return true;
        else if(box.clickedButton()==cancelBtn)return false;
    }

    return true;    //文档未修改
}

//保存
bool MainWindow::save()
{
    //曾经未保存过 弹出另存为
    if(isUntitled)
    {
        return saveAs();
    }
    //曾经保存过 保存到原地址
    else
    {
        return saveFile(curFile);
    }
}

//另存为
bool MainWindow::saveAs()
{
    QString fileName=QFileDialog::getSaveFileName(this,"Save as...",curFile);

    if(fileName.isEmpty())
    {
        return false;   //保存失败
    }
    return saveFile(fileName);  //保存到选定目录
}

//根据文件名保存
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);

    //打开文件失败
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Text Editor",tr("Can't write in file %1: \n %2").arg(fileName).arg(file.errorString()));

        return false;
    }

    //创建输出流
    QTextStream out(&file);
    out.setCodec("utf-8");

    //输出到文件
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<ui->textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    isUntitled=false;

    curFile=QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);

    ui->textEdit->document()->setModified(false);

    return true;
}

//打开文件
bool MainWindow::openFile(const QString &fileName)
{
    QFile file(fileName);

    //打开文件失败
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"Text Editor",tr("Can't read the file %1: \n %2").arg(fileName).arg(file.errorString()));

        return false;
    }
    //创建输入流
    QTextStream in(&file);
    in.setCodec("utf-8");

    //从文件读取
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    //设置基本信息
    curFile=QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);    //设置标题
    isUntitled=false;   //默认已保存过
    ui->textEdit->document()->setModified(false);   //设置为未修改
    setWindowModified(false);   //不显示修改提示

    return true;
}

//点击右上角图标退出
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isSaved())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

//打开简介菜单
void MainWindow::showIntro()
{
    IntroDlg *introDlg;
    introDlg=new IntroDlg();
    introDlg->setWindowTitle("Introduction");
    introDlg->show();
}

//搜索菜单
void MainWindow::showFind()
{
    findDlg = new QDialog(this);
    findDlg->setWindowTitle("Find");

    findLineEdit = new QLineEdit(findDlg);
    QPushButton *btn= new QPushButton("Find Next", findDlg);
    QVBoxLayout *layout= new QVBoxLayout(findDlg);

    QFont btnFont;
    btnFont.setPointSize(12);
    btnFont.setFamily("Bahnschrift SemiCondensed");
    btn->setFont(btnFont);

    //加入控件到界面中
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);

    connect(btn, SIGNAL(clicked()), this, SLOT(findText()));
}

//搜索
void MainWindow::findText()
{
    QString str = findLineEdit->text();

    //若输出内容为空 报错
    if(QString(str).isEmpty())
    {
        QMessageBox::warning(this,"Warning","No input.",QMessageBox::Ok);
    }
    else
    {
        if(ui->textEdit->find(str))
        {
            // 查找到后高亮显示
            QPalette palette = ui->textEdit->palette();
            palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
            ui->textEdit->setPalette(palette);
        }
        //若未找到
        else
        {
            QMessageBox::StandardButton btn;
            btn=QMessageBox::information(this,"Warning","Couldn't find, try again?",QMessageBox::Ok,QMessageBox::No);

            //指针返回文档开头 重新搜索
            if(btn==QMessageBox::Ok)
            {
                ui->textEdit->moveCursor(QTextCursor::Start);
                findText();
            }
        }
    }
}


/*事件定义部分*/


//菜单栏new事件
void MainWindow::on_actionNew_triggered()
{
    newFile();
}
//菜单栏save事件
void MainWindow::on_actionSave_S_triggered()
{
    save();
}
//菜单栏saveas事件
void MainWindow::on_actionSave_as_A_triggered()
{
    saveAs();
}
//菜单栏open事件
void MainWindow::on_actionOpen_O_triggered()
{
    if(isSaved())
    {

        QString fileName=QFileDialog::getOpenFileName(this);

        if(!fileName.isEmpty())
        {
            openFile(fileName);
            ui->textEdit->setVisible(true); //将窗体设置为可见
        }
    }
}
//菜单栏close事件
void MainWindow::on_actionClose_C_triggered()
{
    if(isSaved())
    {
        ui->textEdit->setVisible(false);    //将窗体设置为不可见
        ui->textEdit->document()->setModified(false);   //设置为未修改 以防关闭程序时重复询问是否保存
    }
}
//菜单栏exit事件
void MainWindow::on_actionExit_X_triggered()
{
    on_actionClose_C_triggered();
    qApp->quit();
}
//菜单栏intro事件
void MainWindow::on_actionIntro_I_triggered()
{
    showIntro();
}
//菜单栏copy事件
void MainWindow::on_actionCopy_C_triggered()
{
    ui->textEdit->copy();
}
//菜单栏paste事件
void MainWindow::on_actionPaste_V_triggered()
{
    ui->textEdit->paste();
}
//菜单栏cut事件
void MainWindow::on_actionCut_X_triggered()
{
    ui->textEdit->cut();
}
//菜单栏undo事件
void MainWindow::on_actionUndo_Z_triggered()
{
    ui->textEdit->undo();
}
//菜单栏redo事件
void MainWindow::on_actionRedo_Y_triggered()
{
    ui->textEdit->redo();
}
//菜单栏find事件
void MainWindow::on_actionFind_F_triggered()
{
    findDlg->show();
}
//菜单栏font事件
void MainWindow::on_actionFont_N_triggered()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok,QFont("微软雅黑",16),this);
    if(ok)
    {
        ui->textEdit->setFont(font);
    }
}
//菜单栏color事件
void MainWindow::on_actionColor_M_triggered()
{
    QPalette pal=ui->textEdit->palette();   //获取调色板
    QColor inColor=pal.color(QPalette::Text);   //获取初始颜色

    QColor color=QColorDialog::getColor(inColor,this,"Set color",QColorDialog::ShowAlphaChannel);

    pal.setColor(QPalette::Text,color);
    ui->textEdit->setPalette(pal);
}
