#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int 
class DCMReader
{
private:
	string	filename; //文件名
	BYTE	*pMemFile;//文件装入的指针
	BYTE	*DataStart;//图像数据开始的指针

	
	bool	isLittleEndian;//放弃对数据大小端的处理，留作扩展

	bool	bHasVR;	//默认为显式 是否为VR标签。
	int		len;	//文件长度
	int		color;	//灰度or彩色

	WORD	row;	//图像行数
	WORD	col;	//图像列数

	WORD	bitAlloc; //图像单元位数
	WORD	bitInfect;//图像实际用的位数
	WORD	bitHigh;  //图像最高位

	WORD	wndWidth; //窗宽
	WORD	wndPos;   //窗位
	DWORD   DataLenth;//图像数据长度
	
	int LoadFile( ); //将文件装入内存
	int GetFileLenth( );//获取文件长度
	int ReadHeadInfo( );//读取文件信息
	
public:	
	DCMReader(string fileName );
	~DCMReader();

	bool  success; //注释见下面的模块
#if 0
	请按如下形式处理该变量：
	DCMReader test("filename");
	if(!test.success)
		return -1;
#endif

	BYTE* GetDataStart();  //获取像素数据开头指针
	DWORD GetDataLenth();//获取像素数据长度（字节）
	WORD  GetBitAlloc();//获取像素数据位数
	WORD  GetBitHigh();//获取像素数据最高有效位
	WORD  GetBitInfect();//获取实际位数
	WORD  GetCol();//获取图像列数
	WORD  GetRow();//获取图像行数
	WORD  GetWndPos();//获取窗位
	WORD  GetWndWidth();//获取窗宽
	int	  GetColor();//获取色彩数 若1则灰度图 若3则彩色
	
	int SaveAs();//另存为

};
