// stdafx.cpp : source file that includes just the standard includes
//	WIN32API_7.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "PeFiles.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

//////////////////////////////////////////////////////////////////////
// 此文件提供遍历进程和模块及界面功能函数
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// _TLS_Directory

WINAPI TLSDirectory::_TLSDirectory()
{
	/*
		初始化全部成员
	*/
	
	// 全局标志
	this -> m_dwGlobalFlag = 0x00000000;

	// 文件缓冲区指针
	this -> m_fpFileBuffer = 0;

	// 用于存储自身的模块句柄 
	this -> m_hAppInstance = 0;

	// 初始化将存储的路径的空间大小
	this -> m_szFileName = (LPTSTR)malloc(sizeof(CHAR) * 0x100);
	memset(this -> m_szFileName, 0x00000000, sizeof(CHAR) * 0x100);

}

WINAPI TLSDirectory::_TLSDirectory(IN DWORD dwNumber)
{
	/*
		初始化全部成员
		允许输入存储字符串的缓冲区大小
	*/
	
	// 全局标志
	this -> m_dwGlobalFlag = 0x00000000;

	// 文件缓冲区指针
	this -> m_fpFileBuffer = 0;

	// 用于存储自身的模块句柄 
	this -> m_hAppInstance = 0;

	// 初始化将存储的路径的空间大小
	this -> m_szFileName = (LPTSTR)malloc(sizeof(CHAR) * dwNumber);
	memset(this -> m_szFileName, 0x00000000, sizeof(CHAR) * dwNumber);
}

WINAPI TLSDirectory::~_TLSDirectory()
{
	/*
		释放空间
	*/
	free(this -> m_szFileName);
	this -> m_szFileName = NULL;
	this -> m_fpFileBuffer = NULL;
	this -> m_hAppInstance = NULL;
	this -> m_dwGlobalFlag = NULL;
}

// END _TLS_Directory
//////////////////////////////////////////////////////////////////////

void WINAPIV OutputDebugStringF(const char *format, ...)  
{
	/*
		这是调试相关的函数 只在 _DEBUG 模式有作用
	*/
    va_list vlArgs;  
    char    *strBuffer = (char*)GlobalAlloc(GPTR, 4096);  

    va_start(vlArgs, format);  
    _vsnprintf(strBuffer, 4096 - 1, format, vlArgs);  
    va_end(vlArgs);  
    strcat(strBuffer, "\n");  
    OutputDebugStringA(strBuffer);  
    GlobalFree(strBuffer); 

    return;  
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI CheckIsExsist(void)
{
	/*
		此函数通过遍历进程 简单判断自身是否多开
	*/
	
	// 如果成功返回进程快照指针
	LPVOID pSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// 给 PROCESSENTRY32 的 dwSize 赋值
	PROCESSENTRY32 StructProcessEntry32 = {0};
	StructProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

	// 给 MODULEENTRY32 的 dwSize 赋值
	MODULEENTRY32 StructModuleEntry32 = {0};
	StructModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	// 获取第一个进程 放进 StructProcessEntry32.szExeFile 中
	Process32First(pSnapshot, &StructProcessEntry32);

	StructModuleEntry32.th32ProcessID = 0;

	// 如果成功返回模块快照指针
	HANDLE pModSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, StructProcessEntry32.th32ProcessID);

	// 获取第一个模块 放进 StructModuleEntry32.szExeFile 中
	Module32First(pModSnapshot, &StructModuleEntry32);

	// 得到当前进程名称
	PTCHAR szBaseName = StructModuleEntry32.szModule;

	// 计数 用于判断此程序(有此名字的exe) 是否是唯一的
	DWORD dwCount = 0;
	do
	{
		// 比较两个字符串是否相等
		DWORD dwResult = strcmp(szBaseName, StructProcessEntry32.szExeFile);
		if (!dwResult)
		{
			dwCount++;
		}
	}
	// 当没有任何进程时跳出循环
	while(Process32Next(pSnapshot, &StructProcessEntry32));

	if (dwCount == 1)
	{
		// 如果只有一个进程则继续执行
		return;
	}
	else
	{
		// 还有另一个进程则直接退出
		exit(0);
	}
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI SetListViewText(IN HWND hListProcess, 
							IN DWORD Count, 
							IN PTCHAR szNumber, 
							IN PTCHAR szString, 
							IN PTCHAR szProcessID, 
							IN PTCHAR szImageBase, 
							IN PTCHAR szSizeOfImage
							)
{
	/*
		将得到的各种数据写入到IDC_LIST_PROCESS_UPPER上
	*/

	// 用于让操作系统显示值的结构
	LV_ITEM vitem = {0};

	// 成员 pszText 有效或必须填写。
	vitem.mask = LVIF_TEXT;

	// 序号
	vitem.pszText = TEXT(szNumber);
	vitem.iItem = Count;
	vitem.iSubItem = 0;
	SendMessage(hListProcess, LVM_INSERTITEM, 0, (DWORD)&vitem);

	// 进程名称
	vitem.pszText = TEXT(szString);
	vitem.iSubItem = 1;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	// PID
	vitem.pszText = TEXT(szProcessID);	
	vitem.iSubItem = 2;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	// 镜像基址
	vitem.pszText = TEXT(szImageBase);	
	vitem.iSubItem = 3;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	// 镜像大小
	vitem.pszText = TEXT(szSizeOfImage);	
	vitem.iSubItem = 4;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	return;
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI SetListViewTextLow(IN HWND hListProcess, 
							   IN DWORD Count, 
							   IN PTCHAR szNumber, 
							   IN PTCHAR szName, 
							   IN PTCHAR szModBaseAddr, 
							   IN PTCHAR szModBaseSize, 
							   IN PTCHAR szLocal
							   )
{
	/*
		将得到的各种数据写入到IDC_LIST_PROCESS_LOWER上
	*/

	// 用于让操作系统显示值的结构
	LV_ITEM vitem = {0};

	// 成员 pszText 有效或必须填写。
	vitem.mask = LVIF_TEXT;

	// 序号
	vitem.pszText = TEXT(szNumber);
	vitem.iItem = Count;
	vitem.iSubItem = 0;
	SendMessage(hListProcess, LVM_INSERTITEM, 0, (DWORD)&vitem);

	// 模块名称
	vitem.pszText = TEXT(szName);
	vitem.iSubItem = 1;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	// 模块ImageBase
	vitem.pszText = TEXT(szModBaseAddr);
	vitem.iSubItem = 2;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	// 模块ImageBase
	vitem.pszText = TEXT(szModBaseSize);
	vitem.iSubItem = 3;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	// 模块位置
	vitem.pszText = TEXT(szLocal);	
	vitem.iSubItem = 4;
	SendMessage(hListProcess, LVM_SETITEM, 0, (DWORD)&vitem);

	return;
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI SetListViewTextOfSection(IN HWND hListSection, 
									 IN DWORD Count, 
									 IN PTCHAR szNumber, 
									 IN PTCHAR szName, 
									 IN PTCHAR szVirtualSize, 
									 IN PTCHAR szVirtualAddress, 
									 IN PTCHAR szSizeOfRawData, 
									 IN PTCHAR szPointerToRawData, 
									 IN PTCHAR szCharacteristics
									 )
{
	/*
		将得到的各种数据写入到IDC_LIST_Section上
	*/

	// 用于让操作系统显示值的结构
	LV_ITEM vitem = {0};

	// 成员 pszText 有效或必须填写。
	vitem.mask = LVIF_TEXT;

	// 序号
	vitem.pszText = TEXT(szNumber);
	vitem.iItem = Count;
	vitem.iSubItem = 0;
	SendMessage(hListSection, LVM_INSERTITEM, 0, (DWORD)&vitem);

	// 节名
	vitem.pszText = TEXT(szName);
	vitem.iSubItem = 1;
	SendMessage(hListSection, LVM_SETITEM, 0, (DWORD)&vitem);

	// 内存大小
	vitem.pszText = TEXT(szVirtualSize);
	vitem.iSubItem = 2;
	SendMessage(hListSection, LVM_SETITEM, 0, (DWORD)&vitem);

	// 内存偏移
	vitem.pszText = TEXT(szVirtualAddress);
	vitem.iSubItem = 3;
	SendMessage(hListSection, LVM_SETITEM, 0, (DWORD)&vitem);

	// 文件大小
	vitem.pszText = TEXT(szSizeOfRawData);	
	vitem.iSubItem = 4;
	SendMessage(hListSection, LVM_SETITEM, 0, (DWORD)&vitem);

	// 文件偏移
	vitem.pszText = TEXT(szPointerToRawData);	
	vitem.iSubItem = 5;
	SendMessage(hListSection, LVM_SETITEM, 0, (DWORD)&vitem);

	// 块属性
	vitem.pszText = TEXT(szCharacteristics);	
	vitem.iSubItem = 6;
	SendMessage(hListSection, LVM_SETITEM, 0, (DWORD)&vitem);

	return;
}
//////////////////////////////////////////////////////////////////////

BOOL WINAPI GetPidByListViewText(IN HWND hwndDlg, OUT PTCHAR szResult)
{
	/*
		根据用户点击的行数得到此行的pid的值
	*/

	// 创建此结构用于存储获取到的值
	LV_ITEM vitem = {0};

	// 检测用户点击了哪一行
	DWORD dwResult = SendMessage(hwndDlg, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
	if (dwResult == ERROR_CLICK)
	{
		// 没有点击在指定位置
		return RETURN_FAILURE;
	}

	vitem.iSubItem = 2;				// 要获取的列
	vitem.pszText = szResult;		// pid的值存储将存储的位置
	vitem.cchTextMax = 0x10;		// 指定缓冲区的大小

	// 获取 PID 中的值(第三列)
	SendMessage(hwndDlg, LVM_GETITEMTEXT, dwResult, (LPARAM)&vitem);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI EnumMoudelByProcID(IN HWND hListViewLower, IN DWORD ProcessID)
{
	/*
		根据进程 PID 遍历此进程中全部的模块
	*/
	
	// 定义一个 MODULEENTRY32 结构
	MODULEENTRY32 StructModuleEntry32 = {0};
	
	// 给 MODULEENTRY32 的 dwSize 赋值
	StructModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	// 获取模块快照
	LPVOID pModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcessID);
	if (pModuleSnapshot == INVALID_HANDLE_VALUE)
	{
		// 如果返回 -1 则代表失败 即:没有权限
		return;
	}

	// 根据输入进来的进程 ID 遍历模块
	StructModuleEntry32.th32ProcessID = ProcessID;

	// 第一次遍历
	Module32First(pModuleSnapshot, &StructModuleEntry32);

	// 行数
	DWORD dwListNumber = NULL;

	// 用于存储 编号 转换后的字符串
	TCHAR szNumber[0x10] = {0};

	// 用于存储模块 ImageBase 转换后的字符串
	TCHAR szBaseAddr[0x10] = {0};

	// 用于存储模块 大小 转换后的字符串
	TCHAR szBaseSize[0x10] = {0};

	// 获取第一个模块 放进 StructModuleEntry32.szExeFile 中
	Module32First(pModuleSnapshot, &StructModuleEntry32);

	do
	{
		// 将格式化数据转换为字符串
		sprintf(szNumber, "%8d", dwListNumber);
		sprintf(szBaseAddr, "%08X", StructModuleEntry32.modBaseAddr);
		sprintf(szBaseSize, "%08X", StructModuleEntry32.modBaseSize);


		if (pModuleSnapshot != INVALID_HANDLE_VALUE)
		{
			// 向 ListView 写入数据(有权限的情况)
			SetListViewTextLow(
				hListViewLower, 
				dwListNumber, 
				szNumber, 
				StructModuleEntry32.szModule, 
				szBaseAddr, 
				szBaseSize, 
				StructModuleEntry32.szExePath
				);
		}

		dwListNumber++;
	}
	// 获取下一个模块
	while(Module32Next(pModuleSnapshot, &StructModuleEntry32));

	return;
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI EnumProcByWinApi(IN HWND hListViewUpper)
{
	/*
		将系统中的所有进程遍历出来
	*/

	// 如果成功返回进程快照指针
	HANDLE pSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// 给 PROCESSENTRY32 的 dwSize 赋值
	PROCESSENTRY32 StructProcessEntry32 = {0};
	StructProcessEntry32.dwSize = sizeof(PROCESSENTRY32);

	// 给 MODULEENTRY32 的 dwSize 赋值
	MODULEENTRY32 StructModuleEntry32 = {0};
	StructModuleEntry32.dwSize = sizeof(MODULEENTRY32);

	// 获取第一个进程 放进 StructProcessEntry32.szExeFile 中
	Process32First(pSnapshot, &StructProcessEntry32);

	// 第几行
	DWORD dwListNumber = 0;

	// 用于存储 编号 转换后的字符串
	TCHAR szNumber[0x10] = {0};

	// 用于存储 PID 转换后的字符串
	TCHAR szBufferPID[0x10] = {0};

	// 用于存储 ImageBase 转换后的字符串
	TCHAR szBufferImageBase[0x10] = {0};

	// 用于存储 SizeOfImage 转换后的字符串
	TCHAR szBufferSizeOfImage[0x10] = {0};

	do
	{
		// 如果成功返回模块快照指针
		HANDLE pModSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, StructProcessEntry32.th32ProcessID);

		// 获取第一个模块 放进 StructModuleEntry32.szExeFile 中
		Module32First(pModSnapshot, &StructModuleEntry32);

		// 将格式化数据转换为字符串
		sprintf(szNumber, "%8d", dwListNumber);
		sprintf(szBufferPID, "%8d", StructProcessEntry32.th32ProcessID);
		sprintf(szBufferImageBase, "%08X", StructModuleEntry32.modBaseAddr);
		sprintf(szBufferSizeOfImage, "%08X", StructModuleEntry32.modBaseSize);

		if (pModSnapshot != INVALID_HANDLE_VALUE)
		{
			// 向 ListView 写入数据
			SetListViewText(
				hListViewUpper, 
				dwListNumber, 
				szNumber, 
				StructProcessEntry32.szExeFile, 
				szBufferPID, 
				szBufferImageBase, 
				szBufferSizeOfImage
				);
		}
		else
		{
			// 向 ListView 写入数据 (没有权限的情况)
			SetListViewText(
				hListViewUpper, 
				dwListNumber, 
				szNumber, 
				StructProcessEntry32.szExeFile, 
				szBufferPID, 
				"非32位进程", 
				"非32位进程"
				);
		}
		
		dwListNumber++;
	}
	// 当没有任何进程时跳出循环
	while(Process32Next(pSnapshot, &StructProcessEntry32));

	return;
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI InitProcessListViewUpper(IN HWND hwndDlg)
{
	/*
		设置 ListView 样式
	*/

	LV_COLUMN lv = {0};
	HWND hListProcess = {0};

	// 获取IDC_LIST_PROCESS句柄
	hListProcess = GetDlgItem(hwndDlg, IDC_LIST_PROCESS_UPPER);

	// 设置整行选中
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	// 设置属性
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	
	// 第 0 列
	lv.pszText = TEXT("序号");
	lv.cx = 100;
	lv.iSubItem = 0;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 0, (DWORD)&lv);

	// 第 1 列
	lv.pszText = TEXT("进程");
	lv.cx = 300;
	lv.iSubItem = 1;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 1, (DWORD)&lv);

	// 第 2 列
	lv.pszText = TEXT("PID");
	lv.cx = 100;
	lv.iSubItem = 2;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 2, (DWORD)&lv);


	// 第 3 列
	lv.pszText = TEXT("镜像基址");
	lv.cx = 200;
	lv.iSubItem = 3;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 3, (DWORD)&lv);

	// 第 4 列
	lv.pszText = TEXT("镜像大小");
	lv.cx = 200;
	lv.iSubItem = 4;
	SendMessage(hListProcess, LVM_INSERTCOLUMN, 4, (DWORD)&lv);
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI InitProcessListViewLower(HWND hwndDlg)
{
	/*
		设置第二个的 ListView 样式
	*/

	LV_COLUMN lv = {0};
	HWND hListProcess = {0};

	// 获取IDC_LIST_PROCESS句柄
	hListProcess = GetDlgItem(hwndDlg, IDC_LIST_PROCESS_LOWER);

	// 设置整行选中
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	
	// 设置属性
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	// 第0列
	lv.pszText = TEXT("序号");
	lv.cx = 100;
	lv.iSubItem = 0;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 0, (DWORD)&lv);

	// 第1列
	lv.pszText = TEXT("模块名称");
	lv.cx = 300;
	lv.iSubItem = 1;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 1, (DWORD)&lv);

	// 第2列
	lv.pszText = TEXT("模块基址");
	lv.cx = 200;
	lv.iSubItem = 2;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 2, (DWORD)&lv);

	// 第3列
	lv.pszText = TEXT("模块大小");
	lv.cx = 200;
	lv.iSubItem = 3;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 3, (DWORD)&lv);


	// 第4列
	lv.pszText = TEXT("模块位置");
	lv.cx = 800;
	lv.iSubItem = 4;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 4, (DWORD)&lv);
}

//////////////////////////////////////////////////////////////////////

VOID WINAPI InitSectionListView(HWND hwndDlg)
{
	/*
		设置IDD_DIALOG_Section的ListView的样式
	*/

	LV_COLUMN lv = {0};
	HWND hListProcess = {0};

	// 获取IDC_LIST_PROCESS句柄
	hListProcess = GetDlgItem(hwndDlg, IDC_LIST_Section);

	// 设置整行选中
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	
	// 设置属性
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	// 第0列
	lv.pszText = TEXT("序号");
	lv.cx = 100;
	lv.iSubItem = 0;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 0, (DWORD)&lv);

	// 第1列
	lv.pszText = TEXT("节名");
	lv.cx = 200;
	lv.iSubItem = 1;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 1, (DWORD)&lv);

	// 第2列
	lv.pszText = TEXT("内存大小");
	lv.cx = 200;
	lv.iSubItem = 2;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 2, (DWORD)&lv);

	// 第3列
	lv.pszText = TEXT("内存偏移");
	lv.cx = 200;
	lv.iSubItem = 3;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 3, (DWORD)&lv);


	// 第4列
	lv.pszText = TEXT("文件大小");
	lv.cx = 200;
	lv.iSubItem = 4;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 4, (DWORD)&lv);

	// 第5列
	lv.pszText = TEXT("文件偏移");
	lv.cx = 200;
	lv.iSubItem = 5;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 5, (DWORD)&lv);

	// 第6列
	lv.pszText = TEXT("块属性");
	lv.cx = 200;
	lv.iSubItem = 6;
	SendMessage(hListProcess,LVM_INSERTCOLUMN, 6, (DWORD)&lv);

	return;

}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI SetIcon(IN HINSTANCE hAppInstance, IN HWND hwndDlg, IN DWORD IconSmall, IN DWORD IconBig)
{
	/*
		设置图标
		需要模块句柄 hAppInstance
		根据 hwnd 改变指定的窗口的图标
		IconSmall 小图标资源编号
		IconBig 大图标资源编号
	*/

	HICON hIcon_SMALL = LoadIcon(hAppInstance, MAKEINTRESOURCE(IconSmall));
	HICON hIcon_BIG = LoadIcon(hAppInstance, MAKEINTRESOURCE(IconBig));
	SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon_SMALL);
	SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon_BIG);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI PushViewButton(IN HWND hwndDlg, IN LPTSTR TEXT(szFileName), IN PTCHAR TEXT(szTitle))
{
	/* 
		此函数用于处理 当用户点击"查看"按钮时应该进行的操作
		如果用户选择成功则返回非0
	*/

	// 初始化 OPENFILENAME 结构 用于获取文件路径的结构
	OPENFILENAME stOpenFile = {0};

	// 指定结构的长度
	stOpenFile.lStructSize = sizeof(OPENFILENAME);

	// 指定在哪个窗口上(主程序窗口)
	stOpenFile.hwndOwner = hwndDlg;

	// 将要过滤的文件扩展名 | exe、dll、sys、vxd、vdm
	stOpenFile.lpstrFilter = TEXT("exe;dll;sys;vxd;vdm;\0*exe;*dll;*sys;*vxd;*vdm;\0exe;\0*.exe;\0dll;\0*.dll\0sys;\0*.sys\0vxd;\0*.vxd;\0vdm;\0*vdm\0All Files(*.*)\0*.*\0\0");

	// 指定 lpstrFile 所指向的缓冲区的大小 以 TCHAR 为单位
	stOpenFile.nMaxFile = MAX_PATH;
	
	// 写入路径存储地址
	stOpenFile.lpstrFile = szFileName;

	// 要显示的标题
	stOpenFile.lpstrTitle = TEXT(szTitle);

	// 文件显示样式及特性
	stOpenFile.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	

	return GetOpenFileName(&stOpenFile);
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisOfPEData32(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS)
{
	/*
		如果使用此函数,则证明打开的文件已经通过PE文件检测
		解析PE文件,将解析好的数据分别写入到IDD_DIALOG_DOS_HEADER上的各个Edit中
		将根据32位PE文件来解析

		需要获取的值有:

		程序入口点	(IDC_EDIT_EntryPoint)			子系统		(IDC_EDIT_SubSystem)
		镜像基址	(IDC_EDIT_ImageBase)			区段数目	(IDC_EDIT_NumberOfSections)
		镜像大小	(IDC_EDIT_SizeOfImage)			时间戳		(IDC_EDIT_TimeDateStamp)
		代码基址	(IDC_EDIT_BaseOfCode)			PE头大小	(IDC_EDIT_SizeOfHeaders)
		数据基址	(IDC_EDIT_BaseOfData)			特征值		(IDC_EDIT_Magic)
		内存对齐	(IDC_EDIT_SectionAlignment)		校验和		(IDC_EDIT_CheckSum)
		文件对齐	(IDC_EDIT_FileAlignment)		可选PE头	(IDC_EDIT_SizeOfOptionalHeader)
		标志字		(IDC_EDIT_Characteristics)		目录项目数	(IDC_EDIT_NumberOfRvaAndSizes)
	*/
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 用于存储各个Edit的编号
	DWORD ElementalArrays[0x10] =
	{
		IDC_EDIT_EntryPoint,
		IDC_EDIT_ImageBase,
		IDC_EDIT_SizeOfImage,
		IDC_EDIT_BaseOfCode,
		IDC_EDIT_BaseOfData,
		IDC_EDIT_SectionAlignment,
		IDC_EDIT_FileAlignment,
		IDC_EDIT_Characteristics,
		IDC_EDIT_SubSystem,
		IDC_EDIT_NumberOfSections,
		IDC_EDIT_TimeDateStamp,
		IDC_EDIT_SizeOfHeaders,
		IDC_EDIT_Magic,
		IDC_EDIT_CheckSum,
		IDC_EDIT_SizeOfOptionalHeader,
		IDC_EDIT_NumberOfRvaAndSizes,
	};

	// 用于存储对应ElementalArrays数组的解析出来的值
	DWORD dwDATA[0x10] = 
	{
		pIMAGE_OPTIONAL_HEADER32 -> AddressOfEntryPoint,
		pIMAGE_OPTIONAL_HEADER32 -> ImageBase,
		pIMAGE_OPTIONAL_HEADER32 -> SizeOfImage,
		pIMAGE_OPTIONAL_HEADER32 -> BaseOfCode,
		pIMAGE_OPTIONAL_HEADER32 -> BaseOfData,
		pIMAGE_OPTIONAL_HEADER32 -> SectionAlignment,
		pIMAGE_OPTIONAL_HEADER32 -> FileAlignment,
		pIMAGE_FILE_HEADER -> Characteristics,
		pIMAGE_OPTIONAL_HEADER32 -> Subsystem,
		pIMAGE_FILE_HEADER -> NumberOfSections,
		pIMAGE_FILE_HEADER -> TimeDateStamp,
		pIMAGE_OPTIONAL_HEADER32 -> SizeOfHeaders,
		pIMAGE_OPTIONAL_HEADER32 -> Magic,
		pIMAGE_OPTIONAL_HEADER32 -> CheckSum,
		pIMAGE_FILE_HEADER -> SizeOfOptionalHeader,
		pIMAGE_OPTIONAL_HEADER32 -> NumberOfRvaAndSizes,
	};

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};
	
	// 用于指定循环次数
	DWORD dwTempNumber = 0xF;
	do
	{
		// 找到数据并将其转换为字符串类型
		sprintf(szDATA, "%08X", dwDATA[dwTempNumber]);
		SendMessage(GetDlgItem(hwndDlg, ElementalArrays[dwTempNumber]), WM_SETTEXT, 0, (LPARAM)TEXT(szDATA));
	}
	while(dwTempNumber--);
		
	return RETURN_SUCCESS;
}


//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisOfPEData64(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS)
{
	/*
		如果使用此函数,则证明打开的文件已经通过PE文件检测
		解析PE文件,将解析好的数据分别写入到IDD_DIALOG_DOS_HEADER上的各个Edit中
		将根据64位PE文件来解析
		需要获取的值有:
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 用于存储各个Edit的编号
	DWORD ElementalArrays[0xE] = 
	{
		IDC_EDIT_EntryPoint,
		IDC_EDIT_SizeOfImage,
		IDC_EDIT_BaseOfCode,
		IDC_EDIT_SectionAlignment,
		IDC_EDIT_FileAlignment,
		IDC_EDIT_Characteristics,
		IDC_EDIT_SubSystem,
		IDC_EDIT_NumberOfSections,
		IDC_EDIT_TimeDateStamp,
		IDC_EDIT_SizeOfHeaders,
		IDC_EDIT_Magic,
		IDC_EDIT_CheckSum,
		IDC_EDIT_SizeOfOptionalHeader,
		IDC_EDIT_NumberOfRvaAndSizes,
	};


	// 用于存储对应ElementalArrays数组的解析出来的值
	DWORD dwDATA[0xE] = 
	{
		pIMAGE_OPTIONAL_HEADER64 -> AddressOfEntryPoint,
		pIMAGE_OPTIONAL_HEADER64 -> SizeOfImage,
		pIMAGE_OPTIONAL_HEADER64 -> BaseOfCode,
		pIMAGE_OPTIONAL_HEADER64 -> SectionAlignment,
		pIMAGE_OPTIONAL_HEADER64 -> FileAlignment,
		pIMAGE_FILE_HEADER -> Characteristics,
		pIMAGE_OPTIONAL_HEADER64 -> Subsystem,
		pIMAGE_FILE_HEADER -> NumberOfSections,
		pIMAGE_FILE_HEADER -> TimeDateStamp,
		pIMAGE_OPTIONAL_HEADER64 -> SizeOfHeaders,
		pIMAGE_OPTIONAL_HEADER64 -> Magic,
		pIMAGE_OPTIONAL_HEADER64 -> CheckSum,
		pIMAGE_FILE_HEADER -> SizeOfOptionalHeader,
		pIMAGE_OPTIONAL_HEADER64 -> NumberOfRvaAndSizes,
	};

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};
	
	// 用于指定循环次数
	DWORD dwTempNumber = 0xD;
	do
	{
		// 找到数据并将其转换为字符串类型
		sprintf(szDATA, "%08X", dwDATA[dwTempNumber]);
		SendMessage(GetDlgItem(hwndDlg, ElementalArrays[dwTempNumber]), WM_SETTEXT, 0, (LPARAM)TEXT(szDATA));
	}
	while(dwTempNumber--);

	// 数据基址(BaseOfData)使用32位即可
	sprintf(szDATA,"%08X", pIMAGE_OPTIONAL_HEADER32 -> BaseOfData);
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_BaseOfData), WM_SETTEXT, 0, (LPARAM)TEXT(szDATA));

	// 64位的ImageBase单独赋值(ULONGLONG类型)
	sprintf(szDATA,"%I64X", pIMAGE_OPTIONAL_HEADER64 -> ImageBase);
	SendMessage(GetDlgItem(hwndDlg, IDC_EDIT_ImageBase), WM_SETTEXT, 0, (LPARAM)TEXT(szDATA));

	return RETURN_SUCCESS;
}
	

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDirectoryOfPEData32(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS)
{
	/*
		进一步解析32位PE文件的数据目录

		"导出表", "导入表", "资源表", "异常信息表", 
		"安全证书表", "重定位表", "调试信息表", "版权所有表", 
		"全局指针表", "TLS表", "加载配置表", "绑定导入表",
		"IAT表", "延迟导入表", "COM信息表", "保留且未使用的表",
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 用于存储各个Edit的编号
	DWORD ElementalArrays[0x30] = 
	{
		IDC_EDIT_D_Export_FOA,
		IDC_EDIT_D_Export_RVA,
		IDC_EDIT_D_Export_SIZE,
		IDC_EDIT_D_Import_FOA,
		IDC_EDIT_D_Import_RVA,
		IDC_EDIT_D_Import_SIZE,
		IDC_EDIT_D_Resource_FOA,
		IDC_EDIT_D_Resource_RVA,
		IDC_EDIT_D_Resource_SIZE,
		IDC_EDIT_D_Exception_FOA,
		IDC_EDIT_D_Exception_RVA,
		IDC_EDIT_D_Exception_SIZE,
		IDC_EDIT_D_Security_FOA,
		IDC_EDIT_D_Security_RVA,
		IDC_EDIT_D_Security_SIZE,
		IDC_EDIT_D_BaseReloc_FOA,
		IDC_EDIT_D_BaseReloc_RVA,
		IDC_EDIT_D_BaseReloc_SIZE,
		IDC_EDIT_D_Debug_FOA,
		IDC_EDIT_D_Debug_RVA,
		IDC_EDIT_D_Debug_SIZE,
		IDC_EDIT_D_CopyRight_FOA,
		IDC_EDIT_D_CopyRight_RVA,
		IDC_EDIT_D_CopyRight_SIZE,
		IDC_EDIT_D_GlobalPtr_FOA,
		IDC_EDIT_D_GlobalPtr_RVA,
		IDC_EDIT_D_GlobalPtr_SIZE,
		IDC_EDIT_D_TLS_FOA,
		IDC_EDIT_D_TLS_RVA,
		IDC_EDIT_D_TLS_SIZE,
		IDC_EDIT_D_LoadConfig_FOA,
		IDC_EDIT_D_LoadConfig_RVA,
		IDC_EDIT_D_LoadConfig_SIZE,
		IDC_EDIT_D_BoundImport_FOA,
		IDC_EDIT_D_BoundImport_RVA,
		IDC_EDIT_D_BoundImport_SIZE,
		IDC_EDIT_D_IAT_FOA,
		IDC_EDIT_D_IAT_RVA,
		IDC_EDIT_D_IAT_SIZE,
		IDC_EDIT_D_DelayImport_FOA,
		IDC_EDIT_D_DelayImport_RVA,
		IDC_EDIT_D_DelayImport_SIZE,
		IDC_EDIT_D_ComDescriptor_FOA,
		IDC_EDIT_D_ComDescriptor_RVA,
		IDC_EDIT_D_ComDescriptor_SIZE,
		IDC_EDIT_D_Retain_FOA,
		IDC_EDIT_D_Retain_RVA,
		IDC_EDIT_D_Retain_SIZE,
	};

	// 用于存储对应ElementalArrays数组的解析出来的值
	DWORD dwDATA[0x30] = {0};

	// 将要循环的次数+1
	DWORD dwTempNumber = 0x30;

	// 数据目录下标
	DWORD dwDIRECTORY = 0xF;

	do
	{
		// 循环存储需要的值
		dwTempNumber--;
		dwDATA[dwTempNumber--] = (pIMAGE_DATA_DIRECTORY + dwDIRECTORY) -> Size;
		dwDATA[dwTempNumber--] = (pIMAGE_DATA_DIRECTORY + dwDIRECTORY) -> VirtualAddress;
		dwDATA[dwTempNumber] = ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + dwDIRECTORY--) -> VirtualAddress, g_pStTLS -> m_fpFileBuffer);
	}
	while(dwTempNumber);

	// 重新给dwTempNumber赋值
	dwTempNumber = 0x2F;

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	do
	{
		// 找到数据并将其转换为字符串类型
		sprintf(szDATA, "%08X", dwDATA[dwTempNumber]);
		SendMessage(GetDlgItem(hwndDlg, ElementalArrays[dwTempNumber]), WM_SETTEXT, 0, (LPARAM)TEXT(szDATA));
	}
	while(dwTempNumber--);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDirectoryOfPEData64(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS)
{
	/*
		进一步解析64位PE文件的数据目录
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER64 -> DataDirectory);

	// 用于存储各个Edit的编号
	DWORD ElementalArrays[0x30] = 
	{
		IDC_EDIT_D_Export_FOA,
		IDC_EDIT_D_Export_RVA,
		IDC_EDIT_D_Export_SIZE,
		IDC_EDIT_D_Import_FOA,
		IDC_EDIT_D_Import_RVA,
		IDC_EDIT_D_Import_SIZE,
		IDC_EDIT_D_Resource_FOA,
		IDC_EDIT_D_Resource_RVA,
		IDC_EDIT_D_Resource_SIZE,
		IDC_EDIT_D_Exception_FOA,
		IDC_EDIT_D_Exception_RVA,
		IDC_EDIT_D_Exception_SIZE,
		IDC_EDIT_D_Security_FOA,
		IDC_EDIT_D_Security_RVA,
		IDC_EDIT_D_Security_SIZE,
		IDC_EDIT_D_BaseReloc_FOA,
		IDC_EDIT_D_BaseReloc_RVA,
		IDC_EDIT_D_BaseReloc_SIZE,
		IDC_EDIT_D_Debug_FOA,
		IDC_EDIT_D_Debug_RVA,
		IDC_EDIT_D_Debug_SIZE,
		IDC_EDIT_D_CopyRight_FOA,
		IDC_EDIT_D_CopyRight_RVA,
		IDC_EDIT_D_CopyRight_SIZE,
		IDC_EDIT_D_GlobalPtr_FOA,
		IDC_EDIT_D_GlobalPtr_RVA,
		IDC_EDIT_D_GlobalPtr_SIZE,
		IDC_EDIT_D_TLS_FOA,
		IDC_EDIT_D_TLS_RVA,
		IDC_EDIT_D_TLS_SIZE,
		IDC_EDIT_D_LoadConfig_FOA,
		IDC_EDIT_D_LoadConfig_RVA,
		IDC_EDIT_D_LoadConfig_SIZE,
		IDC_EDIT_D_BoundImport_FOA,
		IDC_EDIT_D_BoundImport_RVA,
		IDC_EDIT_D_BoundImport_SIZE,
		IDC_EDIT_D_IAT_FOA,
		IDC_EDIT_D_IAT_RVA,
		IDC_EDIT_D_IAT_SIZE,
		IDC_EDIT_D_DelayImport_FOA,
		IDC_EDIT_D_DelayImport_RVA,
		IDC_EDIT_D_DelayImport_SIZE,
		IDC_EDIT_D_ComDescriptor_FOA,
		IDC_EDIT_D_ComDescriptor_RVA,
		IDC_EDIT_D_ComDescriptor_SIZE,
		IDC_EDIT_D_Retain_FOA,
		IDC_EDIT_D_Retain_RVA,
		IDC_EDIT_D_Retain_SIZE,
	};

	// 用于存储对应ElementalArrays数组的解析出来的值
	DWORD dwDATA[0x30] = {0};

	// 将要循环的次数+1
	DWORD dwTempNumber = 0x30;

	// 数据目录下标
	DWORD dwDIRECTORY = 0xF;

	do
	{
		// 循环存储需要的值
		dwTempNumber--;
		dwDATA[dwTempNumber--] = (pIMAGE_DATA_DIRECTORY + dwDIRECTORY) -> Size;
		dwDATA[dwTempNumber--] = (pIMAGE_DATA_DIRECTORY + dwDIRECTORY) -> VirtualAddress;
		dwDATA[dwTempNumber] = ChangeRvaToFoa64((pIMAGE_DATA_DIRECTORY + dwDIRECTORY--) -> VirtualAddress, g_pStTLS -> m_fpFileBuffer);
	}
	while(dwTempNumber);

	// 重新给dwTempNumber赋值
	dwTempNumber = 0x2F;

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	do
	{
		// 找到数据并将其转换为字符串类型
		sprintf(szDATA, "%08X", dwDATA[dwTempNumber]);
		SendMessage(GetDlgItem(hwndDlg, ElementalArrays[dwTempNumber]), WM_SETTEXT, 0, (LPARAM)TEXT(szDATA));
	}
	while(dwTempNumber--);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisSectionOfPEData32(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS)
{
	/*
		进一步解析32位PE文件的区段
		并将节表信息显示在IDD_DIALOG_Section的ListView上
	*/
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER32 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);
	
	// 用于判断循环次数
	DWORD dwTempNumber = 0;

	// 用于存储转换后得到的字符串
	TCHAR szNumber[0x10] = {0};
	TCHAR szName[0x10] = {0};
	TCHAR szVirtualSize[0x10] = {0};
	TCHAR szVirtualAddress[0x10] = {0};
	TCHAR szSizeOfRawData[0x10] = {0};
	TCHAR szPointerToRawData[0x10] = {0};
	TCHAR szCharacteristics[0x10] = {0};

	do
	{
		// 将节表名称拷贝到更大的空间以防止名称刚好8个字节的情况
		memcpy(szName, &pIMAGE_SECTION_HEADER -> Name, 0x8);

		// 找到数据并将其转换为字符串类型
		sprintf(szNumber, "%8d", dwTempNumber);
		sprintf(szVirtualSize, "%08X", pIMAGE_SECTION_HEADER -> Misc.VirtualSize);
		sprintf(szVirtualAddress, "%08X", pIMAGE_SECTION_HEADER -> VirtualAddress);
		sprintf(szSizeOfRawData, "%08X", pIMAGE_SECTION_HEADER -> SizeOfRawData);
		sprintf(szPointerToRawData, "%08X", pIMAGE_SECTION_HEADER -> PointerToRawData);
		sprintf(szCharacteristics, "%08X", pIMAGE_SECTION_HEADER -> Characteristics);
		
		// 将需要显示的字符串输入到IDD_DIALOG_Section的ListView上
		SetListViewTextOfSection(
			GetDlgItem(hwndDlg, IDC_LIST_Section), 
			dwTempNumber++, 
			szNumber, 
			szName, 
			szVirtualSize, 
			szVirtualAddress, 
			szSizeOfRawData, 
			szPointerToRawData, 
			szCharacteristics
			);

		// 跳转到下一个节表
		pIMAGE_SECTION_HEADER++;
	}
	while(dwTempNumber < pIMAGE_FILE_HEADER -> NumberOfSections);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisSectionOfPEData64(IN HWND hwndDlg, IN PTLSDirectory g_pStTLS)
{
	/*
		进一步解析64位PE文件的区段
		并将节表信息显示在IDD_DIALOG_Section的ListView上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义节表结构体指针
	PIMAGE_SECTION_HEADER pIMAGE_SECTION_HEADER = (IMAGE_SECTION_HEADER*)((DWORD)pIMAGE_OPTIONAL_HEADER64 + pIMAGE_FILE_HEADER -> SizeOfOptionalHeader);

	// 用于判断循环次数
	DWORD dwTempNumber = 0;

	// 用于存储转换后得到的字符串
	TCHAR szNumber[0x10] = {0};
	TCHAR szName[0x10] = {0};
	TCHAR szVirtualSize[0x10] = {0};
	TCHAR szVirtualAddress[0x10] = {0};
	TCHAR szSizeOfRawData[0x10] = {0};
	TCHAR szPointerToRawData[0x10] = {0};
	TCHAR szCharacteristics[0x10] = {0};

	do
	{
		// 将节表名称拷贝到更大的空间以防止名称刚好8个字节的情况
		memcpy(szName, &pIMAGE_SECTION_HEADER -> Name, 0x8);

		// 找到数据并将其转换为字符串类型
		sprintf(szNumber, "%8d", dwTempNumber);
		sprintf(szVirtualSize, "%08X", pIMAGE_SECTION_HEADER -> Misc.VirtualSize);
		sprintf(szVirtualAddress, "%08X", pIMAGE_SECTION_HEADER -> VirtualAddress);
		sprintf(szSizeOfRawData, "%08X", pIMAGE_SECTION_HEADER -> SizeOfRawData);
		sprintf(szPointerToRawData, "%08X", pIMAGE_SECTION_HEADER -> PointerToRawData);
		sprintf(szCharacteristics, "%08X", pIMAGE_SECTION_HEADER -> Characteristics);
		
		// 将需要显示的字符串输入到IDD_DIALOG_Section的ListView上
		SetListViewTextOfSection(
			GetDlgItem(hwndDlg, IDC_LIST_Section), 
			dwTempNumber++, 
			szNumber, 
			szName, 
			szVirtualSize, 
			szVirtualAddress, 
			szSizeOfRawData, 
			szPointerToRawData, 
			szCharacteristics
			);

		// 跳转到下一个节表
		pIMAGE_SECTION_HEADER++;
	}
	while(dwTempNumber < pIMAGE_FILE_HEADER -> NumberOfSections);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI DsEditHandle(IN PTLSDirectory g_pStTLS, IN HWND hwndDlg, IN DWORD dwCallBackFunction, IN DWORD dwFlag)
{
	/*
		根据点击的不同按钮更改不不同标志位
		统一都调用DialogProcDS_ALL回调函数来处理
		如此可减少资源文件的使用
	*/

	// 初始化DS标志位
	CLICK_DS_INIT(g_pStTLS -> m_dwGlobalFlag);

	// 将指定的标志位,置1
	switch (dwFlag)
	{
	case 0:
		CLICK_DS_SELECT_EXPORT(g_pStTLS -> m_dwGlobalFlag);
		break;
	case 1:
		CLICK_DS_SELECT_IMPORT(g_pStTLS -> m_dwGlobalFlag);
		break;
	case 2:
		CLICK_DS_SELECT_RESOURCE(g_pStTLS -> m_dwGlobalFlag);
		break;
	case 3:
		CLICK_DS_SELECT_BASERELOC(g_pStTLS -> m_dwGlobalFlag);
		break;
	case 4:
		CLICK_DS_SELECT_BOUNDIMPORT(g_pStTLS -> m_dwGlobalFlag);
		break;
	case 5:
		CLICK_DS_SELECT_IAT(g_pStTLS -> m_dwGlobalFlag);
		break;
	}
	
	// 调用DialogProcDS_ALL
	DialogBox(g_pStTLS -> m_hAppInstance, 
		MAKEINTRESOURCE(IDD_DIALOG_DS_ALL), 
		hwndDlg,
		(int (WINAPI*)(struct HWND__*, unsigned int, unsigned int, long))dwCallBackFunction);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsExportOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析32位的PE文件的导出表并将其内容显示在IDC_EDIT_DS_ALL上
	*/
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 首先判断PE文件是否有导出表
	if (!pIMAGE_DATA_DIRECTORY -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有导出表");
		return RETURN_FAILURE;
	}

	// 定义导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa32(pIMAGE_DATA_DIRECTORY -> VirtualAddress, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"导出表信息:\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------\r\n");

	// 用于存储名称
	PTCHAR ElementalArrays[0x8] =
	{
		"\r\nTimeDateStamp\r\t\t[时间戳]\r\t\t\t",
		"\r\nName\r\t\t\t[模块名称]\r\t\t\t",
		"\r\nBase\r\t\t\t[导出函数起始序号]\r\t\t",
		"\r\nNumberOfFunctions\r\t[导出函数个数]\r\t\t",
		"\r\nNumberOfNames\r\t\t[以名字导出的函数个数]\r\t",
		"\r\nAddressOfFunctions\r\t[导出函数地址表RVA]\r\t",
		"\r\nAddressOfNames\r\t\t[导出函数名称表RVA]\r\t",
		"\r\nAddressOfNameOrdinals\r\t[导出函数序号表RVA]\r\t",
	};

	// 用于存储数据
	DWORD dwDATA[0x8] =
	{
		pIMAGE_EXPORT_DIRECTORY -> TimeDateStamp,
		pIMAGE_EXPORT_DIRECTORY -> Name,
		pIMAGE_EXPORT_DIRECTORY -> Base,
		pIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions,
		pIMAGE_EXPORT_DIRECTORY -> NumberOfNames,
		pIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions,
		pIMAGE_EXPORT_DIRECTORY -> AddressOfNames,
		pIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals,
	};

	// 临时变量
	DWORD dwTempNumber = 0;
	
	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 先写入模块名称
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"模块名称:\r\t");
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)(ChangeRvaToFoa32(pIMAGE_EXPORT_DIRECTORY -> Name, g_pStTLS -> m_fpFileBuffer)) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	do
	{
		// 转换为字符串类型并循环写入
		sprintf(szDATA, "%08X", dwDATA[dwTempNumber]);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)ElementalArrays[dwTempNumber]);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	}
	while(dwTempNumber++ < 0x7);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n------------------------------------------------\r\n");

	// 得到导出函数地址表/名称表/序号表的实际所在地址
	LPDWORD pExportFunctions = (LPDWORD)(ChangeRvaToFoa32(pIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	LPDWORD pExportNames = (LPDWORD)(ChangeRvaToFoa32(pIMAGE_EXPORT_DIRECTORY -> AddressOfNames, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	LPWORD pExportNameOrdinals = (LPWORD)(ChangeRvaToFoa32(pIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	// 重新给dwTempNumber赋值为0
	dwTempNumber = 0;

	// 用于计数
	DWORD dwNumberOfOrdinals = 0;

	// 有导出表但是没有导出函数
	if (!pIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions)
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"此PE文件没有任何导出函数!");
		return RETURN_FAILURE;
	}

	// 标题栏
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"[函数导出序号]\r\t[函数地址RVA]\r\t[函数名称]\r\n");

	// 打印导出表深层结构
	do
	{
		if (*(pExportFunctions + dwTempNumber))
		{
			// 如果函数地址表有地址则先初始化导出函数标志位
			EXPORT_FUNC_INIT(g_pStTLS -> m_dwGlobalFlag);
			
			// 开始解析和打印
			do
			{
				// 用函数地址表的下标到函数序号表里对比,看有没有这个下标
				if (dwTempNumber == *(pExportNameOrdinals + dwNumberOfOrdinals))
				{
					// 如果找到了对应的序号首先更改全局标志位为以名字导出
					EXPORT_BY_NAME(g_pStTLS -> m_dwGlobalFlag);

					// 先打印序号 导出序号等于序号表序号+base
					sprintf(szDATA, "%04X", *(pExportNameOrdinals + dwNumberOfOrdinals) + pIMAGE_EXPORT_DIRECTORY -> Base);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\t");

					// 再打印函数的RVA
					sprintf(szDATA, "%08X", *(pExportFunctions + dwTempNumber));
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");

					// 使用着序号表里的下标到名字表里去找对应的名称的地址,再将函数名称显示在Edit上
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)(ChangeRvaToFoa32(*(pExportNames + dwNumberOfOrdinals), g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer));
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

					// 将dwNumberOfOrdinals的值清零以备下次使用
					dwNumberOfOrdinals = 0;

					// 如果找到了则直接跳出内层循环
					break;
				}
			}
			while(++dwNumberOfOrdinals < pIMAGE_EXPORT_DIRECTORY -> NumberOfNames);

			// 当内层do-while循环结束后,首先判断是否在序号表找到了对应的值(是否是以名字导出的)
			if (!EXPORT_OF_NAME_COMPARE(g_pStTLS -> m_dwGlobalFlag))
			{
				// 首先将dwNumberOfOrdinals置零
				dwNumberOfOrdinals = 0;

				// 以序号导出则导出序号等于下标+base
				sprintf(szDATA, "%04X", dwTempNumber + pIMAGE_EXPORT_DIRECTORY -> Base);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\t");

				// RVA
				sprintf(szDATA, "%08X", *(pExportFunctions + dwTempNumber));
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");

				// 名字写成"-"
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"-");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}
		} // End if
	}	
	while(++dwTempNumber < pIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsExportOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析64位的PE文件的导出表并将其内容显示在IDC_EDIT_DS_ALL上
	*/
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER64 -> DataDirectory);

	// 首先判断PE文件是否有导出表
	if (!pIMAGE_DATA_DIRECTORY -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有导出表");
		return RETURN_FAILURE;
	}

	// 定义导出表结构体指针
	PIMAGE_EXPORT_DIRECTORY pIMAGE_EXPORT_DIRECTORY = (IMAGE_EXPORT_DIRECTORY*)(ChangeRvaToFoa64(pIMAGE_DATA_DIRECTORY -> VirtualAddress, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"导出表信息:\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------\r\n");

	// 用于存储名称
	PTCHAR ElementalArrays[0x8] =
	{
		"\r\nTimeDateStamp\r\t\t[时间戳]\r\t\t\t",
		"\r\nName\r\t\t\t[模块名称]\r\t\t\t",
		"\r\nBase\r\t\t\t[导出函数起始序号]\r\t\t",
		"\r\nNumberOfFunctions\r\t[导出函数个数]\r\t\t",
		"\r\nNumberOfNames\r\t\t[以名字导出的函数个数]\r\t",
		"\r\nAddressOfFunctions\r\t[导出函数地址表RVA]\r\t",
		"\r\nAddressOfNames\r\t\t[导出函数名称表RVA]\r\t",
		"\r\nAddressOfNameOrdinals\r\t[导出函数序号表RVA]\r\t",
	};

	// 用于存储数据
	DWORD dwDATA[0x8] =
	{
		pIMAGE_EXPORT_DIRECTORY -> TimeDateStamp,
		pIMAGE_EXPORT_DIRECTORY -> Name,
		pIMAGE_EXPORT_DIRECTORY -> Base,
		pIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions,
		pIMAGE_EXPORT_DIRECTORY -> NumberOfNames,
		pIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions,
		pIMAGE_EXPORT_DIRECTORY -> AddressOfNames,
		pIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals,
	};

	// 临时变量
	DWORD dwTempNumber = 0;
	
	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 先写入模块名称
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"模块名称:\r\t");
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)(ChangeRvaToFoa64(pIMAGE_EXPORT_DIRECTORY -> Name, g_pStTLS -> m_fpFileBuffer)) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	do
	{
		// 转换为字符串类型并循环写入
		sprintf(szDATA, "%08X", dwDATA[dwTempNumber]);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)ElementalArrays[dwTempNumber]);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	}
	while(dwTempNumber++ < 0x7);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n------------------------------------------------\r\n");

	// 得到导出函数地址表/名称表/序号表的实际所在地址
	LPDWORD pExportFunctions = (LPDWORD)(ChangeRvaToFoa64(pIMAGE_EXPORT_DIRECTORY -> AddressOfFunctions, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	LPDWORD pExportNames = (LPDWORD)(ChangeRvaToFoa64(pIMAGE_EXPORT_DIRECTORY -> AddressOfNames, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	LPWORD pExportNameOrdinals = (LPWORD)(ChangeRvaToFoa64(pIMAGE_EXPORT_DIRECTORY -> AddressOfNameOrdinals, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	// 重新给dwTempNumber赋值为0
	dwTempNumber = 0;

	// 用于计数
	DWORD dwNumberOfOrdinals = 0;

	// 有导出表但是没有导出函数
	if (!pIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions)
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"此PE文件没有任何导出函数!");
		return RETURN_FAILURE;
	}

	// 标题栏
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"[函数导出序号]\r\t[函数地址RVA]\r\t[函数名称]\r\n");

	// 打印导出表深层结构
	do
	{
		if (*(pExportFunctions + dwTempNumber))
		{
			// 如果函数地址表有地址则先初始化导出函数标志位
			EXPORT_FUNC_INIT(g_pStTLS -> m_dwGlobalFlag);
			
			// 开始解析和打印
			do
			{
				// 用函数地址表的下标到函数序号表里对比,看有没有这个下标
				if (dwTempNumber == *(pExportNameOrdinals + dwNumberOfOrdinals))
				{
					// 如果找到了对应的序号首先更改全局标志位为以名字导出
					EXPORT_BY_NAME(g_pStTLS -> m_dwGlobalFlag);

					// 先打印序号 导出序号等于序号表序号+base
					sprintf(szDATA, "%04X", *(pExportNameOrdinals + dwNumberOfOrdinals) + pIMAGE_EXPORT_DIRECTORY -> Base);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\t");

					// 再打印函数的RVA
					sprintf(szDATA, "%08X", *(pExportFunctions + dwTempNumber));
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");

					// 使用着序号表里的下标到名字表里去找对应的名称的地址,再将函数名称显示在Edit上
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)(ChangeRvaToFoa64(*(pExportNames + dwNumberOfOrdinals), g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer));
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

					// 将dwNumberOfOrdinals的值清零以备下次使用
					dwNumberOfOrdinals = 0;

					// 如果找到了则直接跳出内层循环
					break;
				}
			}
			while(++dwNumberOfOrdinals < pIMAGE_EXPORT_DIRECTORY -> NumberOfNames);

			// 当内层do-while循环结束后,首先判断是否在序号表找到了对应的值(是否是以名字导出的)
			if (!EXPORT_OF_NAME_COMPARE(g_pStTLS -> m_dwGlobalFlag))
			{
				// 首先将dwNumberOfOrdinals置零
				dwNumberOfOrdinals = 0;

				// 以序号导出则导出序号等于下标+base
				sprintf(szDATA, "%04X", dwTempNumber + pIMAGE_EXPORT_DIRECTORY -> Base);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\t");

				// RVA
				sprintf(szDATA, "%08X", *(pExportFunctions + dwTempNumber));
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");

				// 名字写成"-"
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"-");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}
		} // End if
	}	
	while(++dwTempNumber < pIMAGE_EXPORT_DIRECTORY -> NumberOfFunctions);

	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsImportOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析32位的PE文件的导入表并将其内容显示在IDC_EDIT_DS_ALL上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 首先判断是否有导入表
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有导入表");
		return RETURN_FAILURE;
	}
	
	// 定义导入表结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 如果导入表结构体指针有值则开始打印
	if (!pIMAGE_IMPORT_DESCRIPTOR -> Name)
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"此PE文件没有任何导入函数!");
		return RETURN_FAILURE;
	}

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"导入表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	do
	{
		// 判断是否导入表外层结构结束 结束则跳出
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;

		// 遍历需要导入的全部的 DLL 循环打印 _IMAGE_IMPORT_DESCRIPTOR 导入表信息
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------ \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"OriginalFirstThunk:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"TimeDateStamp:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"ForwarderChain:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Name:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> Name);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstThunk:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		// 打印模块名称
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"模块名称:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\t");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)(LPTSTR)(ChangeRvaToFoa32(pIMAGE_IMPORT_DESCRIPTOR -> Name, \
			g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer));
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
		// 打印深层结构

		// 打印 INT 表
		// 找到 PIMAGE_THUNK_DATA32 的 FOA (4字节)
		DWORD pINT_PIMAGE_THUNK_DATA32 = (DWORD)(ChangeRvaToFoa32(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk, \
			g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
		while(*(LPDWORD)pINT_PIMAGE_THUNK_DATA32 != 0)
		{
			// 值为0则循环结束
			
			// 判断最高位是否为1
			if (*(LPDWORD)pINT_PIMAGE_THUNK_DATA32 & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1 那么除去最高位的值 就是函数的导出序号
				DWORD Original = *(LPDWORD)pINT_PIMAGE_THUNK_DATA32 & ~IMAGE_ORDINAL_FLAG32;
				sprintf(szDATA, "%08X", Original);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"INT[序号]");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}
			// 如果不是，那么这个值是一个RVA 指向 IMAGE_IMPORT_BY_NAME 
			else
			{	// 打印 INT 表
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa32(*(LPDWORD)pINT_PIMAGE_THUNK_DATA32, \
					g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"INT[名称]");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}

			// 找到下一个地址
			pINT_PIMAGE_THUNK_DATA32 += 4;
		}

		// 打印 IAT 表
		// 找到 PIMAGE_THUNK_DATA32 的 FOA (4字节)
		DWORD pIMAGE_THUNK_DATA32 = (DWORD)(ChangeRvaToFoa32(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk, \
			g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
		
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n\n");

		// 定义 IAT 表结构体指针 FirstThunk 指向 IAT 表
		while(*(LPDWORD)pIMAGE_THUNK_DATA32 != 0)
		{
			// 值为0则循环结束

			// 判断此DLL是否被绑定 如果被绑定直接打印函数地址
			if (pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp)
			{
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"地址[Bound]: ");
				sprintf(szDATA, "%08X", *(LPDWORD)pIMAGE_THUNK_DATA32);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
				
				// 找到下一个地址
				pIMAGE_THUNK_DATA32 += 4;
				continue;
			}

			// 判断最高位是否为1
			if (*(LPDWORD)pIMAGE_THUNK_DATA32 & IMAGE_ORDINAL_FLAG32)
			{
				// 如果为1 那么除去最高位的值 就是函数的导出序号
				DWORD Original = *(LPDWORD)pIMAGE_THUNK_DATA32 & ~IMAGE_ORDINAL_FLAG32;

				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"IAT[序号]: ");
				sprintf(szDATA, "%08X", Original);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}

			// 如果不是，那么这个值是一个RVA 指向 IMAGE_IMPORT_BY_NAME 
			else
			{
				PIMAGE_IMPORT_BY_NAME pIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa32(*(LPDWORD)pIMAGE_THUNK_DATA32, \
					g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"IAT[名称]: ");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pIMAGE_IMPORT_BY_NAME -> Name);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}

			// 找到下一个地址
			pIMAGE_THUNK_DATA32 += 4;
		}

		// 找到下一个模块
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);

	return RETURN_SUCCESS; 
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsImportOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析64位的PE文件的导入表并将其内容显示在IDC_EDIT_DS_ALL上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER64 -> DataDirectory);

	// 首先判断是否有导入表
	if (!(pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有导入表");
		return RETURN_FAILURE;
	}
	
	// 定义导入表结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa64((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 如果导入表结构体指针有值则开始打印
	if (!pIMAGE_IMPORT_DESCRIPTOR -> Name)
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"此PE文件没有任何导入函数!");
		return RETURN_FAILURE;
	}

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"导入表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	do
	{
		// 判断是否导入表外层结构结束 结束则跳出
		if (!(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk | pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp |\
			pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain | pIMAGE_IMPORT_DESCRIPTOR -> Name | \
			pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk))
			break;

		// 遍历需要导入的全部的 DLL 循环打印 _IMAGE_IMPORT_DESCRIPTOR 导入表信息
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------ \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"OriginalFirstThunk:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"TimeDateStamp:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"ForwarderChain:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> ForwarderChain);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Name:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> Name);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstThunk:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
		sprintf(szDATA, "%08X", pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

		// 打印模块名称
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"模块名称:");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\t");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)(LPTSTR)(ChangeRvaToFoa64(pIMAGE_IMPORT_DESCRIPTOR -> Name, \
			g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer));
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n"); // continue...
		// 打印深层结构

		// 打印 INT 表
		// 找到 PIMAGE_THUNK_DATA64 的 FOA (8字节)
		ULONGLONG pINT_PIMAGE_THUNK_DATA64 = (ULONGLONG)(ChangeRvaToFoa64(pIMAGE_IMPORT_DESCRIPTOR -> OriginalFirstThunk, \
			g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
		while(*(PULONGLONG)pINT_PIMAGE_THUNK_DATA64 != 0)
		{
			// 值为0则循环结束
			
			// 判断最高位是否为1
			if (*(PULONGLONG)pINT_PIMAGE_THUNK_DATA64 & IMAGE_ORDINAL_FLAG64)
			{
				// 如果为1 那么除去最高位的值 就是函数的导出序号
				ULONGLONG Original = *(PULONGLONG)pINT_PIMAGE_THUNK_DATA64 & ~IMAGE_ORDINAL_FLAG64;
				sprintf(szDATA, "%08X", Original);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"INT[序号]");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}
			// 如果不是，那么这个值是一个RVA 指向 IMAGE_IMPORT_BY_NAME 
			else
			{	// 打印 INT 表
				PIMAGE_IMPORT_BY_NAME pINT_PIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa64(*(LPDWORD)pINT_PIMAGE_THUNK_DATA64, \
					g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"INT[名称]");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pINT_PIMAGE_IMPORT_BY_NAME -> Name);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}

			// 找到下一个地址
			pINT_PIMAGE_THUNK_DATA64 += 8;
		}

		// 打印 IAT 表
		// 找到 PIMAGE_THUNK_DATA64 的 FOA (4字节)
		ULONGLONG pIMAGE_THUNK_DATA64 = (ULONGLONG)(ChangeRvaToFoa64(pIMAGE_IMPORT_DESCRIPTOR -> FirstThunk, \
			g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
		
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n\n");

		// 定义 IAT 表结构体指针 FirstThunk 指向 IAT 表
		while(*(PULONGLONG)pIMAGE_THUNK_DATA64 != 0)
		{
			// 值为0则循环结束

			// 判断此DLL是否被绑定 如果被绑定直接打印函数地址
			if (pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp)
			{
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"地址[Bound]: ");
				sprintf(szDATA, "%08X", *(LPDWORD)pIMAGE_THUNK_DATA64);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
				
				// 找到下一个地址
				pIMAGE_THUNK_DATA64 += 8;
				continue;
			}

			// 判断最高位是否为1
			if (*(PULONGLONG)pIMAGE_THUNK_DATA64 & IMAGE_ORDINAL_FLAG64)
			{
				// 如果为1 那么除去最高位的值 就是函数的导出序号
				ULONGLONG Original = *(PULONGLONG)pIMAGE_THUNK_DATA64 & ~IMAGE_ORDINAL_FLAG64;

				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"IAT[序号]: ");
				sprintf(szDATA, "%08X", Original);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}

			// 如果不是，那么这个值是一个RVA 指向 IMAGE_IMPORT_BY_NAME 
			else
			{
				PIMAGE_IMPORT_BY_NAME pIMAGE_IMPORT_BY_NAME  = (IMAGE_IMPORT_BY_NAME*)(ChangeRvaToFoa64(*(LPDWORD)pIMAGE_THUNK_DATA64, \
					g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"IAT[名称]: ");
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pIMAGE_IMPORT_BY_NAME -> Name);
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
			}

			// 找到下一个地址
			pIMAGE_THUNK_DATA64 += 8;
		}

		// 找到下一个模块
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);

	return RETURN_SUCCESS; 
}


//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsResourceOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析32位的PE文件的资源表并将其内容显示在IDC_EDIT_DS_ALL上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 首先判断是否有资源表
	if (!(pIMAGE_DATA_DIRECTORY + 2) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有资源表");
		return RETURN_FAILURE;
	}

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 定义资源表结构体头部指针
	PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY = (IMAGE_RESOURCE_DIRECTORY*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 2) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"资源表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 找到有多少个不同的资源类型
	DWORD dwNumberOfTypes = pIMAGE_RESOURCE_DIRECTORY -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY -> NumberOfNamedEntries;

	// 定义资源区段指针
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY + 1);
	do
	{ 
		// 遍历有多少种不同的资源类型
		if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameIsString)
		{
			// 如果高位为0 则此4个字节的后31位就是 资源类型的ID
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------ \r\n资源类型(ID):");
			sprintf(szDATA, "%08X", pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
			switch(pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset)
			{

			case RT_CURSOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(光标)\r\n");
					break;
				}
			case RT_BITMAP:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(位图)\r\n");
					break;
				}
			case RT_ICON:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(图标)\r\n");
					break;
				}
			case RT_MENU:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(菜单)\r\n");
					break;
				}
			case RT_DIALOG:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(对话框)\r\n");
					break;
				}
			case RT_STRING:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(字符串组)\r\n");
					break;
				}
			case RT_FONTDIR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(字体目录)\r\n");
					break;
				}
			case RT_FONT:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(字体)\r\n");
					break;
				}
			case RT_ACCELERATOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(加速器表)\r\n");
					break;
				}
			case RT_RCDATA:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(自定义资源)\r\n");
					break;
				}
			case RT_MESSAGETABLE:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(消息表)\r\n");
					break;
				}
			case RT_GROUP_CURSOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(图标组)\r\n");
					break;
				}
			case RT_GROUP_ICON:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(光标组)\r\n");
					break;
				}
			case RT_DLGINCLUDE:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(获取资源)\r\n");
					break;
				}
			case RT_VERSION:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(版本信息)\r\n");
					break;
				}
			case RT_PLUGPLAY:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(即插即用资源)\r\n");
					break;
				}
			case RT_VXD:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(VXD)\r\n");
					break;
				}
			case RT_ANICURSOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(动态光标)\r\n");
					break;
				}
			case RT_ANIICON:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(动态图标)\r\n");
					break;
				}
			case RT_HTML:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(HTML文档)\r\n");
					break;
				}
			default:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(未知资源类型)\r\n");
				}
			};
		}
		else
		{
			// 如果高位为1 则指向 _IMAGE_RESOURCE_DIR_STRING_U 结构 资源类型就是自定义的 是一个字符串
			PIMAGE_RESOURCE_DIR_STRING_U pIMAGE_RESOURCE_DIR_STRING_U = (IMAGE_RESOURCE_DIR_STRING_U*)(pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset + (DWORD)pIMAGE_RESOURCE_DIRECTORY);

			// 资源类型的字符串长度
			DWORD IMAGE_RESOURCE_DIR_STRING_U_Length = pIMAGE_RESOURCE_DIR_STRING_U -> Length;

			// 资源类型的起始位置
			PTCHAR StringOfResource = (PTCHAR)(&pIMAGE_RESOURCE_DIR_STRING_U -> NameString);

			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"资源类型(名称): ");
			do
			{
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)StringOfResource);
				StringOfResource += 2;
			}
			while(--IMAGE_RESOURCE_DIR_STRING_U_Length);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
		}
		
		// 开始遍历下一层(编号)
		if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY -> DataIsDirectory)
		{
			// 如果高位是0
			// 这里一定不是0
		}
		else
		{
			// 如果高位是1 找到下一层目录节点的起始位置 (编号的起始地址)
			PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY_Digit = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY -> OffsetToDirectory);
			
			// 编号数量
			DWORD dwNumberOfDigit = pIMAGE_RESOURCE_DIRECTORY_Digit -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY_Digit -> NumberOfNamedEntries;

			// 找到第二层的资源编号区段
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY_Digit + 1);
			do
			{
				if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameIsString)
				{
					// 如果高位为0 则此4个字节的后31位就是 资源编号
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"资源编号:");
					sprintf(szDATA, "%08X", pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameOffset);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
				}	
				else
				{
					// 如果高位为1 则指向 _IMAGE_RESOURCE_DIR_STRING_U 结构 资源编号就是自定义的 是一个字符串
					PIMAGE_RESOURCE_DIR_STRING_U pIMAGE_RESOURCE_DIR_STRING_U = (IMAGE_RESOURCE_DIR_STRING_U*)(pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameOffset + (DWORD)pIMAGE_RESOURCE_DIRECTORY);

					// 资源类型的字符串长度
					DWORD IMAGE_RESOURCE_DIR_STRING_U_Length = pIMAGE_RESOURCE_DIR_STRING_U -> Length;

					// 资源类型的起始位置
					PTCHAR StringOfResource = (PTCHAR)(&pIMAGE_RESOURCE_DIR_STRING_U -> NameString);

					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"资源编号(名称): ");
					do
					{
						SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)StringOfResource);
						StringOfResource += 2;
					}
					while(--IMAGE_RESOURCE_DIR_STRING_U_Length);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
				}

				// 找到代码页
				PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY_CodePage = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> OffsetToDirectory);

				// 代码页数量
				DWORD dwNumberOfCodePage = pIMAGE_RESOURCE_DIRECTORY_CodePage -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY_CodePage -> NumberOfNamedEntries;
				// 找到第三层的代码页区段
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY_CodePage + 1);
				do
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"代码页:");
					sprintf(szDATA, "%08X", pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> NameOffset);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					switch (pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> NameOffset)
					{
					case 1033:
						{
							SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(英文)\r\n");
							break;
						}
					case 2052:
						{
							SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(中文)\r\n");
							break;
						}
					default:
						{
							SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(其它语言)\r\n");
						}
					};
					// 找到节点
					PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> OffsetToDirectory);
					
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Rva: ");
					sprintf(szDATA, "%08X", pIMAGE_DATA_DIRECTORY -> VirtualAddress);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"OffSet: ");
					sprintf(szDATA, "%08X", ChangeRvaToFoa32(pIMAGE_DATA_DIRECTORY -> VirtualAddress, g_pStTLS -> m_fpFileBuffer));
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Size: ");
					sprintf(szDATA, "%08X", pIMAGE_DATA_DIRECTORY -> Size);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n\r\n");
					
					pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage++;
				}
				while(--dwNumberOfCodePage);

				// 找到下一个资源编号的起始位置
				pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element++;
			}
			while(--dwNumberOfDigit);
		}
		// 找到下一个资源类型的起始位置
		pIMAGE_RESOURCE_DIRECTORY_ENTRY++;
	}
	while(--dwNumberOfTypes);

	return RETURN_SUCCESS; 
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsResourceOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析64位的PE文件的资源表并将其内容显示在IDC_EDIT_DS_ALL上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER64 -> DataDirectory);

	// 首先判断是否有资源表
	if (!(pIMAGE_DATA_DIRECTORY + 2) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有资源表");
		return RETURN_FAILURE;
	}

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 定义资源表结构体头部指针
	PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY = (IMAGE_RESOURCE_DIRECTORY*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 2) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"资源表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 找到有多少个不同的资源类型
	DWORD dwNumberOfTypes = pIMAGE_RESOURCE_DIRECTORY -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY -> NumberOfNamedEntries;

	// 定义资源区段指针
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY + 1);
	do
	{ 
		// 遍历有多少种不同的资源类型
		if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameIsString)
		{
			// 如果高位为0 则此4个字节的后31位就是 资源类型的ID
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------ \r\n资源类型(ID):");
			sprintf(szDATA, "%08X", pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t");
			switch(pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset)
			{

			case RT_CURSOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(光标)\r\n");
					break;
				}
			case RT_BITMAP:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(位图)\r\n");
					break;
				}
			case RT_ICON:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(图标)\r\n");
					break;
				}
			case RT_MENU:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(菜单)\r\n");
					break;
				}
			case RT_DIALOG:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(对话框)\r\n");
					break;
				}
			case RT_STRING:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(字符串组)\r\n");
					break;
				}
			case RT_FONTDIR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(字体目录)\r\n");
					break;
				}
			case RT_FONT:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(字体)\r\n");
					break;
				}
			case RT_ACCELERATOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(加速器表)\r\n");
					break;
				}
			case RT_RCDATA:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(自定义资源)\r\n");
					break;
				}
			case RT_MESSAGETABLE:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(消息表)\r\n");
					break;
				}
			case RT_GROUP_CURSOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(图标组)\r\n");
					break;
				}
			case RT_GROUP_ICON:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(光标组)\r\n");
					break;
				}
			case RT_DLGINCLUDE:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(获取资源)\r\n");
					break;
				}
			case RT_VERSION:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(版本信息)\r\n");
					break;
				}
			case RT_PLUGPLAY:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(即插即用资源)\r\n");
					break;
				}
			case RT_VXD:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(VXD)\r\n");
					break;
				}
			case RT_ANICURSOR:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(动态光标)\r\n");
					break;
				}
			case RT_ANIICON:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(动态图标)\r\n");
					break;
				}
			case RT_HTML:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(HTML文档)\r\n");
					break;
				}
			default:
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(未知资源类型)\r\n");
				}
			};
		}
		else
		{
			// 如果高位为1 则指向 _IMAGE_RESOURCE_DIR_STRING_U 结构 资源类型就是自定义的 是一个字符串
			PIMAGE_RESOURCE_DIR_STRING_U pIMAGE_RESOURCE_DIR_STRING_U = (IMAGE_RESOURCE_DIR_STRING_U*)(pIMAGE_RESOURCE_DIRECTORY_ENTRY -> NameOffset + (DWORD)pIMAGE_RESOURCE_DIRECTORY);

			// 资源类型的字符串长度
			DWORD IMAGE_RESOURCE_DIR_STRING_U_Length = pIMAGE_RESOURCE_DIR_STRING_U -> Length;

			// 资源类型的起始位置
			PTCHAR StringOfResource = (PTCHAR)(&pIMAGE_RESOURCE_DIR_STRING_U -> NameString);

			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"资源类型(名称): ");
			do
			{
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)StringOfResource);
				StringOfResource += 2;
			}
			while(--IMAGE_RESOURCE_DIR_STRING_U_Length);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
		}
		
		// 开始遍历下一层(编号)
		if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY -> DataIsDirectory)
		{
			// 如果高位是0
			// 这里一定不是0
		}
		else
		{
			// 如果高位是1 找到下一层目录节点的起始位置 (编号的起始地址)
			PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY_Digit = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY -> OffsetToDirectory);
			
			// 编号数量
			DWORD dwNumberOfDigit = pIMAGE_RESOURCE_DIRECTORY_Digit -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY_Digit -> NumberOfNamedEntries;

			// 找到第二层的资源编号区段
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY_Digit + 1);
			do
			{
				if (!pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameIsString)
				{
					// 如果高位为0 则此4个字节的后31位就是 资源编号
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"资源编号:");
					sprintf(szDATA, "%08X", pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameOffset);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
				}	
				else
				{
					// 如果高位为1 则指向 _IMAGE_RESOURCE_DIR_STRING_U 结构 资源编号就是自定义的 是一个字符串
					PIMAGE_RESOURCE_DIR_STRING_U pIMAGE_RESOURCE_DIR_STRING_U = (IMAGE_RESOURCE_DIR_STRING_U*)(pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> NameOffset + (DWORD)pIMAGE_RESOURCE_DIRECTORY);

					// 资源类型的字符串长度
					DWORD IMAGE_RESOURCE_DIR_STRING_U_Length = pIMAGE_RESOURCE_DIR_STRING_U -> Length;

					// 资源类型的起始位置
					PTCHAR StringOfResource = (PTCHAR)(&pIMAGE_RESOURCE_DIR_STRING_U -> NameString);

					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"资源编号(名称): ");
					do
					{
						SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)StringOfResource);
						StringOfResource += 2;
					}
					while(--IMAGE_RESOURCE_DIR_STRING_U_Length);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");
				}

				// 找到代码页
				PIMAGE_RESOURCE_DIRECTORY pIMAGE_RESOURCE_DIRECTORY_CodePage = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element -> OffsetToDirectory);

				// 代码页数量
				DWORD dwNumberOfCodePage = pIMAGE_RESOURCE_DIRECTORY_CodePage -> NumberOfIdEntries + pIMAGE_RESOURCE_DIRECTORY_CodePage -> NumberOfNamedEntries;
				// 找到第三层的代码页区段
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pIMAGE_RESOURCE_DIRECTORY_CodePage + 1);
				do
				{
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"代码页:");
					sprintf(szDATA, "%08X", pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> NameOffset);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					switch (pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> NameOffset)
					{
					case 1033:
						{
							SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(英文)\r\n");
							break;
						}
					case 2052:
						{
							SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(中文)\r\n");
							break;
						}
					default:
						{
							SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"(其它语言)\r\n");
						}
					};
					// 找到节点
					PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)((DWORD)pIMAGE_RESOURCE_DIRECTORY + pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage -> OffsetToDirectory);
					
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Rva: ");
					sprintf(szDATA, "%08X", pIMAGE_DATA_DIRECTORY -> VirtualAddress);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"OffSet: ");
					sprintf(szDATA, "%08X", ChangeRvaToFoa32(pIMAGE_DATA_DIRECTORY -> VirtualAddress, g_pStTLS -> m_fpFileBuffer));
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Size: ");
					sprintf(szDATA, "%08X", pIMAGE_DATA_DIRECTORY -> Size);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
					SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n\r\n");
					
					pIMAGE_RESOURCE_DIRECTORY_ENTRY_CodePage++;
				}
				while(--dwNumberOfCodePage);

				// 找到下一个资源编号的起始位置
				pIMAGE_RESOURCE_DIRECTORY_ENTRY_Digit_Element++;
			}
			while(--dwNumberOfDigit);
		}
		// 找到下一个资源类型的起始位置
		pIMAGE_RESOURCE_DIRECTORY_ENTRY++;
	}
	while(--dwNumberOfTypes);

	return RETURN_SUCCESS; 
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBaseRelocOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析32位的PE文件的重定位表并将其内容显示在IDC_EDIT_DS_ALL上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 首先判断是否有重定位表
	if (!(pIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有重定位表");
		return RETURN_FAILURE;
	}

	// 定义重定位表结构体指针
	PIMAGE_BASE_RELOCATION pIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"重定位表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" ------------------------------------------------\r\n");

	// 判断重定位表有没有数据
	if (!pIMAGE_BASE_RELOCATION -> VirtualAddress || !pIMAGE_BASE_RELOCATION -> SizeOfBlock)
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"此PE文件重定位表里没有值!");
		return RETURN_FAILURE;
	}

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 用于判断项目数循环次数
	DWORD dwTempNumber = 0;
	DWORD dwCount = 0;

	// 开始打印重定位表
	do
	{
		// 标题
		dwTempNumber = (pIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(WORD);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"VirtualAddress[页RVA]: \r\t");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"SizeOfBlock[块大小]: \r\t");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Items[项目数]: \r\t\r\n");

		// VirtualAddress
		sprintf(szDATA, "%08X", pIMAGE_BASE_RELOCATION -> VirtualAddress);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\t");
		
		// SizeOfBlock
		sprintf(szDATA, "%08X", pIMAGE_BASE_RELOCATION -> SizeOfBlock);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\t");
		
		// Items
		sprintf(szDATA, "%08X", (pIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(WORD));
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\n");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"块内数据[RVA]: \r\n");
		do
		{
			// 块内数据RVA
			sprintf(szDATA, "%08X", GET_OVER_LOBYTE(*(LPWORD)((DWORD)(pIMAGE_BASE_RELOCATION) + \
				IMAGE_SIZEOF_BASE_RELOCATION + dwCount++ * sizeof(WORD))) + pIMAGE_BASE_RELOCATION -> VirtualAddress);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

			// 判断是否到达重定位表末尾
			if (!*(LPWORD)((DWORD)(pIMAGE_BASE_RELOCATION) + IMAGE_SIZEOF_BASE_RELOCATION + dwCount * sizeof(WORD)))
			{
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"00000000[结束符]\r\n");
				break;
			}
		}
		while(--dwTempNumber);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" ------------------------------------------------\r\n");

		// 初始化dwCount的值
		dwCount = 0;

		// 跳过当前块的全部地址
		pIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pIMAGE_BASE_RELOCATION + pIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}
	// 当下一个块的VirtualAddress和SizeOfBlock均不为零才继续执行
	while(pIMAGE_BASE_RELOCATION -> VirtualAddress || pIMAGE_BASE_RELOCATION -> SizeOfBlock);
	
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBaseRelocOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析64位的PE文件的重定位表并将其内容显示在IDC_EDIT_DS_ALL上
	*/

	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER64 -> DataDirectory);

	// 首先判断是否有重定位表
	if (!(pIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有重定位表");
		return RETURN_FAILURE;
	}

	// 定义重定位表结构体指针
	PIMAGE_BASE_RELOCATION pIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)(ChangeRvaToFoa64((pIMAGE_DATA_DIRECTORY + 5) -> VirtualAddress, g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"重定位表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" ------------------------------------------------\r\n");

	// 判断重定位表有没有数据
	if (!pIMAGE_BASE_RELOCATION -> VirtualAddress || !pIMAGE_BASE_RELOCATION -> SizeOfBlock)
	{
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"此PE文件重定位表里没有值!");
		return RETURN_FAILURE;
	}

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 用于判断项目数循环次数
	DWORD dwTempNumber = 0;
	DWORD dwCount = 0;

	// 开始打印重定位表
	do
	{
		// 标题
		dwTempNumber = (pIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(WORD);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"VirtualAddress[页RVA]: \r\t");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"SizeOfBlock[块大小]: \r\t");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Items[项目数]: \r\n");

		// VirtualAddress
		sprintf(szDATA, "%08X", pIMAGE_BASE_RELOCATION -> VirtualAddress);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\t");
		
		// SizeOfBlock
		sprintf(szDATA, "%08X", pIMAGE_BASE_RELOCATION -> SizeOfBlock);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\t\r\t");
		
		// Items
		sprintf(szDATA, "%08X", (pIMAGE_BASE_RELOCATION -> SizeOfBlock - IMAGE_SIZEOF_BASE_RELOCATION) / sizeof(WORD));
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"块内数据[RVA]: \r\n");

		do
		{
			// 块内数据RVA
			sprintf(szDATA, "%08X", GET_OVER_LOBYTE(*(LPWORD)((DWORD)(pIMAGE_BASE_RELOCATION) + \
				IMAGE_SIZEOF_BASE_RELOCATION + dwCount++ * sizeof(WORD))) + pIMAGE_BASE_RELOCATION -> VirtualAddress);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
			SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" \r\n");

			// 判断是否到达重定位表末尾
			if (!*(LPWORD)((DWORD)(pIMAGE_BASE_RELOCATION) + IMAGE_SIZEOF_BASE_RELOCATION + dwCount * sizeof(WORD)))
			{
				SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"00000000[结束符]\r\n");
				break;
			}
		}
		while(--dwTempNumber);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"------------------------------------------------ \r\n");

		// 初始化dwCount的值
		dwCount = 0;

		// 跳过当前块的全部地址
		pIMAGE_BASE_RELOCATION = (IMAGE_BASE_RELOCATION*)((DWORD)pIMAGE_BASE_RELOCATION + pIMAGE_BASE_RELOCATION -> SizeOfBlock);
	}
	// 当下一个块的VirtualAddress和SizeOfBlock均不为零才继续执行
	while(pIMAGE_BASE_RELOCATION -> VirtualAddress || pIMAGE_BASE_RELOCATION -> SizeOfBlock);
	
	return RETURN_SUCCESS;
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBoundImportOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析32位的PE文件的绑定导入表并将其内容显示在IDC_EDIT_DS_ALL上
	*/
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS32 pIMAGE_NT_HEADERS32 = (IMAGE_NT_HEADERS32*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS32 + sizeof(pIMAGE_NT_HEADERS32 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER32 pIMAGE_OPTIONAL_HEADER32 = (IMAGE_OPTIONAL_HEADER32*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER32 -> DataDirectory);

	// 首先判断是否有绑定导入表
	if (!(pIMAGE_DATA_DIRECTORY + 11) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有绑定导入表");
		return RETURN_FAILURE;
	}

	// 定义导入表结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 定义绑定导入表结构体指针
	PIMAGE_BOUND_IMPORT_DESCRIPTOR pIMAGE_BOUND_IMPORT_DESCRIPTOR = (IMAGE_BOUND_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa32((pIMAGE_DATA_DIRECTORY + 11) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 绑定导入表中的第一个结构的 DESCRIPTOR 加上 OffsetModuleName 可以得到模块名
	DWORD dwNameBase = (DWORD)pIMAGE_BOUND_IMPORT_DESCRIPTOR;

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"绑定导入表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" ------------------------------------------------\r\n");

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 打印第一个绑定导入表中的其它值
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstTimeDateStamp: ");
	sprintf(szDATA, "%08X", pIMAGE_BOUND_IMPORT_DESCRIPTOR -> TimeDateStamp);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstModuleName: ");
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pIMAGE_BOUND_IMPORT_DESCRIPTOR -> OffsetModuleName + dwNameBase);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	// 打印第一个绑定导入表中 NumberOfModuleForwarderRefs 里记录的剩余表数量 这个值可能不准(可能为0)
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstNumberOfModuleForwarderRefs: ");
	sprintf(szDATA, "%08X", pIMAGE_BOUND_IMPORT_DESCRIPTOR -> NumberOfModuleForwarderRefs);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	// 定义绑定导入表的 _IMAGE_BOUND_FORWARDER_REF 结构
	PIMAGE_BOUND_FORWARDER_REF pIMAGE_BOUND_FORWARDER_REF = (IMAGE_BOUND_FORWARDER_REF*)++pIMAGE_BOUND_IMPORT_DESCRIPTOR;

	do
	{
		// 如果有一个这样的结构体的值全为0则跳出
		if (!(pIMAGE_BOUND_FORWARDER_REF ->TimeDateStamp  | \
			pIMAGE_BOUND_FORWARDER_REF -> OffsetModuleName | \
			pIMAGE_BOUND_FORWARDER_REF -> Reserved))
			break;

		// 判断是否此模块有绑定导入表
		if (!pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp)
		{
			// 如果此模块没有绑定导入表则跳过
			pIMAGE_IMPORT_DESCRIPTOR++;

			// 转到下一个绑定导入表
			pIMAGE_BOUND_FORWARDER_REF++;

			// 没有绑定导入表就跳转至下一个
			continue;
		}

		// 打印绑定导入表
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"TimeDateStamp: ");
		sprintf(szDATA, "%08X", pIMAGE_BOUND_FORWARDER_REF -> TimeDateStamp);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pIMAGE_BOUND_FORWARDER_REF -> OffsetModuleName + dwNameBase);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Reserved: ");
		sprintf(szDATA, "%08X", pIMAGE_BOUND_FORWARDER_REF -> Reserved);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n\r\n");

		// 转到下一个绑定导入表
		pIMAGE_BOUND_FORWARDER_REF++;

		// 转到下一个导入表
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);

	return RETURN_SUCCESS; 
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsBoundImportOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	/*
		解析64位的PE文件的绑定导入表并将其内容显示在IDC_EDIT_DS_ALL上
	*/
	
	// 定义DOS头结构体指针
	PIMAGE_DOS_HEADER pIMAGE_DOS_HEADER = (IMAGE_DOS_HEADER*)g_pStTLS -> m_fpFileBuffer;

	// 定义NT头结构体指针
	PIMAGE_NT_HEADERS64 pIMAGE_NT_HEADERS64 = (IMAGE_NT_HEADERS64*)((DWORD)pIMAGE_DOS_HEADER + pIMAGE_DOS_HEADER -> e_lfanew);

	// 定义标准PE头结构体指针
	PIMAGE_FILE_HEADER pIMAGE_FILE_HEADER = (IMAGE_FILE_HEADER*)((DWORD)pIMAGE_NT_HEADERS64 + sizeof(pIMAGE_NT_HEADERS64 -> Signature));

	// 定义可选PE头结构体指针
	PIMAGE_OPTIONAL_HEADER64 pIMAGE_OPTIONAL_HEADER64 = (IMAGE_OPTIONAL_HEADER64*)((DWORD)pIMAGE_FILE_HEADER + IMAGE_SIZEOF_FILE_HEADER); 

	// 定义数据目录结构体指针
	PIMAGE_DATA_DIRECTORY pIMAGE_DATA_DIRECTORY = (IMAGE_DATA_DIRECTORY*)(pIMAGE_OPTIONAL_HEADER64 -> DataDirectory);

	// 首先判断是否有绑定导入表
	if (!(pIMAGE_DATA_DIRECTORY + 11) -> VirtualAddress)
	{
		SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"此PE文件没有绑定导入表");
		return RETURN_FAILURE;
	}

	// 定义导入表结构体指针
	PIMAGE_IMPORT_DESCRIPTOR pIMAGE_IMPORT_DESCRIPTOR = (IMAGE_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa64((pIMAGE_DATA_DIRECTORY + 1) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 定义绑定导入表结构体指针
	PIMAGE_BOUND_IMPORT_DESCRIPTOR pIMAGE_BOUND_IMPORT_DESCRIPTOR = (IMAGE_BOUND_IMPORT_DESCRIPTOR*)(ChangeRvaToFoa64((pIMAGE_DATA_DIRECTORY + 11) -> VirtualAddress, \
		g_pStTLS -> m_fpFileBuffer) + (DWORD)g_pStTLS -> m_fpFileBuffer);
	
	// 绑定导入表中的第一个结构的 DESCRIPTOR 加上 OffsetModuleName 可以得到模块名
	DWORD dwNameBase = (DWORD)pIMAGE_BOUND_IMPORT_DESCRIPTOR;

	// 覆盖写入
	SendMessage(hwndEdit, WM_SETTEXT, 0, (LPARAM)"绑定导入表信息(当前版本数据过多将显示不全):\r\n");

	// 设置为末尾追加的模式
	SendMessage(hwndEdit, EM_SETSEL, -2, -1);

	// 分隔符
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)" ------------------------------------------------\r\n");

	// 用于存储转换后得到的字符串
	TCHAR szDATA[0x10] = {0};

	// 打印第一个绑定导入表中的其它值
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstTimeDateStamp: ");
	sprintf(szDATA, "%08X", pIMAGE_BOUND_IMPORT_DESCRIPTOR -> TimeDateStamp);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstModuleName: ");
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pIMAGE_BOUND_IMPORT_DESCRIPTOR -> OffsetModuleName + dwNameBase);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	// 打印第一个绑定导入表中 NumberOfModuleForwarderRefs 里记录的剩余表数量 这个值可能不准(可能为0)
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"FirstNumberOfModuleForwarderRefs: ");
	sprintf(szDATA, "%08X", pIMAGE_BOUND_IMPORT_DESCRIPTOR -> NumberOfModuleForwarderRefs);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

	// 定义绑定导入表的 _IMAGE_BOUND_FORWARDER_REF 结构
	PIMAGE_BOUND_FORWARDER_REF pIMAGE_BOUND_FORWARDER_REF = (IMAGE_BOUND_FORWARDER_REF*)++pIMAGE_BOUND_IMPORT_DESCRIPTOR;

	do
	{
		// 如果有一个这样的结构体的值全为0则跳出
		if (!(pIMAGE_BOUND_FORWARDER_REF ->TimeDateStamp  | \
			pIMAGE_BOUND_FORWARDER_REF -> OffsetModuleName | \
			pIMAGE_BOUND_FORWARDER_REF -> Reserved))
			break;

		// 判断是否此模块有绑定导入表
		if (!pIMAGE_IMPORT_DESCRIPTOR -> TimeDateStamp)
		{
			// 如果此模块没有绑定导入表则跳过
			pIMAGE_IMPORT_DESCRIPTOR++;

			// 转到下一个绑定导入表
			pIMAGE_BOUND_FORWARDER_REF++;

			// 没有绑定导入表就跳转至下一个
			continue;
		}

		// 打印绑定导入表
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"TimeDateStamp: ");
		sprintf(szDATA, "%08X", pIMAGE_BOUND_FORWARDER_REF -> TimeDateStamp);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)pIMAGE_BOUND_FORWARDER_REF -> OffsetModuleName + dwNameBase);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n");

		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"Reserved: ");
		sprintf(szDATA, "%08X", pIMAGE_BOUND_FORWARDER_REF -> Reserved);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)szDATA);
		SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"\r\n\r\n");

		// 转到下一个绑定导入表
		pIMAGE_BOUND_FORWARDER_REF++;

		// 转到下一个导入表
		pIMAGE_IMPORT_DESCRIPTOR++;
	}
	while(TRUE);

	return RETURN_SUCCESS; 
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsIATOfPEData32(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"请在导入表中查看!");
	return RETURN_SUCCESS; 
}

//////////////////////////////////////////////////////////////////////

DWORD WINAPI AnalysisDsIATOfPEData64(IN HWND hwndEdit, IN PTLSDirectory g_pStTLS)
{
	SendMessage(hwndEdit, EM_REPLACESEL, 1, (LPARAM)"请在导入表中查看!");
	return RETURN_SUCCESS;  
}