// makeProtocolFile.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "InfoModel.h"


int _tmain(int argc, _TCHAR* argv[])
{
	
	InfoModel info;
	info.ReadFile("test.txt");
	return 0;
}

