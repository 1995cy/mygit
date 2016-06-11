#include "mainwindow.h"

#include "ui_mainwindow.h"

bool isop = false;			//标记是否为新建的文件还是打开的旧文件
int lineNumber = 1;			//标记行号
bool ischange = false;		//标记是否修改了文本

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //进行菜单栏的按钮的信号和槽的绑定
    QObject::connect(ui->action_new,SIGNAL(triggered())
              ,this,SLOT(on_new()));
    QObject::connect(ui->action_exit,SIGNAL(triggered())
              ,this,SLOT(on_exit()));

    QObject::connect(ui->action_save,SIGNAL(triggered())
              ,this,SLOT(on_save()));
    QObject::connect(ui->action_about,SIGNAL(triggered())
              ,this,SLOT(on_about()));
    QObject::connect(ui->action_help,SIGNAL(triggered())
              ,this,SLOT(on_help()));
    QObject::connect(ui->action_open,SIGNAL(triggered())
              ,this,SLOT(on_open()));
    QObject::connect(ui->action_Asave,SIGNAL(triggered())
              ,this,SLOT(on_Asave()));
    QObject::connect(ui->action_font,SIGNAL(triggered())
              ,this,SLOT(on_font()));
    QObject::connect(ui->action_color,SIGNAL(triggered())
              ,this,SLOT(on_color()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//文本变化时进行的操作
void MainWindow::on_textEdit_textChanged()
{
    /*//获取到当前的行数
    int i = ui->textEdit->document()->lineCount();

    if(i > lineNumber){
       while(lineNumber < i){
            lineNumber++;

          // ui->listWidget->addItem(QString::number(lineNumber));
        }
    }*/
    ischange = true;
}

//保存文件
void MainWindow::saveFile(QString fileName){
    QFile *file = new QFile;		//文件对象指针
    file->setFileName(fileName);	//设置文件的绝对路径和名称

    //根据绝对路径文件名打开
    bool isopen = file->open(QIODevice::WriteOnly);
    if(isopen){
        QTextStream out(file);	//打开文件流
        out<<ui->textEdit->toPlainText();	//通过操作符进行文件写操作

        file->close();	//关闭文件指针
        delete file;	//回收文件指针

        isop = true;
        ischange = false;
    }else{
        //使用对话框进行提示
        QMessageBox::warning(this,"失败",file->errorString());
    }
}

//新建文件的槽
void MainWindow::on_new(){
    if(ui->textEdit->document()->isModified()){//判断是否进行了修改
        isSave(true);

    }else{//如果没有进行修改则直接新建
       clear_new();
    }
}

void MainWindow::clear_new(){
    ui->textEdit->clear();
    this->setWindowTitle("新建");
    isop = false;
}

void MainWindow::isSave(bool isnew){
    if(ischange){
     switch( QMessageBox::information(this,ui->textEdit->windowTitle(),
            "该文档有修改未保存，是否进行保存?",
           "保存!", "不存!", "取消!",
            0,      // Enter == button 0
            2 ) ) { // Escape == button 2
        case 0: // Save被点击或者Alt+S被按下或者Enter被按下。
            // 保存
                on_save();
            if(isnew){
                clear_new();
            }else{
               this->close();
            }

            break;
        case 1: // Discard被点击或者Alt+D被按下。
            // 不保存但退出
            if(isnew){
                clear_new();
            }else{
                this->close();
            }
            break;
        case 2: // Cancel被点击或者Alt+C被按下或者Escape被按下。
            // 不退出
            break;
    }
   }
}

//退出程序的槽
void MainWindow::on_exit(){
    isSave(false);
}

//保存文件的槽
void MainWindow::on_save(){
    if(isop){//判断是新建的文件还是已经存在的文件
        QString fileName = this->windowTitle();
        saveFile(fileName);
    }else{
         on_Asave();
    }

}

//另存为的槽
void MainWindow::on_Asave(){
    //打开保存文件的对话框
    QString fileName = QFileDialog::getSaveFileName(this,
                  "保存文件",QDir::currentPath());
    if(fileName.isEmpty()){//判断是否进行了操作
       return ;
    }
    saveFile(fileName);

}

//帮助的槽
void MainWindow::on_help(){
    QMessageBox::warning(this,"帮助","这么简单的记事本还要写帮助啊!");
}

//关于的槽
void MainWindow::on_about(){
    QMessageBox::about(this,"关于Cnotepad","由C_H制作，虽然简陋，"
         "但是基本功能还是有的，凑合着用吧!");
}

//打开文件的槽
void MainWindow::on_open(){
    //打开文件的对话框
    QString fileName = QFileDialog::getOpenFileName(this,
               "打开文件",QDir::currentPath());

    if(fileName.isEmpty()){
       return ;
    }

    //将文件的绝对路径设置为标题
    this->setWindowTitle(fileName);

    //文件操作指针
    QFile *file = new QFile;
    //将文件指针指向相应的文件
    file->setFileName(fileName);

    //打开文件
    bool isopen = file->open(QIODevice::ReadOnly);
    if(isopen){
        QTextStream in(file);//打开文件流
        ui->textEdit->setText(in.readAll());//获取文件的内容

        //关闭文件指针
        file->close();
        //删除文件指针
        delete file;

        isop = true;
    }else{
        QMessageBox::warning(this,"打开失败",file->errorString());
    }

}

//修改字体的槽
void MainWindow::on_font(){
    bool ok;
    QFont font = QFontDialog::getFont(&ok,ui->textEdit->font());

    if(ok){
        ui->textEdit->setFont(font);
    }else{
       return ;
    }
}
//修改颜色的槽
void MainWindow::on_color(){
    QColor color = QColorDialog::getColor(Qt::black,this);
    if(color.isValid()){
        ui->textEdit->setTextColor(color);
    }
}
