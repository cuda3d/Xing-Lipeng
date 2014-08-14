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
	string	filename; //�ļ���
	BYTE	*pMemFile;//�ļ�װ���ָ��
	BYTE	*DataStart;//ͼ�����ݿ�ʼ��ָ��

	
	bool	isLittleEndian;//���������ݴ�С�˵Ĵ���������չ

	bool	bHasVR;	//Ĭ��Ϊ��ʽ �Ƿ�ΪVR��ǩ��
	int		len;	//�ļ�����
	int		color;	//�Ҷ�or��ɫ

	WORD	row;	//ͼ������
	WORD	col;	//ͼ������

	WORD	bitAlloc; //ͼ��Ԫλ��
	WORD	bitInfect;//ͼ��ʵ���õ�λ��
	WORD	bitHigh;  //ͼ�����λ

	WORD	wndWidth; //����
	WORD	wndPos;   //��λ
	DWORD   DataLenth;//ͼ�����ݳ���
	
	int LoadFile( ); //���ļ�װ���ڴ�
	int GetFileLenth( );//��ȡ�ļ�����
	int ReadHeadInfo( );//��ȡ�ļ���Ϣ
	
public:	
	DCMReader(string fileName );
	~DCMReader();

	bool  success; //ע�ͼ������ģ��
#if 0
	�밴������ʽ����ñ�����
	DCMReader test("filename");
	if(!test.success)
		return -1;
#endif

	BYTE* GetDataStart();  //��ȡ�������ݿ�ͷָ��
	DWORD GetDataLenth();//��ȡ�������ݳ��ȣ��ֽڣ�
	WORD  GetBitAlloc();//��ȡ��������λ��
	WORD  GetBitHigh();//��ȡ�������������Чλ
	WORD  GetBitInfect();//��ȡʵ��λ��
	WORD  GetCol();//��ȡͼ������
	WORD  GetRow();//��ȡͼ������
	WORD  GetWndPos();//��ȡ��λ
	WORD  GetWndWidth();//��ȡ����
	int	  GetColor();//��ȡɫ���� ��1��Ҷ�ͼ ��3���ɫ
	
	int SaveAs();//���Ϊ

};
