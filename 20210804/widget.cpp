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
#include <QThread>
#include <QUrl>
#include <QDebug>
#include <QIODevice>
#include <QVector>
static int sum,k,t,isNormal,err;
bool bb=1;QVector<int> qv,pv;
static int bian=0,sss=0;static int s=1,ii=-1;
QString url,z,aa,nitian,zzz,surl;
QStringList uidList,nameList,liveList;
QString url_prefix,url_suffix;
QNetworkAccessManager *manager = new QNetworkAccessManager();
void Widget::remake()
{
  manager->clearAccessCache();s=1;bian=0;sum=0;zzz=nullptr;z=nullptr;
  nameList.clear();liveList.clear();uidList.clear();pv.clear();
  ui->progressBar->setFormat(nullptr);
    QString path = QCoreApplication::applicationDirPath();
    path+="/list.txt";
    readFile(path);
    if (!timer->isActive())
    {
    timer->start();
    timer_0->start();}
    ui->textEdit->setText("目前在线:");
    ui->listWidget->clear();
    ui->label_5->setText("在线人数:");
    if(bb)
    QMessageBox::information(nullptr,"提示","已经重新启动自动请求");
    ui->checkBox->setEnabled(0);
    thread->start();
    emit startThread();
}
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->listWidget->setMouseTracking(1);
    ui->lcdNumber->segmentStyle();ui->lcdNumber_2->segmentStyle();
    connect(timer_1,SIGNAL(timeout()),this,SLOT(slot_changeIcon()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(remake()));
   // connect(timer, SIGNAL(timeout()), this,SLOT(up()));
    ui->checkBox->setEnabled(0);
    ui->progressBar->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    timer->start(10000);
    k=3;t=0;isNormal = 1;aa=nullptr;
    connect(timer_0, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    QString path = QCoreApplication::applicationDirPath();
    path+="/list.txt";
    readFile(path);
    timer_0->start(1000);
    myT = new MyThread;

    thread = new QThread(this);

    myT->moveToThread(thread);

    connect(myT, &MyThread::mySignal, this, &Widget::dealSignal);

    connect(this, &Widget::startThread,myT, &MyThread::myTimeout);
    //当窗口销毁时，关闭线程
    connect(this,&Widget::destroyed, this, &Widget::dealClose);
    ui->pushButton->clicked(1);
    connect(this,SIGNAL(ggooo()),this,SLOT(additem()));
}
void Widget::dealSignal()
{
  int n=ui->listView->model()->rowCount();
  if(n<=bian)
  {bian=0;sum=0;s=0;zzz=z;z=nullptr;pv=qv;qv.clear();}
  QModelIndex Index=ui->listView->model()->index(bian,0);
  QString num=Index.data().toString();
    QUrl url("https://api.bilibili.com/x/space/acc/info?mid="+num+"&jsonp=jsonp");
    m_Reply = manager->get(QNetworkRequest(url));
    connect(m_Reply,SIGNAL(finished()),this,SLOT(finishedSlot()));
}
void Widget::finishedSlot()
{
  m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  m_Reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
  if(m_Reply->error() == QNetworkReply::NoError)
  {int p;
    QByteArray bytes;
    bytes.resize(10000);
    while(!m_Reply->atEnd()/*bytes==nullptr*/)
      bytes += m_Reply->readAll();
    m_Reply->abort();m_Reply->deleteLater();
    const QByteArray cat=bytes;
    QString string =QString::fromUtf8(cat,cat.size());
    if(s){
     // m_Reply = manager->get(QNetworkRequest(QUrl("https://asoulcnki.asia/rank")));
    int i=string.lastIndexOf("liveStatus");
      ii=i;
//      qDebug()<<"i:"<<i;qDebug()<<string;
    int j=string.lastIndexOf("sex");
    QString name=string.mid(j-20,17);
    if (name.lastIndexOf(":")!=-1)
    {
      int a=name.lastIndexOf(":");
      name=name.mid(a+2,-1);
    }
    nameList<<name;
    QString live;
    QString uu=string.mid(i+21,50);
    int nn=uu.lastIndexOf("title");
    live=uu.mid(-1,nn-2);
    liveList<<live;//qDebug()<<live;
    int jj=string.indexOf("face");
    surl=string.mid(jj+7,73);
    m_Reply = manager->get(QNetworkRequest(surl));
    connect(m_Reply,SIGNAL(readyRead()),this,SLOT(getURLImage()));
    }
    int i=string.lastIndexOf("liveStatus");//qDebug()<<"i:"<<i;
    QString q=string.mid(i+12,1);
     p=q.toInt();//qDebug()<<"status:"<<p;
   if(p&&i)
     {z+=nameList.at(bian).data();qv.append(bian);
     z+='\n';sum++;//qDebug()<<z;
   }
  }
  else
    {err=m_Reply->error();Widget::dealClose();}
  bian++; //  qDebug()<<zzz; qDebug()<<"df:"<<bian;
}
//网络信号结束响应
void Widget::dealClose()
{
    if(thread->isRunning() == false)//判断线程是否终止
    {
        return;
    }
    myT->setFlag(true);
    thread->quit();
    thread->wait();//将等待线程完成本次响应操作后终止
    delete myT;
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
           uidList<<line;}
        }
        file.close();
        if (!uidList.isEmpty())
        {
            model = new QStringListModel(uidList);
          ui->listView->setModel(model);
            uidList.clear();}
        else
        {
            timer->stop();timer_0->stop();
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
    QStringList list2 = zzz.split(QLatin1Char('\n'));
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
void Widget::onTimerOut()
{
     if(s)
     ui->label_5->setText("在线人数："+QString::number(sum));
     ui->progressBar->setTextVisible(true);
     ui->lcdNumber->display(myT->id);
     ui->progressBar->setValue(100*(double(bian)/ui->listView->model()->rowCount()));
     if(ui->listWidget->item(bian))
       ui->progressBar->setFormat(ui->listWidget->item(bian)->text());
     if(ui->progressBar->value()/100){up();}
     if(k>=0)
     {ui->lcdNumber_2->display(k);k--;}
     if (k<0)
     k=3;
}
Widget::~Widget()
{
  dealClose();
  delete manager;
  qApp->quit();
  delete ui;
  exit(0);
}
void Widget::getURLImage()
{
  m_Reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
  m_Reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
  if(m_Reply->error() == QNetworkReply::NoError)
  {
    connect(m_Reply, &QNetworkReply::finished, [=]{
        QPixmap pixmap;
        if(pixmap.loadFromData(m_Reply->readAll())){
        pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
          pixmap.save(QString::number(bian)+".jpg",nullptr,-1);
        emit ggooo();
      }
    });
}
  else
        {s=1;err=m_Reply->error();}
//m_Reply->deleteLater();
}

void Widget::additem()
{
     ui->listWidget->setIconSize(QSize(30,30));
     ui->listWidget->addItem(nameList.at(bian-1));ui->listWidget->item(bian-1)->setIcon(QIcon(QString::number(bian)+".jpg"));
     if(!qv.isEmpty()){
       if(qv.back()==(bian-1))
       ui->listWidget->item(bian-1)->setForeground(QColor(255,215,130));
     else
         ui->listWidget->item(bian-1)->setForeground(Qt::gray);}
     else
       ui->listWidget->item(bian-1)->setForeground(Qt::gray);
}

void Widget::up()
{ int n=ui->listView->model()->rowCount();
  if(bian>=n){
    sss=sum;zzz=z;pv=qv;
  }
  if (!err){
    if(n==ui->listWidget->count())
      ui->checkBox->setEnabled(1);
    if(!pv.isEmpty()){
      for(int i=0;i<n;i++)
        ui->listWidget->item(i)->setForeground(Qt::gray);
    for(int i=0;i<pv.size();i++)
        ui->listWidget->item(pv.at(i))->setForeground(QColor(255,215,130));}
    //ui->listWidget->update();
    if(ui->checkBox->checkState())
    {ui->checkBox->toggle();ui->checkBox->toggle();}
    //if(s==0)
    ui->textEdit->setText("目前在线:\n"+zzz);
    //if(s)
    //  ui->label_5->setText("在线人数："+QString::number(sum));
    QString rr="在线人数：";
    rr+=QString("%1").arg(sss);
    ui->label_5->setText(rr);
    if (aa!=zzz)
    yaoLaiLe();
    //ui->listWidget->update();
    aa=zzz;
    }
    else
    {
      timer->stop();timer_0->stop();
        QString e="错误代码:"+QString::number(err)+"\n无法连接到互联网\n请检查电脑联网后继续！";
        if(bb)
        QMessageBox::information(nullptr,"提示",e);
        }ui->listWidget->update();
}
void Widget::on_pushButton_clicked()
{
  thread->start();
  emit startThread();

}
void Widget::on_listWidget_itemClicked()
{
  int row=ui->listWidget->currentRow();
  QModelIndex Index = ui->listView->model()->index(row,0);
  ui->listView->setCurrentIndex(Index);
}
void Widget::on_listWidget_itemDoubleClicked()
{
    QDesktopServices::openUrl(QUrl(liveList.at(ui->listWidget->currentRow())));
}
void Widget::on_listView_clicked(QModelIndex index)
{
    ui->listWidget->setCurrentRow(index.row());
}
void Widget::on_listView_doubleClicked(QModelIndex index)
{
  QDesktopServices::openUrl(QUrl("https://space.bilibili.com/"+index.data().toString()));
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
        myTrayIcon->setToolTip(tr("bilibili直播实时提醒（多线程稳定）"));
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
  ui->listWidget->setMouseTracking(1);
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

