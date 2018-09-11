#pragma once

//ͼ�������ļ�
class CFrameInfo
{
public:
	CFrameInfo(void);
	~CFrameInfo(void);
	//��ȡ����
	double getTableLength() const;
	double getTableHeight() const;
	double getLineLength() const;
	double getFristTableLength() const;
	double getSecondTableLength() const;
	double getThirdTableLength() const;
	double getRfSecondTableLength() const;
	double getRfThirdTableLength() const;
	
	double getHorSpace() const;
	double getVerSpace() const;
	double getSpaceHorGap() const;
	double getSpaceVerGap() const;
	double getGap() const;
	double getLeftVerGap() const;
	double getLeftHorGap() const;
	CString getFrameType() const;
	CString getLrmName() const;

	//��������
	void setTableLength(double dTableLength);
	void setTableHeight(double dTableHeight);
	void setLineLength(double dLineLength);
	void setFirstTableLength(double dFirstTableLength);
	void setSecondTableLength(double dSecondTableLength);
	void setThirdTableLength(double dThirdTableLength);
	void setRfSecondTableLength(double dRfSecondTableLength);
	void setRfThirdTableLength(double dRfThirdTableLength);
	void setHorSpace(double dHorSpace);
	void setVerSpace(double dVerSpace);
	void setSpaceHorGap(double dSpaceHorGap);
	void setSpaceVerGap(double dSpaceVerGap);
	void setGap(double dGap);
	void setLeftVerGap(double dLeftVerGap);
	void setLeftHorGap(double dLeftHorGap);
	void setFrameType(CString strFrameType);
	void setLrmName(CString strLrmName);

	CFrameInfo& operator=(const CFrameInfo& node);

private:
	//���޸�	
	CString m_strFrameType;//ͼ������
	CString m_strLrmName;
	double m_dHorSpace;//��������
	double m_dVerSapce;//���������
	double m_dSpareHorGap;//���ú���
	double m_dSpareVerGap;//��������
	double m_dLeftVerGap;//��������
	double m_dLeftHorGap;//���Ϻ���
	//��ʱ�̶����ݣ��������޸�
	double m_dGap;//��ͼ������ߵļ�϶
	double m_dTableLength;//��񳤶�
	double m_dTableHeight;//���߶�
	double m_dLineLength;//�����߳�
	double m_dFirstTableLength;//��һ������
	double m_dSecondTableLength;//�ڶ�������
	double m_dThirdTableLength;//����������
	double m_dRfSecondTableLength;//RF�ڶ��ڳ���
	double m_dRfThirdTableLength;//RF�����ڳ���
	int m_nCount;//����
};


//ͼ��������
class CFrameTitle
{
public:
	CFrameTitle(void);
	~CFrameTitle(void);
	//��ȡ����
	CString getBmText() const;
	CString getDesigner() const;
	CString getProName() const;
	CString getProGrade() const;
	CString getProStage() const;
	CString getGZJ() const;
	CString getVerifier() const;
	CString getFirstEmpty() const;
	CString getStandard() const;
	CString getSencondEmpty() const;
	CString getApproval() const;

	//��������
	void setBmText(CString strBmText);
	void setDesigner(CString strDesigner);
	void setProName(CString strProName);
	void setProGrade(CString strGrade);
	void setProStage(CString strProStage);
	void setGZJ(CString strGZJ);
	void setVerifier(CString strVerifier);
	void setFirstEmpty(CString strFirstEmpty);
	void setStandard(CString strStandard);
	void setSencondEmpty(CString strEmpty);
	void setApproval(CString strApproval);

	CFrameTitle& operator=(const CFrameTitle& node);

private:
	//�̶�����
	CString m_strBmText;//bm����
	CString m_strDesigner;//���ʦ
	CString m_strProName;//��Ʒ����
	CString m_strProGrade;//��Ŀ�ȼ�
	CString m_strProStage;//��Ŀ�׶�
	CString m_strGZJ;//���ؼ�
	CString m_strVerifier;//�����
	CString m_strFirstEmpty;//���
	CString m_strStandard;//��׼��
	CString m_strSecondEmpty;//�ڶ����հ״�
	CString m_strApproval;//��׼
};

