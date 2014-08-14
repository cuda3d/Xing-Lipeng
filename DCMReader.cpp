#include "DCMReader.h"

DCMReader::DCMReader(string fileName )//��ʼ����װ���ļ���ȡ��Ϣ
{
	bHasVR = true;			//Ĭ����VR
	DataStart = NULL;	
	DataLenth = 0;
	pMemFile = NULL;
	filename=fileName ;
	success = false;

	if ( GetFileLenth( ) == 0 ) //��ȡ�ļ������Թ������ڴ�װ���ļ�
	{
		if(LoadFile( )==0 )
		{
			if( ReadHeadInfo() ==0 )//��ȡ���õ���Ϣ
			{
				success=true;
			}
		}
	}			
}

DCMReader::~DCMReader()
{
	SaveAs(); 
	delete []pMemFile;		//�����ڴ�
}

//�����ļ��ĳ��ȣ��Ա������ڴ潫�ļ�װ��
//�˴�װ���ļ���Ϊ�˴������
//��������-1��ʾ���ļ�����,����0��ʾ��ȡ���ȳɹ�
//�˰汾�ļ�����ӦС��4G
int DCMReader::GetFileLenth( )	//����һ���ļ��ĳ���
{
	ifstream infile(filename.c_str(),ios::binary);
	if( !infile )
	{
		return -1;
	}
	infile.seekg(0,ios::end);
	len = (int)infile.tellg();
	infile.close();
	return 0;
}

//���ļ�װ���ڴ� ָ��Ϊ pMemFile
//����С��0 ��ʾ�ļ�װ�ش��󣬷���0��ʾװ��ɹ�
int DCMReader::LoadFile( )//���ļ�װ���ڴ�
{
	ifstream infile(filename.c_str(),ios::binary);
	if( !infile )
	{
		return -1;
	}
	pMemFile = new BYTE [len];
	if( !pMemFile )
	{
		infile.close();
		return -2;
	}
	
	infile.read((char *)pMemFile,len);
	infile.close();
	return 0;
}

int DCMReader::ReadHeadInfo( )
{
	BYTE *ptFile=pMemFile;	//����ָ��
	WORD group;		//���
	WORD element;	//Ԫ�غ�
	DWORD dataLenth;//���ݳ���
	bool findEnd=false;	
	string  VRTag("VR");
	
	ptFile+=128;//������ʼ��128�ֽڵ���
	
	//����ļ���־
	if(  !( *ptFile == 'D'&& *(ptFile+1) == 'I'&& *(ptFile+2) == 'C'&& *(ptFile+3) == 'M' ))
	{
		return -1;
	}
	ptFile +=4;
	

	//ѭ�������ļ���������Ϣ
	while ( ptFile + 6 < pMemFile+len )
	{
		//�����
		group=*(WORD *)ptFile;
		ptFile+=2;
		cout<<"Group:"<<group<<endl;
		//��Ԫ�غ�
		element =*(WORD *)ptFile;
		ptFile+=2;
		cout<<"Element:"<<element <<endl;

		//��������VR���в�ͬ������ȡ���ݳ��ȣ�
		//��δ����VR��ʽ����ʽ����
		if( group == 0x0002 )
		{
			VRTag[0]=*ptFile ;
			VRTag[1]=*(ptFile +1);
			cout<<VRTag <<endl ;
			ptFile+=2;

			if( VRTag == "OB" || VRTag == "OW" ||VRTag == "OF" ||
				VRTag == "SQ" ||VRTag == "UT" ||VRTag == "UN"  )
			{
				ptFile+=2;
				dataLenth = *(DWORD *)ptFile;
				ptFile+=4;
			}
			else
			{
				dataLenth = *(WORD *)ptFile;
				ptFile+=2; 
			}
		}else if ( (group == 0xfffe) && 
			(element==0xe000  || element==0xe00d || element==0xe0dd) )//�ļ��б�ǩ
		{
			VRTag="**";
			dataLenth= *(DWORD *)ptFile;
			ptFile+=4;
		}
		else if (bHasVR)
		{
			VRTag[0]=*ptFile ;
			VRTag[1]=*(ptFile +1);
			cout<<VRTag <<endl ;
			ptFile+=2;

			if( VRTag == "OB" || VRTag == "OW" ||VRTag == "OF" ||
				VRTag == "SQ" ||VRTag == "UT" ||VRTag == "UN"  )
			{
				ptFile+=2;
				dataLenth = *(DWORD *)ptFile;
				ptFile+=4;
			}
			else
			{
				dataLenth = *(WORD *)ptFile;
				ptFile+=2; 
			}
		}else
		{
			cout<<"NoVR"<<endl;
			dataLenth = *(WORD *)ptFile;
			ptFile+=2; 
			
		}
		cout<<"Lenth"<<dataLenth<<endl;

		//���������ݽ��м�飬��ȡ������Ϣ
		switch (group)
		{
		case 0x0028:
			switch(element)
				{
				case 0X0002:
					color=*(WORD *)ptFile;
					cout<<color<<endl;
					break;
				case 0X0010:
					row=*(WORD *)ptFile;
					cout<<row<<endl;
					break;
				case 0X0011:
					col=*(WORD *)ptFile;cout<<col<<endl;
					break;
				case 0X0100:
					bitAlloc =*(WORD *)ptFile;cout<<bitAlloc<<endl;
					break;
				case 0X0101:
					bitInfect =*(WORD *)ptFile;cout<<bitInfect<<endl;
					break;
				case 0X0102:
					bitHigh =*(WORD *)ptFile;cout<<bitHigh<<endl;
					break;
				case 0X1050:
					wndWidth =*(WORD *)ptFile;cout<<wndWidth<<endl;
					break;
				case 0X1051:
					wndPos =*(WORD *)ptFile;cout<<wndPos<<endl;
					break;
				}
				break;
		case 0x7fe0:
			if(element == 0x0010)
			{
				DataStart = ptFile ;
				DataLenth = dataLenth ;
				findEnd=true;
			}
				
		}	
		ptFile+=dataLenth ;
		
	}
	return 0;
}

DWORD DCMReader::GetDataLenth(void)
{
	return DataLenth;
}

BYTE* DCMReader::GetDataStart(void)
{
	return DataStart;
}

WORD DCMReader::GetBitAlloc(void)
{
	return bitAlloc;
}

WORD DCMReader::GetBitHigh(void)
{
	return bitHigh;
}

WORD DCMReader::GetBitInfect(void)
{
	return bitInfect;
}

WORD DCMReader::GetCol(void)
{
	return col;
}

WORD DCMReader::GetRow(void)
{
	return row;
}

WORD DCMReader::GetWndPos(void)
{
	return wndPos;
}

WORD DCMReader::GetWndWidth(void)
{
	return wndWidth;
}

int	 DCMReader::GetColor()
{
	return color;
}

int DCMReader::SaveAs()
{
	string savName=filename + "_bac.dcm";
	ofstream outfile(savName.c_str(),ios::binary  );
	outfile.write((char *)pMemFile ,len);

	return 0;
}



































#if 0
int GetVR(WORD group,WORD element,string &vr)
{
	 switch (group)
	{
	 case 0x0002:
		 if ( element == 0x0000 )
		 {
			 vr="UL";
		 }
		 else if (element == 0x0010)		 
		 {
			 vr="UI";
		 }
		 else if (element == 0x0013)
		 {
			 vr="SH";
		 }
		 break;
	 case 0x0008:
		  if ( element == 0x0005 )
		 {
			 vr="CS";
		 }
		 else if (element == 0x0008)		 
		 {
			 vr="CS";
		 }
		 else if (element == 0x1032)
		 {
			 vr="SQ";
		 }
		 else if (element == 0x1111)
		 {
			 vr="SQ";
		 }
		 else if (element == 0x0020)
		 {
			 vr="DA";
		 }
		 else if (element == 0x0060)
		 {
			 vr="CS";
		 }
		 else if (element == 0x0070)
		 {
			 vr="LO";
		 }else if (element == 0x0080)
		 {
			 vr="LO";
		 }
		 break;
	 case 0x0010:
		 if ( element == 0x0010 )
		 {
			 vr="PN";
		 }
		 else if (element == 0x0020)		 
		 {
			 vr="LO";
		 }
		 else if (element == 0x0030)
		 {
			 vr="DA";
		 }
		 break;

	 case 0x0018:
		 if ( element == 0x0060 )
		 {
			 vr="DS";
		 }
		 else if (element == 0x1030)		 
		 {
			 vr="LO";
		 }
		 else if (element == 0x1151)
		 {
			 vr="IS";
		 }
		 break;

	 case 0x0020:
		 if ( element == 0x0010 )
		 {
			 vr="SH";
		 }
		 else if (element == 0x0011)		 
		 {
			 vr="IS";
		 }
		 else if (element == 0x0012)
		 {
			 vr="IS";
		 }
		 else if (element == 0x0013)
		 {
			 vr="IS";
		 }
		 break;
	 case 0x0028:
		 if ( element == 0x0002 )
		 {
			 vr="US";
		 }
		 else if (element == 0x0004)		 
		 {
			 vr="CS";
		 }
		 else if (element == 0x0010)
		 {
			 vr="US";
		 }
		 else if (element == 0x0011)
		 {
			 vr="US";
		 }
		  else if (element == 0x0100)		 
		 {
			 vr="US";
		 }
		 else if (element == 0x0101)
		 {
			 vr="US";
		 }
		 else if (element == 0x0102)
		 {
			 vr="US";
		 } 
		 else if (element == 0x1050)		 
		 {
			 vr="DS";
		 }
		 else if (element == 0x1051)
		 {
			 vr="DS";
		 }
		 else if (element == 0x1052)
		 {
			 vr="DS";
		 }
		 else if (element == 0x1053)
		 {
			 vr="DS";
		 }
		 break;
	 case 0x0040:
		 if ( element == 0x0008 )
		 {
			 vr="SQ";
		 }
		 else if (element == 0x0260)		 
		 {
			 vr="SQ";
		 }
		 else if (element == 0x0275)
		 {
			 vr="SQ";
		 }
		 break;
	 case 0x7fe0:
		 if (element == 0x0010)
		 {
			 vr="OW";
		 }
	 default:
		 vr="UN";
    }
}
#endif