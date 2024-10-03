// Win32Api_2.cpp : Defines the entry point for the application.
//

/*
	DWORD Erro = ::GetLastError();
	DbgPrintf("%d\n", Erro);
*/

#include "stdafx.h"

/*
	练习：
	1、 创建一个窗口程序，学习如何查询文档
	2、 查一下 Windows 有多少种消息，概要了解一下每个消息的作用.
	3、 NDCLASS wndclass = {0};
		WNDCLASS wndclass;	(区别是什么)

	// 2、 Windows 有 400 多种消息
	// 3、 区别是：值没有被初始化
*/

LRESULT CALLBACK WindowProc(
						IN  HWND hwnd,					// 窗口的句柄
						IN  UINT uMsg,					// 消息标识符
						IN  WPARAM wParam,				// 第一个消息参数
						IN  LPARAM lParam				// 第二个消息参数
						)
{
	switch(uMsg)
	{

	case WM_DESTROY:
		{
			// 点击 (×) 按钮
			DbgPrintf("WM_DESTROY %d %d\n", wParam, lParam);
			PostQuitMessage(0);

			return 0;
		}
	case WM_MOVE:
		{
			// 窗口移动
			DWORD MOVE_xPos = (DWORD) LOWORD(lParam);   // horizontal position 
			DWORD MOVE_yPos = (DWORD) HIWORD(lParam);   // vertical position
			DbgPrintf(TEXT("MOVE_xPos:%d\nMOVE_yPos:%d\n"), MOVE_xPos, MOVE_yPos);

			return 0;
		}
	case WM_MOUSEMOVE:
		{
			// 鼠标移动
			DWORD MOUSE_xPos = (DWORD) LOWORD(lParam);   // horizontal position 
			DWORD MOUSE_yPos = (DWORD) HIWORD(lParam);   // vertical position
			DbgPrintf(TEXT("MOUSE_xPos:%d\nMOUSE_yPos:%d\n"), MOUSE_xPos, MOUSE_yPos);

			return 0;
		}
	}

	// 将自己不需要关注的消息返还给操作系统
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	// 创建窗口类的对象
	// 一定要先将所有成员(10个)都赋值
	WNDCLASS wndclass;
	wndclass.style = NULL;
	wndclass.lpfnWndProc = WindowProc;						// 窗口过程函数
	wndclass.cbClsExtra = NULL;
	wndclass.cbWndExtra = NULL;
	wndclass.hInstance = hInstance;							// 定义窗口类的应用程序的实例句柄
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)COLOR_MENUTEXT;		// 窗口的背景色
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("My_First_Window");		// 窗口类的名字
	

	RegisterClass(&wndclass);								// 将窗口注册进系统

	// 创建窗口
	HWND hwnd = CreateWindow( 
	TEXT("My_First_Window"),								// 窗口类的名字
	TEXT("我的第一个窗口"),									// 窗口标题
	WS_SYSMENU | WS_MAXIMIZEBOX |\
	WS_MINIMIZEBOX | WS_THICKFRAME,												// 窗口外观样式
	20,														// 相对于父窗口的X坐标
	20,														// 相对于父窗口的Y坐标
	800,													// 窗口的宽度
	800,													// 窗口的高度
	NULL,													// 父窗口句柄,为 NULL
	NULL,													// 菜单句柄,为 NULL
	hInstance,												// 当前应用程序的句柄
	NULL);													// 附加数据一般为NULL

	if(!hwnd)												// 是否创建成功
		return 0;

	// 显示窗口
	ShowWindow(hwnd, SW_SHOW);  

	// 消息循环
	MSG msg = {0};  			
	while(GetMessage(&msg, NULL, 0, 0))
	{  			
		TranslateMessage(&msg);								// 翻译消息(加工消息)
		DispatchMessage(&msg);								// 发回给操作系统
	}

	return 0;
}
