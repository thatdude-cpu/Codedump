#include "poweroff.h"

#include <tlhelp32.h>
#include <vdmdbg.h>

typedef struct
{
  DWORD          dwPID ;
  PROCENUMPROC   lpProc ;
  DWORD          lParam ;
  BOOL           bEnd ;
} EnumInfoStruct ;

typedef PERF_DATA_BLOCK             PERF_DATA,      *PPERF_DATA;
typedef PERF_OBJECT_TYPE            PERF_OBJECT,    *PPERF_OBJECT;
typedef PERF_INSTANCE_DEFINITION    PERF_INSTANCE,  *PPERF_INSTANCE;
typedef PERF_COUNTER_DEFINITION     PERF_COUNTER,   *PPERF_COUNTER;


// ToolHelp Function Pointers.
HINSTANCE      hInstLib=NULL;
HANDLE (WINAPI *lpfCreateToolhelp32Snapshot)(DWORD,DWORD) ;
BOOL (WINAPI *lpfProcess32First)(HANDLE,LPPROCESSENTRY32) ;
BOOL (WINAPI *lpfProcess32Next)(HANDLE,LPPROCESSENTRY32) ;
BOOL (WINAPI *lpfModule32First)(HANDLE,LPMODULEENTRY32) ;
BOOL (WINAPI *lpfModule32Next)(HANDLE,LPMODULEENTRY32) ;

BOOL WINAPI Enum16( DWORD dwThreadId, WORD hMod16, WORD hTask16,
  PSZ pszModName, PSZ pszFileName, LPARAM lpUserDefined ) ;

BOOL GetProcessModule (DWORD dwPID, DWORD dwModuleID, 
     LPMODULEENTRY32 lpMe32, DWORD cbMe32) 
{ 
    BOOL          bRet        = FALSE; 
    BOOL          bFound      = FALSE; 
    HANDLE        hModuleSnap = NULL; 
    MODULEENTRY32 me32        = {0}; 
 
    // Take a snapshot of all modules in the specified process. 

    Log("GetProcessModule start");
    hModuleSnap = lpfCreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 
    if (hModuleSnap == (HANDLE)-1) 
    {
      Log("CreateToolhelp32Snapshot failed");
      Log("GetProcessModule end");
      return (FALSE); 
    }
 
    // Fill the size of the structure before using it. 

    me32.dwSize = sizeof(MODULEENTRY32); 
 
    // Walk the module list of the process, and find the module of 
    // interest. Then copy the information to the buffer pointed 
    // to by lpMe32 so that it can be returned to the caller. 

    if (lpfModule32First(hModuleSnap, &me32)) 
    { 
        do 
        { 
            if (me32.th32ModuleID == dwModuleID) 
            { 
                CopyMemory (lpMe32, &me32, cbMe32); 
                bFound = TRUE; 
            } 
        } 
        while (!bFound && lpfModule32Next(hModuleSnap, &me32)); 
 
        bRet = bFound;   // if this sets bRet to FALSE, dwModuleID 
                         // no longer exists in specified process 
    } 
    else 
        bRet = FALSE;           // could not walk module list 
 
    // Do not forget to clean up the snapshot object. 

    CloseHandle (hModuleSnap); 
 
    Log("GetProcessModule end (bret=%d)",(int)bRet);
    return (bRet); 
} 

PPERF_OBJECT FirstObject (PPERF_DATA pData)
{
  Log("FirstObject, pData=%p",pData);
  if (pData)
    return ((PPERF_OBJECT) ((PBYTE) pData + pData->HeaderLength));
  else
    return NULL;
}

PPERF_OBJECT NextObject (PPERF_OBJECT pObject)
{
  Log("NextObject, pData=%p",pObject);
  if (pObject)
    return ((PPERF_OBJECT) ((PBYTE) pObject + pObject->TotalByteLength));
  else
    return NULL;
}

PPERF_OBJECT FindObject (PPERF_DATA pData, DWORD TitleIndex)
{
  PPERF_OBJECT pObject;
  DWORD        i = 0;

  Log("FindObject start");
  if (pObject = FirstObject(pData))
  {
    Log("pData->NumObjectTypes=%d",(int)pData->NumObjectTypes);
    while (i < pData->NumObjectTypes)
    {
      Log("pObject->ObjectNameTitleIndex=%d, TitleIndex=%d",(int)pObject->ObjectNameTitleIndex,(int)TitleIndex);
      if (pObject->ObjectNameTitleIndex == TitleIndex)
      {
        Log("FindObject end, pObject=%p",pObject);
        return pObject;
      }
      pObject = NextObject(pObject);
      i++;
    }
  }
  Log("FindObject end");
  return NULL;
}

DWORD GetPerfData(HKEY        hPerfKey,
                  LPTSTR      szObjectIndex,
                  PPERF_DATA  *ppData,
                  DWORD       *pDataSize)
{
  DWORD   DataSize;
  DWORD   dwR;
  DWORD   Type;

  Log("GetPerfData start (szObjectIndex=%s,pDataSize=%d)",szObjectIndex,(int)*pDataSize);
  if (!*ppData)
    *ppData = (PPERF_DATA) LocalAlloc (LMEM_FIXED, *pDataSize);
  Log("ppData=%p",*ppData);

  do  
  {
    DataSize = *pDataSize;
    dwR = RegQueryValueEx (hPerfKey,
                           szObjectIndex,
                           NULL,
                           &Type,
                           (BYTE *)*ppData,
                           &DataSize);

    Log("dwR=%d",(int)dwR);
    if (dwR == ERROR_MORE_DATA)
    {
      LocalFree (*ppData);
      *pDataSize += 1024;
      *ppData = (PPERF_DATA) LocalAlloc (LMEM_FIXED, *pDataSize);
      Log("ppData=%p, size=%d",*ppData,(int)*pDataSize);
    }

    if (!*ppData)
    {
      LocalFree (*ppData);
      Log("GetPerfData end (ERROR_NOT_ENOUGH_MEMORY)");
      return ERROR_NOT_ENOUGH_MEMORY;
    }

  } while (dwR == ERROR_MORE_DATA);

  Log("GetPerfData end (dwR=%d)",(int)dwR);
  return dwR;
}


PPERF_DATA RefreshPerfData (HKEY        hPerfKey,
                            LPTSTR      szObjectIndex,
                            PPERF_DATA  pData,
                            DWORD       *pDataSize)
{
  Log("RefreshPerfData");
  if (GetPerfData (hPerfKey, szObjectIndex, &pData, pDataSize) == ERROR_SUCCESS)
    return pData;
  else
    return NULL;
}

DWORD   GetPerfTitleSz (HKEY    hKeyMachine,
                        HKEY    hKeyPerf,
                        LPTSTR  *TitleBuffer,
                        LPTSTR  *TitleSz[],
                        DWORD   *TitleLastIdx)
{
  HKEY	  hKey1;
  HKEY    hKey2;
  DWORD   Type;
  DWORD   DataSize;
  DWORD   dwR;
  DWORD   Len;
  DWORD   Index;
  DWORD   dwTemp;
  BOOL    bNT10;
  LPTSTR  szCounterValueName;
  LPTSTR  szTitle;

  Log("GetPerfTitleSz start");
  // Initialize
  //
  hKey1        = NULL;
  hKey2        = NULL;
  *TitleBuffer = NULL;
  *TitleSz     = NULL;

  // Open the perflib key to find out the last counter's index and system version.
  //
  dwR = RegOpenKeyEx (hKeyMachine,
                      TEXT("software\\microsoft\\windows nt\\currentversion\\perflib"),
                      0,
                      KEY_READ,
                      &hKey1);
  Log("RegOpenKeyEx: dwR=%d",(int)dwR);
  if (dwR != ERROR_SUCCESS)
      goto done;

  // Get the last counter's index so we know how much memory to allocate for TitleSz
  //
  DataSize = sizeof (DWORD);
  dwR = RegQueryValueEx (hKey1, TEXT("Last Counter"), 0, &Type, (LPBYTE)TitleLastIdx, &DataSize);
  Log("RegQueryValueEx(Last Counter): dwR=%d, TitleLastIdx=%d",(int)dwR,(int)*TitleLastIdx);
  if (dwR != ERROR_SUCCESS)
    goto done;



  // Find system version, for system earlier than 1.0a, there's no version value.
  //
  dwR = RegQueryValueEx (hKey1, TEXT("Version"), 0, &Type, (LPBYTE)&dwTemp, &DataSize);
  Log("RegQueryValueEx(Version): dwR=%d, dwTemp=%d",(int)dwR,(int)dwTemp);

  if (dwR != ERROR_SUCCESS)
    // unable to read the value, assume NT 1.0
    bNT10 = TRUE;
  else
    // found the value, so, NT 1.0a or later
    bNT10 = FALSE;

  // Now, get ready for the counter names and indexes.
  //
  if (bNT10)
  {
    // NT 1.0, so make hKey2 point to ...\perflib\009 and get
    //  the counters from value "Counters"
    //
    szCounterValueName = TEXT("Counters");
    dwR = RegOpenKeyEx (hKeyMachine,
                        TEXT("software\\microsoft\\windows nt\\currentversion\\perflib\\009"),
                        0,
                        KEY_READ,
                        &hKey2);
    Log("RegOpenKeyEx: dwR=%d",(int)dwR);
    if (dwR != ERROR_SUCCESS)
        goto done;
  }
  else
  {
    // NT 1.0a or later.  Get the counters in key HKEY_PERFORMANCE_KEY
    //  and from value "Counter 009"
    //
    szCounterValueName = TEXT("Counter 009");
    hKey2 = hKeyPerf;
  }

  // Find out the size of the data.
  //
  dwR = RegQueryValueEx (hKey2, szCounterValueName, 0, &Type, 0, &DataSize);
  Log("RegQueryValueEx(%s): dwR=%d, Type=%d",szCounterValueName,(int)dwR,(int)Type);
  if (dwR != ERROR_SUCCESS)
    goto done;

  // Allocate memory
  //
  *TitleBuffer = (LPTSTR)LocalAlloc (LMEM_FIXED, DataSize);
  if (!*TitleBuffer)
  {
    dwR = ERROR_NOT_ENOUGH_MEMORY;
    Log("ERROR_NOT_ENOUGH_MEMORY");
    goto done;
  }
  Log("TitleBuffer=%p",TitleBuffer);

  *TitleSz = (LPTSTR *)LocalAlloc (LPTR, (*TitleLastIdx+1) * sizeof (LPTSTR));
  if (!*TitleSz)
  {
    dwR = ERROR_NOT_ENOUGH_MEMORY;
    Log("ERROR_NOT_ENOUGH_MEMORY");
    goto done;
  }
  Log("TitleSz=%p",TitleSz);

  // Query the data
  //
  dwR = RegQueryValueEx (hKey2, szCounterValueName, 0, &Type, (BYTE *)*TitleBuffer, &DataSize);
  Log("RegQueryValueEx(%s): dwR=%d, TitleBuffer=%s",szCounterValueName,(int)dwR,*TitleBuffer);
  if (dwR != ERROR_SUCCESS)
    goto done;

  // Setup the TitleSz array of pointers to point to beginning of each title string.
  // TitleBuffer is type REG_MULTI_SZ.
  //
  szTitle = *TitleBuffer;

  while (Len = lstrlen (szTitle))
  {
    Index = atoi (szTitle);
    Log("Index=%d",(int)Index);
    szTitle = szTitle + Len +1;

    Log("Index=%d, *TitleLastIdx=%d",(int)Index,(int)*TitleLastIdx);
    if (Index <= *TitleLastIdx)
    {
      Log("Title=%s",szTitle);
      (*TitleSz)[Index] = szTitle;
    }
    szTitle = szTitle + lstrlen (szTitle) +1;
  }

done:

  Log("Done");
  // Done. Now cleanup!
  //
  if (dwR != ERROR_SUCCESS)
  {
    // There was an error, free the allocated memory
    //
    if (*TitleBuffer) LocalFree (*TitleBuffer);
    if (*TitleSz)     LocalFree (*TitleSz);
  }

  // Close the hKeys.
  //
  if (hKey1) 
    RegCloseKey (hKey1);
  if (hKey2 && hKey2 != hKeyPerf) 
    RegCloseKey (hKey2);

  Log("GetPerfTitleSz end");
  return dwR;
}


DWORD GetTitleIdx(char *name)
{
  DWORD   Index;
  LPTSTR  TitleBuffer;
  LPTSTR  *Title;
  DWORD   Last;
  DWORD   dwR;

  Log("GetTitleIdx start");
  dwR = GetPerfTitleSz (HKEY_LOCAL_MACHINE, HKEY_PERFORMANCE_DATA, &TitleBuffer, &Title, &Last);

  Log("dwR=%d",(int)dwR);
  if (dwR != ERROR_SUCCESS)
  {
    Log("GetTitleIdx start");
    return 0;
  }

  for (Index = 0; Index <= Last; Index++)
  {
    if (Title[Index])
    {
      Log("Title[%d]=%s, name=%s",Index,Title[Index], name);
      if (!lstrcmpi (Title[Index], name))
      {
        LocalFree (TitleBuffer);
        LocalFree (Title);
        Log("GetTitleIdx end (Index=%d)",(int)Index);
        return Index;
      }
    }
  }

  LocalFree (TitleBuffer);
  LocalFree (Title);
  Log("GetTitleIdx end");
  return 0;
}

PPERF_COUNTER FirstCounter (PPERF_OBJECT pObject)
{
  Log("FirstCounter, pObject=%p",pObject);
  if (pObject)
    return (PPERF_COUNTER)((PCHAR) pObject + pObject->HeaderLength);
  else
    return NULL;
}

PPERF_COUNTER NextCounter (PPERF_COUNTER pCounter)
{
  Log("NextCounter, pCounter=%p",pCounter);
  if (pCounter)
    return (PPERF_COUNTER)((PCHAR) pCounter + pCounter->ByteLength);
  else
    return NULL;
}

PPERF_COUNTER FindCounter (PPERF_OBJECT pObject, DWORD TitleIndex)
{
  PPERF_COUNTER pCounter;
  DWORD         i = 0;

  Log("FindCounter start, pObject=%p, TitleIndex=%d",pObject,(int)TitleIndex);
  if (pCounter = FirstCounter (pObject))
  {
    Log("pObject->NumCounters=%d",(int)pObject->NumCounters);
    while (i < pObject->NumCounters)
    {
      Log("pCounter->CounterNameTitleIndex=%d",(int)pCounter->CounterNameTitleIndex);
      if (pCounter->CounterNameTitleIndex == TitleIndex)
      {
        Log("FindCounter end (pCounter=%p)",pCounter);
        return pCounter;
      }

      pCounter = NextCounter (pCounter);
      i++;
    }
  }

  Log("FindCounter end");
  return NULL;
}

PPERF_INSTANCE FirstInstance (PPERF_OBJECT pObject)
{
  Log("FirstInstance (pObject=%p)",pObject);
  if (pObject)
    return (PPERF_INSTANCE)((PCHAR) pObject + pObject->DefinitionLength);
  else
    return NULL;
}

PPERF_INSTANCE NextInstance (PPERF_INSTANCE pInst)
{
  PERF_COUNTER_BLOCK *pCounterBlock;

  Log("NextInstance (pInst=%p)",pInst);
  if (pInst)
  {
    pCounterBlock = (PERF_COUNTER_BLOCK *)((PCHAR) pInst + pInst->ByteLength);
    return (PPERF_INSTANCE)((PCHAR) pCounterBlock + pCounterBlock->ByteLength);
  }
  else
    return NULL;
}

PVOID CounterData (PPERF_INSTANCE pInst, PPERF_COUNTER pCount)
{
  PPERF_COUNTER_BLOCK pCounterBlock;

  Log("CounterData (pInst=%p, pCount=%p)",pInst,pCount);
  if (pCount && pInst)
  {
    pCounterBlock = (PPERF_COUNTER_BLOCK)((PCHAR)pInst + pInst->ByteLength);
    return (PVOID)((PCHAR)pCounterBlock + pCount->CounterOffset);
  }
  else
    return NULL;
}

LPTSTR  InstanceName (PPERF_INSTANCE pInst)
{
  Log("InstanceName (pInst=%p)",pInst);
  if (pInst)
    return (LPTSTR) ((PCHAR) pInst + pInst->NameOffset);
  else
    return NULL;
}

// The EnumProcs function takes a pointer to a callback function
// that will be called once per process in the system providing
// process EXE filename and process ID.
// Callback function definition:
// BOOL CALLBACK Proc( DWORD dw, LPCSTR lpstr, LPARAM lParam ) ;
// 
// lpProc -- Address of callback routine.
// 
// lParam -- A user-defined LPARAM value to be passed to
//           the callback routine.
BOOL WINAPI EnumProcs( PROCENUMPROC lpProc, LPARAM lParam )
{
  OSVERSIONINFO  osver ;
  HANDLE         hSnapShot ;
  PROCESSENTRY32 procentry ;
  BOOL           bFlag ;


  Log("EnumProcs start");
  // Check to see if were running under Windows95 or
  // Windows NT.
  osver.dwOSVersionInfoSize = sizeof( osver ) ;
  if( !GetVersionEx( &osver ) )
  {
     Log("GetVersionEx failed, EnumProcs end");
     return FALSE ;
  }

  // If Windows NT:
  if( osver.dwPlatformId == VER_PLATFORM_WIN32_NT )
  {
    PPERF_INSTANCE  pInstance;
    PPERF_COUNTER   pCounterProcID;
    PPERF_OBJECT    pObject;
    INT             InstanceIndex = 0;
    PPERF_DATA      gpPerfData=NULL;
    TCHAR           INDEX_PROCTHRD_OBJ[20];
    DWORD           gPerfDataSize = 50*1024;
    DWORD           PX_PROCESS;
/*    DWORD           PX_THREAD;*/
    DWORD           *pdwProcID;
    char            procname[MAX_PATH];

    Log("Using NT method");
    PX_PROCESS = GetTitleIdx("Process");
    /*PX_THREAD  = GetTitleIdx("Thread");
    wsprintf (INDEX_PROCTHRD_OBJ, TEXT("%ld %ld"), PX_PROCESS, PX_THREAD);*/
    wsprintf (INDEX_PROCTHRD_OBJ, TEXT("%ld"), PX_PROCESS);
    gpPerfData = RefreshPerfData (HKEY_PERFORMANCE_DATA, INDEX_PROCTHRD_OBJ, gpPerfData, &gPerfDataSize);
    pObject = FindObject (gpPerfData, PX_PROCESS);
    if (pObject)
    {
      if ((pCounterProcID = FindCounter(pObject, GetTitleIdx("ID Process"))))
      {
        pInstance = FirstInstance (pObject);

        while (pInstance && InstanceIndex < pObject->NumInstances)
        {
          pdwProcID = (DWORD *) CounterData (pInstance, pCounterProcID);
          if (*pdwProcID)
          {
            sprintf(procname,"%S.exe",InstanceName(pInstance));
            Log("procname=%s",procname);
            if (!lpProc( *pdwProcID, 0, procname , lParam))
            {
              LocalFree(gpPerfData);
              Log("EnumProcs end");
              return FALSE ;
            }
          }
          pInstance = NextInstance (pInstance);
          InstanceIndex++;
        }
      }
    }
    LocalFree(gpPerfData);


  // If Windows 95:
  }
  else if( osver.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
  {

     Log("Using Win9x method");
     if (hInstLib==NULL)
     {
       hInstLib = LoadLibraryA( "Kernel32.DLL" ) ;
       if( hInstLib == NULL )
          return FALSE ;

       // Get procedure addresses.
       // We are linking to these functions of Kernel32
       // explicitly, because otherwise a module using
       // this code would fail to load under Windows NT,
       // which does not have the Toolhelp32
       // functions in the Kernel 32.
       lpfCreateToolhelp32Snapshot=
          (HANDLE(WINAPI *)(DWORD,DWORD))
          GetProcAddress( hInstLib,
          "CreateToolhelp32Snapshot" ) ;
       lpfProcess32First=
          (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
          GetProcAddress( hInstLib, "Process32First" ) ;
       lpfProcess32Next=
          (BOOL(WINAPI *)(HANDLE,LPPROCESSENTRY32))
          GetProcAddress( hInstLib, "Process32Next" ) ;
       lpfModule32First=
          (BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
          GetProcAddress( hInstLib, "Module32First" ) ;
       lpfModule32Next=
          (BOOL(WINAPI *)(HANDLE,LPMODULEENTRY32))
          GetProcAddress( hInstLib, "Module32Next" ) ;
       if(lpfProcess32Next == NULL ||
          lpfProcess32First == NULL ||
          lpfModule32Next == NULL ||
          lpfModule32First == NULL ||
          lpfCreateToolhelp32Snapshot == NULL )
       {
         Log("Error loading libraries");
          FreeLibrary( hInstLib );
          hInstLib=NULL;
          return FALSE ;
       }
     }

     // Get a handle to a Toolhelp snapshot of the systems
     // processes.
     hSnapShot = lpfCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
     if( hSnapShot == INVALID_HANDLE_VALUE )
     {
        Log("CreateToolhelp32Snapshot failed");
        return FALSE ;
     }

     // Get the first process' information.
     procentry.dwSize = sizeof(PROCESSENTRY32) ;
     bFlag = lpfProcess32First( hSnapShot, &procentry ) ;

     // While there are processes, keep looping.

     while( bFlag )
     {
       // Call the enum func with the filename and ProcID.

        MODULEENTRY32 me32 = {0}; 
 
        if (GetProcessModule(procentry.th32ProcessID, 
                procentry.th32ModuleID, &me32, sizeof(MODULEENTRY32)))
        {
           Log("szModule=%s",me32.szModule);
           if(lpProc( procentry.th32ProcessID, 0,
              me32.szModule, lParam ))
           {
             procentry.dwSize = sizeof(PROCESSENTRY32) ;
             bFlag = lpfProcess32Next( hSnapShot, &procentry );
           }
           else
           {
             CloseHandle(hSnapShot);
             Log("EnumProcs end");
             return FALSE;
           }
        }     
     }
     CloseHandle(hSnapShot);
  }
  else
  {
    Log("EnumProcs end (Unknown OS)");
     return FALSE ;
  }

  Log("EnumProcs end");
  return TRUE ;
}

BOOL WINAPI Enum16( DWORD dwThreadId, WORD hMod16, WORD hTask16,
  PSZ pszModName, PSZ pszFileName, LPARAM lpUserDefined )
{
  BOOL bRet ;

  EnumInfoStruct *psInfo = (EnumInfoStruct *)lpUserDefined ;

  Log("Enum16 start");
  bRet = psInfo->lpProc( psInfo->dwPID, hTask16, pszFileName,
     psInfo->lParam ) ;

  if(!bRet)
  {
     psInfo->bEnd = TRUE ;
  }

  Log("Enum16 end (bRet=%d)",(int)bRet);
  return !bRet;
} 