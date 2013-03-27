#include "stdafx.h"
#include "InfoModel.h"


InfoModel::InfoModel()
{
	m_yuNameIdx = 9;
	m_nameIdx = 0;
	m_typeIdx = 1;
	m_isNecessaryIdx = 9;
	m_nodeIdx = 2;
	m_maxNum = 3;
	m_realType = 3;
}

InfoModel::~InfoModel()
{
}


bool InfoModel::ReadFile(const char* fileName)
{
	ifstream fin;

	char tmp[1000];
	int lastSubDataCount = 0;
	int subDataCount = 0;
	SubData subDataTmp;
	subDataList.push_back(subDataTmp);
	list<SubData>::iterator iterSub = subDataList.begin();

	fin.open(fileName, ios_base::out);

	if (fin.fail())
	{
		return false;
	}
	
	fin >> className;
	fin.getline(tmp, 1000, '\n');
	classNode = tmp;

	while (!fin.eof())
	{
		lastSubDataCount = ReadLine(fin, subDataCount, iterSub);

		if (lastSubDataCount == 0 && subDataCount == 1)
		{
			SubData subTmp;
			subDataList.push_back(subTmp);
			++iterSub;
		}

		subDataCount = lastSubDataCount;
	}

	fin.close();

	WriteFile("test.cpp");

	return true;
}

int InfoModel::ReadLine(ifstream& fin, int subDataCount, list<SubData>::iterator iterSub)
{
	string firstStr;
	char tmp[1000];
	char x;
	fin >> firstStr;

	//读取协议版本号，个数,设置所在行数
	//int idx = firstStr.find("ersion");
	
	
	if (firstStr.find("ersion") != -1)
	{
		verType verTmp;
		fin >> verTmp.version >> verTmp.dataCount >> verTmp.verNode;
		verTmp.line = (int)dataInfo.size();
		verInfo.push_back(verTmp);
	}
	else if (firstStr.find("subdata") != -1)
	{
		verType subDataTmp;
		fin >> subDataTmp.subDataName >> subDataTmp.dataCount >> subDataTmp.verNode;
		subDataTmp.version = -1;
		subDataTmp.line = (int)dataInfo.size();
		subDataCount = subDataTmp.dataCount;

		verInfo.push_back(subDataTmp);
	}
	else 
	{
		string* dataTmp = new string[10];
		dataTmp[0] = firstStr;

		for (int i = 1; i < m_maxNum; ++i)
		{
			if (/*i == m_yuNameIdx - 1 || */i == m_nodeIdx)
			{
				fin.get();
				fin.getline(tmp, 1000, '\n');
				string stringBuf(tmp);
				dataTmp[i] = stringBuf;
			}
			else
			{
				fin >> dataTmp[i];
			}
		}
		dataTmp[m_realType] = GetRealType(dataTmp[m_typeIdx]);

		if (subDataCount > 0)
		{
			(*iterSub).dataInfo.push_back(dataTmp);
			//subData.push_back(dataTmp);
			--subDataCount;
		}
		else
		{
			dataInfo.push_back(dataTmp);
		}
	}

	return subDataCount;
}

bool InfoModel::WriteFile(const char* outputFileName)
{
	ofstream fout;
	list<string*>::iterator iterData = dataInfo.begin();
	list<verType>::iterator iterVer = verInfo.begin();
	list<SubData>::iterator iterSub = subDataList.begin();
	int verCount = 0;
	int len = dataInfo.size();
	int verLine = verInfo.front().line;

	fout.open(outputFileName, ios_base::in | ios_base::trunc);

	if (fout.fail())
	{
		return false;
	}

#pragma region 类名加前花括号
	fout << "/// <summary>\n/// " << classNode << "\n/// </summary>" << endl;
	fout << "public class " << className << " : " << "BaseReq\n{" << endl;
#pragma endregion

#pragma region 参数定义
	for (int i = 0; i < len; i++, ++iterData)
	{
		//  输出变量声明 如：
		//  /// <summary>
		//  /// yuName node
		//  /// </summary>
		//  public string name;
		//
		string* lineInfo = *iterData;
		fout << "\t/// <summary>\n\t/// " << GetParameter(lineInfo, m_yuNameIdx) << " " 
			<< GetParameter(lineInfo, m_nodeIdx) << "\n\t/// </summary>" << endl;
		fout << "\tpublic " << GetParameter(lineInfo, m_realType) << " "
			<< GetParameter(lineInfo, m_nameIdx) << ";" << endl;
	}
	
	iterData = dataInfo.begin();
#pragma endregion

#pragma region PackToBytes函数
	//public override void PackToBytes(BinaryWriter writer)
	fout << "\tpublic override void PackToBytes(BinaryWriter writer)\n\t{" << endl;
	for (int i = 0; i < len; ++i, ++iterData)
	{
		if (i == verLine)
		{
			if ((*iterVer).version == -1)
			{

			}
			else
			{
				verCount = (*iterVer).dataCount;
				fout << "\n\t\t//dwVersion >= " << (*iterVer).version << "  " << (*iterVer).verNode << endl;
				fout << "\t\tif (version >= " << (*iterVer).version << ")\n\t\t{" << endl;
			}
			++iterVer;
		}

		if (verCount > 0)
		{
			fout << '\t';
		}

		fout << "\t\t";
		WritePockToByteParameter(fout, *iterData);

		if (verCount > 0)
		{
			--verCount;

			if (verCount == 0)
				fout << "\t\t}\n" << endl;
		}
	}
	fout << "\t}" << endl;

	iterVer = verInfo.begin();
	iterData = dataInfo.begin();
#pragma endregion

#pragma region 构造函数
	fout << "\n\tpublic " << className << "() \n\t{\n\t}" << endl;

#pragma endregion

#pragma region 类名后花括号
	fout << "}" << endl;
#pragma endregion

#pragma region 数据集类

	for (int i = 0; i < subDataList.size(); ++i, ++iterSub)
	{
		WriteSubData(fout, (*iterSub));
	}

#pragma endregion
	fout.close();
	return true;
}

//安全的获取对应索引出的内容，如果没有则返回空格
string InfoModel::GetParameter(string* str, int idx)
{
	string tmp = "";
	if (idx <= m_maxNum && idx >= 0)
	{
		tmp = str[idx];
	}
	return tmp;
}

//转换C#可用的类型
string InfoModel::GetRealType(string& str)
{
	string tmp = str;

	if (tmp == "word")
		tmp = "short";
	else if (tmp == "wstring")
		tmp = "string";
	else if (tmp == "ifloat")
		tmp = "TFloat";
	else if (tmp == "dword")
		tmp = "int";
	else if (tmp.find("ector") != -1)
		tmp = tmp.substr(tmp.find('<') + 1, tmp.find('>') - tmp.find('<')) + "[]";
	//else if (tmp == "")
	//	tmp = "";
	//else
	//	tmp	= "";

	return tmp;
}

//根据类型写入对应的写入函数
bool InfoModel::WritePockToByteParameter(ofstream& fout, string* str)
{
	string iType = str[m_realType];
	
	if (iType == "int" || iType == "short" || iType == "float" ||iType == "long")
		fout << "writer.write(" << GetParameter(str, m_nameIdx) << ");" << endl;
	else if (iType == "TFloat")
		fout << "writer.writeTFloat(" << GetParameter(str, m_nameIdx) << ");" << endl;
	else if (iType == "string" && str[m_typeIdx] == "string")
		fout << "writer.writeStringA(" << GetParameter(str, m_nameIdx) << ");" << endl;
	else if (iType == "string" && str[m_typeIdx] == "wstring")
		fout << "writer.writeStringU(" << GetParameter(str, m_nameIdx) << ");" << endl;
	else if (iType.find('[') != -1)
	{
		fout << ">>>>>" << GetParameter(str, m_nameIdx) << endl;
	}
	else
	{
		fout << "can't find the type of " << str[m_typeIdx] << endl; 
	}
	return true;
}

bool InfoModel::WriteSubData(ofstream& fout, SubData& subData)
{

	return true;
}

