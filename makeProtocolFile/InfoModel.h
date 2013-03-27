#include "stdafx.h"

struct verType
{
	int version;
	int dataCount;
	int line;
	string verNode;
	string subDataName;
};

struct SubData
{
	string className;
	string classNode;
	list<verType> verInfo;
	list<string*> dataInfo;
};

class InfoModel
{
public:
	InfoModel();
	~InfoModel();
	
	bool ReadFile(const char* fileName);

private:
	int ReadLine(ifstream& fin, int subDataCount, list<SubData>::iterator iter);
	bool ReadVersion(ifstream& fin);


	bool WriteFile(const char* outputFileName);
	bool WritePockToByteParameter(ofstream& fout, string* str);
	bool WriteSubData(ofstream& fout, SubData& subData);

	string GetParameter(string* str, int idx);
	string GetRealType(string& str);

	string className;
	string classNode;

	list<verType> verInfo;
	list<string*> dataInfo;
	list<SubData> subDataList;

	int m_yuNameIdx;
	int m_nameIdx;
	int m_typeIdx;
	int m_isNecessaryIdx;
	int m_nodeIdx;
	int m_realType;
	int m_maxNum;
};

class tmpclass
{
	void ReadLine(ifstream& fin);

};