// Win32Api_2.cpp : Defines the entry point for the application.
//

/*
	DWORD Erro = ::GetLastError();
	DbgPrintf("%d\n", Erro);
*/

#include "stdafx.h"

/*
1、Win32应用程序入口识别
2、ESP寻址的特点
3、窗口回调函数的定位
4、具体事件的处理的定位
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
	case WM_SIZE:
		{
			/*
			SIZE_MAXHIDE		4		当其他一些窗口被最大化时，消息被发送到所有的弹出窗口。
			SIZE_MAXIMIZED		2		窗口最大化
			SIZE_MAXSHOW		3		当其他一些窗口恢复到以前的尺寸时，消息被发送到所有弹出式窗口。
			SIZE_MINIMIZED		1		窗口最小化
			SIZE_RESTORED		0		窗口大小变化 但不是最大化或最小化
			*/

			// 改变后的窗口的宽度和高度
			DWORD SIZE_xPos = (DWORD) LOWORD(lParam);   // 新宽度
			DWORD SIZE_yPos = (DWORD) HIWORD(lParam);   // 新高度

			if (!wParam)
			{
				DbgPrintf(TEXT("窗口大小改变了!\n新宽度:%d\n新高度:%d\n"), SIZE_xPos, SIZE_yPos);
				return 0;
			}
			if (wParam == 1)
			{
				DbgPrintf(TEXT("窗口最小化了!\n"));
				return 0;
			}
			if (wParam == 2)
			{
				DbgPrintf(TEXT("窗口最大化了!\n"));
				return 0;
			}
			if (wParam == 3)
			{
				DbgPrintf(TEXT("其他一些窗口恢复到以前的尺寸了!\n"));
				return 0;
			}
			if (wParam == 4)
			{
				DbgPrintf(TEXT("当其他一些窗口被最大化了!\n"));
				return 0;
			}

			return 0;
		}
	case WM_CREATE:
		{
			// 当应用程序通过调用 CreateWindowEx 或 CreateWindow 函数请求创建一个窗口时，会发送 WM_CREATE 消息
			// wParam 弃用
			// lParam 指向CREATESTRUCT结构的指针，该结构包含关于正在创建的窗口的信息。
			LPCREATESTRUCT plParam = (CREATESTRUCT*)lParam;
			DbgPrintf(TEXT("x: %d\n"), plParam -> x);
			DbgPrintf(TEXT("y: %d\n"), plParam -> y);
			DbgPrintf(TEXT("cx: %d\n"), plParam -> cx);
			DbgPrintf(TEXT("cy: %d\n"), plParam -> cy);

			return 0;
		}
	case WM_DESTROY:
		{
			// 点击 (×) 按钮
			DbgPrintf(TEXT("WM_DESTROY %d %d\n"), wParam, lParam);
			PostQuitMessage(0);

			return 0;
		}
	case WM_MOVE:
		{
			// 窗口移动
			DWORD MOVE_xPos = (DWORD) LOWORD(lParam);   // horizontal position 
			DWORD MOVE_yPos = (DWORD) HIWORD(lParam);   // vertical position
			// DbgPrintf(TEXT("MOVE_xPos:%d\nMOVE_yPos:%d\n"), MOVE_xPos, MOVE_yPos);

			return 0;
		}
	case WM_MOUSEMOVE:
		{
			// 鼠标移动
			DWORD MOUSE_xPos = (DWORD) LOWORD(lParam);   // horizontal position 
			DWORD MOUSE_yPos = (DWORD) HIWORD(lParam);   // vertical position
			// DbgPrintf(TEXT("MOUSE_xPos:%d\nMOUSE_yPos:%d\n"), MOUSE_xPos, MOUSE_yPos);

			return 0;
		}
	}

	// 将自己不需要关注的消息返还给操作系统
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,				// ImageBase
                     HINSTANCE hPrevInstance,			// 一般为 NULL
                     LPSTR     lpCmdLine,				// lpCmdLine 是在命令行中输入的值的指针
                     int       nCmdShow)				// 窗口显示方式
{
 	// TODO: Place code here.

	if (!*lpCmdLine)
	{
		// 如果是调试模式则这个指针指向的值是空
		char* a =  "请在命令行中输入:  \"Win32Api_3.exe [/t | /a | /s]\"  来指定模式";
		::MessageBox(0, a, 0, 0);
		// return 0;
	}
	else if (!*(lpCmdLine + 1))
	{
		::MessageBox(0, "需要在参数前加\"/\"", 0, 0);
		return 0;
	}
	else if (*(lpCmdLine + 1) == *("/t" + 1))
	{
		::MessageBox(0, "值是t", 0, 0);
	}
	else if (*(lpCmdLine + 1) == *("/a" + 1))
	{
		::MessageBox(0, "值是a", 0, 0);
	}
	else if (*(lpCmdLine + 1) == *("/s" + 1))
	{
		::MessageBox(0, "值是s", 0, 0);
	}
	else
	{
		::MessageBox(0, "用法:	Win32Api_3.exe [/t | /a | /s]", 0, 0);
		return 0;
	}

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
	{
		return 0;
	}	
		

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

