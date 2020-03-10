#include "poweroff.h"

BOOL CALLBACK CheckProcessAlive(DWORD dw,WORD w16,LPCSTR lpstr,LPARAM lParam)
{
  PowerSettings *ps;

  ps=(PowerSettings*)lParam;
  if (!_stricmp(lpstr,ps->process.process))
    return FALSE;
  return TRUE;
}

void CheckProcessTimer(HWND hWnd,PowerSettings *ps)
{
  Log("CheckProcessTimer start");
  if (EnumProcs(CheckProcessAlive,(LPARAM)ps))
  {
    KillTimer(hWnd,ps->active_timer);
    ps->active_timer=0;
    PowerOff(hWnd,ps);
  }
  Log("CheckProcessTimer start");
}

BOOL CALLBACK FillProcBrowser(DWORD dw,WORD w16,LPCSTR lpstr,LPARAM lParam)
{
  if (lpstr[0])
  {
    SendDlgItemMessage((HWND)lParam,IDC_PROCESS_LIST,LB_ADDSTRING,0,(LPARAM)lpstr);
  }
  return TRUE;
}

BOOL FAR PASCAL ProcessBrowserProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        ps=(PowerSettings*)lParam;
        EnumProcs(FillProcBrowser,(LPARAM)hWnd);
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
            char str[MAX_PATH];
            int cur_sel;

            cur_sel=SendDlgItemMessage(hWnd,IDC_PROCESS_LIST,LB_GETCURSEL,0,0);
            if (cur_sel==LB_ERR)
              Error(ps,hWnd,"No process selected");
            else
            {
              SendDlgItemMessage(hWnd,IDC_PROCESS_LIST,LB_GETTEXT,(WPARAM)cur_sel,(LPARAM)str);
              SetDlgItemText(GetParent(hWnd),IDC_PROCESS,str);
              EndDialog(hWnd,TRUE);
            }
          }
          break;
        case IDC_PROCESS_LIST:
          if ((HIWORD(wParam))==LBN_DBLCLK)
            SendMessage(hWnd,WM_COMMAND,IDOK,0);
          break;
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}


BOOL FAR PASCAL ProcessProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        ps=(PowerSettings*)lParam;
        SetDlgItemText(hWnd,IDC_PROCESS,ps->process.process);
      }      
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDCANCEL:
          EndDialog(hWnd,TRUE);
          break;
        case IDOK:
          GetDlgItemText(hWnd,IDC_PROCESS,ps->process.process,200);
          EndDialog(hWnd,TRUE);
          break;
        case IDC_BUT_PROCESS:
        {
          FARPROC dlgproc;

          dlgproc=MakeProcInstance((FARPROC)ProcessBrowserProc,hInst);
	        if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_PROCESS_BROWSER),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
            DisplayLastError(ps,hWnd);
	        FreeProcInstance(dlgproc);
        }
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}

void ShowProcessDialog(HWND hWnd,PowerSettings *ps)
{
  FARPROC dlgproc;

  Log("ShowProcessDialog start");
  dlgproc=MakeProcInstance((FARPROC)ProcessProc,hInst);
	if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_PROCESS),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
    DisplayLastError(ps,hWnd);
	FreeProcInstance(dlgproc);
  Log("ShowProcessDialog end");
}