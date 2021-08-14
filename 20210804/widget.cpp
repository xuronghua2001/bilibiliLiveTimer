#include "widget.h"
#include "ui_widget.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <QMessageBox>
#include <QFile>
//#include <QClipboard>
#include <QIcon>
#include <QDesktopServices>
#include <QPixmap>
#include <QThread>
static int sum,k,s,t,r,isNormal;
QString url,z=nullptr,aa,surl;
QStringList nameList,liveList;
QString url_prefix,url_suffix;
QNetworkAccessManager *manager = new QNetworkAccessManager();
void Widget::remake()
{
    ui->pushButton_2->setEnabled(0);
    if (!timer->isActive())
    {timer->start();
    timer_0->start();}
    ui->textEdit->clear();
    ui->listWidget->clear();
    QMessageBox::information(nullptr,"提示","已经重新启动自动请求");
    s=1;
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->pushButton_2->setEnabled(0);
    s=1;this->setMouseTracking(0);
    connect(timer_1,SIGNAL(timeout()),this,SLOT(slot_changeIcon()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(remake()));
    connect(timer, &QTimer::timeout, this, [&](){
    up();
    });
    timer->start(10000);
    k=10,t=0;isNormal = 1;
    connect(timer_0, SIGNAL(timeout()), this, SLOT(onTimerOut()));

    timer_0->start(1000);
    QString path = QCoreApplication::applicationDirPath();
    path+="/list.txt";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        timer->stop();timer_0->stop();//timer->deleteLater();timer_0->deleteLater();
        QMessageBox::warning(nullptr,"提示","没有找到list.txt\n请检查文件后重新打开！");
        }
    QTextStream read(&file);
        while (!read.atEnd())
        {
           QString line = read.readLine();
           if (line.isEmpty())
               break;
           if (line.indexOf("//")==0)
               continue;
           else{
           if(line.lastIndexOf("//"))
           {int i=line.lastIndexOf("/");
               line=line.mid(0,i-1);}
           nameList<<line;}
        }
        file.close();
        if (!nameList.isEmpty())
        {
            model = new QStringListModel(nameList);
            ui->listView->setModel(model);
            nameList.clear();}
        else
        {
            timer->stop();timer_0->stop();s=0;//timer->deleteLater();timer_0->deleteLater();
            QMessageBox::information(nullptr,"提示","程序目录下的list.txt为空\ntxt编辑格式为每行一个uid，//后可以备注名称\n示例：672328094//然然");
            this->on_exitAppAction();
        }
}
void Widget::yaoLaiLe()
{
    if (isHidden())
    timer_1->start(1000);
    QMessageBox::information(nullptr,"提示","在线人数发生变化\n请检查！");
}
void Widget::slot_changeIcon()
{
    //q=QIcon(":/b.png");
    if(isNormal){
        isNormal = 0;
        myTrayIcon ->setVisible(1);
    }else{
        isNormal = 1;
        myTrayIcon ->setVisible(0);
    }
}

class atiNet : public QObject
{
    Q_OBJECT
public:
    explicit atiNet();
     static QString  getHtml(QString url);
     static int checkNetWorkOnline(QString);

signals:
public slots:
     void replyFinished(QNetworkReply*);
};
int atiNet::checkNetWorkOnline(QString url)
{
    QNetworkRequest request;
    QEventLoop eventLoop;
    request.setUrl(QUrl(url));
    QNetworkReply* reply = manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    int i=reply->error();
    reply->deleteLater();
    if (i==0)
        return 1;
    else
        return 0;
}
void atiNet::replyFinished(QNetworkReply*r)
{

     r->deleteLater();
}
QString  atiNet::getHtml(QString url)
{
       QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
//       reply->deleteLater();
       QByteArray responseData;
       QEventLoop eventLoop;
       connect(manager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
       eventLoop.exec();       //block until finish
       responseData = reply->readAll();
       reply->deleteLater();eventLoop.deleteLater();
       return QString(responseData);
}
void Widget::onTimerOut()
{
     if(k>=0)
     {ui->lcdNumber_2->display(k);k--;}
     else
     {k=10;t++;ui->lcdNumber->display(t);}
}
Widget::~Widget()
{
    delete ui;
}
void Widget::Sleep(int msec)
{
    qint64 dieTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    while( QDateTime::currentDateTime().toMSecsSinceEpoch()-dieTime < msec )
    {
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents,100);
        QThread::msleep(1);
    }
}
void Widget::getURLImage(QListWidgetItem *LWI)
{
    QNetworkRequest request;
    request.setUrl(QUrl(surl));
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [=]{
        QPixmap pixmap;
        pixmap.loadFromData(reply->readAll());
        pixmap = pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        reply->deleteLater();
        if (LWI)
            LWI->setIcon(QIcon(pixmap));
    });surl.clear();
}
void Widget::up()
{
    if (atiNet::checkNetWorkOnline("https://www.baidu.com")){
    sum=0;
    if(s)
    {
        r=ui->listView->model()->rowCount();
        for (int n=0;n<ui->listView->model()->rowCount();++n)
        {
            QModelIndex Index=ui->listView->model()->index(n,0);
            QString num=Index.data().toString();
            url_prefix="https://api.bilibili.com/x/space/acc/info?mid=";
            url_suffix="&jsonp=jsonp";
            url=url_prefix+num+url_suffix;
            QString html= atiNet::getHtml(url);
            if(html.indexOf("-412")!=-1)break;
            int j=html.lastIndexOf("sex");
            int jj=html.indexOf("face");
            surl=html.mid(jj+7,73);
            QString name=html.mid(j-20,17);
            if (name.lastIndexOf(":")!=-1)
            {
               int a=name.lastIndexOf(":");
               name=name.mid(a+2,-1);
            }
            if (name!=nullptr)
            {ui->listWidget->addItem(name);
            getURLImage(ui->listWidget->item(n));
            }Sleep(200+2*n);
         }url.clear();
        s=0;Sleep(2000);
      }
    for (int n=0;n<ui->listView->model()->rowCount();n++)
        {
        if (n==0)
        z="在线人数：";
        QModelIndex Index=ui->listView->model()->index(n,0);
        QString uid=Index.data().toString();
        url=url_prefix+uid+url_suffix;
        QString html= atiNet::getHtml(url);
        int i=html.lastIndexOf("liveStatus");
        QString state=html.mid(i+12, 1);
        QString uu=html.mid(i+21,50);
        int nn=uu.lastIndexOf("title");
        uu=uu.mid(-1,nn-2);
        liveList<<uu;
        if(state.toInt()==2)
            {   timer->stop();
                timer_0->stop();
                ui->pushButton_2->setEnabled(1);
                 QMessageBox::warning(nullptr,"提示","请求被拦截\n请过段时间后开启自动检测！\ncheck频繁以及uid过多会导致此问题");
                 break;
            }
        int *ttk = new int[r];
        ttk[n]=state.toInt();
        if (ttk[n])
            {
            sum++;
            ui->listWidget->setCurrentRow(n);
            z+="\n";
            z+=ui->listWidget->item(n)->text();
            }
        delete []ttk;
        Sleep(400+80*n);
        }
    ui->textEdit->setText(z);
    QString bb="在线人数：";
    bb+=QString("%1").arg(sum);
    ui->label_5->setText(bb);
    if (aa!=z)
    yaoLaiLe();
    aa=z;
    }
    else
        {
        timer->stop();timer_0->stop();
        //timer->deleteLater();timer_0->deleteLater();
        ui->pushButton_2->setEnabled(1);
        QMessageBox::information(nullptr,"提示","无法连接到互联网\n请检查电脑联网后继续！");
        }
}
void Widget::on_pushButton_clicked()
{
    up();QMessageBox::warning(nullptr,"提示","已手动请求\n请不要频繁点击！");
}
void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QDesktopServices::openUrl(QUrl(liveList.at(ui->listWidget->currentRow())));

}
/*void Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(item->text());
    //QMessageBox::information(nullptr,"提示","已复制选中名称");
}*/
void Widget::on_exitAppAction()
{
    exit(0);
}
void Widget::on_showMainAction()
{
    QApplication::setQuitOnLastWindowClosed(1);
    myTrayIcon->deleteLater();timer_1->stop();
    this->show();
}
void Widget::on_showAbout()
{
    QMessageBox::about(nullptr,"关于","BUG Reporting:https://github.com/xuronghua2001/bilibili/issues/new\nEmail:xuronghua2001@outlook.com");
}
void Widget::iconIsActived(QSystemTrayIcon::ActivationReason reason)
  {

    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
     {
       on_showMainAction();
       timer_1->stop();timer_1->deleteLater();
       break;
     }
    case QSystemTrayIcon::DoubleClick:
    {
      on_showMainAction(); timer_1->stop();
      break;
     }
    default:
     break;
    }
  }
void Widget::on_pushButton_3_clicked()
{
    QApplication::setQuitOnLastWindowClosed(0);
    this->hide();
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        mExitAction = new QAction(tr("退出"),this);//右键点图标之后的选项
        mExitAction->setIcon(QIcon::fromTheme("application-exit"));
        about=new QAction (tr("关于"),this);
        about->setIcon(QIcon::fromTheme("help-about"));
        no_notice=new QAction(tr("勿扰模式"),this);
        myMenu = new QMenu();
        myMenu->addAction(mExitAction);
        myMenu->addAction(about);
        myMenu->addAction(no_notice);
        myTrayIcon = new QSystemTrayIcon(this);
        myTrayIcon->setIcon(QIcon(":/b.png"));
        myTrayIcon->setToolTip(tr("bilibili直播实时提醒"));
        QApplication::processEvents();
        myTrayIcon->showMessage("托盘","托盘管理",QSystemTrayIcon::Information,1000);
        myTrayIcon->setContextMenu(myMenu);
        myTrayIcon->show();}
        connect(about,SIGNAL(triggered()),this,SLOT(on_showAbout()));
        //connect(no_notice,SIGNAL(triggered()),this,SLOT(event->hide()));
        connect(mExitAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
        connect(myTrayIcon , SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
}


