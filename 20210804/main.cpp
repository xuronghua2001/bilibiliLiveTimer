#include "widget.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
 /* QFile file(":/Ubuntu.qss");
  file.open(QFile::ReadOnly);
  QTextStream filetext(&file);
  QString styleSheet = filetext.readAll();
  qApp->setStyleSheet(styleSheet);*/
    Widget w;
    w.show();
    return a.exec();
}
