// 123321.cpp : Defines the entry point for the console application.
//

/*
// 难以阅读的HelloWorld
int main(int argc, char* argv[])
{
	int i = 0;
	unsigned int a = 4294941587;
	unsigned int b = 2375067792;
	unsigned int c = 2475924151;
	unsigned char* Text = (unsigned char*)&c;
A:
	*((char*)(Text + i)) = ~*((char*)(Text + i));
	if (i++ < 11) goto A;
	printf("%s\n", Text);
	system("pause"); 
	return 0;
}
*/

#include "stdafx.h"
#include <stdlib.h>
#include <Windows.h>

HWND g_hWeChatWindow = NULL;
HWND g_hEdit = NULL;
HMODULE hModuleHandle = NULL;

LRESULT CALLBACK WindowProc(
						IN  HWND hwnd,					// 窗口的句柄
						IN  UINT uMsg,					// 消息标识符
						IN  WPARAM wParam,				// 第一个消息参数
						IN  LPARAM lParam				// 第二个消息参数
						)
{
	switch(uMsg)
	{
	case WM_SIZE:
		{
			// 大小改变
			return 0;
		}
	case WM_CREATE:
		{
			// 创建编辑框
			return 0;
		}
	case WM_DESTROY:
		{
			// 点击 (×) 按钮
			PostQuitMessage(0);
			return 0;
		}
	case WM_MOVE:
		{
			// 窗口移动
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			// 鼠标移动
			return 0;
		}

	case WM_COMMAND:
		{
            // 编辑框的通知消息处理
            return 0;
        }
		break;
	}

	// 将自己不需要关注的消息返还给操作系统
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

int main(int argc, char* argv[])
{
	g_hWeChatWindow = FindWindowA("WeChatMainWndForPC", "微信");
	if (!g_hWeChatWindow)
	{
		return -1;
	}
	hModuleHandle = GetModuleHandleA(NULL);
	if(!hModuleHandle)
	{
		return -2;
	}
	// 创建窗口类的对象
	// 一定要先将所有成员(10个)都赋值
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS | CS_VREDRAW | CS_SAVEBITS;
	wndclass.lpfnWndProc = WindowProc;							// 窗口过程函数
	wndclass.cbClsExtra = NULL;
	wndclass.cbWndExtra = NULL;
	wndclass.hInstance = (HINSTANCE)hModuleHandle;				// 定义窗口类的应用程序的实例句柄
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)COLOR_MENUTEXT;			// 窗口的背景色
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = "ChildWindow";						// 窗口类的名字
	RegisterClass(&wndclass);									// 将窗口注册进系统
	// 创建窗口
	HWND hwnd = CreateWindowExA(
		0x02000000L,
		"ChildWindow",											// 窗口类的名字
		"子窗口",												// 窗口标题
		WS_CHILD | WS_VISIBLE | WS_CAPTION |\
		WS_SIZEBOX | WS_MAXIMIZEBOX |\
		WS_MINIMIZEBOX  | WS_SYSMENU | WS_CLIPCHILDREN, 		// 窗口样式
		100,													// 相对于父窗口的X坐标
		100,													// 相对于父窗口的Y坐标
		400,													// 窗口的宽度
		400,													// 窗口的高度
		g_hWeChatWindow,										// 父窗口句柄,为 NULL
		NULL,													// 菜单句柄,为 NULL
		(HINSTANCE)hModuleHandle,								// 当前应用程序的句柄
		NULL);													// 附加数据一般为NULL
	if(!hwnd)													// 是否创建成功
	{
		return 0;
	}
	// 创建编辑框
	g_hEdit = CreateWindowExA(
	WS_EX_TOPMOST,                          // 扩展样式
	"Edit",                     // 类名
	"编辑框",                   // 窗口标题（Edit控件没有标题）
	WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_MULTILINE | ES_WANTRETURN,
	10,                         // X坐标
	10,                         // Y坐标
	380,                        // 宽度
	300,                        // 高度
	(HWND)hwnd,					// 父窗口句柄
	(HMENU)NULL,                // 菜单句柄，如果没有可以为NULL
	(HINSTANCE)hModuleHandle,   // 应用程序实例句柄
	NULL);
	if (!g_hEdit)
	{
		MessageBox(hwnd, "无法创建编辑框！", "错误", MB_OK | MB_ICONERROR);
		return -1;
	}
	// 显示窗口;
	ShowWindow(hwnd, SW_SHOW);
	ShowWindow(g_hEdit, SW_SHOW);
	// 消息循环;
	MSG msg = { 0 };  			
	while(GetMessage(&msg, NULL, 0, 0))
	{  			
		TranslateMessage(&msg);									// 翻译消息(加工消息);
		DispatchMessage(&msg);									// 发回给操作系统;
	}

	return 0;
}
