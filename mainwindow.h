#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qstring>
#include<string>
#include<iostream>
//继承qt界面的ui类
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);//构造函数
    ~MainWindow();


private slots:
    void on_btn_compress_clicked();//压缩按钮绑定的信号

    void on_btn_decompress_clicked();//解压按钮绑定的信号

    void on_btn_confirm_clicked();//确认按钮绑定的信号

private:
    Ui::MainWindow *ui;//界面类
    int event=0;//事件 1为压缩，2为解压
    QString file;//存放需要操作的文件名
};

#endif // MAINWINDOW_H
