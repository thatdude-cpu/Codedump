#include "poweroff.h"

SERVICE_STATUS_HANDLE sh;
SERVICE_STATUS status;

int CopyPoweroffToSystem(HWND hWnd,PowerSettings *ps,char *system_poweroff)
{
  char system_dir[MAX_PATH],path[1024]="",original_poweroff[MAX_PATH];
  char *pos,*lastpos;
  HANDLE f=INVALID_HANDLE_VALUE;

  Log("CopyPoweroffToSystem start, executable=%s, system_poweroff=%s",ps->executable,system_poweroff);
  if (strlen(ps->executable)<4 || _stricmp(&ps->executable[strlen(ps->executable)-4],".exe"))
    strcat(ps->executable,".exe");

  if (!strchr(ps->executable,'\\'))
  {
    strcpy(original_poweroff,ps->executable);
    if ((f=CreateFile(ps->executable,
                      0,
                      FILE_SHARE_READ,
                      NULL,
                      OPEN_EXISTING,
                      0,
                      NULL))==INVALID_HANDLE_VALUE)
    {
      GetEnvironmentVariable("PATH",path,1024);
      lastpos=path;
      pos=strchr(lastpos,';');
      while (pos)
      {
        *pos='\0';
        sprintf(original_poweroff,"%s\\%s",lastpos,ps->executable);
        if ((f=CreateFile(original_poweroff,
                          0,
                          FILE_SHARE_READ,
                          NULL,
                          OPEN_EXISTING,
                          0,
                          NULL))!=INVALID_HANDLE_VALUE)
          break;
        lastpos=pos+1;
        pos=strchr(lastpos,';');
      }
      if (f==INVALID_HANDLE_VALUE)
      {
        sprintf(original_poweroff,"%s\\%s",lastpos,ps->executable);
        f=CreateFile(original_poweroff,
                     0,
                     FILE_SHARE_READ,
                     NULL,
                     OPEN_EXISTING,
                     0,
                     NULL);
      }
    }
  }
  else
  {
    strcpy(original_poweroff,ps->executable);
    f=CreateFile(original_poweroff,
                 0,
                 FILE_SHARE_READ,
                 NULL,
                 OPEN_EXISTING,
                 0,
                 NULL);
  }
  if (f==INVALID_HANDLE_VALUE)
  {
    Error(ps,NULL,"Can't find poweroff.exe!");
    return 0;
  }
  CloseHandle(f);
  
  if (GetSystemDirectory(system_dir,MAX_PATH)==0)
  {
    DisplayLastError(ps,hWnd);
    return 0;
  }
  sprintf(system_poweroff,"%s\\poweroff.exe",system_dir);

  if (_stricmp(original_poweroff,system_poweroff))
  {
    Log("Copying %s to %s",original_poweroff,system_poweroff);
    if (CopyFile(original_poweroff,system_poweroff,FALSE)==0)
    {
      DisplayLastError(ps,hWnd);
      CloseHandle(f);
      return 0;
    }
  }
  sprintf(path,"\"%s\" -service",system_poweroff);
  strcpy(system_poweroff,path);
  Log("CopyPoweroffToSystem end");
  return 1;
}

void CreatePoweroffService(HWND hWnd,PowerSettings *ps)
{
  char system_poweroff[MAX_PATH];
  SC_HANDLE hsm,sh;

  Log("CreatePoweroffService start");
  if (!CopyPoweroffToSystem(hWnd,ps,system_poweroff))
    return;
  SaveSettings(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters",ps);

  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
    Log("Creating WIN98 service");
    if (!WriteRegistryString(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServices","Poweroff",system_poweroff))
    {
      DisplayLastError(ps,hWnd);
      return;
    }
    DisplayInformation(ps,hWnd,"Please restart your computer to start the service");
  }
  else /* Create a real service */
  {
    Log("Creating real NT service");
    hsm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
    if (hsm==NULL)
    {
      DisplayLastError(ps,hWnd);
      return;
    }

    sh=CreateService(hsm,
                    "Poweroff",
                    "Poweroff",
                    STANDARD_RIGHTS_REQUIRED|SERVICE_START,
                    SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS,
                    SERVICE_AUTO_START,
                    SERVICE_ERROR_NORMAL,
                    system_poweroff,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    NULL);
    if (sh==NULL)
    {
      DisplayLastError(ps,hWnd);
      CloseServiceHandle(hsm);
      return;
    }
    Log("Starting service");
    if (StartService(sh,0,NULL)==0)
    {
      DisplayLastError(ps,hWnd);
      CloseServiceHandle(sh);
      CloseServiceHandle(hsm);
      return;
    }
    CloseServiceHandle(hsm);
    DisplayInformation(ps,hWnd,"Poweroff service succesfully installed");
  }
  Log("CreatePoweroffService end");
}

void RemovePoweroffService(HWND hWnd,PowerSettings *ps)
{
  SC_HANDLE hsm,sh;

  Log("RemovePoweroffService start");
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
    Log("Deleting WIN98 service");
    if (!DeleteRegistryValue(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\RunServices","Poweroff"))
    {
      DisplayLastError(ps,hWnd);
      return;
    }
    DisplayInformation(ps,hWnd,"Please restart your computer to stop the service");
  }
  else
  {
    Log("Deleting NT service");
    hsm=OpenSCManager(NULL,NULL,GENERIC_WRITE);
    if (hsm==NULL)
    {
      DisplayLastError(ps,hWnd);
      return;
    }
    sh=OpenService(hsm,"Poweroff",DELETE);
    if (sh==NULL)
    {
      DisplayLastError(ps,hWnd);
      CloseServiceHandle(hsm);
      return;
    }
    if (!DeleteService(sh))
    {
      DisplayLastError(ps,hWnd);
    }
    CloseServiceHandle(sh);
    CloseServiceHandle(hsm);
    DisplayInformation(ps,hWnd,"Poweroff service succesfully deleted");
  }
  DeleteRegistryKey(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters");
  Log("RemovePoweroffService end");
}

VOID WINAPI ServiceHandlerProcedure(DWORD fdwControl)
{
  Log("ServiceHandlerProcedure start, fdwControl=%d",fdwControl);
  switch (fdwControl)
  {
    case SERVICE_CONTROL_STOP:
      status.dwServiceType=SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
      status.dwCurrentState=SERVICE_STOPPED;
      status.dwControlsAccepted=SERVICE_ACCEPT_STOP;
      status.dwWin32ExitCode=NO_ERROR;
      status.dwCheckPoint++;
      status.dwWaitHint=1000;
      SetServiceStatus(sh,&status);
      RemoveFromTray(g_hWnd);
      Cleanup(NULL);
      SetTimer(g_hWnd,ID_STOP_TIMER,1000,NULL);
      break;
    case SERVICE_CONTROL_PAUSE:
      break;
    case SERVICE_CONTROL_CONTINUE:
      break;
    case SERVICE_CONTROL_INTERROGATE:
      status.dwServiceType=SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
      status.dwCurrentState=SERVICE_RUNNING;
      status.dwControlsAccepted=SERVICE_ACCEPT_STOP;
      status.dwWin32ExitCode=NO_ERROR;
      status.dwCheckPoint++;
      status.dwWaitHint=1000;
      SetServiceStatus(sh,&status);
      break;
    case SERVICE_CONTROL_SHUTDOWN:
      status.dwServiceType=SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
      status.dwCurrentState=SERVICE_STOPPED;
      status.dwControlsAccepted=SERVICE_ACCEPT_STOP;
      status.dwWin32ExitCode=NO_ERROR;
      status.dwCheckPoint++;
      status.dwWaitHint=1000;
      SetServiceStatus(sh,&status);
      SendMessage(g_hWnd,WM_QUIT,0,0);
      break;
  }
  Log("ServiceHandlerProcedure end");
}

VOID WINAPI ServiceProcedure(DWORD dwArgc,LPTSTR *lpszArgv)
{
  Log("ServiceProcedure start");
  sh=RegisterServiceCtrlHandler("Poweroff",ServiceHandlerProcedure);
  if (sh==0)
  {
    return;
  }
  status.dwServiceType=SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
  status.dwCurrentState=SERVICE_START_PENDING;
  status.dwControlsAccepted=SERVICE_ACCEPT_STOP;
  status.dwWin32ExitCode=NO_ERROR;
  status.dwCheckPoint=0;
  status.dwWaitHint=1000;
  SetServiceStatus(sh,&status);
  status.dwServiceType=SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
  status.dwCurrentState=SERVICE_RUNNING;
  status.dwControlsAccepted=SERVICE_ACCEPT_STOP;
  status.dwWin32ExitCode=NO_ERROR;
  status.dwCheckPoint=0;
  status.dwWaitHint=1000;
  SetServiceStatus(sh,&status);
  Log("Waiting for service to start");
  while (status.dwCurrentState==SERVICE_RUNNING)
    Sleep(500);
  Log("ServiceProcedure end");
} 

DWORD WINAPI CheckService(PowerSettings *ps)
{
  SERVICE_TABLE_ENTRY service_table[2];

  Log("CheckService start");
  if (ps->run_as_service && ps->windowsversion.dwPlatformId!=VER_PLATFORM_WIN32_WINDOWS)
  {
    Log("Starting as NT service");
    service_table[0].lpServiceName="Poweroff";
    service_table[0].lpServiceProc=ServiceProcedure;
    service_table[1].lpServiceName=NULL;
    service_table[1].lpServiceProc=NULL;
    StartServiceCtrlDispatcher(service_table);
  }
  else if (ps->run_as_service)
  {
    HMODULE hm;
    DWORD (WINAPI * RegisterServiceProcess)(DWORD dwProcessId,DWORD dwType);

    Log("Starting as WIN98 service");
    hm=LoadLibrary("kernel32");
    if (hm==NULL)
    {
      DisplayLastError(ps,NULL);
      return 0;
    }
    RegisterServiceProcess=(void*)GetProcAddress(hm,"RegisterServiceProcess");
    if (RegisterServiceProcess==NULL)
    {
      DisplayLastError(ps,NULL);
      return 0;
    }
    RegisterServiceProcess(0,1);
    FreeLibrary(hm);
  }
  Log("CheckService end");
  return 0;
}

int IsPoweroffServiceRunning(HWND hWnd,PowerSettings *ps)
{
  Log("IsPoweroffServiceRunning start");
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
    return 0;
  }
  else
  {
    SERVICE_STATUS ss;
    SC_HANDLE hsm,sh;

    hsm=OpenSCManager(NULL,NULL,GENERIC_READ);
    if (hsm==NULL)
    {
      return 0;
    }
    sh=OpenService(hsm,"Poweroff",SERVICE_QUERY_STATUS);
    if (sh==NULL)
    {
      CloseServiceHandle(hsm);
      return 0;
    }
    if (!QueryServiceStatus(sh,&ss))
    {
      CloseServiceHandle(sh);
      CloseServiceHandle(hsm);
      return 0;
    }
    CloseServiceHandle(sh);
    CloseServiceHandle(hsm);
    if (ss.dwCurrentState==SERVICE_RUNNING)
      return 1;
  }
  Log("IsPoweroffServiceRunning stop");
  return 0;
}

void StartPoweroffService(HWND hWnd,PowerSettings *ps)
{
  SC_HANDLE hsm,sh;

  Log("StartPoweroffService start");
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
  }
  else
  {
    hsm=OpenSCManager(NULL,NULL,GENERIC_WRITE);
    if (hsm==NULL)
    {
      DisplayLastError(ps,hWnd);
      return;
    }
    sh=OpenService(hsm,"Poweroff",SERVICE_START);
    if (sh==NULL)
    {
      DisplayLastError(ps,hWnd);
      CloseServiceHandle(hsm);
      return;
    }
    if (!StartService(sh,0,NULL))
    {
      DisplayLastError(ps,hWnd);
    }
    CloseServiceHandle(sh);
    CloseServiceHandle(hsm);
  }
  Log("StartPoweroffService end");
}

void StopPoweroffService(HWND hWnd,PowerSettings *ps)
{
  SC_HANDLE hsm,sh;

  Log("StopPoweroffService start");
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
  }
  else
  {
    SERVICE_STATUS ss;

    hsm=OpenSCManager(NULL,NULL,GENERIC_WRITE);
    if (hsm==NULL)
    {
      DisplayLastError(ps,hWnd);
      return;
    }
    sh=OpenService(hsm,"Poweroff",SERVICE_STOP);
    if (sh==NULL)
    {
      DisplayLastError(ps,hWnd);
      CloseServiceHandle(hsm);
      return;
    }
    if (!ControlService(sh,SERVICE_CONTROL_STOP,&ss))
    {
      DisplayLastError(ps,hWnd);
    }
    CloseServiceHandle(sh);
    CloseServiceHandle(hsm);
  }
  Log("StopPoweroffService end");
}
