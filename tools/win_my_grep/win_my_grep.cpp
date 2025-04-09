#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>  // 添加头文件
using namespace std;

//melody.shop.ele.me/login
//https://melody.shop.ele.me/login


void upcase(char* str, int size, char* dststr) {
	for (int i = 0; i < size; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			dststr[i] = str[i] + 0x20;
		}
		else {
			dststr[i] = str[i];
		}
	}
}


void lowcase(char* str, int size, char* dststr) {
	for (int i = 0; i < size; i++) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			dststr[i] = str[i] - 0x20;
		}
		else {
			dststr[i] = str[i];
		}
	}
}


int searchStrInFile(const char* szFileName, const char* pDstContent, const wchar_t* wszcontent)
{
	HANDLE hFile = CreateFileA(szFileName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("%s CreateFile:%s error:%d\r\n", __FUNCTION__, szFileName, GetLastError());
		return FALSE;
	}

	int asclen = lstrlenA(pDstContent);
	int iSize = GetFileSize(hFile, 0);
	if (iSize < asclen)
	{
		CloseHandle(hFile);
		return FALSE;
	}

	char* pFileBuf = new char[iSize + 16];
	DWORD dwCnt = 0;
	int iRes = ReadFile(hFile, pFileBuf, iSize, &dwCnt, 0);
	*(pFileBuf + iSize) = 0;
	CloseHandle(hFile);

	if (iRes == 0)
	{
		delete[] pFileBuf;
		return FALSE;
	}

	// === 构建每一行的起始位置 ===
	vector<int> lineOffsets;
	lineOffsets.push_back(0); // 第一行从 0 开始
	for (int i = 0; i < iSize; i++)
	{
		if (pFileBuf[i] == '\n')
		{
			lineOffsets.push_back(i + 1); // 新一行从 i+1 开始
		}
	}

	bool found = false;

	// === ASCII 匹配 ===
	for (int i = 0; i <= iSize - asclen; i++)
	{
		upcase(pFileBuf + i, asclen, pFileBuf + i);  // 小写化
		if (memcmp(pDstContent, pFileBuf + i, asclen) == 0)
		{
			// 找到对应行号
			int lineNumber = 1;
			for (int k = 0; k < lineOffsets.size(); ++k)
			{
				if (lineOffsets[k] > i) break;
				lineNumber = k + 1;
			}

			printf("found string:%s in %s, line:%d\r\n", pDstContent, szFileName, lineNumber);
			found = true;
		}
	}

	// === 宽字符匹配（wchar_t） ===
	int unilen = wcslen(wszcontent) * sizeof(wchar_t);
	for (int i = 0; i <= iSize - unilen; i++)
	{
		upcase(pFileBuf + i, unilen, pFileBuf + i);  // 小写化
		if (memcmp((char*)wszcontent, pFileBuf + i, unilen) == 0)
		{
			int lineNumber = 1;
			for (int k = 0; k < lineOffsets.size(); ++k)
			{
				if (lineOffsets[k] > i) break;
				lineNumber = k + 1;
			}

			printf("found string:%s in file:%s, line:%d\r\n", pDstContent, szFileName, lineNumber);
			found = true;
		}
	}

	delete[] pFileBuf;
	return found ? TRUE : FALSE;
}
int searchFile(const char* PreStrPath, int iLayer, const char* pDstContent, const wchar_t* wszcontent)
{
	int ret = 0;

	static int result = 0;

	int prepathlen = lstrlenA(PreStrPath);

	char strPath[0x1000] = { 0 };
	memcpy(strPath, PreStrPath, prepathlen);
	memcpy(strPath + prepathlen, "\\*.*", lstrlenA("\\*.*"));


	WIN32_FIND_DATAA stWfd = { 0 };
	HANDLE hFind = FindFirstFileA(strPath, &stWfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFileA:%s error:%d\r\n", strPath, GetLastError());
		return 0;
	}

	char szLastDir[] = { '.','.',0 };
	char szCurDir[] = { '.',0 };
	do
	{
		if (stWfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmpiA(stWfd.cFileName, szLastDir) == 0 || lstrcmpiA(stWfd.cFileName, szCurDir) == 0)
			{
				continue;
			}

			char strNextPath[0x1000] = { 0 };
			memcpy(strNextPath, PreStrPath, prepathlen);
			*(strNextPath + prepathlen) = 0x5c;
			memcpy(strNextPath + prepathlen + 1, stWfd.cFileName, lstrlenA(stWfd.cFileName));
			ret = searchFile(strNextPath, iLayer + 1, pDstContent, wszcontent);
			if (ret)
			{

			}
			else {

			}
		}
		else
		{
			char szFileName[0x1000] = { 0 };
			memcpy(szFileName, PreStrPath, prepathlen);
			*(szFileName + prepathlen) = 0x5c;
			memcpy(szFileName + prepathlen + 1, stWfd.cFileName, lstrlenA(stWfd.cFileName));
			int iFilePos = searchStrInFile(szFileName, pDstContent, wszcontent);
			if (iFilePos)
			{

				result++;
			}
		}
	} while (FindNextFileA(hFind, &stWfd));

	FindClose(hFind);

	return result;
}



int main(int argc, char** argv)
{
	int ret = 0;

	wchar_t wszcontent[1024] = { 0 };

	char* searchpath = 0;
	char* searchstr = 0;
	if (argc < 2)
	{
		//printf("parameter wrong!\r\n");
		//printf("usage 1 (search in defined directory):c:\\test\\ stringvalue\r\n");
		//printf("usage 2 (search in current directory):stringvalue\r\n");
		//getchar();
		//return FALSE;

		printf("input the path the content in which you want to search:");
		char inputpath[1024];
		char inputstr[1024];
		scanf("%s", inputpath);

		printf("input the content you want to search:");
		scanf("%s", inputstr);


		MultiByteToWideChar(CP_ACP, 0, inputstr, -1, wszcontent, sizeof(wszcontent));

		ret = searchFile(inputpath, 1, inputstr, wszcontent);
	}
	else {
		if (argc == 2)
		{
			MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wszcontent, sizeof(wszcontent));

			char szcurdir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szcurdir);

			printf("start searching string:\"%s\" in path:\"%s\",please to wait patiently...\r\n\r\n", argv[1], szcurdir);

			char szcontent[1024];
			upcase(argv[1], lstrlenA(argv[1]), szcontent);
			ret = searchFile(szcurdir, 1, szcontent, wszcontent);

		}
		else if (argc >= 3)
		{
			string path = argv[1];
			if (path.back() == '/' || path.back() == '\\')
			{
				path = path.substr(0, path.length() - 1);
			}
			MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wszcontent, sizeof(wszcontent));

			printf("start searching string:\"%s\" in path:\"%s\",please wait patiently...\r\n\r\n", argv[2], path.c_str());

			ret = GetFileAttributesA(path.c_str());

			char szcontent[1024];
			upcase(argv[2], lstrlenA(argv[1]), szcontent);

			if (ret & FILE_ATTRIBUTE_DIRECTORY)
			{
				ret = searchFile(path.c_str(), 1, szcontent, wszcontent);
			}
			else {
				ret = searchStrInFile(path.c_str(), szcontent, wszcontent);
			}
		}
	}

	if (ret == 0)
	{
		printf("Search string in path failed!\r\n");
		getchar();
		return FALSE;
	}
	else
	{
		printf("Search string successfully, found string position(offset from beginning):%u\r\n", ret);
		getchar();
		return TRUE;
	}
	return TRUE;
}