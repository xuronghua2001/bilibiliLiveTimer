#include "MyThread.h"
#include <QThread>
#include<QDebug>
MyThread::MyThread(QObject *parent) : QObject(parent)
{
  isStop = false;
}

void MyThread::myTimeout()
{//qDebug()<<"开启多线程";
  while(isStop == false)
  {
    id++;
    QThread::sleep(3);//强制暂停3秒，使得每三秒产生一次信号，
    emit mySignal();//每3秒发送一次信号
    //qDebug()<<"子线程号"<<QThread::currentThread();
  }
}
void MyThread::setFlag(bool flag)
{
  isStop = flag;//定义标志位用于判断线程是否终止
}
