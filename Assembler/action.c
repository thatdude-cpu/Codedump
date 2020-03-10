#include "poweroff.h"
#include <winnetwk.h>

void RunProgram(HWND hWnd,PowerSettings *ps)
{
	PROCESS_INFORMATION p;
  STARTUPINFO si;
  DWORD exit_code;
  char *directory=NULL;

  Log("RunProgram start");
  if (ps->program.directory[0]!='\0')
    directory=ps->program.directory;

	si.cb=sizeof(STARTUPINFO);
	si.lpReserved=NULL;
	si.lpDesktop=NULL;
	si.lpTitle=NULL;
	si.dwFlags=0;
	si.cbReserved2=0;
	si.lpReserved2=NULL;
  Log("Running program %s in directory %s",ps->program.program,ps->program.directory);
  if (CreateProcess(NULL,ps->program.program,NULL,NULL,FALSE,0,NULL,directory,&si,&p)==0)
  {
    DisplayLastError(ps,hWnd);
    return;
  }
  Log("Waiting for program to finish");
	do
	{
		GetExitCodeProcess(p.hProcess,&exit_code);
	  Sleep(100);
  } while (exit_code==STILL_ACTIVE);
  Log("RunProgram end");
}

char *GetModeStr(PowerSettings *ps)
{
  static char str[100];

  Log("GetModeStr start");
  str[0]='\0';
  if (ps->options.force)
    strcpy(str,"FORCE ");
  switch (ps->action)
	{
	  case LOGOFF:
      strcat(str,"LOGOFF");
      break;
	  case REBOOT:
      strcat(str,"REBOOT");
      break;
	  case SHUTDOWN:
      strcat(str,"SHUTDOWN");
      break;
	  case POWEROFF:
      strcat(str,"POWEROFF");
      break;
	  case STANDBY:
      strcat(str,"STANDBY");
      break;
	  case HIBERNATE:
      strcat(str,"HIBERNATE");
      break;
	  case LOCK:
      strcat(str,"LOCK");
      break;
	  case WAKE_ON_LAN:
      strcat(str,"WAKE_ON_LAN");
      break;
	  case MONITOR_OFF:
      strcat(str,"MONITOR_OFF");
      break;
	  case MONITOR_ON:
      strcat(str,"MONITOR_ON");
      break;
	  case NO_ACTION:
      strcat(str,"NO_ACTION");
      break;
		default:
      strcat(str,"UNKNOWN");
	}
  Log("GetModeStr end, str=%s",str);
  return str;
}

int AquirePrivileges(HWND hWnd,PowerSettings *ps)
{
  HANDLE current_thread,token;
  TOKEN_PRIVILEGES privs;
  LUID luid;

  Log("AquirePrivileges start");
  current_thread=GetCurrentProcess();
  if (!OpenProcessToken(current_thread,TOKEN_ADJUST_PRIVILEGES,&token))
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  if (!LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&luid))
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  privs.PrivilegeCount=1;
  privs.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
  privs.Privileges[0].Luid=luid;
  if (!AdjustTokenPrivileges(token,FALSE,&privs,0,NULL,NULL))
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  Log("AquirePrivileges end");
  return 0;
}

BOOL CALLBACK CheckProcessIsRunning(DWORD dw,WORD w16,LPCSTR lpstr,LPARAM lParam)
{
  if (!_stricmp(lpstr,(char*)lParam))
    return FALSE;
  return TRUE;
}


int UserIsLoggedOn(PowerSettings *ps)
{
  char shell[MAX_PATH];

  Log("UserIsLoggedOn start");
  if (!ReadRegistryString(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon","Shell",shell,MAX_PATH))
  {
    Log("Cannot read HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\Shell");
    Log("UserIsLoggedOn end, assume user is logged on");
    return 1;
  }
  Log("Check for program %s",shell);
  if (!EnumProcs(CheckProcessIsRunning,(LPARAM)shell))
  {
    Log("UserIsLoggedOn end, user is logged on");
    return 1;
  }
  Log("UserIsLoggedOn end, user is NOT logged on");
  return 0;
}

int DoExitWindowsEx(HWND hWnd,PowerSettings *ps)
{
  UINT mode;

  Log("DoExitWindowsEx start, action=%d, force=%d",ps->action,ps->options.force);
  switch (ps->action)
  {
    case LOGOFF:
      if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_NT)
      {
        if (!UserIsLoggedOn(ps))
        {
          WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Wanted to LOGOFF the user, but nobody was logged on");
          Log("DoExitWindowsEx end");
          return 0;
        }
      }
      mode=EWX_LOGOFF;
      break;
    case REBOOT:
      mode=EWX_REBOOT;
      break;
    case SHUTDOWN:
      mode=EWX_SHUTDOWN;
      break;
    case POWEROFF:
      if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
        mode=EWX_SHUTDOWN;
      else
        mode=EWX_POWEROFF;
      break;
  }
  if (ps->options.force)
    mode|=EWX_FORCE;

  Log("Warning=%d",ps->options.warning);
  if (ps->options.warning)
  {
    if (!ShowWarningMessage(hWnd,ps))
      return 1;
  }

  Log("Run_Program=%d",ps->options.run_program);
  if (ps->options.run_program)
  {
    RunProgram(hWnd,ps);
  }
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Initiating %s",GetModeStr(ps));

  if (ps->simulate)
    Error(ps,hWnd,"ExitWindowsEx");
  else if (!ExitWindowsEx(mode,0))
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  else
    WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"ExitWindowsEx executed succesfully!");
  Log("DoExitWindowsEx end");
  return 0;
}

int DoSetSystemPowerState(HWND hWnd,PowerSettings *ps)
{
  Log("DoSetSystemPowerState start");
  Log("Warning=%d",ps->options.warning);
  if (ps->options.warning)
  {
    if (!ShowWarningMessage(hWnd,ps))
      return 1;
  }

  Log("Run_Program=%d",ps->options.run_program);
  if (ps->options.run_program)
  {
    RunProgram(hWnd,ps);
  }
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Initiating %s",GetModeStr(ps));

  if (MySetSuspendState==NULL)
  {
    if (ps->simulate)
      Error(ps,hWnd,"SetSystemPowerState");
    else if (!SetSystemPowerState((ps->action==STANDBY)?TRUE:FALSE,ps->options.force?TRUE:FALSE))
    {
      DisplayLastError(ps,hWnd);
      return -1;
    }
    else
      WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"SetSystemPowerState executed succesfully!");
  }
  else
  {
    if (ps->simulate)
      Error(ps,hWnd,"SetSuspendState");
    else if (!MySetSuspendState((ps->action==STANDBY)?FALSE:TRUE,(ps->options.force)?TRUE:FALSE,FALSE))
    {
      /* I've read somewhere that win98 doesn't support this, so try the old method too */
      if (!SetSystemPowerState((ps->action==STANDBY)?TRUE:FALSE,ps->options.force?TRUE:FALSE))
      {
        DisplayLastError(ps,hWnd);
        return -1;
      }
      else
        WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"SetSystemPowerState executed succesfully!");
    }
    else
      WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"SetSuspendState executed succesfully!");
  }
  return 0;
}

int DoLockWorkstation(HWND hWnd,PowerSettings *ps)
{
  HINSTANCE lInst;
  FARPROC MyLockWorkstation;

  Log("DoLockWorkstation start");
  Log("Warning=%d",ps->options.warning);
  if (ps->options.warning)
  {
    if (!ShowWarningMessage(hWnd,ps))
      return 1;
  }

  Log("Run_Program=%d",ps->options.run_program);
  if (ps->options.run_program)
  {
    RunProgram(hWnd,ps);
  }
  if ((lInst=LoadLibrary("user32.dll"))==NULL)
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  MyLockWorkstation=GetProcAddress(lInst,"LockWorkStation");
  if (MyLockWorkstation==NULL)
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Initiating %s",GetModeStr(ps));
  if (ps->simulate)
    Error(ps,hWnd,"MyLockWorkstation");
  else if (!MyLockWorkstation())
  {
    DisplayLastError(ps,hWnd);
    return -1;
  }
  else
    WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"LockWorkStation executed succesfully!");
  FreeLibrary(lInst);
  Log("DoLockWorkstation end");
  return 0;
}

int DoInitiateSystemShutdown(HWND hWnd,PowerSettings *ps)
{
  Log("DoInitiateSystemShutdown start");
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Initiating %s on remote computer %s",GetModeStr(ps),ps->remote.computer_name);
  if (ps->simulate)
    Error(ps,hWnd,"InitiateSystemShutdown");
  else
  {
    NETRESOURCE net;
    char str[MAX_PATH];

    Log("current_credentials=%d",ps->remote.current_credentials);
    if (!ps->remote.current_credentials)
    {
      if (ps->remote.computer_name[0]!='\\')
        sprintf(str,"\\\\%s\\admin$",ps->remote.computer_name);
      else
        sprintf(str,"%s\\admin$",ps->remote.computer_name);

      net.dwScope=RESOURCE_GLOBALNET;
      net.dwType=RESOURCETYPE_ANY;
      net.dwDisplayType=RESOURCEDISPLAYTYPE_GENERIC;
      net.dwUsage=RESOURCEUSAGE_CONNECTABLE;
      net.lpLocalName=NULL;
      net.lpRemoteName=str;
      net.lpComment=NULL;
      net.lpProvider=NULL;
      if (WNetAddConnection2(&net,ps->remote.password,ps->remote.username,0)!=NO_ERROR)
      {
        DisplayLastError(ps,hWnd);
        return -1;
      }
    }
    Log("InitiateSystemShutdown");
    if (!InitiateSystemShutdown(ps->remote.computer_name,ps->warning.message,ps->warning.seconds,ps->options.force,(ps->action==REBOOT)?1:0))
    {
      DisplayLastError(ps,hWnd);
      return -1;
    }
    if (!ps->remote.current_credentials)
    {
      WNetCancelConnection2(str,0,FALSE);
    }
  }
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Remote computer has accepted the request");
  if (ps->interactive)
  {
    DisplayInformation(ps,hWnd,"Remote computer has accepted the request");
  }
  Log("DoInitiateSystemShutdown end");
  return 0;
}

int DoTurnOffMonitor(HWND hWnd,PowerSettings *ps)
{
  Log("DoTurnOffMonitor start");
  Log("Warning=%d",ps->options.warning);
  if (ps->options.warning)
  {
    if (!ShowWarningMessage(hWnd,ps))
      return 1;
  }

  Log("Run_Program=%d",ps->options.run_program);
  if (ps->options.run_program)
  {
    RunProgram(hWnd,ps);
  }
  if (!ps->interactive)
    Sleep(500);
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Initiating %s",GetModeStr(ps));
  if (ps->simulate)
    Error(ps,hWnd,"DoTurnOffMonitor");
  else
  {
    if (ps->action==MONITOR_OFF)
      SendMessage(hWnd,WM_SYSCOMMAND,SC_MONITORPOWER,2);
    else
      SendMessage(hWnd,WM_SYSCOMMAND,SC_MONITORPOWER,-1);
  }
  Log("DoTurnOffMonitor end");
  return 0;
}

int DoNoAction(HWND hWnd,PowerSettings *ps)
{
  Log("DoNoAction start");
  Log("Warning=%d",ps->options.warning);
  if (ps->options.warning)
  {
    if (!ShowWarningMessage(hWnd,ps))
      return 1;
  }

  Log("Run_Program=%d",ps->options.run_program);
  if (ps->options.run_program)
  {
    RunProgram(hWnd,ps);
  }
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Initiating %s",GetModeStr(ps));
  Log("DoNoAction end");
  return 0;
}

int DoWOL(HWND hWnd,PowerSettings *ps)
{
  Log("DoWOL start");
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Waking up remote computer %s",ps->remote.computer_name);
  if (ps->simulate)
    Error(ps,hWnd,"WakeOnLan");
  else if (WakeOnLan(ps,ps->remote.ip_address,ps->remote.subnet_mask,ps->remote.mac_address))
  {
    Error(ps,hWnd,"WakeOnLAN failed");
    return -1;
  }
  WriteEvent(ps,EVENTLOG_INFORMATION_TYPE,"Wake-On-LAN packet has been sent");
  if (ps->interactive)
  {
    DisplayInformation(ps,hWnd,"Wake-On-LAN packet has been sent");
  }
  Log("DoWOL end");
  return 0;
}

int PowerOff(HWND hWnd,PowerSettings *ps)
{
  int ret;

  Log("PowerOff start");
  ps->schedule.wait=60;
  Log("who=%d, use_nt=%d, action=%d",ps->who,ps->remote.use_nt,ps->action);
  if (ps->who==REMOTE_COMPUTER && !ps->remote.use_nt && ps->action!=WAKE_ON_LAN)
  {
    char str[500];

    if ((ret=SendRemoteCommand(hWnd,ps,str))!=0)
    {
      Log("ret=%d",ret);
      if (ret==-2)
        Error(ps,hWnd,str);
      return -1;
    }
    else
    {
      Log("PowerOff end");
      return 0;
    }
  }
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_NT)
  {
    if (AquirePrivileges(hWnd,ps)!=0)
    {
      return -1;
    }
  }
  switch (ps->action)
  {
    case LOGOFF:
    case REBOOT:
    case SHUTDOWN:
    case POWEROFF:
      if (ps->who==LOCAL_COMPUTER)
      {
        ret=DoExitWindowsEx(hWnd,ps);
      }
      else
        ret=DoInitiateSystemShutdown(hWnd,ps);
      break;
    case STANDBY:    
    case HIBERNATE:
      ret=DoSetSystemPowerState(hWnd,ps);
      break;
    case LOCK:
      ret=DoLockWorkstation(hWnd,ps);
      break;
    case WAKE_ON_LAN:
      ret=DoWOL(hWnd,ps);
      break;
    case MONITOR_OFF:
    case MONITOR_ON:
      ret=DoTurnOffMonitor(hWnd,ps);
      break;
    case NO_ACTION:
      ret=DoNoAction(hWnd,ps);
      break;
  }
  Log("PowerOff end");
  return ret;
}

void DoQuickAction(PowerSettings *ps,Action action)
{
  char actionstr[100];
  Action tmp;

  Log("DoQuickAction start");
  tmp=ps->action;
  ps->action=action;
  sprintf(actionstr,"Are you sure that you want to %s your pc now?",GetModeStr(ps));
  if (!ps->options.allow_cancel || MessageBox(NULL,actionstr,"Are you sure?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1)==IDYES)
  {
    ps->when=IMMEDIATE;
    ps->who=LOCAL_COMPUTER;
    ps->options.warning=0;
    ps->options.run_program=0;
    PowerOff(g_hWnd,ps);
  }
  else
    ps->action=tmp;
  Log("DoQuickAction end");
}

int DoIt(HWND hWnd,PowerSettings *ps)
{
  char str[500];

  Log("DoIt start");
  if (!SanityCheck(hWnd,ps,str))
  {
    if (ps->who==REMOTE_COMPUTER && ps->remote.use_nt==0 && (ps->when==IMMEDIATE || ps->remote.schedule_remote) && ps->action!=WAKE_ON_LAN)
    {
      HCURSOR c;

      c=SetCursor(LoadCursor(NULL, IDC_WAIT));
      if (SendRemoteCommand(hWnd,ps,str)==-2)
      {
        SetCursor(c);
        Error(ps,hWnd,str);
        return -1;
      }
      SetCursor(c);
    }
    else if (ps->when==IMMEDIATE)
    {
      HCURSOR c;

      c=SetCursor(LoadCursor(NULL, IDC_WAIT));
      PowerOff(hWnd,ps);
      SetCursor(c);
    }
    else if (ps->when==SCHEDULED)
    {
      ps->remaining_seconds=ps->schedule.seconds;
      ps->active_timer=SetTimer(hWnd,ID_SCHEDULE_TIMER,1000,NULL);
      MinimizePoweroff(hWnd,ps);
    }
    else if (ps->when==PROCESS)
    {
      ps->active_timer=SetTimer(hWnd,ID_PROCESS_TIMER,1000,NULL);
      MinimizePoweroff(hWnd,ps);
    }
  }
  else
  {
    Error(ps,hWnd,str);
    return -2;
  }
  Log("DoIt end");
  return 0;
}