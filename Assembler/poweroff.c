#include "poweroff.h"

HINSTANCE hInst;
UINT g_tid=0,g_time_tid=0;
HMENU g_menu;
UINT tray_timer=0;
short debug=0;
SETSUSPENDSTATEPROC MySetSuspendState=NULL;

void Log(char *fmt,...)
{
  va_list param;
  char str[5000],str2[5000];
  FILE *f;
  time_t t;
  char timebuf[100];

  if (debug)
  {
    va_start(param,fmt);
    _vsnprintf(str,5000,fmt,param);
    va_end(param);
    time(&t);
    strftime(timebuf,100,"%d-%m-%Y %H:%M:%S",localtime(&t));
    if (str[strlen(str)-1]=='\n')
      sprintf(str2,"%19s -> %s",timebuf,str);
    else
      sprintf(str2,"%19s -> %s\n",timebuf,str);

    f=fopen("poweroff.log","a+");
    if (f==NULL)
		  return;
    fprintf(f,"%s",str2);
    fclose(f);
  }
}

void Error(PowerSettings *ps,HWND hWnd,char *str)
{
  Log("Error start");
  if (!ps || ps->quiet==0)
    MessageBox(hWnd,str,"poweroff",MB_OK|MB_ICONERROR);
  Log("ERROR: %s",str);
  Log("Error end");
}

void DisplayInformation(PowerSettings *ps,HWND hWnd,char *str)
{
  Log("DisplayInformation start");
  if (hWnd && (!ps || ps->quiet==0))
    MessageBox(NULL,str,"Poweroff",MB_OK|MB_ICONINFORMATION);
  Log("INFO: %s",str);
  Log("DisplayInformation end");
}

void WriteEvent(PowerSettings *ps,WORD event_type,char *fmt,...)
{
	HANDLE event_log;
  va_list param;
  char *str;

  Log("WriteEvent start");
  str=GlobalAlloc(GMEM_FIXED,5000);
  va_start(param,fmt);
  vsprintf(str,fmt,param);
  va_end(param);

  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_NT)
  {
    event_log=RegisterEventSource(NULL,"Poweroff");
		if (event_log==NULL)
		{
      GlobalFree(str);
			return;
		}
		ReportEvent(event_log,event_type,0,0,NULL,1,0,&str,NULL);
    DeregisterEventSource(event_log);
	}
  GlobalFree(str);
  Log("WriteEvent end");
}

void DisplayLastError(PowerSettings *ps,HWND hWnd)
{
  LPVOID lpMsgBuf;
  
  Log("DisplayLastError start");
  FormatMessage( 
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    GetLastError(),
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR) &lpMsgBuf,    0,    NULL );
  WriteEvent(ps,EVENTLOG_ERROR_TYPE,lpMsgBuf);
  if (hWnd)
    Error(ps, hWnd, lpMsgBuf);
  Log("ERROR: %s",lpMsgBuf);
  LocalFree( lpMsgBuf );
  Log("DisplayLastError end");
}

void EnableFields(HWND hWnd,PowerSettings *ps)
{
  Log("EnableFields start");
  if (ps->action==WAKE_ON_LAN)
  {
    ps->who=REMOTE_COMPUTER;
    CheckRadioButton(hWnd,IDC_LOCAL_COMPUTER,IDC_REMOTE_COMPUTER,IDC_REMOTE_COMPUTER);
    EnableWindow(GetDlgItem(hWnd,IDC_LOCAL_COMPUTER),FALSE);
  }
  else
  {
    EnableWindow(GetDlgItem(hWnd,IDC_LOCAL_COMPUTER),TRUE);
  }
  if (ps->who==LOCAL_COMPUTER)
  {
    if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
    {
      EnableWindow(GetDlgItem(hWnd,IDC_LOGOFF),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_REBOOT),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_SHUTDOWN),TRUE);
      /*EnableWindow(GetDlgItem(hWnd,IDC_POWEROFF),FALSE);*/
      EnableWindow(GetDlgItem(hWnd,IDC_POWEROFF),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_STANDBY),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_HIBERNATE),TRUE);
      EnableWindow(GetDlgItem(hWnd,IDC_LOCK),FALSE);
      if (ps->action==LOCK)
      {
        CheckRadioButton(hWnd,IDC_LOGOFF,IDC_WAKE_ON_LAN,IDC_SHUTDOWN);
        ps->action=POWEROFF;
      }
    }
    else if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_NT
          && ps->windowsversion.dwMajorVersion<5)
    {
      EnableWindow(GetDlgItem(hWnd,IDC_LOCK),FALSE);
      if (ps->action==LOCK)
      {
        CheckRadioButton(hWnd,IDC_LOGOFF,IDC_WAKE_ON_LAN,IDC_POWEROFF);
        ps->action=POWEROFF;
      }
    }
  }
  else
  {
    EnableWindow(GetDlgItem(hWnd,IDC_LOGOFF),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_REBOOT),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_SHUTDOWN),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_POWEROFF),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_STANDBY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_HIBERNATE),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_LOCK),TRUE);
  }
  if (ps->who==LOCAL_COMPUTER)
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_REMOTE),FALSE);
  else
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_REMOTE),TRUE);

  if (ps->options.warning)
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_MESSAGE),TRUE);
  else
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_MESSAGE),FALSE);

  if (ps->options.run_program)
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_PROGRAM),TRUE);
  else
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_PROGRAM),FALSE);

  if (ps->when==SCHEDULED)
  {
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_SCHEDULE),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_PROCESS),FALSE);
  }
  else if (ps->when==PROCESS)
  {
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_SCHEDULE),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_PROCESS),TRUE);
  }
  else
  {
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_SCHEDULE),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_PROCESS),FALSE);
  }
  if (ps->options.allow_remote_control)
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_REMOTE_CONTROL),TRUE);
  else
    EnableWindow(GetDlgItem(hWnd,IDC_BUT_REMOTE_CONTROL),FALSE);
  Log("EnableFields end");
}

void ApplySettings(HWND hWnd,PowerSettings *ps)
{
  Log("ApplySettings start");
  if (IsDlgButtonChecked(hWnd,IDC_LOCAL_COMPUTER))
    ps->who=LOCAL_COMPUTER;
  else
    ps->who=REMOTE_COMPUTER;
  if (IsDlgButtonChecked(hWnd,IDC_LOGOFF))
    ps->action=LOGOFF;
  else if (IsDlgButtonChecked(hWnd,IDC_REBOOT))
    ps->action=REBOOT;
  else if (IsDlgButtonChecked(hWnd,IDC_REBOOT))
    ps->action=REBOOT;
  else if (IsDlgButtonChecked(hWnd,IDC_SHUTDOWN))
    ps->action=SHUTDOWN;
  else if (IsDlgButtonChecked(hWnd,IDC_POWEROFF))
    ps->action=POWEROFF;
  else if (IsDlgButtonChecked(hWnd,IDC_STANDBY))
    ps->action=STANDBY;
  else if (IsDlgButtonChecked(hWnd,IDC_HIBERNATE))
    ps->action=HIBERNATE;
  else if (IsDlgButtonChecked(hWnd,IDC_LOCK))
    ps->action=LOCK;
  else if (IsDlgButtonChecked(hWnd,IDC_WAKE_ON_LAN))
    ps->action=WAKE_ON_LAN;
  else if (IsDlgButtonChecked(hWnd,IDC_MONITOR_OFF))
    ps->action=MONITOR_OFF;
  else if (IsDlgButtonChecked(hWnd,IDC_MONITOR_ON))
    ps->action=MONITOR_ON;
  else if (IsDlgButtonChecked(hWnd,IDC_NO_ACTION))
    ps->action=NO_ACTION;
  ps->options.warning=IsDlgButtonChecked(hWnd,IDC_WARNING);
  ps->options.force=IsDlgButtonChecked(hWnd,IDC_FORCE);
  ps->options.in_tray=IsDlgButtonChecked(hWnd,IDC_TRAY);
  ps->options.run_program=IsDlgButtonChecked(hWnd,IDC_PRE_JOB);
  ps->options.allow_cancel=IsDlgButtonChecked(hWnd,IDC_ALLOW_CANCEL);
  ps->options.allow_remote_control=IsDlgButtonChecked(hWnd,IDC_ALLOW_REMOTE_CONTROL);
  if (IsDlgButtonChecked(hWnd,IDC_IMMEDIATE))
    ps->when=IMMEDIATE;
  else if (IsDlgButtonChecked(hWnd,IDC_SCHEDULED))
    ps->when=SCHEDULED;
  else if (IsDlgButtonChecked(hWnd,IDC_AFTER_PROCESS))
    ps->when=PROCESS;
  EnableFields(hWnd,ps);
  if (ps->run_as_service && ps->options.allow_remote_control && ps->serversocket==INVALID_SOCKET)
    CreateServerSocket(hWnd,ps);
  else if (!ps->options.allow_remote_control && ps->serversocket!=INVALID_SOCKET)
    CloseServerSocket(hWnd,ps);
  Log("ApplySettings end");
}

void PutInTray(HWND hWnd,PowerSettings *ps)
{
  NOTIFYICONDATA nid;

  Log("PutInTray start");
  nid.cbSize=sizeof(NOTIFYICONDATA);
  nid.hWnd=hWnd;
  nid.uID=1;
  nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
  nid.uCallbackMessage=NMESSAGE;
  nid.hIcon=LoadIcon(hInst, MAKEINTRESOURCE(IDI_POWEROFF_SMALL));
  if (ps->active_timer && ps->when==SCHEDULED)
  {
    if (ps->schedule.schedule==FIXED_DAY)
      _snprintf(nid.szTip,63,"%s scheduled at %02d:%02d on %02d/%02d/%04d",GetModeStr(ps),ps->schedule.time.wHour,ps->schedule.time.wMinute,ps->schedule.date.wDay,ps->schedule.date.wMonth,ps->schedule.date.wYear);
    else if (ps->schedule.schedule==DAILY)
    {
      char str[20]="";

      if (ps->schedule.monday)
        ADD_DAY("m")
      if (ps->schedule.tuesday)
        ADD_DAY("t")
      if (ps->schedule.wednesday)
        ADD_DAY("w")
      if (ps->schedule.thursday)
        ADD_DAY("th")
      if (ps->schedule.friday)
        ADD_DAY("f")
      if (ps->schedule.saturday)
        ADD_DAY("s")
      if (ps->schedule.sunday)
        ADD_DAY("su")
      _snprintf(nid.szTip,63,"%s scheduled at %02d:%02d on %s",GetModeStr(ps),ps->schedule.time.wHour,ps->schedule.time.wMinute,str);
    }
    else if (ps->schedule.schedule==DAY_OF_MONTH)
      _snprintf(nid.szTip,63,"%s scheduled at %02d:%02d on day %d",GetModeStr(ps),ps->schedule.time.wHour,ps->schedule.time.wMinute,ps->schedule.day);
    else if (ps->schedule.schedule==AFTER_X_SECONDS)
      _snprintf(nid.szTip,63,"%s scheduled after %d seconds",GetModeStr(ps),ps->remaining_seconds);
  }
  else if (ps->active_timer && ps->when==PROCESS)
  {
    _snprintf(nid.szTip,63,"%s when %s finishes",GetModeStr(ps),ps->process.process);
  }
  else
    strcpy(nid.szTip,"Poweroff idle");
  Shell_NotifyIcon(NIM_ADD,&nid);
  if (ps->run_as_service && tray_timer==0)
  {
    tray_timer=SetTimer(hWnd,ID_TRAY_TIMER,5000,NULL);
    if (tray_timer==0)
      DisplayLastError(ps,hWnd);
  }
  Log("PutInTray end");
}

void MinimizePoweroff(HWND hWnd,PowerSettings *ps)
{
  Log("MinimizePoweroff start");
  ShowWindow(hWnd,SW_HIDE);
  if ((!ps->run_as_service && !ps->active_timer) || ps->options.in_tray)
  {
    PutInTray(hWnd,ps);
  }
  if (ps->active_timer && ps->options.allow_cancel)
  {
    EnableMenuItem(g_menu,IDC_CANCEL,MF_ENABLED);
    EnableMenuItem(g_menu,IDC_EXIT,MF_ENABLED);
    EnableMenuItem(g_menu,IDC_RESTORE,MF_GRAYED);
  }
  else if (ps->active_timer && !ps->options.allow_cancel)
  {
    EnableMenuItem(g_menu,IDC_CANCEL,MF_GRAYED);
    EnableMenuItem(g_menu,IDC_EXIT,MF_GRAYED);
    EnableMenuItem(g_menu,IDC_RESTORE,MF_GRAYED);
  }
  else
  {
    EnableMenuItem(g_menu,IDC_CANCEL,MF_GRAYED);
    EnableMenuItem(g_menu,IDC_EXIT,MF_ENABLED);
    EnableMenuItem(g_menu,IDC_RESTORE,MF_ENABLED);
  }
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
    EnableMenuItem(g_menu,IDC_Q_LOCK,MF_GRAYED);
  }
  if (ps->run_as_service)
    EnableMenuItem(g_menu,IDC_EXIT,MF_GRAYED);
  Log("MinimizePoweroff end");
}

void RemoveFromTray(HWND hWnd)
{
  NOTIFYICONDATA nid;

  Log("RemoveFromTray start");
  nid.cbSize=sizeof(NOTIFYICONDATA);
  nid.hWnd=hWnd;
  nid.uID=1;
  nid.uFlags=0;
  Shell_NotifyIcon(NIM_DELETE,&nid);
  SetForegroundWindow(hWnd);
  if (tray_timer)
  {
    KillTimer(hWnd,tray_timer);
    tray_timer=0;
  }
  Log("RemoveFromTray end");
}

void RestorePoweroff(HWND hWnd,PowerSettings *ps)
{
  Log("RestorePoweroff start");
  if (!ps->active_timer)
  {
    RemoveFromTray(hWnd);
    ShowWindow(hWnd,SW_SHOWNORMAL);
    SetForegroundWindow(hWnd);
  }
  Log("RestorePoweroff end");
}

void DisplaySettings(HWND hWnd,PowerSettings *ps)
{
  Log("DisplaySettings start");
  switch (ps->who)
  {
    case LOCAL_COMPUTER:
      CheckRadioButton(hWnd,IDC_LOCAL_COMPUTER,IDC_REMOTE_COMPUTER,IDC_LOCAL_COMPUTER);
      break;
    case REMOTE_COMPUTER:
      CheckRadioButton(hWnd,IDC_LOCAL_COMPUTER,IDC_REMOTE_COMPUTER,IDC_REMOTE_COMPUTER);
      break;
  }
  switch (ps->action)
  {
    case LOGOFF:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_LOGOFF);
      break;
    case REBOOT:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_REBOOT);
      break;
    case SHUTDOWN:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_SHUTDOWN);
      break;
    case POWEROFF:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_POWEROFF);
      break;
    case STANDBY:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_STANDBY);
      break;
    case HIBERNATE:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_HIBERNATE);
      break;
    case LOCK:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_LOCK);
      break;
    case WAKE_ON_LAN:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_WAKE_ON_LAN);
      break;
    case MONITOR_OFF:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_MONITOR_OFF);
      break;
    case MONITOR_ON:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_MONITOR_ON);
      break;
    case NO_ACTION:
      CheckRadioButton(hWnd,IDC_LOGOFF,IDC_NO_ACTION,IDC_NO_ACTION);
      break;
  }
  switch (ps->when)
  {
    case IMMEDIATE:
      CheckRadioButton(hWnd,IDC_IMMEDIATE,IDC_AFTER_PROCESS,IDC_IMMEDIATE);
      break;
    case SCHEDULED:
      CheckRadioButton(hWnd,IDC_IMMEDIATE,IDC_AFTER_PROCESS,IDC_SCHEDULED);
      break;
    case PROCESS:
      CheckRadioButton(hWnd,IDC_IMMEDIATE,IDC_AFTER_PROCESS,IDC_AFTER_PROCESS);
      break;
  }
  CheckDlgButton(hWnd,IDC_FORCE,ps->options.force?BST_CHECKED:BST_UNCHECKED);
  CheckDlgButton(hWnd,IDC_TRAY,ps->options.in_tray?BST_CHECKED:BST_UNCHECKED);
  CheckDlgButton(hWnd,IDC_PRE_JOB,ps->options.run_program?BST_CHECKED:BST_UNCHECKED);
  CheckDlgButton(hWnd,IDC_WARNING,ps->options.warning?BST_CHECKED:BST_UNCHECKED);
  CheckDlgButton(hWnd,IDC_ALLOW_CANCEL,ps->options.allow_cancel?BST_CHECKED:BST_UNCHECKED);
  CheckDlgButton(hWnd,IDC_ALLOW_REMOTE_CONTROL,ps->options.allow_remote_control?BST_CHECKED:BST_UNCHECKED);
  EnableFields(hWnd,ps);
  if (ps->run_as_service && ps->options.allow_remote_control && ps->serversocket==INVALID_SOCKET)
    CreateServerSocket(hWnd,ps);
  else if (!ps->options.allow_remote_control && ps->serversocket!=INVALID_SOCKET)
    CloseServerSocket(hWnd,ps);
  Log("DisplaySettings end");
}
 
BOOL FAR PASCAL DlgProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        char str[30];

        g_hWnd=hWnd;
        sprintf(str,"Poweroff %s",POWEROFF_VERSION);
        SetWindowText(hWnd,str);
        SendMessage(hWnd,WM_SETICON,(WPARAM)ICON_BIG,(LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_POWEROFF)));
        SendMessage(hWnd,WM_SETICON,(WPARAM)ICON_SMALL,(LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_POWEROFF_SMALL)));
        ps=(PowerSettings*)lParam;
        DisplaySettings(hWnd,ps);

        if (!ps->interactive)
        {
          int r;

          r=DoIt(hWnd,ps);
          if (r!=0 || ps->when==IMMEDIATE)
            PostQuitMessage(0);
          return FALSE;
        }
        else if (ps->run_as_service)
        {
          ShowWindow(hWnd,SW_SHOWNA);
          if (ps->when==SCHEDULED)
            DoIt(hWnd,ps);
          else
            MinimizePoweroff(hWnd,ps);
          return FALSE;
        }
        else if (ps->start_minimized)
        {
          MinimizePoweroff(hWnd,ps);
          return FALSE;
        }
        else
        {
          ShowWindow(hWnd,SW_SHOWNORMAL);
        }
        SetTimer(hWnd,10,1000,NULL);
      }
      return TRUE;
    case WM_CLOSE:
      if (!ps->run_as_service)
        PostQuitMessage(0);
      else
        MinimizePoweroff(hWnd,ps);
      return TRUE;
      break;
    case WM_QUIT:
    case WM_DESTROY:
      RemoveFromTray(hWnd);
      return TRUE;
      break;
    case WM_TIMER:
      if (wParam==ID_TRAY_TIMER)
        PutInTray(hWnd,ps);
      else if (wParam==ID_STOP_TIMER)
        PostQuitMessage(0);
      else if (ps->active_timer)
      {
        if (wParam==ID_SCHEDULE_TIMER)
          CheckScheduleTimer(hWnd,ps);
        else if (wParam==ID_PROCESS_TIMER)
          CheckProcessTimer(hWnd,ps);
      }
      return TRUE;
      break;
    case WM_COMMAND:
      switch (LOWORD(wParam))
      {
        case IDC_CANCEL:
          if (!ps->run_as_service)
            PostQuitMessage(0);
          else
            MinimizePoweroff(hWnd,ps);
          break;
        case IDC_OK:
          DoIt(hWnd,ps);
          break;
        case IDC_BUT_REMOTE:
          ShowRemoteDialog(hWnd,ps);
          break;
        case IDC_BUT_MESSAGE:
          ShowMessageDialog(hWnd,ps);
          break;
        case IDC_BUT_PROGRAM:
          ShowProgramDialog(hWnd,ps);
          break;
        case IDC_BUT_SCHEDULE:
          ShowScheduleDialog(hWnd,ps);
          break;
        case IDC_BUT_PROCESS:
          ShowProcessDialog(hWnd,ps);
          break;
        case IDC_BUT_REMOTE_CONTROL:
          ShowRemoteControlDialog(hWnd,ps);
          break;
        case IDC_SAVE:
          SaveSettings(HKEY_CURRENT_USER,"SOFTWARE\\JoBo\\Poweroff",ps);
          break;
        case IDC_LOAD_SETTINGS:
          if (!ReadSettings(HKEY_CURRENT_USER,"SOFTWARE\\JoBo\\Poweroff",ps))
            ReadSettings(HKEY_LOCAL_MACHINE,"SOFTWARE\\JoBo\\Poweroff",ps);
          DisplaySettings(hWnd,ps);
          break;
        case IDC_REMOVE_SETTINGS:
          if (!DeleteRegistryKey(HKEY_CURRENT_USER,"SOFTWARE\\JoBo\\Poweroff"))
          {
            DisplayLastError(ps,hWnd);
            break;
          }
          InitializeSettings(ps);
          DisplaySettings(hWnd,ps);
          break;
        case ID_SAVESETTINGSTOSERVICE:
          SaveSettings(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters",ps);
          if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
          {
            DisplayInformation(ps,hWnd,"Please restart your computer for the changes to take effect");
          }
          else
          {
            if (IsPoweroffServiceRunning(hWnd,ps))
            {
              StopPoweroffService(hWnd,ps);
              StartPoweroffService(hWnd,ps);
            }
          }
          break;
        case ID_LOADSETTINGSFROMSERVICE:
          ReadSettings(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters",ps);
          DisplaySettings(hWnd,ps);
          break;
        case ID_CREATESERVICE:
          CreatePoweroffService(hWnd,ps);
          break;
        case IDC_REMOVE_SERVICE:
          if (IsPoweroffServiceRunning(hWnd,ps))
            StopPoweroffService(hWnd,ps);
          RemovePoweroffService(hWnd,ps);
          break;
        default:
          ApplySettings(hWnd,ps);
	        break;
      }
      return TRUE;
      break;
    case WM_SIZE:
      if (wParam==SIZE_MINIMIZED)
      {
        MinimizePoweroff(hWnd,ps);
      }
      return TRUE;
      break;
    case UWM_ACCEPT:
      if (WSAGETSELECTERROR(lParam)==0)
        AcceptClientSocket(hWnd,ps);
      return TRUE;
      break;
    case UWM_READ:
      if (WSAGETSELECTERROR(lParam)==0)
      {
        if (WSAGETSELECTEVENT(lParam)==FD_READ)
          ReadFromClientSocket(hWnd,ps);
        else if (WSAGETSELECTEVENT(lParam)==FD_CLOSE)
          CloseClientSocket(hWnd,ps);
      }
      return TRUE;
      break;
    case NMESSAGE:
      switch (lParam)
      {
        case WM_LBUTTONDBLCLK:
          if (!ps->active_timer || ps->options.allow_cancel)
            RestorePoweroff(hWnd,ps);
          break;
        case WM_RBUTTONDOWN:
          {
            POINT p;

            GetCursorPos(&p);

            /* Fix for stupid menu behaviour */
            SetForegroundWindow (hWnd);
    
            switch (TrackPopupMenu(GetSubMenu(g_menu,0),
                                   TPM_RIGHTALIGN|TPM_BOTTOMALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON,
                                   p.x,
                                   p.y,
                                   0,
                                   hWnd,
                                   NULL))
            {
              case IDC_LOGOFF:
                DoQuickAction(ps,LOGOFF);
                break;             
              case IDC_REBOOT:
                DoQuickAction(ps,REBOOT);
                break;              
              case IDC_SHUTDOWN:
                DoQuickAction(ps,SHUTDOWN);
                break;              
              case IDC_POWEROFF:
                DoQuickAction(ps,POWEROFF);
                break;              
              case IDC_STANDBY:
                DoQuickAction(ps,STANDBY);
                break;              
              case IDC_Q_HIBERNATE:
                DoQuickAction(ps,HIBERNATE);
                break;
              case IDC_Q_LOCK:
                DoQuickAction(ps,LOCK);
                break;
              case IDC_Q_MONITOR_OFF:
                DoQuickAction(ps,MONITOR_OFF);
                break;
              case IDC_CANCEL:
                if (ps->active_timer)
                {
                  KillTimer(hWnd,ps->active_timer);
                  ps->active_timer=0;
                  RestorePoweroff(hWnd,ps);
                }
                break;
              case IDC_RESTORE:
                RestorePoweroff(hWnd,ps);
                break;
              case IDC_EXIT:
                if (!ps->run_as_service)
                {
                  NOTIFYICONDATA nid;

                  nid.cbSize=sizeof(NOTIFYICONDATA);
                  nid.hWnd=hWnd;
                  nid.uID=1;
                  nid.uFlags=0;
                  Shell_NotifyIcon(NIM_DELETE,&nid);
                  PostQuitMessage(0);
                }
                break;
            }

            /* Fix for stupid menu behaviour */
            PostMessage (hWnd, WM_NULL, 0, 0);
          }
          break;

      }
      return TRUE;
      break;               
  }
  return FALSE;
}

BOOL InitDialog(HWND hWnd,HINSTANCE hInstance,int nCmdShow,PowerSettings *ps)
{
  FARPROC dlgproc;
  
  Log("InitDialog start");
  dlgproc=MakeProcInstance((FARPROC)DlgProc,hInstance);
	if (CreateDialogParam(hInstance,MAKEINTRESOURCE(IDD_POWEROFF),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==NULL)
    DisplayLastError(ps,hWnd);
	FreeProcInstance(dlgproc);
  Log("InitDialog end");
  return TRUE;
}

void TrySetSuspendState(void)
{
  HINSTANCE hInstLib;

  Log("TrySetSuspendState start");
  hInstLib = LoadLibrary( "Powrprof.dll" ) ;
  if( hInstLib == NULL )
  {
    Log("TrySetSuspendState end, failed to load library powrprof.dll");
    return;
  }
  MySetSuspendState=(SETSUSPENDSTATEPROC)GetProcAddress(hInstLib,"SetSuspendState");
  Log("TrySetSuspendState end, MySetSuspendState=%p",MySetSuspendState);
}  

int PASCAL WinMain( HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
  MSG msg;
  INITCOMMONCONTROLSEX controls;
  PowerSettings ps;
  DWORD tid;

  controls.dwSize=sizeof(INITCOMMONCONTROLSEX);
  controls.dwICC=ICC_DATE_CLASSES|ICC_INTERNET_CLASSES;
  InitCommonControlsEx(&controls);

  hInst=hInstance;
  InitializeSockets();
  InitializeSettings(&ps);
  TrySetSuspendState();
  if (!ParseCommandLine(GetCommandLine(),&ps))
    ReadDefaultSettings(&ps);

  Log("Starting poweroff %s",POWEROFF_VERSION);
  Log("Commandline: %s",GetCommandLine());
  if (debug)
    DebugSettings(&ps);
  if (ps.windowsversion.dwPlatformId==VER_PLATFORM_WIN32_NT)
    Log("PC is running Windows NT version %d.%d build %d (%s)",ps.windowsversion.dwMajorVersion,ps.windowsversion.dwMinorVersion,ps.windowsversion.dwBuildNumber,ps.windowsversion.szCSDVersion);
  else
    Log("PC is running Windows 9x version %d.%d build %d (%s)",ps.windowsversion.dwMajorVersion,ps.windowsversion.dwMinorVersion,LOWORD(ps.windowsversion.dwBuildNumber),ps.windowsversion.szCSDVersion);

  CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)CheckService,&ps,0,&tid);

 
  g_menu=LoadMenu(hInst,MAKEINTRESOURCE(IDR_QUICKMENU));

  if (!InitDialog(NULL,hInstance,nCmdShow,&ps))
    return FALSE;

  while (GetMessage(&msg,NULL,0,0)==1)
  {
    if (!IsDialogMessage(g_hWnd,&msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  Log("Poweroff stopped");
  Cleanup(&ps);
  return (msg.wParam);
}

void Cleanup(PowerSettings *ps)
{
  Log("Cleanup");
  if (g_menu)
  {
    DestroyMenu(g_menu);
    g_menu=NULL;
  }
  DestroySockets(NULL,ps);
}