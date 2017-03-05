#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<Qstring>
#include<QFileDialog>
#include<QMessageBox>
#include<string>
#include<stdio.h>
#include<ctime>
#include"lzw_util.h"

//窗口类
//构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //绑定ui
    ui->setupUi(this);
    //设置进度条不可见
    ui->bar->setVisible(false);
}
//析构函数
MainWindow::~MainWindow()
{
    //注销界面
    delete ui;
}

//压缩按钮
void MainWindow::on_btn_compress_clicked()
{
     //显示进度条
    ui->bar->setVisible(false);
     //待压缩的bmp文件名
    QString filename;
    //打开一个qt自带的文件选择Dialog，限制可选文件类型为bmp
    filename=QFileDialog::getOpenFileName(this,
                                            tr("选择图像"),
                                            "",
                                            tr("Images (*.bmp )"));
      if(filename.isEmpty())//判断文件是否为空
      {
           return;
      }
      else
      {

          QImage* img=new QImage;//声明一个Image对象

          if(! ( img->load(filename) ) ) //加载图像
          {
              //图像打开失败则弹出
              QMessageBox::information(this,
                                       tr("打开图像失败"),
                                       tr("打开图像失败!"));
              delete img;//清楚img对象
              return;
          }
          event=1;//若图像打开成功则设置事情判断为压缩（见头文件）
          file=filename;//将此文件名赋给头文件
          ui->label_image->setPixmap(QPixmap::fromImage(*img));//在主界面上显示图片
      }


    }

//解压按钮
void MainWindow::on_btn_decompress_clicked()
{
    //设置进度条不可见
    ui->bar->setVisible(false);

    //寻找zjz文件（我名字缩写，同样是打开一个Dialog
    QString filename;
      filename=QFileDialog::getOpenFileName(this,
                                            tr("选择lzw文件"),
                                            "",
                                            tr("Images (*.zjz )"));
      //判读文件是否为空
      if(filename.isEmpty())
      {
           return;
      }
      else
      {
          event=2;//设置事情为解压
          file=filename;//将文件名传给赋值给头文件
          ui->label_image->setText(filename);//在主窗口显示文件路径及名字
      }


}

//确定按钮
void MainWindow::on_btn_confirm_clicked()
{
    //根据头文件的event来判断状态
    switch (event) {
    case 1://如果为压缩
   click_compresss(file.toStdString(),ui->bar);
        break;
    case 2://如果为解压
        click_decompress(file.toStdString(),ui->bar);
        break;
    default:
        break;
    }
}
