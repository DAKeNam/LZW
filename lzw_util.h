#ifndef LZW_UTIL_H
#define LZW_UTIL_H
#include <string>
#include <map>
#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include<QProgressBar>
class lzw_util
{

public:

    void click_compresss(std::string str,QProgressBar* bar);//点击压缩按钮后调用的函数，str为文件名，bar为进度条
    void click_decompress(std::string str,QProgressBar* bar);//点击解压按钮后调用的函数
    lzw_util();//构造函数
private:

    void compress(uchar line_mat[], std::vector<ushort>& vec,int len);//压缩函数
    std::vector<uchar> decompress(std::vector<int>& vec,int len) ;//解压函数
};
//析构函数
lzw_util::lzw_util()
{

}
//压缩函数
//uchar数组用于存放待压缩的数据
//vector用于存放压缩后的数据
//len为最大允许长度
void compress(uchar line_mat[], std::vector<ushort>& vec,int len) {
    int dictSize = 256;//声明字典大小
    std::map<std::string, int> dictionary;//定义字典
    for (int i = 0; i < 256; i++)
    {
        dictionary[std::string(1, i)] = i;//初始化字典
    }

    std::string w;//一个词条
    for (int i=0;i<len;i+=3){//循环每一个元素点（灰度bmp图像的r，g，b相等）
        char c = line_mat[i];//获取像素点的值
        std::string wc = w + c;//添加这个值构成字符串
        if (dictionary.count(wc))//判断这个字符串是否已经存在
            w = wc;//存在则更新w
        else {
            vec.push_back(dictionary[w]);//不存在则将新的字符串添加进字典
            dictionary[wc] = dictSize++;//绑定map
            w = std::string(1, c);//更新w
        }
    }

    // Output the code for w.
    if (!w.empty())
        vec.push_back(dictionary[w]);//将结果添加进vec
}

//解压函数
//vec为结果，len为待解压
//返回的vector为结果
std::vector<uchar> decompress(std::vector<int>& vec,int len) {

    int dictSize = 256;//申明字典大小
    std::map<int, std::string> dictionary;//定义map
    for (int i = 0; i < 256; i++)
        dictionary[i] = std::string(1, i);//初始化

    std::vector<int>::iterator it = vec.begin();//声明遍历器
    std::string w(1, *it);//获得待压缩的第一个字符串
    std::string result = w;//第一个字符串一定是一个字符，并且一定是第一个数
    std::string entry;//暂时存贮解出的字符串
    for (it++; it != vec.end(); it++) {
        int k = *it;
        if (dictionary.count(k)){//判断k对应的字符是否已经在字典中
            entry = dictionary[k];//直接赋值
            }
        else if (k == dictSize){//不在字典并且满足k刚好满足边界（说明k表示的是下一个不同字符串对应的值）
            entry = w + w[0];//将字符串的第一个字符提取出来作为新的不同的字符串的后缀
            }
        else{//k不合法
           //可以添加一个报错的界面
         }
        result += entry;//将新的解压出的字符串加入结果

        dictionary[dictSize++] = w + entry[0];//将新的提取的字符串加入字典

        w = entry;//作为下一个前缀
    }

    std::vector<uchar> temp_vec;

    for (int i = 0; i < len; i++) {
        temp_vec.push_back((uchar)result[i]);//将结果的数据挨个提取出
    }

    return temp_vec;
}
//点击压缩函数
void click_compresss(std::string str,QProgressBar* bar){

    cv::Mat src_mat = cv::imread(str);//读取文件，写入一个Mat中

    ushort nr = src_mat.rows; // 获取行数
    ushort nc = src_mat.cols * src_mat.channels();//获取列数，bmp图像有r，g，b三个值所以要乘上频道数

    std::ofstream fout("flower.zjz", std::ios::binary);//打开二进制文件
    fout.write((char*)&nr,2);//写入列数
    fout.write((char*)&nc,2);//写入行数
    ushort temp;
    for (float i = 0; i < nr; i++) {//每一行压缩一次
        std::vector<ushort> mat_vec; //定义一个存放压缩结果的容器
        uchar *array = new uchar[nc];//带压缩数据放进一个数组中
        array = (src_mat.row(i)).data;//将mat的每一行指针赋给数组
        compress(array, mat_vec, nc);//调用压缩函数

        //文件写入
        for (std::vector<ushort>::iterator it = mat_vec.begin(); it != mat_vec.end();) {
            temp = *it;
            fout.write((char*)&(temp), 2);//写入数据，每个数据2个字节
            it = mat_vec.erase(it);
        }
        //进度条
        (*bar).setVisible(true);
        (*bar).setValue(((i+1)/nr)*100);//设置进度条进度
         temp = 65535;//换行标记
        fout.write((char*)&(temp), 2);

    }
     temp = 65534;//结束标记
    fout.write((char*)&(temp), 2);
    fout.close();

}

//点击解压函数
void click_decompress(std::string str,QProgressBar* bar){
    std::ifstream fin(str, std::ios::binary);//打开文件写入
    ushort temp;
    ushort nr;
    fin.read((char*)&nr,2);//读取行数
    ushort nc;
    fin.read((char*)&nc,2);//读取列数
    fin.read((char*)&temp, 2);//读取第一个字符

    cv::Mat result_mat(nr, nc/3, CV_8UC(1), cv::Scalar::all(0));//创建一个Mat，存放像素点
    float i = 0;
    while (temp != 65534) {//如果不是结束标记
        std::vector<int> vec_in;//存放结果
        while (temp != 65535) {//如果不是换行标记
            vec_in.push_back(temp);
            fin.read((char*)&temp, 2);//每两个字节读取
        }
        fin.read((char*)&temp, 2);
        std::vector<uchar> vec_temp=decompress(vec_in, nc / 3);//调用解压函数
        //遍历结果，将像素值赋还进Mat中
        int j = 0;
        for (std::vector<uchar>::iterator it = vec_temp.begin(); it != vec_temp.end();j++) {

            result_mat.at<uchar>(i, j) = *(it);
            it = vec_temp.erase(it);

         }
        i++;
        //进度条显示
        (*bar).setVisible(true);
         (*bar).setValue((i/nr)*100);
    }
    fin.close();
   //读取结束
        cv::imshow("result",result_mat);
}

#endif // LZW_UTIL_H
