#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <map>
#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include<Qstring>
#include <opencv2/opencv.hpp>
#include<QFileDialog>


 //主方法
 //加载ui类，打开界面


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  MainWindow win;//声明主窗口
   win.show();//打开主窗口

    return a.exec();
}
