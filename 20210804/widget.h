#ifndef WIDGET_H
#define WIDGET_H
#include <QTimer>
#include <QMenu>
#include <QWidget>
#include <QStringListModel>
#include <QListWidgetItem>
#include <QSystemTrayIcon>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QTimer *timer=new QTimer(this);
    QTimer *timer_0=new QTimer(this);
    QTimer *timer_1=new QTimer(this);
private slots:
    void Sleep(int msec);
    void getURLImage(QListWidgetItem *LWI);
    void slot_changeIcon();
    void onTimerOut();
    void on_pushButton_clicked();
    void up();
    void yaoLaiLe();
    void remake();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_3_clicked();
    void on_showMainAction();
    void on_exitAppAction();
    void on_showAbout();
    void iconIsActived(QSystemTrayIcon::ActivationReason reason);
    void readFile(QString path);
    void on_pushButton_4_clicked();
    void on_listWidget_itemEntered(QListWidgetItem *item);
    void on_no();
    void on_checkBox_stateChanged(int arg1);

  signals:
    void muade();
private:
    QAction *mShowMainAction;
    QMenu *myMenu;
    QAction *mExitAction;
    QAction *about;
    QAction *no_notice;
    QSystemTrayIcon *myTrayIcon;
    QStringListModel *model;
    Ui::Widget *ui;
};
#endif // WIDGET_H
