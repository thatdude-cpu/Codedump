#include "poweroff.h"

void CheckScheduleTimer(HWND hWnd,PowerSettings *ps)
{
  SYSTEMTIME systime;

  Log("CheckScheduleTimer start, wait=%d",ps->schedule.wait);
  if (ps->schedule.wait)
    ps->schedule.wait--;
  else
  {
    GetLocalTime(&systime);
    switch (ps->schedule.schedule)
    {
      case FIXED_DAY:
        Log("FIXED DAY, hour=%d, minute=%d, day=%d, month=%d, year=%d",ps->schedule.time.wHour,ps->schedule.time.wMinute,ps->schedule.date.wDay,ps->schedule.date.wMonth,ps->schedule.date.wYear);
        Log("systime, hour=%d, minute=%d, day=%d, month=%d, year=%d",systime.wHour,systime.wMinute,systime.wDay,systime.wMonth,systime.wYear);
        if (ps->schedule.time.wHour==systime.wHour && 
	          ps->schedule.time.wMinute==systime.wMinute &&
	          ps->schedule.date.wDay==systime.wDay &&
	          ps->schedule.date.wMonth==systime.wMonth &&
	          ps->schedule.date.wYear==systime.wYear)
        {
          PowerOff(hWnd,ps);
        }
        break;
      case DAILY:
        Log("DAILY, hour=%d, minute=%d, day=%d, month=%d, year=%d",ps->schedule.time.wHour,ps->schedule.time.wMinute,ps->schedule.date.wDay,ps->schedule.date.wMonth,ps->schedule.date.wYear);
        Log("systime, hour=%d, minute=%d, day=%d, month=%d, year=%d",systime.wHour,systime.wMinute,systime.wDay,systime.wMonth,systime.wYear);
        Log("monday=%d, tuesday=%d, wednesday=%d, thursday=%d, friday=%d, saturday=%d, sunday=%d",ps->schedule.monday,ps->schedule.tuesday,ps->schedule.wednesday,ps->schedule.thursday,ps->schedule.friday,ps->schedule.saturday,ps->schedule.sunday);
        Log("DayOfWeek=%d, Day=%d",systime.wDayOfWeek,(systime.wDay-1)/7+1);
        Log("week1=%d, week2=%d, week3=%d, week4=%d, week5=%d",ps->schedule.week1,ps->schedule.week2,ps->schedule.week3,ps->schedule.week4,ps->schedule.week5);
        if (ps->schedule.time.wHour==systime.wHour && 
	          ps->schedule.time.wMinute==systime.wMinute &&
            ((ps->schedule.monday && systime.wDayOfWeek==1) ||
             (ps->schedule.tuesday && systime.wDayOfWeek==2) ||
             (ps->schedule.wednesday && systime.wDayOfWeek==3) ||
             (ps->schedule.thursday && systime.wDayOfWeek==4) ||
             (ps->schedule.friday && systime.wDayOfWeek==5) ||
             (ps->schedule.saturday && systime.wDayOfWeek==6) ||
             (ps->schedule.sunday && systime.wDayOfWeek==0)) &&
            (((systime.wDay-1)/7+1==1 && ps->schedule.week1) ||
             ((systime.wDay-1)/7+1==2 && ps->schedule.week2) ||
             ((systime.wDay-1)/7+1==3 && ps->schedule.week3) ||
             ((systime.wDay-1)/7+1==4 && ps->schedule.week4) ||
             ((systime.wDay-1)/7+1==5 && ps->schedule.week5)))
        {
          PowerOff(hWnd,ps);
        }
        break;
      case DAY_OF_MONTH:
        Log("DAY_OF_MONTH: day=%d, hour=%d, minute=%d",ps->schedule.day,ps->schedule.time.wHour,ps->schedule.time.wMinute);
        Log("systime: day=%d, hour=%d, minute=%d",systime.wDay,systime.wHour,systime.wMinute);
        if (ps->schedule.time.wHour==systime.wHour && 
	          ps->schedule.time.wMinute==systime.wMinute &&
	          ps->schedule.day==systime.wDay)
        {
          PowerOff(hWnd,ps);
        }
        break;
      case AFTER_X_SECONDS:
        Log("AFTER_X_SECONDS: remaining=%d",ps->remaining_seconds);
        ps->remaining_seconds--;
        if (ps->remaining_seconds<=0)
          PowerOff(hWnd,ps);
    }           
  }
  Log("CheckScheduleTimer end");
}

void RoundToHour(SYSTEMTIME *st)
{
  FILETIME ft;
  LARGE_INTEGER li;

  Log("RoundToHour start");
  st->wMilliseconds=0;
  st->wSecond=0;
  st->wMinute=0;
  SystemTimeToFileTime(st,&ft);
  li.LowPart=ft.dwLowDateTime;
  li.HighPart=ft.dwHighDateTime;
  li.QuadPart+=(LONGLONG)10000000*(LONGLONG)60*(LONGLONG)60;
  ft.dwLowDateTime=li.LowPart;
  ft.dwHighDateTime=li.HighPart;
  FileTimeToSystemTime(&ft,st);
  Log("RoundToHour end");
}

void EnableScheduleFields(HWND hWnd)
{
  Log("EnableScheduleFields start");
  if (IsDlgButtonChecked(hWnd,IDC_FIXED_DAY))
  {
    EnableWindow(GetDlgItem(hWnd,IDC_TIME),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_DATE),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_TUESDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEDNESDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_THURSDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_FRIDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_SATURDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_SUNDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK1),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK2),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK3),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK4),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK5),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONTH_DAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_AFTER_SECONDS),FALSE);
  }
  else if (IsDlgButtonChecked(hWnd,IDC_FIXED_DAYS))
  {
    EnableWindow(GetDlgItem(hWnd,IDC_TIME),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_DATE),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_TUESDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEDNESDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_THURSDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_FRIDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_SATURDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_SUNDAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK1),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK2),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK3),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK4),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK5),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONTH_DAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_AFTER_SECONDS),FALSE);
  }
  else if (IsDlgButtonChecked(hWnd,IDC_FIXED_MONTH_DAY))
  {
    EnableWindow(GetDlgItem(hWnd,IDC_TIME),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_DATE),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_TUESDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEDNESDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_THURSDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_FRIDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_SATURDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_SUNDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK1),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK2),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK3),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK4),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK5),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONTH_DAY),TRUE);
    EnableWindow(GetDlgItem(hWnd,IDC_AFTER_SECONDS),FALSE);
  }
  else if (IsDlgButtonChecked(hWnd,IDC_AFTER_X_SECONDS))
  {
    EnableWindow(GetDlgItem(hWnd,IDC_TIME),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_DATE),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_TUESDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEDNESDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_THURSDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_FRIDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_SATURDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_SUNDAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK1),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK2),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK3),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK4),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_WEEK5),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_MONTH_DAY),FALSE);
    EnableWindow(GetDlgItem(hWnd,IDC_AFTER_SECONDS),TRUE);
  }
  Log("EnableScheduleFields end");
}

void ShowCurrentTime(HWND hWnd)
{
  SYSTEMTIME systime;
  char str[20];

  Log("ShowCurrentTime start");
  GetLocalTime(&systime);
  sprintf(str,"%02d:%02d:%02d",systime.wHour,systime.wMinute,systime.wSecond);
  SetDlgItemText(hWnd,IDC_CURRENT_TIME,str);
  Log("ShowCurrentTime end, time=%s",str);
}

BOOL FAR PASCAL ScheduleProc(HWND hWnd, unsigned message,DWORD wParam, LONG lParam)
{
  static PowerSettings *ps;

  switch (message) 
  {
    case WM_INITDIALOG:
      {
        SYSTEMTIME systime;
        char str[20];

        ps=(PowerSettings*)lParam;
        DateTime_SetFormat(GetDlgItem(hWnd,IDC_TIME),"HH:mm");
        DateTime_SetFormat(GetDlgItem(hWnd,IDC_DATE),"dd/MM/yyy");
        GetLocalTime(&systime);
        RoundToHour(&systime);
        if (ps->schedule.time.wHour==25)
          DateTime_SetSystemtime(GetDlgItem(hWnd,IDC_TIME),GDT_VALID ,&systime);
        else
          DateTime_SetSystemtime(GetDlgItem(hWnd,IDC_TIME),GDT_VALID ,&ps->schedule.time);
        if (ps->schedule.date.wYear==0)
          DateTime_SetSystemtime(GetDlgItem(hWnd,IDC_DATE),GDT_VALID ,&systime);
        else
          DateTime_SetSystemtime(GetDlgItem(hWnd,IDC_DATE),GDT_VALID ,&ps->schedule.date);
        CheckDlgButton(hWnd,IDC_MONDAY,ps->schedule.monday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_TUESDAY,ps->schedule.tuesday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_WEDNESDAY,ps->schedule.wednesday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_THURSDAY,ps->schedule.thursday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_FRIDAY,ps->schedule.friday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_SATURDAY,ps->schedule.saturday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_SUNDAY,ps->schedule.sunday?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_WEEK1,ps->schedule.week1?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_WEEK2,ps->schedule.week2?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_WEEK3,ps->schedule.week3?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_WEEK4,ps->schedule.week4?BST_CHECKED:BST_UNCHECKED);
        CheckDlgButton(hWnd,IDC_WEEK5,ps->schedule.week5?BST_CHECKED:BST_UNCHECKED);
        sprintf(str,"%d",ps->schedule.day);
        SetDlgItemText(hWnd,IDC_MONTH_DAY,str);
        sprintf(str,"%d",ps->schedule.seconds);
        SetDlgItemText(hWnd,IDC_AFTER_SECONDS,str);
        switch (ps->schedule.schedule)
        {
          case FIXED_DAY:
            CheckRadioButton(hWnd,IDC_FIXED_DAY,IDC_AFTER_X_SECONDS,IDC_FIXED_DAY);
            break;
          case DAILY:
            CheckRadioButton(hWnd,IDC_FIXED_DAY,IDC_AFTER_X_SECONDS,IDC_FIXED_DAYS);
            break;
          case DAY_OF_MONTH:
            CheckRadioButton(hWnd,IDC_FIXED_DAY,IDC_AFTER_X_SECONDS,IDC_FIXED_MONTH_DAY);
            break;
          case AFTER_X_SECONDS:
            CheckRadioButton(hWnd,IDC_FIXED_DAY,IDC_AFTER_X_SECONDS,IDC_AFTER_X_SECONDS);
            break;
        }
        EnableScheduleFields(hWnd);
        sprintf(str,"%d",ps->schedule.wait);
        SetDlgItemText(hWnd,IDC_WAITTIME,str);
        ShowCurrentTime(hWnd);
        SetTimer(hWnd,ID_TIME_TIMER,1000,NULL);
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
            char str[20];

            DateTime_GetSystemtime(GetDlgItem(hWnd,IDC_TIME),&ps->schedule.time);
            if (IsDlgButtonChecked(hWnd,IDC_FIXED_DAY))
            {
              DateTime_GetSystemtime(GetDlgItem(hWnd,IDC_DATE),&ps->schedule.date);
              ps->schedule.schedule=FIXED_DAY;
            }
            else if (IsDlgButtonChecked(hWnd,IDC_FIXED_DAYS))
            {
              ps->schedule.monday=IsDlgButtonChecked(hWnd,IDC_MONDAY);
              ps->schedule.tuesday=IsDlgButtonChecked(hWnd,IDC_TUESDAY);
              ps->schedule.wednesday=IsDlgButtonChecked(hWnd,IDC_WEDNESDAY);
              ps->schedule.thursday=IsDlgButtonChecked(hWnd,IDC_THURSDAY);
              ps->schedule.friday=IsDlgButtonChecked(hWnd,IDC_FRIDAY);
              ps->schedule.saturday=IsDlgButtonChecked(hWnd,IDC_SATURDAY);
              ps->schedule.sunday=IsDlgButtonChecked(hWnd,IDC_SUNDAY);
              ps->schedule.week1=IsDlgButtonChecked(hWnd,IDC_WEEK1);
              ps->schedule.week2=IsDlgButtonChecked(hWnd,IDC_WEEK2);
              ps->schedule.week3=IsDlgButtonChecked(hWnd,IDC_WEEK3);
              ps->schedule.week4=IsDlgButtonChecked(hWnd,IDC_WEEK4);
              ps->schedule.week5=IsDlgButtonChecked(hWnd,IDC_WEEK5);
			        if (!ps->schedule.week1 && !ps->schedule.week2 && !ps->schedule.week3 && !ps->schedule.week4 && !ps->schedule.week5)
              {
                Error(ps,hWnd,"You must select at least one week");
                return 0;
              }
              if (ps->schedule.monday==0 && ps->schedule.tuesday==0 && ps->schedule.wednesday==0 && ps->schedule.thursday==0 && ps->schedule.friday==0 && ps->schedule.saturday==0 && ps->schedule.sunday==0)
              {
                Error(ps,hWnd,"You must select at least one day of the week");
                return 0;
              }
              ps->schedule.date.wYear=0;
              ps->schedule.schedule=DAILY;
              DisplayInformation(ps,hWnd,"Please make sure that you create or update the poweroff service, so that the action can be performed according to your schedule");
            }
            else if (IsDlgButtonChecked(hWnd,IDC_FIXED_MONTH_DAY))
            {
              GetDlgItemText(hWnd,IDC_MONTH_DAY,str,20);
              ps->schedule.day=atoi(str);
              ps->schedule.schedule=DAY_OF_MONTH;
              ps->schedule.date.wYear=0;
              DisplayInformation(ps,hWnd,"Please make sure that you create or update the poweroff service, so that the action can be performed according to your schedule");
            }
            else if (IsDlgButtonChecked(hWnd,IDC_AFTER_X_SECONDS))
            {
              GetDlgItemText(hWnd,IDC_AFTER_SECONDS,str,20);
              ps->schedule.seconds=atoi(str);
              ps->schedule.schedule=AFTER_X_SECONDS;
            }
            GetDlgItemText(hWnd,IDC_WAITTIME,str,20);
            ps->schedule.wait=atoi(str);
            EndDialog(hWnd,TRUE);
          }
          break;
        case IDC_FIXED_DAY:
        case IDC_FIXED_DAYS:
        case IDC_FIXED_MONTH_DAY:
        case IDC_AFTER_X_SECONDS:
          EnableScheduleFields(hWnd);
          break;
      }
      break;
    case WM_TIMER:
      ShowCurrentTime(hWnd);
      break;
  	default:
 	    break;
  }
  return FALSE;
}

void ShowScheduleDialog(HWND hWnd,PowerSettings *ps)
{
  FARPROC dlgproc;

  Log("ShowScheduleDialog start");
  dlgproc=MakeProcInstance((FARPROC)ScheduleProc,hInst);
	if (DialogBoxParam(hInst,MAKEINTRESOURCE(IDD_SCHEDULE),hWnd,(DLGPROC)dlgproc,(LPARAM)ps)==-1)
    DisplayLastError(ps,hWnd);
	FreeProcInstance(dlgproc);
  Log("ShowScheduleDialog end");
}