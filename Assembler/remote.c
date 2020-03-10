#include "poweroff.h"
#include <winsock2.h>

#define MAXLEN_PHYSADDR 8

typedef struct {
  DWORD dwIndex;
  DWORD dwPhysAddLen;
  BYTE  bPhysAddr[MAXLEN_PHYSADDR];
  DWORD dwAddr;
  DWORD dwType;
} MIB_IPNETROW, *PMIB_IPNETROW;

typedef struct {
  DWORD        dwNumEntries;
  MIB_IPNETROW table[100];
} MIB_IPNETTABLE, *PMIB_IPNETTABLE;

typedef struct {
    unsigned char Ttl;                         // Time To Live
    unsigned char Tos;                         // Type Of Service
    unsigned char Flags;                       // IP header flags
    unsigned char OptionsSize;                 // Size in bytes of options data
    unsigned char *OptionsData;                // Pointer to options data
} IP_OPTION_INFORMATION, * PIP_OPTION_INFORMATION;

typedef struct {
    DWORD Address;                             // Replying address
    unsigned long  Status;                     // Reply status
    unsigned long  RoundTripTime;              // RTT in milliseconds
    unsigned short DataSize;                   // Echo data size
    unsigned short Reserved;                   // Reserved for system use
    void *Data;                                // Pointer to the echo data
    IP_OPTION_INFORMATION Options;             // Reply options
} IP_ECHO_REPLY, * PIP_ECHO_REPLY;




DWORD (WINAPI *GetIpNetTable)(PMIB_IPNETTABLE pIpNetTable,PULONG pdwSize,BOOL bOrder);
HANDLE (WINAPI *IcmpCreateFile)(void); 
DWORD (WINAPI *IcmpSendEcho)(HANDLE IcmpHandle,DWORD DestinationAddress,LPVOID RequestData,WORD RequestSize,PIP_OPTION_INFORMATION RequestOptions,LPVOID ReplyBuffer,DWORD ReplySize,DWORD Timeout);
BOOL (WINAPI *IcmpCloseHandle)(HANDLE IcmpHandle); 


void BrowseComputers(HWND hWnd)
{
  BROWSEINFO bi;
  char buffer[MAX_PATH]="";
  ITEMIDLIST *ppidl;
  LPMALLOC malc;

  Log("BrowseComputers start");
  SHGetMalloc(&malc);
  SHGetSpecialFolderLocation(hWnd,CSIDL_NETWORK,&ppidl);
  bi.hwndOwner=hWnd;
  bi.pidlRoot=ppidl;
  bi.pszDisplayName=buffer;
  bi.lpszTitle="Select remote computer";
  bi.ulFlags=BIF_BROWSEFORCOMPUTER;
  bi.lpfn=NULL;
  bi.lParam=0;
  if (SHBrowseForFolder(&bi)!=NULL)
    SetDlgItemText(hWnd,IDC_COMPUTER,buffer);
  malc->lpVtbl->Free(malc,ppidl);
  Log("BrowseComputers end, computer=%s",buffer);
}

void EnableRemoteFields(HWND hWnd)
{
  Log("EnableRemoteFields start");
  if (IsDlgButtonChecked(hWnd,IDC_REMOTE_NT))
  {
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_CREDENTIALS),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_PORT),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_SCHEDULE),FALSE);
    if (IsDlgButtonChecked(hWnd,IDC_REMOTE_CREDENTIALS))
    {
      EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_USERNAME),FALSE);
      EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_PASSWORD),FALSE);
      EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_SAVE_PASSWORD),FALSE);
    }
    else
    {
      EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_USERNAME),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_PASSWORD),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_SAVE_PASSWORD),TRUE);
    }
  }
  else
  {
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_PORT),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_PASSWORD),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_SAVE_PASSWORD),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_SCHEDULE),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_CREDENTIALS),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_USERNAME),FALSE);
  }
  Log("EnableRemoteFields end");
}

BOOL FAR PASCAL RemoteProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      ps=(PowerSettings*)lParam;
      SetDlgItemText(hWnd,IDC_COMPUTER,ps->remote.computer_name);
      if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
      {
        ps->remote.use_nt=0;
        EnableWindow(GetDlgItem(hWnd,IDC_REMOTE_NT),FALSE);
      }
      CheckDlgButton(hWnd,IDC_REMOTE_NT,ps->remote.use_nt?BST_CHECKED:BST_UNCHECKED);
      if (!ps->remote.use_nt)
      {
        char str[40];

        sprintf(str,"%d",ps->remote.port);
        SetDlgItemText(hWnd,IDC_REMOTE_PORT,str);
        CheckDlgButton(hWnd,IDC_REMOTE_SAVE_PASSWORD,ps->remote.save_password?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_REMOTE_SCHEDULE,ps->remote.schedule_remote?BST_CHECKED:BST_UNCHECKED);
      }
      CheckDlgButton(hWnd,IDC_REMOTE_CREDENTIALS,ps->remote.current_credentials?BST_CHECKED:BST_UNCHECKED);
      SetDlgItemText(hWnd,IDC_REMOTE_USERNAME,ps->remote.username);
      SetDlgItemText(hWnd,IDC_REMOTE_PASSWORD,"********");
      EnableRemoteFields(hWnd);
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDCANCEL:
          EndDialog(hWnd,TRUE);
          break;
        case IDOK:
          {
            char str[40];

            GetDlgItemText(hWnd,IDC_COMPUTER,ps->remote.computer_name,100);
            ps->remote.use_nt=IsDlgButtonChecked(hWnd,IDC_REMOTE_NT);
            if (!ps->remote.use_nt)
            {
              GetDlgItemText(hWnd,IDC_REMOTE_PORT,str,40);
              ps->remote.port=atoi(str);
              if (ps->remote.port<=0)
                ps->remote.port=LISTEN_PORT;
              ps->remote.schedule_remote=IsDlgButtonChecked(hWnd,IDC_REMOTE_SCHEDULE);
            }
            else
            {
              ps->remote.port=LISTEN_PORT;
              ps->remote.password[0]='\0';
              ps->remote.save_password=0;
              ps->remote.schedule_remote=0;
            }
            ps->remote.current_credentials=IsDlgButtonChecked(hWnd,IDC_REMOTE_CREDENTIALS);
            GetDlgItemText(hWnd,IDC_REMOTE_USERNAME,ps->remote.username,40);
            GetDlgItemText(hWnd,IDC_REMOTE_PASSWORD,str,40);
            if (strcmp(str,"********"))
              strcpy(ps->remote.password,str);
            ps->remote.save_password=IsDlgButtonChecked(hWnd,IDC_REMOTE_SAVE_PASSWORD);
            EndDialog(hWnd,TRUE);
          }
          break;
        case IDC_BROWSE:
          BrowseComputers(hWnd);
          break;
        case IDC_REMOTE_NT:
        case IDC_REMOTE_CREDENTIALS:
          EnableRemoteFields(hWnd);
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}

int IsValidMACAddress(char *str)
{
  int x;

  Log("IsValidMACAddress start");
  if (strlen(str)!=12)
  {
    Log("Length is not 12");
    return 0;
  }
  for (x=0;x<12;x++)
  {
    str[x]=toupper(str[x]);
    if (strchr("0123456789ABCDEF",str[x])==NULL)
    {
      Log("MAC address contains invalid character");
      return 0;
    }
  }
  Log("IsValidMACAddress end");
  return 1;
}

int IsValidIPAddress(char *str)
{
  int a1,a2,a3,a4;

  Log("IsValidIPAddress start");
  if (sscanf(str,"%d.%d.%d.%d",&a1,&a2,&a3,&a4)!=4)
    return 0;
  if (a1<0 || a1>255)
    return 0;
  if (a2<0 || a2>255)
    return 0;
  if (a3<0 || a3>255)
    return 0;
  if (a4<0 || a4>255)
    return 0;
  Log("IsValidIPAddress end");
  return 1;
}

BOOL FAR PASCAL WOLProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        int b1=0,b2=0,b3=0,b4=0;

        ps=(PowerSettings*)lParam;
        SetDlgItemText(hWnd,IDC_MAC_ADDRESS,ps->remote.mac_address);
        sscanf(ps->remote.ip_address,"%d.%d.%d.%d",&b1,&b2,&b3,&b4);
        SendDlgItemMessage(hWnd,IDC_IP_ADDRESS,IPM_SETADDRESS,0,MAKEIPADDRESS(b1,b2,b3,b4));
        sscanf(ps->remote.subnet_mask,"%d.%d.%d.%d",&b1,&b2,&b3,&b4);
        SendDlgItemMessage(hWnd,IDC_SUBNET_MASK,IPM_SETADDRESS,0,MAKEIPADDRESS(b1,b2,b3,b4));
        SendDlgItemMessage(hWnd,IDC_MAC_ADDRESS,EM_LIMITTEXT,12,0);
      }
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDCANCEL:
          EndDialog(hWnd,TRUE);
          break;
        case IDOK:
          {
            DWORD ip;
            char tmpstr[14];

            GetDlgItemText(hWnd,IDC_MAC_ADDRESS,tmpstr,14);
            if (!IsValidMACAddress(tmpstr))
            {
              Error(ps,hWnd,"Invalid MAC address specified");
            }
            else
            {
              strcpy(ps->remote.mac_address,tmpstr);
              SendDlgItemMessage(hWnd,IDC_IP_ADDRESS,IPM_GETADDRESS,0,(LPARAM)&ip);
              sprintf(ps->remote.ip_address,"%d.%d.%d.%d",(int)FIRST_IPADDRESS(ip),
                                                          (int)SECOND_IPADDRESS(ip),
                                                          (int)THIRD_IPADDRESS(ip),
                                                          (int)FOURTH_IPADDRESS(ip));
              SendDlgItemMessage(hWnd,IDC_SUBNET_MASK,IPM_GETADDRESS,0,(LPARAM)&ip);
              sprintf(ps->remote.subnet_mask,"%d.%d.%d.%d",(int)FIRST_IPADDRESS(ip),
                                                           (int)SECOND_IPADDRESS(ip),
                                                           (int)THIRD_IPADDRESS(ip),
                                                           (int)FOURTH_IPADDRESS(ip));
              EndDialog(hWnd,TRUE);
            }
          }
          break;
        case IDC_BUT_LOOKUP:
          {
            DWORD ip,ipsize;
            char mac_address[20]="",str[20];
            char acPingBuffer[64];
            int x,y;
            MIB_IPNETTABLE iptable;
            HINSTANCE lh;
            HANDLE ph;
            PIP_ECHO_REPLY pIpe;

            SendDlgItemMessage(hWnd,IDC_IP_ADDRESS,IPM_GETADDRESS,0,(LPARAM)&ip);
            ip=htonl(ip);
            lh=LoadLibrary("icmp.dll");
            if (lh==NULL)
            {
              DisplayLastError(ps,hWnd);
              break;
            }
            IcmpCreateFile=(void*)GetProcAddress(lh,"IcmpCreateFile");
            if (IcmpCreateFile ==NULL)
            {
              DisplayLastError(ps,hWnd);
              FreeLibrary(lh);
              break;
            }
            IcmpSendEcho=(void*)GetProcAddress(lh,"IcmpSendEcho");
            if (IcmpSendEcho==NULL)
            {
              DisplayLastError(ps,hWnd);
              FreeLibrary(lh);
              break;
            }
            IcmpCloseHandle=(void*)GetProcAddress(lh,"IcmpCloseHandle");
            if (IcmpCloseHandle==NULL)
            {
              DisplayLastError(ps,hWnd);
              FreeLibrary(lh);
              break;
            }
            if ((ph=IcmpCreateFile())==INVALID_HANDLE_VALUE)
            {
              DisplayLastError(ps,hWnd);
              break;
            }
            memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
            pIpe = (PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
            if (pIpe == 0) 
            {
              Error(ps,hWnd,"Failed to allocate global ping packet buffer");
              IcmpCloseHandle(ph);
              FreeLibrary(lh);
              GlobalFree(pIpe);
              break;
            }
            pIpe->Data = acPingBuffer;
            pIpe->DataSize = sizeof(acPingBuffer);      

            IcmpSendEcho(ph,ip,acPingBuffer,sizeof(acPingBuffer),NULL,pIpe,sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer),5000);
            IcmpCloseHandle(ph);
            FreeLibrary(lh);
            GlobalFree(pIpe);

            lh=LoadLibrary("Iphlpapi.dll");
            if (lh==NULL)
            {
              DisplayLastError(ps,hWnd);
              break;
            }
            GetIpNetTable=(void*)GetProcAddress(lh,"GetIpNetTable");
            if (GetIpNetTable==NULL)
            {
              DisplayLastError(ps,hWnd);
              FreeLibrary(lh);
              break;
            }
            ipsize=sizeof(iptable);
            if (GetIpNetTable(&iptable,&ipsize,FALSE)!=NO_ERROR)
            {
              Error(ps,hWnd,"Error during GetIpNetTable()");
              FreeLibrary(lh);
              break;
            }
            for (x=0;x<(int)iptable.dwNumEntries && mac_address[0]=='\0';x++)
            {
              if (iptable.table[x].dwAddr==ip)
              {
                for (y=0;y<(int)iptable.table[x].dwPhysAddLen;y++)
                {
                  sprintf(str,"%02X",(int)iptable.table[x].bPhysAddr[y]);
                  strcat(mac_address,str);
                }
                SetDlgItemText(hWnd,IDC_MAC_ADDRESS,mac_address);
                break;
              }
            }
            FreeLibrary(lh);
            if (mac_address[0]=='\0')
            {
              Error(ps,hWnd,"MAC address not found");
              break;
            }
          }
          break;
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}

void ShowRemoteDialog(HWND hWnd,PowerSettings *ps)
{
  FARPROC dlgproc;

  Log("ShowRemoteDialog start");
  if (ps->action==WAKE_ON_LAN)
  {
    Log("Action=WOL");
    dlgproc=MakeProcInstance((FARPROC)WOLProc,hInst);
	  if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_WOL),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
      DisplayLastError(ps,hWnd);
	  FreeProcInstance(dlgproc);
  }
  else
  {
    Log("Action!=WOL");
    dlgproc=MakeProcInstance((FARPROC)RemoteProc,hInst);
	  if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_REMOTE),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
      DisplayLastError(ps,hWnd);
	  FreeProcInstance(dlgproc);
  }
  Log("ShowRemoteDialog end");
}