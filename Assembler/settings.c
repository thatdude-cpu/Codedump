#include "poweroff.h"

#define CHKREGISTRY(f) if (!(f)) { return 0;}

void InitializeSettings(PowerSettings *ps)
{
  memset(ps,0,sizeof(PowerSettings));
  ps->action=POWEROFF;
  ps->schedule.schedule=FIXED_DAY;
  GetLocalTime(&ps->schedule.date);
  ps->when=IMMEDIATE;
  ps->who=LOCAL_COMPUTER;
  ps->schedule.time.wHour=25;
  ps->schedule.date.wYear=0;
  ps->interactive=1;
  ps->serversocket=INVALID_SOCKET;
  ps->schedule.day=1;
  ps->options.allow_cancel=1;
  ps->options.in_tray=1;
  strcpy(ps->remote.mac_address,"000000000000");
  ps->remote.current_credentials=1;
  ps->remote.port=LISTEN_PORT;
  ps->schedule.week1=1;
  ps->schedule.week2=1;
  ps->schedule.week3=1;
  ps->schedule.week4=1;
  ps->schedule.week5=1;
  ps->windowsversion.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
  if (!GetVersionEx(&ps->windowsversion))
  {
    DisplayLastError(ps,NULL);
  }
}

void ReadDefaultSettings(PowerSettings *ps)
{
  if (!ps->run_as_service)
  {
    if (!ReadSettings(HKEY_CURRENT_USER,"SOFTWARE\\JoBo\\Poweroff",ps))
      if (!ReadSettings(HKEY_LOCAL_MACHINE,"SOFTWARE\\JoBo\\Poweroff",ps))
        if (!ReadSettings(HKEY_CURRENT_USER,"SOFTWARE\\Poweroff",ps))
          ReadSettings(HKEY_LOCAL_MACHINE,"SOFTWARE\\Poweroff",ps);
  }
  else
  {
    ReadSettings(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services\\Poweroff\\Parameters",ps);
  }
  if (ps->remote.port<=0)
    ps->remote.port=LISTEN_PORT;
  if (ps->remote_control.port<=0)
    ps->remote_control.port=LISTEN_PORT;
}

int SaveSettings(HKEY hive,char *base,PowerSettings *ps)
{
  char str[20];

  Log("SaveSettings start, base=%s",base);
  CHKREGISTRY(WriteRegistryInteger(hive,base,"action",ps->action));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"who",ps->who));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"when",ps->when));
  CHKREGISTRY(WriteRegistryString(hive,base,"computer_name",ps->remote.computer_name));
  CHKREGISTRY(WriteRegistryString(hive,base,"mac_address",ps->remote.mac_address));
  CHKREGISTRY(WriteRegistryString(hive,base,"ip_address",ps->remote.ip_address));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"remote_use_nt",ps->remote.use_nt));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"remote_schedule",ps->remote.schedule_remote));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"remote_port",ps->remote.port));
  if (ps->remote.save_password)
  {
    CHKREGISTRY(WriteRegistryString(hive,base,"remote_password",ps->remote.password));
  }
  else
  {
    CHKREGISTRY(WriteRegistryString(hive,base,"remote_password",""));
  }
  CHKREGISTRY(WriteRegistryInteger(hive,base,"save_password",ps->remote.save_password));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"current_credentials",ps->remote.current_credentials));
  CHKREGISTRY(WriteRegistryString(hive,base,"remote_username",ps->remote.username));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"remote_control_port",ps->remote_control.port));
  CHKREGISTRY(WriteRegistryString(hive,base,"remote_control_password",ps->remote_control.password));
  CHKREGISTRY(WriteRegistryString(hive,base,"subnet_mask",ps->remote.subnet_mask));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"seconds",ps->warning.seconds));
  CHKREGISTRY(WriteRegistryString(hive,base,"message",ps->warning.message));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"play_sound",ps->warning.play_sound));
  CHKREGISTRY(WriteRegistryString(hive,base,"sound_file",ps->warning.sound_file));
  CHKREGISTRY(WriteRegistryString(hive,base,"program",ps->program.program));
  CHKREGISTRY(WriteRegistryString(hive,base,"directory",ps->program.directory));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"schedule",ps->schedule.schedule));
  sprintf(str,"%02d:%02d",ps->schedule.time.wHour,ps->schedule.time.wMinute);
  CHKREGISTRY(WriteRegistryString(hive,base,"time",str));
  sprintf(str,"%d/%d/%d",ps->schedule.date.wDay,ps->schedule.date.wMonth,ps->schedule.date.wYear);
  CHKREGISTRY(WriteRegistryString(hive,base,"date",str));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"monday",ps->schedule.monday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"tuesday",ps->schedule.tuesday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"wednesday",ps->schedule.wednesday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"thursday",ps->schedule.thursday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"friday",ps->schedule.friday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"saturday",ps->schedule.saturday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"sunday",ps->schedule.sunday));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"week1",ps->schedule.week1));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"week2",ps->schedule.week2));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"week3",ps->schedule.week3));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"week4",ps->schedule.week4));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"week5",ps->schedule.week5));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"day",ps->schedule.day));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"wait",ps->schedule.wait));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"after_seconds",ps->schedule.seconds));
  CHKREGISTRY(WriteRegistryString(hive,base,"process",ps->process.process));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"warning",ps->options.warning));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"run_program",ps->options.run_program));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"force",ps->options.force));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"in_tray",ps->options.in_tray));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"allow_cancel",ps->options.allow_cancel));
  CHKREGISTRY(WriteRegistryInteger(hive,base,"allow_remote_control",ps->options.allow_remote_control));
  Log("SaveSettings end");
  return 1;
}

int ReadSettings(HKEY hive,char *base,PowerSettings *ps)
{
  char str[20];
  int hour,minute,day,month,year;

  Log("ReadSettings start, base=%s",base);
  CHKREGISTRY(ReadRegistryInteger(hive,base,"action",&ps->action));
  ReadRegistryInteger(hive,base,"who",&ps->who);
  ReadRegistryShort(hive,base,"debug",&debug);
  ReadRegistryInteger(hive,base,"when",&ps->when);
  ReadRegistryString(hive,base,"computer_name",ps->remote.computer_name,100);
  ReadRegistryString(hive,base,"mac_address",ps->remote.mac_address,14);
  ReadRegistryString(hive,base,"ip_address",ps->remote.ip_address,20);
  ReadRegistryString(hive,base,"subnet_mask",ps->remote.subnet_mask,20);
  ReadRegistryShort(hive,base,"remote_use_nt",&ps->remote.use_nt);
  ReadRegistryShort(hive,base,"remote_schedule",&ps->remote.schedule_remote);
  ReadRegistryInteger(hive,base,"remote_port",&ps->remote.port);
  ReadRegistryString(hive,base,"remote_password",ps->remote.password,40);
  ReadRegistryShort(hive,base,"current_credentials",&ps->remote.current_credentials);
  ReadRegistryString(hive,base,"remote_username",ps->remote.username,40);
  ReadRegistryInteger(hive,base,"remote_control_port",&ps->remote_control.port);
  ReadRegistryString(hive,base,"remote_control_password",ps->remote_control.password,40);
  ReadRegistryShort(hive,base,"save_password",&ps->remote.save_password);
  ReadRegistryInteger(hive,base,"seconds",&ps->warning.seconds);
  ReadRegistryString(hive,base,"message",ps->warning.message,200);
  ReadRegistryShort(hive,base,"play_sound",&ps->warning.play_sound);
  ReadRegistryString(hive,base,"sound_file",ps->warning.sound_file,MAX_PATH);
  ReadRegistryString(hive,base,"program",ps->program.program,MAX_PATH);
  ReadRegistryString(hive,base,"directory",ps->program.directory,MAX_PATH);
  ReadRegistryInteger(hive,base,"schedule",&ps->schedule.schedule);
  ReadRegistryString(hive,base,"time",str,20);
  GetLocalTime(&ps->schedule.time);
  sscanf(str,"%d:%d",&hour,&minute);
  ps->schedule.time.wHour=hour;
  ps->schedule.time.wMinute=minute;
  ReadRegistryString(hive,base,"date",str,20);
  GetLocalTime(&ps->schedule.date);
  sscanf(str,"%d/%d/%d",&day,&month,&year);
  ps->schedule.date.wDay=day;
  ps->schedule.date.wMonth=month;
  ps->schedule.date.wYear=year;
  ReadRegistryShort(hive,base,"monday",&ps->schedule.monday);
  ReadRegistryShort(hive,base,"tuesday",&ps->schedule.tuesday);
  ReadRegistryShort(hive,base,"wednesday",&ps->schedule.wednesday);
  ReadRegistryShort(hive,base,"thursday",&ps->schedule.thursday);
  ReadRegistryShort(hive,base,"friday",&ps->schedule.friday);
  ReadRegistryShort(hive,base,"saturday",&ps->schedule.saturday);
  ReadRegistryShort(hive,base,"sunday",&ps->schedule.sunday);
  ReadRegistryShort(hive,base,"week1",&ps->schedule.week1);
  ReadRegistryShort(hive,base,"week2",&ps->schedule.week2);
  ReadRegistryShort(hive,base,"week3",&ps->schedule.week3);
  ReadRegistryShort(hive,base,"week4",&ps->schedule.week4);
  ReadRegistryShort(hive,base,"week5",&ps->schedule.week5);
  ReadRegistryInteger(hive,base,"day",&ps->schedule.day);
  ReadRegistryInteger(hive,base,"wait",&ps->schedule.wait);
  ReadRegistryInteger(hive,base,"after_seconds",&ps->schedule.seconds);
  ReadRegistryString(hive,base,"process",ps->process.process,200);
  ReadRegistryShort(hive,base,"warning",&ps->options.warning);
  ReadRegistryShort(hive,base,"run_program",&ps->options.run_program);
  ReadRegistryShort(hive,base,"force",&ps->options.force);
  ReadRegistryShort(hive,base,"in_tray",&ps->options.in_tray);
  ReadRegistryShort(hive,base,"allow_cancel",&ps->options.allow_cancel);
  ReadRegistryShort(hive,base,"allow_remote_control",&ps->options.allow_remote_control);
  Log("ReadSettings end");
  return 1;
}

void DebugSettings(PowerSettings *ps)
{
  Log("SebugSettings start");
  switch (ps->action)
  {
    case LOGOFF:
      Log("action=LOGOFF");
      break;
    case REBOOT:
      Log("action=REBOOT");
      break;
    case SHUTDOWN:
      Log("action=SHUTDOWN");
      break;
    case POWEROFF:
      Log("action=POWEROFF");
      break;
    case STANDBY:
      Log("action=STANDBY");
      break;
    case HIBERNATE:
      Log("action=HIBERNATE");
      break;
    case LOCK:
      Log("action=LOCK");
      break;
    case WAKE_ON_LAN:
      Log("action=WAKE_ON_LAN");
      break;
    case MONITOR_OFF:
      Log("action=MONITOR_OFF");
      break;
    case MONITOR_ON:
      Log("action=MONITOR_ON");
      break;
    case NO_ACTION:
      Log("action=NO_ACTION");
      break;
    default:
      Log("action=UNKNOWN");
      break;
  }
  switch (ps->who)
  {
    case LOCAL_COMPUTER:
      Log("who=LOCAL_COMPUTER");
      break;
    case REMOTE_COMPUTER:
      Log("who=REMOTE_COMPUTER");
      break;
    default:
      Log("who=UNKNOWN");
      break;
  }
  switch (ps->when)
  {
    case IMMEDIATE:
      Log("when=IMMEDIATE");
      break;
    case SCHEDULED:
      Log("when=SCHEDULED");
      break;
    case PROCESS:
      Log("when=PROCESS");
      break;
    default:
      Log("when=UNKNOWN");
      break;
  }
  Log("interactive=%d",ps->interactive);
  Log("active_timer=%u",ps->active_timer);
  Log("simulate=%d",ps->simulate);
  Log("start_minimized=%d",ps->start_minimized);
  Log("run_as_service=%d",ps->run_as_service);
  Log("executable=%s",ps->executable);
  Log("authenticated=%d",ps->authenticated);
  Log("quiet=%d",ps->quiet);
  Log("remaining_seconds=%d",ps->remaining_seconds);
  Log("remote.computer_name=%s",ps->remote.computer_name);
  Log("remote.mac_address=%s",ps->remote.mac_address);
  Log("remote.ip_address=%s",ps->remote.ip_address);
  Log("remote.subnet_mask=%s",ps->remote.subnet_mask);
  Log("remote.use_nt=%d",ps->remote.use_nt);
  Log("remote.port=%d",ps->remote.port);
  Log("remote.save_password=%d",ps->remote.save_password);
  Log("remote.password=%s",ps->remote.password);
  Log("remote.schedule_remote=%d",ps->remote.schedule_remote);
  Log("remote.current_credentials=%d",ps->remote.current_credentials);
  Log("remote.username=%s",ps->remote.username);
  Log("remote_control.port=%d",ps->remote_control.port);
  Log("remote_control.password=%s",ps->remote_control.password);
  Log("warning.seconds=%d",ps->warning.seconds);
  Log("warning.message=%s",ps->warning.message);
  Log("warning.play_sound=%d",ps->warning.play_sound);
  Log("warning.sound_file=%s",ps->warning.sound_file);
  Log("program.program=%s",ps->program.program);
  Log("program.directory=%s",ps->program.directory);
  switch (ps->schedule.schedule)
  {
    case FIXED_DAY:
      Log("schedule.schedule=FIXED_DAY");
      break;
    case DAILY:
      Log("schedule.schedule=DAILY");
      break;
    case DAY_OF_MONTH:
      Log("schedule.schedule=DAY_OF_MONTH");
      break;
    case AFTER_X_SECONDS:
      Log("schedule.schedule=AFTER_X_SECONDS");
      break;
    default:
      Log("schedule.schedule=UNKNOWN");
      break;
  }
  Log("schedule.monday=%d",ps->schedule.monday);
  Log("schedule.tuesday=%d",ps->schedule.tuesday);
  Log("schedule.wednesday=%d",ps->schedule.wednesday);
  Log("schedule.thursday=%d",ps->schedule.thursday);
  Log("schedule.friday=%d",ps->schedule.friday);
  Log("schedule.saturday=%d",ps->schedule.saturday);
  Log("schedule.sunday=%d",ps->schedule.sunday);
  Log("schedule.week1=%d",ps->schedule.week1);
  Log("schedule.week2=%d",ps->schedule.week2);
  Log("schedule.week3=%d",ps->schedule.week3);
  Log("schedule.week4=%d",ps->schedule.week4);
  Log("schedule.week5=%d",ps->schedule.week5);
  Log("schedule.day=%u",ps->schedule.day);
  Log("schedule.seconds=%u",ps->schedule.seconds);
  Log("schedule.wait=%d",ps->schedule.wait);
  Log("process.process=%s",ps->process.process);
  Log("options.warning=%d",ps->options.warning);
  Log("options.run_program=%d",ps->options.run_program);
  Log("options.force=%d",ps->options.force);
  Log("options.in_tray=%d",ps->options.in_tray);
  Log("options.allow_cancel=%d",ps->options.allow_cancel);
  Log("options.allow_remote_control=%d",ps->options.allow_remote_control);
  Log("SebugSettings end");
}