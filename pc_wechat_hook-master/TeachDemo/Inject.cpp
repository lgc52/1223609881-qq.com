#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <wchar.h>
#include <TlHelp32.h>
#include "Inject.h"
//ͨ���������Ʋ��ҽ���ID
DWORD ProcessNameToPID(LPCWSTR processName)
{
	wchar_t buffText[0x100] = {0};
	//�������̿���
	HANDLE ProcessAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);
	PROCESSENTRY32 processInfo = {0};
	processInfo.dwSize = sizeof(PROCESSENTRY32);
	do
	{
		if (wcscmp(processName, processInfo.szExeFile) == 0) {
			swprintf_s(buffText, L"��������=%s ����ID=%d \r\n", processInfo.szExeFile, processInfo.th32ProcessID);
			OutputDebugString(buffText);
			return processInfo.th32ProcessID;
		}
	} while (Process32Next(ProcessAll, &processInfo));
	
	return 0;
}

//ע��dll
VOID injectDll(char * dllPath)
{
	wchar_t buff[0x100] = {0};
	//��ȡĿ�����PID
	DWORD PID = ProcessNameToPID(INJECT_PROCESS_NAME);
	if (PID == 0) {
		MessageBox(NULL,L"û���ҵ��ý��̣�����Ϊ���������",L"û���ҵ�",MB_OK);
		return;
	}
	else {
		//�ҵ�pid���Ǿʹ򿪽���
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,TRUE, PID);
		if (NULL == hProcess) {
			MessageBox(NULL,L"���̴�ʧ��",L"����",MB_OK);
			return;
		}
		else {
			DWORD strSize = strlen(dllPath) * 2;
			//���̴򿪺����ǰ����ǵ�dll·�����ȥ
			//��������һƬ�ڴ����ڴ���dll·��
			LPVOID allocRes = VirtualAllocEx(hProcess,NULL, strSize, MEM_COMMIT, PAGE_READWRITE);
			if (NULL == allocRes) {
				MessageBox(NULL,L"�ڴ�����ʧ��",L"����",MB_OK);
				return;
			}

			//����ú� ����д��·����Ŀ���ڴ浱��
			if (WriteProcessMemory(hProcess, allocRes, dllPath, strSize, NULL) == 0) {
				MessageBox(NULL, L"DLL·��д��ʧ��", L"����", MB_OK);
				return;
			}
			//·��д�� �ɹ����������ڻ�ȡLoadLibraryW ��ַ
			//LoadLibraryW ��Kernel32.dll���� ���������Ȼ�ȡ���dll�Ļ�ַ
			HMODULE hModule = GetModuleHandle(L"Kernel32.dll");
			LPVOID address = GetProcAddress(hModule,"LoadLibraryA");
			swprintf_s(buff,L"loadLibrary=%p path=%p", address, allocRes);
			OutputDebugString(buff);
			//ͨ��Զ���߳�ִ��������� �������� ����dll�ĵ�ַ
			//��ʼע��dll
			HANDLE hRemote = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)address, allocRes,0, NULL);
			if (NULL == hRemote) {
				MessageBox(NULL, L"Զ��ִ��ʧ��", L"����", MB_OK);
				return;
			}
		}
	}
}

//��ȡ�ڴ�
VOID readMemory()
{
	DWORD PID = ProcessNameToPID(INJECT_PROCESS_NAME);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, PID);
	LPCVOID phoneAdd = (LPCVOID)0x10611E80;
	DWORD reSize = 0xB;
	char buff[0x100] = {0};
	wchar_t buffTest[0x100] = {0};
	ReadProcessMemory(hProcess, phoneAdd, buff, reSize,NULL);
	swprintf_s(buffTest,L"add=%p %s ", buff, buff);
	OutputDebugString(buffTest);
}


VOID setWindow(HWND thisWindow)
{
	HWND wechatWindow = FindWindow(L"WeChatMainWndForPC", NULL);
	//�ϣ�20 �£�620 ��10 �ң�720
	//MoveWindow(wechatWindow, 10, 20, 100, 600, TRUE);
	
	RECT wechatHandle = {0};
	GetWindowRect(wechatWindow, &wechatHandle);
	LONG width = wechatHandle.right - wechatHandle.left;
	LONG height = wechatHandle.bottom - wechatHandle.top;
	MoveWindow(thisWindow, wechatHandle.left - 230, wechatHandle.top, 240, height, TRUE);
	SetWindowPos(thisWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	wchar_t buff[0x100] = {};
	swprintf_s(buff,L"�ϣ�%d �£�%d ��%d �ң�%d\r\n", wechatHandle.top, wechatHandle.bottom, wechatHandle.left, wechatHandle.right);
	
	
	OutputDebugString(buff);
}

//����΢��
//CreateProcess ����Ŀ����� ����ʱ������ý���.
//Ȼ��ע��
//Ȼ����ResumeThread ��Ŀ���������
VOID runWechat(TCHAR * dllPath, TCHAR * wechatPath)
{
	//TCHAR dllPath[0x200] = {L"D://code//c//TeachDemo//Release//GetQrcode.dll"};
	//injectDll(paths);
	//TCHAR szDll[] = dllPath;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;//SW_SHOW
	//TCHAR szCommandLine[MAX_PATH] = TEXT("E:\\Program Files (x86)\\Tencent\\WeChat\\WeChat.exe");

	CreateProcess(NULL, wechatPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
	LPVOID Param = VirtualAllocEx(pi.hProcess, NULL, MAX_PATH, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	TCHAR add[0x100] = {0};
	
	WriteProcessMemory(pi.hProcess, Param, dllPath, wcslen(dllPath) * 2 + sizeof(char), NULL);
	//swprintf_s(add, L"��ַ=%p W=%p", Param, pi.hProcess);
	//MessageBox(NULL, add, L"aa", 0);
	//HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, Param, CREATE_SUSPENDED, NULL);

	TCHAR buff[0x100] = {0};
	HMODULE hModule = GetModuleHandle(L"Kernel32.dll");
	LPVOID address = GetProcAddress(hModule, "LoadLibraryW");
	//swprintf_s(buff, L"loadLibrary=%p path=%p", address, Param);
	//MessageBox(NULL,buff,L"aa",0);
	//OutputDebugString(buff);
	//ͨ��Զ���߳�ִ��������� �������� ����dll�ĵ�ַ
	//��ʼע��dll
	HANDLE hRemote = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)address, Param, 0, NULL);
	if (NULL == hRemote) {
		MessageBox(NULL, L"Զ��ִ��ʧ��", L"����", MB_OK);
		return;
	}

	ResumeThread(pi.hThread);
}