// 
//  File:  		AutoLaunch.cpp
//  Version:		v320
//  Catalog name:  	AutoLaunch_v320_Compact2013
//  OS Version:		Windows Embedded Compact 2013
//  Project URL:	http://AutoLaunch4CE.Codeplex.com
//
//  Updated:		Jan. 21, 2015
//  Updated by:		Samuel Phung
// 
//

#include "stdafx.h"
#include "winsock2.h"
#include "pkfuncs.h"

// Constants ///////////////////////
#define MAX_APPSTART_KEYNAME 128
#define LENGTH_WAIT_FOR_NETWORK_READY 1000 // 1 second
#define MAX_NUM_WAITS_FOR_NETWORK_READY 30 // 30 tries

// prototypes //////////////////////
static BOOL IsNetworkReady(void);
void WalkStartupKey();
DWORD WINAPI ProcessThread(LPVOID lpParameter);

// typedefs ////////////////////////
typedef struct _ProcessStruct {
	WCHAR szName[MAX_APPSTART_KEYNAME];
	DWORD dwDelay;
} PROCESS_STRUCT,*LPPROCESS_STRUCT;

/* ==================================================
   Main thread spawns and joins "launcher threads" 
  =================================================*/
int _tmain(int argc, TCHAR *argv[], TCHAR *envp[])
{
	// network is required
    for (int i = 0; i < MAX_NUM_WAITS_FOR_NETWORK_READY; i++)
    {
        if (IsNetworkReady())
        {
			WalkStartupKey(); // does the job
			break;
        }
        else
        {
			// wait before retry
            Sleep(LENGTH_WAIT_FOR_NETWORK_READY);
        }
    }
	return 0;
}

/* ---------------------------------------------------
	Test network availability
 ---------------------------------------------------*/
BOOL IsNetworkReady()
{
    // Wait up to 2 seconds for the Winsock API to report ready.
    if (WAIT_OBJECT_0 != WaitForAPIReady(HT_SOCKET, 2000))
        return (FALSE);

    // Wait up to 2 seconds for the communications API's to report ready.
    if (WAIT_OBJECT_0 != WaitForAPIReady(SH_COMM, 2000))
        return (FALSE);

    // Open Winsock
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		return (FALSE);

    // Obtain the name of the host we are running on.
    char szHostName[80];
    if (gethostname(szHostName, sizeof(szHostName)) == SOCKET_ERROR) 
        return (FALSE);

    // Obtain the IP addresses of our current host, using name just retrieved.
    // At least one IP address should present when the stack is configured and
    // received an IP address from the DHCP server.
    struct hostent *pHostEntry = gethostbyname(szHostName);
    if (pHostEntry == NULL || pHostEntry->h_addr_list[0] == NULL) 
        return (FALSE);

    WSACleanup();

    // The network appears to be functional
    return (TRUE);
}

/* ---------------------------------------------------
	Read Registry keys and launch theads
 ---------------------------------------------------*/
void WalkStartupKey()
{
	HKEY   hKey;
	WCHAR  szName[MAX_APPSTART_KEYNAME];
	WCHAR  szVal[MAX_APPSTART_KEYNAME];
	WCHAR  szDelay[MAX_APPSTART_KEYNAME];
	LPWSTR lpszArg = NULL;
	DWORD  dwTemp, dwType, dwNameSize, dwValSize, i,dwDelay;

	DWORD dwMaxTimeout=0;
	HANDLE hWaitThread=NULL;
	HANDLE ThreadHandles[100];
	int iThreadCount=0;

	// enters the registry
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("Startup"), 0, KEY_READ, &hKey) != ERROR_SUCCESS)
		return;

	dwNameSize = MAX_APPSTART_KEYNAME;
	dwValSize = MAX_APPSTART_KEYNAME * sizeof(WCHAR);
	i = 0;
	// Explore all keys in HKLM/startup
	while (RegEnumValue(hKey, i, szName, &dwNameSize, 0, &dwType,(LPBYTE)szVal, &dwValSize) == ERROR_SUCCESS)
	{
		// test prefix "Process" 
		if ((dwType == REG_SZ) && !wcsncmp(szName, TEXT("Process"), 7))  // 7 letters in "Process"
		{ 
			// Prepare delay key name according to process key
			wsprintf(szDelay,L"%sDelay",szName);
			dwValSize=sizeof(dwDelay);

			// read delay value from delay key name
			if (ERROR_SUCCESS == RegQueryValueEx(hKey,szDelay,0,&dwType,(LPBYTE)&dwDelay,&dwValSize))
			{
				// we now have the process name and the process delay
				// spawn a thread to "Sleep" and then create the process.
				LPPROCESS_STRUCT ps = new PROCESS_STRUCT;
				ps->dwDelay=dwDelay;
				wcscpy(ps->szName,szVal);

				DWORD dwThreadID;
				OutputDebugString(L"Spawning launch Thread...\n");

				HANDLE hThread=CreateThread(NULL,0,ProcessThread,(LPVOID)ps,0,&dwThreadID);
				ThreadHandles[iThreadCount++] = hThread;

				if (dwDelay > dwMaxTimeout)
				{
					hWaitThread = hThread;
					dwMaxTimeout = dwDelay;
				}
			}
		} 
      
		dwNameSize = MAX_APPSTART_KEYNAME;
		dwValSize = MAX_APPSTART_KEYNAME * sizeof(WCHAR);
		i++;
	}

	// wait on the thread with the longest delay.
	DWORD dwWait=WaitForSingleObject(hWaitThread,INFINITE);
	if (WAIT_FAILED == dwWait)
	{
		OutputDebugString(L"Wait Failed!\n");
	}
	for(int x=0;x < iThreadCount;x++)
	{
		CloseHandle(ThreadHandles[x]);
	}
	RegCloseKey(hKey);
}

/* ---------------------------------------------------
	Launcher thread function
 ---------------------------------------------------*/
DWORD WINAPI ProcessThread(LPVOID lpParameter)
{
	TCHAR tcModuleName[MAX_APPSTART_KEYNAME];
	TCHAR tcMessage[MAX_APPSTART_KEYNAME + 20];
	TCHAR *ptrCmdLine;

	LPPROCESS_STRUCT ps=(LPPROCESS_STRUCT)lpParameter;

	Sleep(ps->dwDelay);	// Wait for delay period

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	si.cb=sizeof(si);

	// output to debug
	wcscat(tcMessage,L"Creating Process ");
	wcscat(tcMessage,ps->szName);
	OutputDebugString(tcMessage);

	wcscpy(tcModuleName,ps->szName);

	TCHAR *tcPtrSpace=wcsrchr(ps->szName,L' '); // Launch command has a space, assume command line.
	if (NULL != tcPtrSpace)
	{
		tcModuleName[lstrlen(ps->szName)-lstrlen(tcPtrSpace)]=0x00;	// overwrite the space with null, break the app and cmd line.
		tcPtrSpace++; // move past space character.
	}

	CreateProcess( tcModuleName, // Module Name    
			tcPtrSpace,     // Command line -- NULL or PTR to command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi );          // Pointer to PROCESS_INFORMATION structure

	return 0;
}

