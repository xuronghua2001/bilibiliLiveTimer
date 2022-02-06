<<<<<<< Updated upstream
#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QObject>
class MyThread : public QObject
{
  Q_OBJECT
      public:
  int id=0;
  explicit MyThread(QObject *parent = nullptr);

  void setFlag(bool flag = true);
signals:

  void mySignal();

public slots:  void myTimeout();
private:
  bool isStop;
};

#endif // MYTHREAD_H
=======
#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QObject>
class MyThread : public QObject
{
  Q_OBJECT
      public:
  int id=0;
  explicit MyThread(QObject *parent = nullptr);

  void setFlag(bool flag = true);
signals:

  void mySignal();

public slots:  void myTimeout();
private:
  bool isStop;
};

#endif // MYTHREAD_H
>>>>>>> Stashed changes
