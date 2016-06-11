#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lastwindows.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    lastWindows *last = new lastWindows;
    last->show();
}


void MainWindow::on_actionAdd_bookmark_triggered()
{
    QDomDocument doc( "MyML" );

    QFile file("MyML.xml");
        //将文件指针指向相应的文件
   // file->setFileName("MyML.xml");
      if ( !file.open(QIODevice::ReadOnly) ){
          qDebug()<<"fin";
          return;
      }
      if ( !doc.setContent(&file)) {
qDebug()<<"f";
          file.close();
          return;
      }
      file.close();


      QDomElement docElem = doc.documentElement();

       QDomNode n = docElem.firstChild();
       while( !n.isNull() ) {
           qDebug()<<"in";
           //QDomElement e = n.toElement(); // try to convert the node to an element.
          // if( !e.isNull() ) {
              qDebug()<<qPrintable(n.toElement().text()); // the node really is an element.
          // }
              n = n.nextSibling();
           // docElem.removeChild(n);
       }

       // Here we append a new element to the end of the document
       QDomElement elem = doc.createElement( "img" );

       docElem.appendChild( elem );

       QDomText t = doc.createTextNode("where are you");
       elem.appendChild( t );

       QString xml = doc.toString();

       QFile fil("MyML.xml");
       fil.open(QIODevice::WriteOnly);
       QTextStream out(&fil);
       out<<xml;
       fil.close();

      /* QDomDocument doc( "MyML" );
         QDomElement root = doc.createElement( "MyML" );
         doc.appendChild( root );

         QDomElement tag = doc.createElement( "Greeting" );
         root.appendChild( tag );

         QDomText t = doc.createTextNode( "Hello World" );
         tag.appendChild( t );

         QString xml = doc.toString();

         QFile file("MyML.xml");
         file.open(QIODevice::WriteOnly);
         QTextStream out(&file);
         out<<xml;
         file.close();*/


}
