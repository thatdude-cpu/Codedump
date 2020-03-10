#include "poweroff.h"

void BrowseFolders(HWND hWnd)
{
  BROWSEINFO bi;
  char buffer[MAX_PATH]="";
  LPITEMIDLIST lpiddl;
  LPMALLOC malc;

  Log("BrowseFolders start");
  SHGetMalloc(&malc);
  bi.hwndOwner=hWnd;
  bi.pidlRoot=NULL;
  bi.pszDisplayName=buffer;
  bi.lpszTitle="Select Directory";
  bi.ulFlags=BIF_RETURNONLYFSDIRS ;
  bi.lpfn=NULL;
  bi.lParam=0;
  if ((lpiddl=SHBrowseForFolder(&bi))!=NULL)
  {
    SHGetPathFromIDList(lpiddl,buffer);
    SetDlgItemText(hWnd,IDC_PROGRAM_DIRECTORY,buffer);
  }
  malc->lpVtbl->Free(malc,lpiddl);
  Log("BrowseFolders end, directory=%s",buffer);
}

void BrowseProgram(HWND hWnd)
{
  OPENFILENAME openFile;
  char filename[MAX_PATH]="\0";
  char filter[100]="Programs\0*.EXE;*.CMD;*.BAT;*.COM\0All Files\0*.*\0\0\0";

  Log("BrowseProgram start");
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
    SetDlgItemText(hWnd,IDC_PROGRAM_NAME,filename);
  }
  Log("BrowseProgram end, filename=%s",filename);
}


BOOL FAR PASCAL ProgramProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        ps=(PowerSettings*)lParam;
        SetDlgItemText(hWnd,IDC_PROGRAM_NAME,ps->program.program);
        SetDlgItemText(hWnd,IDC_PROGRAM_DIRECTORY,ps->program.directory);
      }      
      return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDCANCEL:
          EndDialog(hWnd,TRUE);
          break;
        case IDOK:
          GetDlgItemText(hWnd,IDC_PROGRAM_NAME,ps->program.program,MAX_PATH);
          GetDlgItemText(hWnd,IDC_PROGRAM_DIRECTORY,ps->program.directory,MAX_PATH);
          EndDialog(hWnd,TRUE);
          break;
        case IDC_BROWSE:
          BrowseProgram(hWnd);
          break;
        case IDC_BROWSE_DIRECTORY:
          BrowseFolders(hWnd);
          break;
      }
      break;
  	default:
 	    break;
  }
  return FALSE;
}

void ShowProgramDialog(HWND hWnd,PowerSettings *ps)
{
  FARPROC dlgproc;

  Log("ShowProgramDialog start");
  dlgproc=MakeProcInstance((FARPROC)ProgramProc,hInst);
	if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_PROGRAM),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
    DisplayLastError(ps,hWnd);
	FreeProcInstance(dlgproc);
  Log("ShowProgramDialog end");
}