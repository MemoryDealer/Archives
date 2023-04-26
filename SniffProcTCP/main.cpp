#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

typedef DWORD (WINAPI *xGetExtendedTcpTable)(PVOID pTcpTable,PDWORD pdwSize,BOOL bOrder,ULONG ulAf,DWORD TableClass,ULONG Reserved);
xGetExtendedTcpTable _GetExtendedTcpTable;

int main(void)
{
    struct in_addr  addr;
    ///PMIB_TCPTABLE   table;
    PMIB_TCPTABLE_OWNER_PID table;
    DWORD           size,
                    ret;
    int             i;
    char            szLocalAddr[128]   = {0};
    char            szRemoteAddr[128]  = {0};

    _GetExtendedTcpTable = (xGetExtendedTcpTable)GetProcAddress(LoadLibrary("iphlpapi.dll"), "GetExtendedTcpTable");
    if(_GetExtendedTcpTable == NULL)
        return 666;

    ///table = (PMIB_TCPTABLE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MIB_TCPTABLE));
    table = (PMIB_TCPTABLE_OWNER_PID)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MIB_TCPTABLE_OWNER_PID));

    ///size = sizeof(MIB_TCPTABLE);
    size = sizeof(MIB_TCPTABLE_OWNER_PID);

    ///if((ret = GetTcpTable(table, &size, TRUE)) == ERROR_INSUFFICIENT_BUFFER)
    if((ret = _GetExtendedTcpTable(table, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) == ERROR_INSUFFICIENT_BUFFER)
    {
        HeapFree(GetProcessHeap(), 0, table);

       /// table = (PMIB_TCPTABLE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
       table = (PMIB_TCPTABLE_OWNER_PID)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
        if(!table)
            return 1;
    }

    ///if((ret = GetTcpTable(table, &size, TRUE)) == NO_ERROR)
    if((ret = _GetExtendedTcpTable(table, &size, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) == NO_ERROR)
    {
        printf("Entries: %d\n", (int)table->dwNumEntries);
        for(i=0; i<(int)table->dwNumEntries; ++i)
        {
            addr.S_un.S_addr = (u_long)table->table[i].dwLocalAddr;
            strcpy(szLocalAddr, inet_ntoa(addr));

            addr.S_un.S_addr = (u_long)table->table[i].dwRemoteAddr;
            strcpy(szRemoteAddr, inet_ntoa(addr));

            printf("\nTCP[%d] state: %ld - ", i, table->table[i].dwState);

            switch(table->table[i].dwState)
            {
                case MIB_TCP_STATE_CLOSED:
                printf("CLOSED\n");
                    break;
                case MIB_TCP_STATE_LISTEN:
                    printf("LISTEN\n");
                    break;
                case MIB_TCP_STATE_SYN_SENT:
                    printf("SYN-SENT\n");
                    break;
                case MIB_TCP_STATE_SYN_RCVD:
                    printf("SYN-RECEIVED\n");
                    break;
                case MIB_TCP_STATE_ESTAB:
                    printf("ESTABLISHED\n");
                    break;
                case MIB_TCP_STATE_FIN_WAIT1:
                    printf("FIN-WAIT-1\n");
                    break;
                case MIB_TCP_STATE_FIN_WAIT2:
                    printf("FIN-WAIT-2 \n");
                    break;
                case MIB_TCP_STATE_CLOSE_WAIT:
                    printf("CLOSE-WAIT\n");
                    break;
                case MIB_TCP_STATE_CLOSING:
                    printf("CLOSING\n");
                    break;
                case MIB_TCP_STATE_LAST_ACK:
                    printf("LAST-ACK\n");
                    break;
                case MIB_TCP_STATE_TIME_WAIT:
                    printf("TIME-WAIT\n");
                    break;
                case MIB_TCP_STATE_DELETE_TCB:
                    printf("DELETE-TCB\n");
                    break;
                default:
                    printf("UNKNOWN dwState value\n");
                    break;

            }

            printf("TCP[%d] Local Addr: %s\n", i, szLocalAddr);
            printf("TCP[%d] Local Port: %d \n", i, ntohs((u_short)table->table[i].dwLocalPort));

            printf("TCP[%d] Remote Addr: %s\n", i, szRemoteAddr);
            printf("TCP[%d] Remote Port: %d\n", i, ntohs((u_short)table->table[i].dwRemotePort));

            printf("TCP[%d] Process PID: %d\n", i, ntohs((u_short)table->table[i].dwOwningPid));
        }
    }

    if(table)
        HeapFree(GetProcessHeap(), 0, table);

    return 0;
}








/*#include <winsock2.h>
#include <ws2tcpip.h>
#include <Tlhelp32.h>
#include <stdio.h>
#include <windows.h>

#define TCP_TABLE_OWNER_PID_ALL 5

typedef struct _MIB_TCPROW_OWNER_PID {
  DWORD dwState;
  DWORD dwLocalAddr;
  DWORD dwLocalPort;
  DWORD dwRemoteAddr;
  DWORD dwRemotePort;
  DWORD dwOwningPid;
} MIB_TCPROW_OWNER_PID, *PMIB_TCPROW_OWNER_PID;

typedef struct {
  DWORD                dwNumEntries;
  MIB_TCPROW_OWNER_PID table[1];
} MIB_TCPTABLE_OWNER_PID, *PMIB_TCPTABLE_OWNER_PID;

typedef DWORD (WINAPI *xGetExtendedTcpTable)(PVOID pTcpTable,PDWORD pdwSize,BOOL bOrder,ULONG ulAf,DWORD TableClass,ULONG Reserved);
xGetExtendedTcpTable mGetExtendedTcpTable;

int main() {

	MIB_TCPTABLE_OWNER_PID *tPID;
	PROCESSENTRY32 ProcessInfo;
	struct in_addr IpAddr;
	DWORD Size;
	HMODULE KERNEL;
	unsigned int i;
	HANDLE handle;


	//Get Process PID

	 ProcessInfo.dwSize = sizeof(PROCESSENTRY32);
	 handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	   while(Process32Next(handle, &ProcessInfo))
    {
       if(strcmp(ProcessInfo.szExeFile, "palemoon.exe")==0)
       {
		   break;

       }
    }

	//Get Connections
    KERNEL = LoadLibrary("Iphlpapi.dll");
    mGetExtendedTcpTable = (xGetExtendedTcpTable) GetProcAddress(KERNEL,"GetExtendedTcpTable");
	tPID = (PMIB_TCPTABLE_OWNER_PID)malloc(sizeof(MIB_TCPTABLE_OWNER_PID));
	mGetExtendedTcpTable(tPID,&Size,FALSE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0);

	while(1) {
		Sleep(50);

    //free(tPID);
	//tPID = (PMIB_TCPTABLE_OWNER_PID)malloc(Size);
	tPID = (PMIB_TCPTABLE_OWNER_PID)realloc(tPID, Size);printf("lolol\n\n");
	mGetExtendedTcpTable(tPID,&Size,FALSE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0);printf("lolol\n\n");
     Sleep(10);

	for(i = 0;i<tPID->dwNumEntries;i++) {
		Sleep(3);
		if(tPID->table[i].dwOwningPid == ProcessInfo.th32ProcessID) {
	printf("%d ",tPID->table[i].dwOwningPid); //display PID
	IpAddr.S_un.S_addr = tPID->table[i].dwRemoteAddr;
	printf(": %s\n",inet_ntoa(IpAddr)); //display remote IP
		}

	}
	Sleep(20);
	}


	return 0;
}
*/
