#include "stdafx.h"
#include "Inject.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <wchar.h>
VOID setWindow(HWND thisWindow)
{
	HWND wechatWindow = FindWindow(L"WeChatMainWndForPC", NULL);
	//�ϣ�20 �£�620 ��10 �ң�720
	//MoveWindow(wechatWindow, 10, 20, 100, 600, TRUE);

	RECT wechatHandle = { 0 };
	GetWindowRect(wechatWindow, &wechatHandle);
	LONG width = wechatHandle.right - wechatHandle.left;
	LONG height = wechatHandle.bottom - wechatHandle.top;
	MoveWindow(thisWindow, wechatHandle.left - 230, wechatHandle.top, 240, height, TRUE);
	SetWindowPos(thisWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	wchar_t buff[0x100] = {};
	swprintf_s(buff, L"�ϣ�%d �£�%d ��%d �ң�%d\r\n", wechatHandle.top, wechatHandle.bottom, wechatHandle.left, wechatHandle.right);


	OutputDebugString(buff);
}