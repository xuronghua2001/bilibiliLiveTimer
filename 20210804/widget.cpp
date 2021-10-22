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
#include <QUrl>
#include <QDebug>
static int sum,k,s,t,r,isNormal,err;
bool bb=1;
QString url,z=nullptr,aa,surl;
QStringList nameList,liveList;
QString url_prefix,url_suffix;
QNetworkAccessManager *manager = new QNetworkAccessManager();
void Widget::remake()
{

    liveList.clear();
    QString path = QCoreApplication::applicationDirPath();
    path+="/list.txt";
    readFile(path);
    if (!timer->isActive())
    {
    timer->start();
    timer_0->start();}
    ui->textEdit->clear();
    ui->listWidget->clear();
    ui->label_5->setText("在线人数:");
    if(bb)
    QMessageBox::information(nullptr,"提示","已经重新启动自动请求");
    ui->checkBox->setEnabled(0);
    s=1;//k=10;
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    s=1;ui->listWidget->setMouseTracking(1);
    connect(timer_1,SIGNAL(timeout()),this,SLOT(slot_changeIcon()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(remake()));
    connect(timer, &QTimer::timeout, this, [&](){
        //if(l)
        up();
    });
    ui->checkBox->setEnabled(0);
    timer->start(10000);
    k=10;t=0;isNormal = 1;aa="目前在线：";
    connect(timer_0, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    QString path = QCoreApplication::applicationDirPath();
    path+="/list.txt";
    readFile(path);
    timer_0->start(1000);

}
void Widget::readFile(QString path)
{

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        timer->stop();timer_0->stop();
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
            timer->stop();timer_0->stop();s=0;
            QMessageBox::information(nullptr,"提示","程序目录下的list.txt为空\ntxt编辑格式为每行一个uid，//后可以备注名称\n示例：672328094//然然");
            this->on_exitAppAction();
        }
}

void Widget::yaoLaiLe()
{
    if (isHidden())
    timer_1->start(1000);
    QString off,on,ss;
    QStringList offline,online,ima;
    QStringList list1 = aa.split(QLatin1Char('\n'));
    QStringList list2 = z.split(QLatin1Char('\n'));
       for(QString &str:list1)
       if(!list2.contains(str))
           offline<<str;
    for(QString &str:list2)
    if(!list1.contains(str))
        online<<str;
    ima=list2;
    if(bb){
        if(!offline.isEmpty())
        {off=offline.join(",\n");
        off+="下线了!";}
        if(!online.isEmpty())
        {on=online.join(",\n");
        on+="上线了！";}
        if(on==nullptr)
        ss=on+off;
        else
        ss=on+'\n'+off;
        if(ui->checkBox->checkState())
        {ui->checkBox->toggle();ui->checkBox->toggle();}
    QMessageBox::information(nullptr,"提示","在线人数发生变化\n"+ss);}
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
    err=reply->error();
    reply->deleteLater();

        return err;
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
       err=reply->error();
       reply->deleteLater();eventLoop.deleteLater();
       return QString(responseData);
}
void Widget::onTimerOut()
{
     if(k>=0)
     {ui->lcdNumber_2->display(k);k--;}
     if (k==0)
     {k=10;ui->lcdNumber->display(t);}
}
Widget::~Widget()
{
  if(thread()->isRunning())
    thread()->quit();
  delete manager;
  qApp->quit();
    exit(0);delete ui;
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
        pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        if(reply->error())
        {s=1;err=reply->error();}
        reply->deleteLater();
        if (LWI)
            LWI->setIcon(QIcon(pixmap));
    });surl.clear();
}
void Widget::up()
{
    t++;
    if (!atiNet::checkNetWorkOnline("https://www.baidu.com")){
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
            int i=html.lastIndexOf("liveStatus");
            QString uu=html.mid(i+21,50);
            int nn=uu.lastIndexOf("title");
            uu=uu.mid(-1,nn-2);
            liveList<<uu;
            int j=html.lastIndexOf("sex");
            int jj=html.indexOf("face");
            surl=html.mid(jj+7,73);
            QString name=html.mid(j-20,17);
            if (name.lastIndexOf(":")!=-1)
            {
               int a=name.lastIndexOf(":");
               name=name.mid(a+2,-1);
            }
            ui->listWidget->setIconSize(QSize(30,30));
            if (name!=nullptr)
            {
                ui->listWidget->addItem(name);ui->listWidget->update();
            getURLImage(ui->listWidget->item(n));ui->listWidget->update();
            }Sleep(200+2*n);
         }url.clear();s=0;
         ui->checkBox->setEnabled(1);
        if(ui->listView->model()->rowCount()!=ui->listWidget->count())
        {s=1;ui->listWidget->clear();}
        Sleep(2000);
      }

    for (int n=0;n<ui->listView->model()->rowCount();n++)
        {

        if (n==0)
        z="目前在线：";
        QModelIndex Index=ui->listView->model()->index(n,0);
        QString uid=Index.data().toString();
        url=url_prefix+uid+url_suffix;
        QString html= atiNet::getHtml(url);
        int i=html.lastIndexOf("liveStatus");
        QString state=html.mid(i+12, 1);
        /*QString uu=html.mid(i+21,50);
        int nn=uu.lastIndexOf("title");
        uu=uu.mid(-1,nn-2);
        liveList<<uu;*/
        if(state.toInt()==2)
            {
             timer->stop();
             timer_0->stop();
                //ui->pushButton_2->setEnabled(1);
                if(bb)
             {
               ui->checkBox->setEnabled(0);
               QMessageBox::warning(nullptr,"提示","请求被拦截\n请过段时间后开启自动检测！\ncheck频繁以及uid过多会导致此问题");}
             break;
            }
        if (s)break;
        int *ttk = new int[r];
        ttk[n]=state.toInt();
        if (ttk[n])
            {
            sum++;
            //ui->listWidget->setCurrentRow(n);
            ui->listWidget->item(n)->setForeground(QColor(255,215,130));
            z+="\n";
            z+=ui->listWidget->item(n)->text();
            }
        else
            ui->listWidget->item(n)->setForeground(Qt::gray);
        delete []ttk;
        if(n!=r-1)
        Sleep(300+64*n);
        if(n==r-1)
        Sleep(200);
    }
    if(ui->checkBox->checkState()){ui->checkBox->toggle();ui->checkBox->toggle();}
    ui->textEdit->setText(z);
    QString rr="在线人数：";
    rr+=QString("%1").arg(sum);
    ui->label_5->setText(rr);
    if (aa!=z)
    yaoLaiLe();
    aa=z;
    }
    else
        {
      QString tc;
      switch (err) {
      case 2:
        tc="远程服务器在接收和处理整个回复之前过早关闭了连接";
      case 3:
        tc="未找到远程主机名（无效主机名）";
      case 4:
        tc="与远程服务器的连接超时";
      default :
        tc="检测到与服务器响应相关的未知错误";

      }
        timer->stop();timer_0->stop();
        //ui->pushButton_2->setEnabled(1);
        QString e="错误代码:"+QString::number(err)+"\n"+tc+"\n无法连接到互联网\n请检查电脑联网后继续！";
        if(bb)
        QMessageBox::information(nullptr,"提示",e);
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
    delete manager;
    exit(0);
}
void Widget::on_showMainAction()
{
    bb=true;
    QApplication::setQuitOnLastWindowClosed(1);
    if(timer_1->isActive())timer_1->stop();
    delete myTrayIcon;
    this->show();
}
void Widget::on_showAbout()
{
  QString link="https://github.com/xuronghua2001/bilibili/issues/new";
  QString msg = "BUG Reporting:<a href='https://github.com/xuronghua2001/bilibiliLiveTimer/issues'>Github</a><br>Update:     <a href='https://download.csdn.net/download/duckSoup_2001/20817924'>CSDN</a><br>E-mail:xuronghua2001@outlook.com" ;
    QMessageBox::about(nullptr,"关于",msg);
}
void Widget::iconIsActived(QSystemTrayIcon::ActivationReason reason)
  {

    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
     {
       on_showMainAction();
       if(timer_1->isActive())
       {timer_1->stop();timer_1->deleteLater();}
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
void Widget::on_no()
{
   no_notice->setChecked(0);
   if(bb==true)
       {bb=false;no_notice->setText("已开启勿扰模式");}
   else
       {bb=true;no_notice->setText("勿扰模式");}
}
void Widget::on_pushButton_3_clicked()
{
    QApplication::setQuitOnLastWindowClosed(0);
    this->hide();
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        mExitAction = new QAction(QIcon(":/exit.png"),tr("退出"),this);//右键点图标之后的选项
        about=new QAction (QIcon(":/about.png"),tr("关于"),this);
        no_notice=new QAction(QIcon(":/sleep.png"),tr("勿扰模式"),this);
        connect(no_notice,SIGNAL(triggered()),this,SLOT(on_no()));
        myMenu = new QMenu();
        myMenu->addAction(mExitAction);
        myMenu->addAction(about);
        myMenu->addAction(no_notice);
        myTrayIcon = new QSystemTrayIcon(this);
        myTrayIcon->setIcon(QIcon(":/b.png"));
        myTrayIcon->setToolTip(tr("bilibili直播实时提醒"));
        QApplication::processEvents();
        myTrayIcon->showMessage("托盘","托盘管理",QSystemTrayIcon::Information,10000);
        myTrayIcon->setContextMenu(myMenu);
        myTrayIcon->show();}
        connect(about,SIGNAL(triggered()),this,SLOT(on_showAbout()));
        connect(mExitAction,SIGNAL(triggered()),this,SLOT(on_exitAppAction()));
        connect(myTrayIcon , SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconIsActived(QSystemTrayIcon::ActivationReason)));
}
void Widget::on_pushButton_4_clicked()
{
    QString path = QCoreApplication::applicationDirPath();
    path+="/list.txt";
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
void Widget::on_listWidget_itemEntered(QListWidgetItem *item)
{
    if(item!=nullptr){
    QString s="打开";s+=item->text();s+="的直播间";
    item->setToolTip(s);}
}


void Widget::on_checkBox_stateChanged(int arg1)
{
  if(ui->listWidget->item(0)!=nullptr){
  int r=ui->listView->model()->rowCount();
  if(arg1==2&&s==0)
    for(int n=0;n<r;n++)
    {
      if(ui->listWidget->item(n)->foreground()!=QColor(255,215,130))
      ui->listWidget->item(n)->setHidden(1);
    }
  else
    for(int n=0;n<r;n++)
  ui->listWidget->item(n)->setHidden(0);
  }
}

