#include "poweroff.h"

int SanityCheck(HWND hWnd,PowerSettings *ps,char *str)
{
  Log("SanityCheck start");
  if (ps->who==REMOTE_COMPUTER)
  {
    if (ps->action!=WAKE_ON_LAN)
    {
      if (ps->remote.use_nt)
      {
        if (ps->action!=SHUTDOWN && ps->action!=REBOOT)
        {
          strcpy(str,"Unsupported action for specified remote protocol");
          return 1;
        }
      }
      if (ps->remote.computer_name[0]=='\0')
      {
        strcpy(str,"No remote computer specified");
        return 1;
      }
    }
    else
    {
      if (ps->remote.ip_address[0]=='\0')
      {
        strcpy(str,"No ip address specified");
        return 1;
      }
      if (ps->remote.mac_address[0]=='\0')
      {
        strcpy(str,"No MAC address specified");
        return 1;
      }
      if (ps->remote.subnet_mask[0]=='\0')
      {
        strcpy(str,"No subnet mask specified");
        return 1;
      }
    }
  }
  else if (ps->action==WAKE_ON_LAN)
  {
    strcpy(str,"WAKE-On-LAN requires remote computer");
    return 1;
  }
  if (ps->options.warning && ps->warning.play_sound && ps->warning.sound_file[0]=='\0')
  {
    strcpy(str,"No sound file specified");
    return 1;
  }
  if (ps->options.run_program && ps->program.program[0]=='\0')
  {
    strcpy(str,"No program specified");
    return 1;
  }
  if (ps->when==SCHEDULED && ps->schedule.time.wHour==25 && ps->schedule.schedule!=AFTER_X_SECONDS)
  {
    strcpy(str,"No time specified");
    return 1;
  }
  if (ps->when==SCHEDULED && ps->schedule.schedule==FIXED_DAY && ps->schedule.time.wYear==0)
  {
    strcpy(str,"No date specified");
    return 1;
  }
  if (ps->when==SCHEDULED && ps->schedule.schedule==DAILY && ps->schedule.monday==0 && ps->schedule.tuesday==0 && ps->schedule.wednesday==0 && ps->schedule.thursday==0 && ps->schedule.friday==0 && ps->schedule.saturday==0 && ps->schedule.sunday==0)
  {
    strcpy(str,"No day of the week specified");
    return 1;
  }
  if (ps->when==SCHEDULED && ps->schedule.schedule==DAILY && ps->schedule.week1==0 &&ps->schedule.week2==0 &&ps->schedule.week3==0 &&ps->schedule.week4==0 &&ps->schedule.week5==0)
  {
    strcpy(str,"No week specified");
    return 1;
  }
  if (ps->when==SCHEDULED && ps->schedule.schedule==DAY_OF_MONTH && (ps->schedule.day<=0 || ps->schedule.day>31))
  {
    strcpy(str,"Invalid day specified");
    return 1;
  }
  if (ps->when==SCHEDULED && ps->schedule.schedule==AFTER_X_SECONDS && ps->schedule.seconds<0)
  {
    strcpy(str,"Number of seconds < 0");
    return 1;
  }
  if (ps->when==PROCESS && ps->process.process[0]=='\0')
  {
    strcpy(str,"No process specified");
    return 1;
  }
  if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
    if (ps->action==LOCK)
    {
      strcpy(str,"Action not supported by this windows version");
      return 1;
    }
  }
  else if (ps->windowsversion.dwPlatformId==VER_PLATFORM_WIN32_NT
        && ps->windowsversion.dwMajorVersion<5)
  {
    if (ps->action==LOCK)
    {
      strcpy(str,"Action not supported by this windows version");
      return 1;
    }
  }
  Log("SanityCheck end");
  return 0;
}