// test11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <time.h>
#include <iostream>
#include <io.h>
using namespace std;

void exec(char* proPath,char *para)
{
	SHELLEXECUTEINFO ShExecInfo = {0}; 
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
	ShExecInfo.hwnd = NULL; 
	ShExecInfo.lpVerb = NULL; 
	ShExecInfo.lpFile = proPath; //别的程序的exe文件 
	ShExecInfo.lpParameters = para; 
	ShExecInfo.lpDirectory = NULL; 
	ShExecInfo.nShow = SW_HIDE; 
	ShExecInfo.hInstApp = NULL; 
	if (_access(ShExecInfo.lpFile, 0) == -1)
		exit(1);
	ShellExecuteEx(&ShExecInfo); 
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0;
	char tmp[64];
	char numtostr[64];
	char *capProPath = "C:\\captureImageExe.exe";
	char *sendProPath = "C:\\sm.exe";
	char *zipPath = "C:\\Users\\canzha\\Desktop\\big\\";
	strcpy(tmp, "C:\\Users\\canzha\\Desktop\\big\\");
	for(;;)
	{
		itoa(i, numtostr, 10);
		strcat(tmp, numtostr);
		strcat(tmp, ".bmp");
		try
		{
			exec(capProPath, tmp);
		}
		catch(double)
		{ 
			exit(1);
		}
		strcpy(tmp, "C:\\Users\\canzha\\Desktop\\big\\");
		i++;
		if(i % 15 == 0)
		{
			exec(sendProPath, zipPath);
		}
		Sleep(120000); 
	}    
	return 0;
}

