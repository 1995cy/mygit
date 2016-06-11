#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    scribbleArea = new ScribbleArea;
    this->setFixedSize(250,305);
    ui->controlButton->setText("未打开");

    QDesktopWidget *desktopWidget = QApplication::desktop();
    QRect deskRect = desktopWidget->screenGeometry();
    int g_width = deskRect.width();
    move(g_width,0);

setWindowFlags(Qt::WindowStaysOnTopHint);

    QObject::connect(ui->colorButton,SIGNAL(clicked()),this,SLOT(penColor()));
    QObject::connect(ui->sizeButton,SIGNAL(clicked()),this,SLOT(penWidth()));

    QPalette pal = ui->colorButton->palette();
    pal.setColor(QPalette::Button,Qt::red);
    ui->colorButton->setPalette(pal);
    ui->colorButton->setAutoFillBackground(true);
    ui->colorButton->setFlat(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event){
    scribbleArea->close();
    event->accept();
}

void Widget::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid()){
        scribbleArea->setPenColor(newColor);
        QPalette pal = ui->colorButton->palette();
        pal.setColor(QPalette::Button,newColor);
        ui->colorButton->setPalette(pal);
        ui->colorButton->setAutoFillBackground(true);
        ui->colorButton->setFlat(true);
    }

}

void Widget::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("笔迹粗细"),
                                        tr("请输入笔迹尺寸:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void Widget::on_controlButton_clicked()
{
    if(scribbleArea->isopen){
        scribbleArea->hide();
        scribbleArea->clearImage();
        scribbleArea->isopen = false;
        ui->controlButton->setText("未打开");

    }else{
        scribbleArea->openImage();
       // scribbleArea->showFullScreen();
        scribbleArea->showMaximized();
       // scribbleArea->show();
        scribbleArea->isopen = true;
        ui->controlButton->setText("已打开");
    }
}

void Widget::open_is(){
    if(!scribbleArea->isopen){
        scribbleArea->openImage();
       // scribbleArea->showFullScreen();
        scribbleArea->showMaximized();
       // scribbleArea->show();
        scribbleArea->isopen = true;
        ui->controlButton->setText("已打开");
    }
}

void Widget::on_lineButton_clicked()
{
    open_is();
    scribbleArea->what_do = "LINE";
}

void Widget::on_rectButton_clicked()
{
    open_is();
    scribbleArea->what_do = "RECT";
}
//yuan
void Widget::on_circularButton_clicked()
{
    open_is();
    scribbleArea->what_do = "CIR";
}

void Widget::on_penButton_clicked()
{
    open_is();
    scribbleArea->what_do = "PEN";
}

void Widget::on_backButton_clicked()
{
    open_is();
    scribbleArea->ph_back();
}

void Widget::on_redoButton_clicked()
{
    open_is();
    scribbleArea->re_do();
}

void Widget::on_wordButton_clicked()
{
    QString text = ui->textEdit->toPlainText();
    if(!text.isEmpty()){
        open_is();
        scribbleArea->what_do = "TEXT";
        scribbleArea->text = text;
    }
}

void Widget::on_sizeWordButton_clicked()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("文字粗细"),
                                        tr("请输入文字尺寸:"),
                                        scribbleArea->wordWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}
