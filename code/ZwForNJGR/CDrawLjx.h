#pragma once

//��������������
//Date:2018��7��25��11:56:38

class CDrawLjx
{
public:
	CDrawLjx(void);
	~CDrawLjx(void);
	//����
	bool doIt();
	//ѡ����ʵ��
	bool selEnt();
	
	//�������ߺͺ���
	bool drawSpecialLine();
private:
	map<double, AcGePoint3d> m_pointMap;
	AcDbObjectId m_LayerId;
};
