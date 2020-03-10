#include "poweroff.h"

char *EncodePassword(char *password)
{
  static char str[40];
  int x,c=65,len;

  Log("EncodePassword start");
  if (password[0]=='\0')
    return "";
  len=strlen(password);
  c=len;
  for (x=0;x<len;x++)
    c=(c+password[x])%256;
  for (x=0;x<40;x++)
  {
    c=(c+password[x%len])%80;
    str[x]=33+c;
  }
  str[39]='\0';
  Log("EncodePassword end, encoded=%s",str);
  return str;
}

BOOL FAR PASCAL RemoteControlProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        char str[40];

        ps=(PowerSettings*)lParam;
        sprintf(str,"%d",ps->remote_control.port);
        SetDlgItemText(hWnd,IDC_REMOTE_PORT,str);
        SetDlgItemText(hWnd,IDC_REMOTE_PASSWORD,"********");
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
            char str[40];

            GetDlgItemText(hWnd,IDC_REMOTE_PORT,str,40);
            ps->remote_control.port=atoi(str);
            if (ps->remote_control.port<=0)
              ps->remote_control.port=LISTEN_PORT;
            GetDlgItemText(hWnd,IDC_REMOTE_PASSWORD,str,40);
            if (strcmp(str,"********"))
              strcpy(ps->remote_control.password,EncodePassword(str));
            EndDialog(hWnd,TRUE);
          }
          break;
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}

void ShowRemoteControlDialog(HWND hWnd,PowerSettings *ps)
{
  FARPROC dlgproc;

  Log("ShowRemoteControlDialog start");
  dlgproc=MakeProcInstance((FARPROC)RemoteControlProc,hInst);
	if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_REMOTE_CONTROL),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
    DisplayLastError(ps,hWnd);
	FreeProcInstance(dlgproc);
  Log("ShowRemoteControlDialog end");
}