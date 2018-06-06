#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

using namespace std;

//��ȡ����id
DWORD GetProcessID(char *Filename)
{
	HANDLE hProcess;
	PROCESSENTRY32 pe;
	BOOL bRet;
	//���н��̿���
	hProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//��ʼ���̲���
	bRet = ::Process32First(hProcess, &pe);
	//ѭ���Ƚϣ��ó�processid
	while (bRet)
	{
		if (strcmp(Filename, pe.szExeFile) == 0)
			return pe.th32ProcessID;
		else
			bRet = ::Process32Next(hProcess, &pe);
	}
	return 99;
}

//ȡģ���ַ
HMODULE GetProcessModuleByName(DWORD pid, LPCSTR ModuleName)
{
	MODULEENTRY32 ModuleInfo;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, pid);
	if (!hSnapshot)
	{
		return 0;
	}
	ZeroMemory(&ModuleInfo, sizeof(MODULEENTRY32));
	ModuleInfo.dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(hSnapshot, &ModuleInfo))
	{
		return 0;
	}
	do 
	{
		if (!lstrcmpi(ModuleInfo.szModule, ModuleName))
		{
			CloseHandle(hSnapshot);
			return ModuleInfo.hModule;
		}
	} while (Module32Next(hSnapshot, &ModuleInfo));
	CloseHandle(hSnapshot);
	return 0;
}

/*
 * ��װ��д�ڴ溯��
 *
 */

//���ڴ�������
DWORD ReadMemory_Int(HANDLE Handle, LPVOID Address)
{
	DWORD Buffer = NULL;
	ReadProcessMemory(Handle, Address, &Buffer, sizeof(DWORD), NULL);
	return Buffer;
}
//���ڴ渡����
float ReadMemory_Float(HANDLE Handle, LPVOID Address)
{
	float Buffer = NULL;
	ReadProcessMemory(Handle, Address, &Buffer, sizeof(float), NULL);
	return Buffer;
}
//д�ڴ�������
BOOL WriteMemory_Int(HANDLE Handle, LPVOID Address, int Value)
{
	if (!WriteProcessMemory(Handle, Address, &Value, sizeof(int), NULL) == 0)
		return false;
	else
		return true;
}
//д�ڴ渡����
BOOL WriteMemory_Float(HANDLE Handle, LPVOID Address, float Value)
{
	if (!WriteProcessMemory(Handle, Address, &Value, sizeof(float), NULL) == 0)
		return false;
	else
		return true;
}

//�ϵ��ӽ�
void GodHigh()
{
	DWORD Buffer = 0;
	DWORD NewValue = 250;
	//���޸�Ȩ��
	HANDLE GameHandle = OpenProcess(PROCESS_ALL_ACCESS, 1, GetProcessID("h1.exe"));
	//���ڴ��е�ֵ���ŵ�buffer��
	ReadProcessMemory(GameHandle, LPVOID(0x25069Bc), &Buffer, sizeof(DWORD), NULL);
	//�����µ�ֵ���ŵ��µ�buffer��
	WriteProcessMemory(GameHandle, LPVOID(Buffer + (0x1fc)), &NewValue, sizeof(DWORD), NULL);
	cout << Buffer << endl;
}

//��ȡѪ��
void Readblood()
{
	DWORD Buffer = 0;
	float Health = 0.0;
	HANDLE GameHandle = OpenProcess(PROCESS_ALL_ACCESS, 1, GetProcessID("h1.exe"));
	DWORD Address = (DWORD)GetProcessModuleByName(GetProcessID("h1.exe"), "amxmodx_mm.dll") + 0x97260;
	Buffer = ReadMemory_Int(GameHandle, (LPVOID)Address);
	Health = ReadMemory_Float(GameHandle, LPVOID(Buffer + 0x1e0));
	cout << "���˵�Ѫ����" << Health << endl;
}




int main()
{
	GodHigh();
	system("pouse");
	return 0;
}