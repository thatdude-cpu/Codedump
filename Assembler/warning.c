#include "poweroff.h"

BOOL FAR PASCAL WarningProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static int time_left;
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        char tmpstr[100];
 
        ps=(PowerSettings*)lParam;
        time_left=ps->warning.seconds;
        sprintf(tmpstr,"Time left: %d seconds",ps->warning.seconds);
        SetDlgItemText(hWnd,IDC_SHUTDOWN_TIME,tmpstr);
        if (ps->warning.message!=NULL && ps->warning.message[0]!='\0')
          SetDlgItemText(hWnd,IDC_SHUTDOWN_TEXT,ps->warning.message);
        else
          SetDlgItemText(hWnd,IDC_SHUTDOWN_TEXT,"Please save all your work and close all applications");
        EnableWindow(GetDlgItem(hWnd,IDC_SHUTDOWN_TEXT),FALSE);
        EnableWindow(GetDlgItem(hWnd,IDC_SHUTDOWN_TEXT),TRUE);
        SetDlgItemText(hWnd,IDC_SHUTDOWN_INFO,GetModeStr(ps));
        if (ps->options.allow_cancel)
          EnableWindow(GetDlgItem(hWnd,IDC_CANCEL),TRUE);
        else
          EnableWindow(GetDlgItem(hWnd,IDC_CANCEL),FALSE);
        if (ps->warning.play_sound)
          PlaySound(ps->warning.sound_file,NULL,SND_ASYNC|SND_FILENAME);
        SetTimer(hWnd,TIME_TIMER,1000,NULL);
      }      
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDC_CANCEL:
          EndDialog(hWnd,FALSE);
          break;
        case IDC_DO_IT_NOW:
          EndDialog(hWnd,TRUE);
          break;
      }
      break;
    case WM_TIMER:
      time_left--;
      if (time_left<=0)
        EndDialog(hWnd,TRUE);
      else
      {
        char tmpstr[100];

        sprintf(tmpstr,"Time left: %d seconds",time_left);
        SetDlgItemText(hWnd,IDC_SHUTDOWN_TIME,tmpstr);
      }
      return TRUE;
  	default:
 	    break;
  }
  return FALSE;
}

int ShowWarningMessage(HWND hWnd,PowerSettings *ps)
{
  FARPROC warningproc;
  int ret;

  Log("ShowWarningMessage start");
  warningproc=MakeProcInstance((FARPROC)WarningProc,hInst);
	ret=DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_WARNING),hWnd,(DLGPROC)warningproc,(LPARAM)ps);
	FreeProcInstance(warningproc);
  Log("ShowWarningMessage end");
  return ret;
}