#include "poweroff.h"

void BrowseSoundFile(HWND hWnd)
{
  OPENFILENAME openFile;
  char filename[MAX_PATH]="\0";
  char filter[100]="Sound Files\0*.WAV\0All Files\0*.*\0\0\0";

  Log("BrowseSoundFile start");
  openFile.lStructSize=sizeof(OPENFILENAME);
  openFile.hwndOwner=hWnd;
  openFile.hInstance=NULL;
  openFile.lpstrFilter=filter;
  openFile.lpstrCustomFilter=NULL;
  openFile.nMaxCustFilter=255;
  openFile.nFilterIndex=0;
  openFile.lpstrFile=filename;
  openFile.nMaxFile=MAX_PATH;
  openFile.lpstrFileTitle=NULL;
  openFile.nMaxFileTitle=0;
  openFile.lpstrInitialDir=NULL;
  openFile.lpstrTitle=NULL;
  openFile.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
  openFile.nFileOffset=0;
  openFile.nFileExtension=0;
  openFile.lpstrDefExt=NULL;
  openFile.lCustData=0;
  openFile.lpfnHook=NULL;
  openFile.lpTemplateName=NULL;
  GetOpenFileName(&openFile);
  if (filename[0])
  {
    SetDlgItemText(hWnd,IDC_SOUND_FILE,filename);
  }
  Log("BrowseSoundFile end, filename=%s",filename);
}


BOOL FAR PASCAL MessageProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        char tmpstr[100];

        ps=(PowerSettings*)lParam;
        sprintf(tmpstr,"%u",ps->warning.seconds);
        SetDlgItemText(hWnd,IDC_WARNING_TIME,tmpstr);
        SetDlgItemText(hWnd,IDC_WARNING_MESSAGE,ps->warning.message);
        CheckDlgButton(hWnd,IDC_PLAY_SOUND,ps->warning.play_sound?BST_CHECKED:BST_UNCHECKED);
        if (ps->warning.play_sound)
          SetDlgItemText(hWnd,IDC_SOUND_FILE,ps->warning.sound_file);
        else
        {
          EnableWindow(GetDlgItem(hWnd,IDC_SOUND_FILE),FALSE);
          EnableWindow(GetDlgItem(hWnd,IDC_BROWSE_SOUND),FALSE);
        }
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
            char tmpstr[100];

            GetDlgItemText(hWnd,IDC_WARNING_TIME,tmpstr,100);
            ps->warning.seconds=atoi(tmpstr);
            GetDlgItemText(hWnd,IDC_WARNING_MESSAGE,ps->warning.message,200);
            ps->warning.play_sound=IsDlgButtonChecked(hWnd,IDC_PLAY_SOUND);
            if (ps->warning.play_sound)
              GetDlgItemText(hWnd,IDC_SOUND_FILE,ps->warning.sound_file,MAX_PATH);
            EndDialog(hWnd,TRUE);
          }
          break;
        case IDC_PLAY_SOUND:
          EnableWindow(GetDlgItem(hWnd,IDC_SOUND_FILE),IsDlgButtonChecked(hWnd,IDC_PLAY_SOUND));
          EnableWindow(GetDlgItem(hWnd,IDC_BROWSE_SOUND),IsDlgButtonChecked(hWnd,IDC_PLAY_SOUND));
          break;
        case IDC_BROWSE_SOUND:
          BrowseSoundFile(hWnd);
          break;
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}

void ShowMessageDialog(HWND hWnd,PowerSettings *ps)
{
  FARPROC dlgproc;

  Log("ShowMessageDialog start");
  dlgproc=MakeProcInstance((FARPROC)MessageProc,hInst);
	if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_WARNING_MESSAGE),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
    DisplayLastError(ps,hWnd);
	FreeProcInstance(dlgproc);
  Log("ShowMessageDialog end");
}